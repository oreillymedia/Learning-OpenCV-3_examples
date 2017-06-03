// Example 15-5. Cleanup using connected components
// This cleans up the foreground segmentation mask derived from calls
// to backgroundDiff
//
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

// polygons will be simplified using DP algorithm with 'epsilon' a fixed
// fraction of the polygon's length. This number is that divisor.
//
#define DP_EPSILON_DENOMINATOR 20.0
// How many iterations of erosion and/or dilation there should be
//
#define CVCLOSE_ITR 1	

void findConnectedComponents(
	cv::Mat& mask,				// Is a grayscale (8-bit depth) "raw" mask image
								// that will be cleaned up
	int poly1_hull0,	 		// If set, approximate connected component by
								// (DEFAULT: 1) polygon, or else convex hull (0)
	float perimScale, 			// Len = (width+height)/perimScale. If contour
								// len < this, delete that contour (DEFAULT: 4)
	vector<cv::Rect>& bbs,		// Ref to bounding box rectangle return vector
	vector<cv::Point>& centers	// Ref to contour centers return vector
	) {
	bbs.clear();
	centers.clear();
	
	// CLEAN UP RAW MASK
	//
	cv::morphologyEx(
		mask, mask, cv::MORPH_OPEN, cv::Mat(), cv::Point(-1,-1), CVCLOSE_ITR);
	cv::morphologyEx(
		mask, mask, cv::MORPH_CLOSE, cv::Mat(), cv::Point(-1,-1), CVCLOSE_ITR);

	
	// FIND CONTOURS AROUND ONLY BIGGER REGIONS
	//
	vector< vector<cv::Point> > contours_all; // all contours found
	vector< vector<cv::Point> > contours;

	// just the ones we want to keep 
	cv::findContours( mask,	contours_all, cv::RETR_EXTERNAL,
		cv::CHAIN_APPROX_SIMPLE);

	for( vector< vector<cv::Point> >::iterator c = contours_all.begin();
			c != contours_all.end(); ++c) {
			
		// length of this contour
		//
		int len = cv::arcLength( *c, true );
		
		// length threshold a fraction of image perimeter
		//
		double q = (mask.rows + mask.cols) / DP_EPSILON_DENOMINATOR;	
		if( len >= q ) { // If the contour is long enough to keep...
			vector<cv::Point> c_new;
			if( poly1_hull0 ) {
				// If the caller wants results as reduced polygons...
				cv::approxPolyDP( *c, c_new, len/200.0, true );
			} else {
				// Convex Hull of the segmentation
				cv::convexHull( *c, c_new );
			}
			contours.push_back(c_new );
		}
	}

	// Just some convenience variables
	const cv::Scalar CVX_WHITE(0xff,0xff,0xff);
	const cv::Scalar CVX_BLACK(0x00,0x00,0x00);
	
	// CALC CENTER OF MASS AND/OR BOUNDING RECTANGLES
	//
	int idx = 0;
	cv::Moments moments;
	cv::Mat scratch = mask.clone();
	for(vector< vector<cv::Point> >::iterator c = contours.begin();
		c != contours.end(); c++, idx++) {
		cv::drawContours( scratch, contours, idx, CVX_WHITE, cv::FILLED);
		
		// Find the center of each contour
		//
		moments = cv::moments( scratch, true );
		cv::Point p;
		p.x = (int)( moments.m10 / moments.m00 );
		p.y = (int)( moments.m01 / moments.m00 );
		centers.push_back(p);
		bbs.push_back( cv::boundingRect(*c) );
		scratch.setTo( 0 );
	}

	// PAINT THE FOUND REGIONS BACK INTO THE IMAGE
	//
	mask.setTo( 0 );
	cv::drawContours( mask, contours, -1, CVX_WHITE, cv::FILLED );
}		

////////////////////////////////////////////////////////////////////////
////////// Use previous example_15-04 and clean up its images //////////
////////////////////////////////////////////////////////////////////////


#define CHANNELS 3			//Always 3 because yuv
int cbBounds[CHANNELS];		// IF pixel is within this bound outside of codebook, learn it, else form new code
int minMod[CHANNELS];		// If pixel is lower than a codebook by this amount, it's matched
int maxMod[CHANNELS];		// If pixel is high than a codebook by this amount, it's matched


//The variable t counts the number of points we’ve accumulated since the start or the last
//clear operation. Here’s how the actual codebook elements are described:
//
class CodeElement {
	public:
		uchar learnHigh[CHANNELS];	//High side threshold for learning
		uchar learnLow[CHANNELS];	//Low side threshold for learning
		uchar max[CHANNELS];		//High side of box boundary
		uchar min[CHANNELS];		//Low side of box boundary
		int t_last_update;			//Allow us to kill stale entries
		int stale; 					//max negative run (longest period of inactivity)

		CodeElement() {
			for(int i = 0; i < CHANNELS; i++)
				learnHigh[i] = learnLow[i] = max[i] = min[i] = 0;
			t_last_update = stale = 0;
		}
		
		CodeElement& operator=( const CodeElement& ce ) {
			for(int i=0; i<CHANNELS; i++ ) {
				learnHigh[i] = ce.learnHigh[i];
				learnLow[i] = ce.learnLow[i];
				min[i] = ce.min[i];
				max[i] = ce.max[i];
			}
			t_last_update = ce.t_last_update;
			stale = ce.stale;
			return *this;
		}
		
		CodeElement( const CodeElement& ce ) { *this = ce; }
};

// You need one of these for each pixel in the video image (rowXcol)
//
class CodeBook : public vector<CodeElement> {
	public:
	int t;     //Count of every image learned on
	
	// count every access
	CodeBook() { t=0; }
	
	// Default is an empty book
	CodeBook( int n ) : vector<CodeElement>(n) { t=0; } // Construct book of size n
};


// Updates the codebook entry with a new data point
// Note: cbBounds must be of length equal to numChannels
//
//
int updateCodebook(  	// return CodeBook index
	const cv::Vec3b& p, // incoming YUV pixel
	CodeBook& c, 		// CodeBook for the pixel
	int* cbBounds,	// Bounds for codebook (usually: {10,10,10})
	int numChannels 	// Number of color channels we're learning
	) {
	if(c.size() == 0) 
		c.t = 0;
	c.t += 1;		//Record learning event
	//SET HIGH AND LOW BOUNDS
	unsigned int high[3], low[3], n;
	for( n=0; n<numChannels; n++ ) {
		high[n] = p[n] + *(cbBounds+n); 
		if( high[n] > 255 ) high[n] = 255;
		low[n] = p[n] - *(cbBounds+n);
		if( low[n] < 0) low[n] = 0;
	}
	
	// SEE IF THIS FITS AN EXISTING CODEWORD
	//
	int i;
	int matchChannel;
	for( i=0; i<c.size(); i++ ) {
		matchChannel = 0;
		for( n=0; n<numChannels; n++ ) {
			if( // Found an entry for this channel
				( c[i].learnLow[n] <= p[n] ) && ( p[n] <= c[i].learnHigh[n]))
				matchChannel++;
			}
		
		if( matchChannel == numChannels ) {// If an entry was found
			c[i].t_last_update = c.t; 
			
			// adjust this codeword for the first channel
			//
			for( n=0; n<numChannels; n++ ) {
				if( c[i].max[n] < p[n] )
					c[i].max[n] = p[n];
				else if( c[i].min[n] > p[n] ) 
					c[i].min[n] = p[n];
			}
			break;
		}
	}
	
	// OVERHEAD TO TRACK POTENTIAL STALE ENTRIES
	//
	for( int s=0; s<c.size(); s++ ) {
	
		// Track which codebook entries are going stale:
		//
		int negRun = c.t - c[s].t_last_update;
		if( c[s].stale < negRun ) c[s].stale = negRun;
	}
	
	// ENTER A NEW CODEWORD IF NEEDED
	//
	if( i == c.size() ) {
		// if no existing codeword found, make one
		CodeElement ce;
		for( n=0; n<numChannels; n++ ) {
			ce.learnHigh[n] = high[n];
			ce.learnLow[n] = low[n];
			ce.max[n] = p[n];
			ce.min[n] = p[n];
		}
			ce.t_last_update = c.t;
			ce.stale = 0;
			c.push_back( ce );
	}
	
	// SLOWLY ADJUST LEARNING BOUNDS
	//
	for( n=0; n<numChannels; n++ ) {
		if( c[i].learnHigh[n] < high[n]) c[i].learnHigh[n] += 1;
		if( c[i].learnLow[n] > low[n] ) c[i].learnLow[n] -= 1;
	}
	return c.size();
}

// During learning, after you've learned for some period of time,
// periodically call this to clear out stale codebook entries
//
int foo = 0;
int clearStaleEntries(
	// return number of entries cleared
	CodeBook &c
	// Codebook to clean up
){
	int staleThresh = c.t>>1;
	int *keep = new int[c.size()];
	int keepCnt = 0;
	
	// SEE WHICH CODEBOOK ENTRIES ARE TOO STALE
	//
	int foogo2 = 0;
	for( int i=0; i<c.size(); i++ ){
		if(c[i].stale > staleThresh) 
			keep[i] = 0; // Mark for destruction
		else
		{
			keep[i] = 1; // Mark to keep
			keepCnt += 1;
		}
	}
	
	// move the entries we want to keep to the front of the vector and then
	// truncate to the correct length once all of the good stuff is saved.
	//
	int k = 0;
	int numCleared = 0;
	for( int ii=0; ii<c.size(); ii++ ) {
		if( keep[ii] ) {
			c[k] = c[ii];
			// We have to refresh these entries for next clearStale
			c[k].t_last_update = 0;
			k++;
		} else {
			numCleared++;
		}
	}
	c.resize( keepCnt );
	delete[] keep;
	return numCleared;
}
	
// Given a pixel and a codebook, determine whether the pixel is
// covered by the codebook
//
// NOTES:
// minMod and maxMod must have length numChannels,
// e.g. 3 channels => minMod[3], maxMod[3]. There is one min and
//      one max threshold per channel.
// 
uchar backgroundDiff( // return 0 => background, 255 => foreground
const cv::Vec3b& p,   // Pixel (YUV)
CodeBook& c,          // Codebook
int numChannels,      // Number of channels we are testing
int* minMod_,          // Add this (possibly negative) number onto max level
                      //    when determining whether new pixel is foreground
int* maxMod_           // Subtract this (possibly negative) number from min
                      //    level when determining whether new pixel is
                      //    foreground
) {
	int matchChannel;
	
	// SEE IF THIS FITS AN EXISTING CODEWORD
	//
	int i;
	for( i=0; i<c.size(); i++ ) {
		matchChannel = 0;
		for( int n=0; n<numChannels; n++ ) {
			if((c[i].min[n] - minMod_[n] <= p[n] ) && (p[n] <= c[i].max[n] + maxMod_[n]))
			{
				matchChannel++; // Found an entry for this channel
			} else {
				break;
			}
		}
		if(matchChannel == numChannels) {
			break; // Found an entry that matched all channels
		}
	}
	if( i >= c.size() )	//No match with codebook => foreground
		return 255;
	return 0; 			//Else background
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// This part adds a "main" to run the above code. ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

// Just make a convienience class (assumes image will not change size in video);
class CbBackgroudDiff {
	public:
	cv::Mat Iyuv;					//Will hold the yuv converted image 
	cv::Mat mask;					//Will hold the background difference mask
	vector<CodeBook> codebooks;   	//Will hold a CodeBook for each pixel
	int row, col, image_length;		//How many pixels are in the image
	
	//Constructor
	void init(cv::Mat &I_from_video) {
		vector<int> v(3,10);
		set_global_vecs(cbBounds, v);
		v[0] = 6; v[1] = 20; v[2] = 8; //Just some decent defaults for low side
		set_global_vecs(minMod, v);
		v[0] = 70; v[1] = 20; v[2] = 6; //Decent defaults for high side
		set_global_vecs(maxMod, v);
		Iyuv.create(I_from_video.size(), I_from_video.type());
		mask.create(I_from_video.size(), CV_8UC1);
		row = I_from_video.rows;
		col = I_from_video.cols;
		image_length = row*col;
		cout << "(row,col,len) = (" << row << ", " << col << ", " << image_length << ")" << endl;
		codebooks.resize(image_length);
	}
	
	CbBackgroudDiff(cv::Mat &I_from_video) {
		init(I_from_video);
	}
	
	CbBackgroudDiff(){};
	
	//Convert to YUV
	void convert_to_yuv(cv::Mat &Irgb)
	{
		cvtColor(Irgb, Iyuv, cv::COLOR_BGR2YUV);
	}
	
	int size_check(cv::Mat &I) { //Check that image doesn't change size, return -1 if size doesn't match, else 0
		int ret = 0;
		if((row != I.rows) || (col != I.cols)) {
			cerr << "ERROR: Size changed! old[" << row << ", " << col << "], now [" << I.rows << ", " << I.cols << "]!" << endl;
			ret = -1;
		}
		return ret;
	}
	
	//Utilities for setting gloabals
	void set_global_vecs(int *globalvec, vector<int> &vec) {
		if(vec.size() != CHANNELS) {
			cerr << "Input vec[" << vec.size() << "] should equal CHANNELS [" << CHANNELS << "]" << endl;
			vec.resize(CHANNELS, 10);
		}
		int i = 0;
		for (vector<int>::iterator it = vec.begin(); it != vec.end(); ++it, ++i) {
			 globalvec[i] = *it;
		 }
	 }

	//Background operations
	int updateCodebookBackground(cv::Mat &Irgb) { //Learn codebook, -1 if error, else total # of codes
		convert_to_yuv(Irgb);
		if(size_check(Irgb))
			return -1;
		int total_codebooks = 0;
		cv::Mat_<cv::Vec3b>::iterator Iit = Iyuv.begin<cv::Vec3b>(), IitEnd = Iyuv.end<cv::Vec3b>();
		vector<CodeBook>::iterator Cit = codebooks.begin(), CitEnd = codebooks.end();
		for(; Iit != IitEnd; ++Iit,++Cit) {
			total_codebooks += updateCodebook(*Iit,*Cit,cbBounds,CHANNELS);
		}
		if(Cit != CitEnd)
			cerr << "ERROR: Cit != CitEnd in updateCodeBackground(...) " << endl;
		return(total_codebooks);
	}

	int clearStaleEntriesBackground() { //Clean out stuff that hasn't been updated for a long time
		int total_cleared = 0;
		vector<CodeBook>::iterator Cit = codebooks.begin(), CitEnd = codebooks.end();
		for(; Cit != CitEnd; ++Cit) {
			total_cleared += clearStaleEntries(*Cit);
		}
		return(total_cleared);
	}

	int backgroundDiffBackground(cv::Mat &Irgb) {  //Take the background difference of the image
		convert_to_yuv(Irgb);
		if(size_check(Irgb))
			return -1;
		cv::Mat_<cv::Vec3b>::iterator Iit = Iyuv.begin<cv::Vec3b>(), IitEnd = Iyuv.end<cv::Vec3b>();
		vector<CodeBook>::iterator Cit = codebooks.begin(), CitEnd = codebooks.end();
		cv::Mat_<uchar>::iterator Mit = mask.begin<uchar>(), MitEnd = mask.end<uchar>();
		for(; Iit != IitEnd; ++Iit,++Cit,++Mit) {
			*Mit = backgroundDiff(*Iit,*Cit,CHANNELS,minMod,maxMod);
		}
		if((Cit != CitEnd)||(Mit != MitEnd)){
			cerr << "ERROR: Cit != CitEnd and, or Mit != MitEnd in updateCodeBackground(...) " << endl;
			return -1;
		}
		return 0;
	}
}; // end CbBackgroudDiff


void help(char** argv ) {
	cout << "\n"
	<< "We test out our connected components algorithm using the background code from example_0=15-04\n"
	<< "First we train a codebook background model on the first <#frames to train on> frames"
	<< " of an incoming video, then run the model on it cleaning it up with findConnectedComponents\n"
	<< argv[0] <<" <#frames to train on> <avi_path/filename>\n"
	<< "For example:\n"
	<< argv[0] << " 50 ../tree.avi\n"
	<< "'A' or 'a' to adjust thresholds, esc, 'q' or 'Q' to quit"
	<< endl;
}

//Adjusting the distance you have to be on the low side (minMod) or high side (maxMod) of a codebook
//to be considered as recognized/background
//
void adjustThresholds(char* charstr, cv::Mat &Irgb, CbBackgroudDiff &bgd) {
	int key = 1;
	int y = 1,u = 0,v = 0, index = 0, thresh = 0;
	if(thresh)
		cout << "yuv[" << y << "][" << u << "][" << v << "] maxMod active" << endl;
	else
		cout << "yuv[" << y << "][" << u << "][" << v << "] minMod active" << endl;
	cout << "minMod[" << minMod[0] << "][" << minMod[1] << "][" << minMod[2] << "]" << endl;
	cout << "maxMod[" << maxMod[0] << "][" << maxMod[1] << "][" << maxMod[2] << "]" << endl;
	while((key = cv::waitKey()) != 27 && key != 'Q' && key != 'q')  // Esc or Q or q to exit
	{
		if(thresh)
			cout << "yuv[" << y << "][" << u << "][" << v << "] maxMod active" << endl;
		else
			cout << "yuv[" << y << "][" << u << "][" << v << "] minMod active" << endl;
		cout << "minMod[" << minMod[0] << "][" << minMod[1] << "][" << minMod[2] << "]" << endl;
		cout << "maxMod[" << maxMod[0] << "][" << maxMod[1] << "][" << maxMod[2] << "]" << endl;
		
		if(key == 'y') { y = 1; u = 0; v = 0; index = 0;}
		if(key == 'u') { y = 0; u = 1; v = 0; index = 1;}
		if(key == 'v') { y = 0; u = 0; v = 1; index = 2;}
		if(key == 'l') { thresh = 0;} //minMod
		if(key == 'h') { thresh = 1;} //maxMod
		if(key == '.') { //Up
			if(thresh == 0) { minMod[index] += 4;}
			if(thresh == 1) { maxMod[index] += 4;}
		}
		if(key == ',') { //Down
			if(thresh == 0) { minMod[index] -= 4;}
			if(thresh == 1) { maxMod[index] -= 4;}
		}
		cout << "y,u,v for channel; l for minMod, h for maxMod threshold; , for down, . for up; esq or q to quit;" << endl;
		bgd.backgroundDiffBackground(Irgb);
		cv::imshow(charstr, bgd.mask);
	}
}


////////////////////////////////////////////////////////////////
vector<cv::Rect> bbs;		// Ref to bounding box rectangle return vector
vector<cv::Point> centers;	// Ref to contour centers return vector


int main( int argc, char** argv) {
	cv::namedWindow( argv[0], cv::WINDOW_AUTOSIZE );
	cv::VideoCapture cap;
	if((argc < 3)|| !cap.open(argv[2])) {
		cerr << "Couldn't run the program" << endl;
		help(argv);
		cap.open(0);
		return -1;
	}
	int number_to_train_on = atoi( argv[1] );
	cv::Mat image; 
	CbBackgroudDiff bgd;

	// FIRST PROCESSING LOOP (TRAINING):
	//
	int frame_count = 0;
	int key;
	bool first_frame = true;
	cout << "Total frames to train on = " << number_to_train_on << endl;
	char seg[] = "Segmentation";
	while(1) {
		cout << "frame#: " << frame_count;
		cap >> image;
		if( !image.data ) exit(1); // Something went wrong, abort
		if(frame_count == 0) { bgd.init(image);}
		
		cout << ", Codebooks: " << bgd.updateCodebookBackground(image) << endl;
		
		cv::imshow( argv[0], image );
		frame_count++;
		if( (key = cv::waitKey(7)) == 27 || key == 'q' || key == 'Q' || frame_count >= number_to_train_on) break; //Allow early exit on space, esc, q
	}

	// We have accumulated our training, now create the models
	//
	cout << "Created the background model" << endl;
	cout << "Total entries cleared = " << bgd.clearStaleEntriesBackground() << endl;
	cout << "Press a key to start background differencing, 'a' to set thresholds, esc or q or Q to quit" << endl;
	
	// SECOND PROCESSING LOOP (TESTING):
	//
	cv::namedWindow( seg, cv::WINDOW_AUTOSIZE );
	cv::namedWindow("Conected Components", cv::WINDOW_AUTOSIZE);
	while((key = cv::waitKey()) != 27 || key == 'q' || key == 'Q'  ) { // esc, 'q' or 'Q' to exit
		cap >> image;
		if( !image.data ) exit(0);
		cout <<  frame_count++ << " 'a' to adjust threholds" << endl;
		if(key == 'a') {
			cout << "Adjusting thresholds" << endl;
		cout << "y,u,v for channel; l for minMod, h for maxMod threshold; , for down, . for up; esq or q to quit;" << endl;
			adjustThresholds(seg,image,bgd);
		}
		else {
			if(bgd.backgroundDiffBackground(image)) {
				cerr << "ERROR, bdg.backgroundDiffBackground(...) failed" << endl;
				exit(-1);
			}
		}
		cv::imshow("Segmentation",bgd.mask);
		findConnectedComponents(bgd.mask, 1, 4, bbs, centers);
		cv::imshow("Conected Components", bgd.mask);
	}
	exit(0);
}




