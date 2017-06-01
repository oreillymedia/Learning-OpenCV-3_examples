//Example 15-3. Computing the on and off-diagonal elements of a variance/covariance model
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

vector<cv::Mat> planes(3);
vector<cv::Mat> sums(3);
vector<cv::Mat> xysums(6);
cv::Mat sum, sqsum;
int image_count = 0;

//A function to accumulate
// the information we need for our variance computation:
//
void accumulateVariance(
	cv::Mat& I) {
	if( sum.empty() ) {
		sum = cv::Mat::zeros( I.size(), CV_32FC(I.channels()) );
		sqsum = cv::Mat::zeros( I.size(), CV_32FC(I.channels()) );
		image_count = 0;
	}
	cv::accumulate( I, sum );
	cv::accumulateSquare( I, sqsum );
	image_count++;
}

//The associated variance computation function would then be:
// (note that 'variance' is sigma^2)
//
void computeVariance(
	cv::Mat& variance) {
	double one_by_N = 1.0 / image_count;
	variance = (one_by_N * sqsum) - ((one_by_N * one_by_N) * sum.mul(sum));
}

//Same as above function, but compute standard deviation
void computeStdev(
	cv::Mat& std__) {
	double one_by_N = 1.0 / image_count;
	cv::sqrt(((one_by_N * sqsum) -((one_by_N * one_by_N) * sum.mul(sum))), std__);
}

//And avg images
void computeAvg(
	cv::Mat& av) {
	double one_by_N = 1.0 / image_count;
	av = one_by_N * sum;
}
	
// ===================================================================//


void accumulateCovariance(
cv::Mat& I
) {
	int i, j, n;

	if( sum.empty() ) {
		image_count = 0;
		for( i=0; i<3; i++ ) {
			// the r, g, and b sums
			sums[i]
			= cv::Mat::zeros( I.size(), CV_32FC1 );
		}
		for( n=0; n<6; n++ ) {
			// the rr, rg, rb, gg, gb, and bb elements
			xysums[n] = cv::Mat::zeros( I.size(), CV_32FC1 );
		}
	}
	cv::split( I, planes );
	for( i=0; i<3; i++ ) {
		cv::accumulate( planes[i], sums[i] );
	}
	n = 0;
	for( i=0; i<3; i++ ) {
		// "row" of Sigma
		for( j=i; j<3; j++ ) {
			// "column" of Sigma
			n++;
			cv::accumulateProduct( planes[i], planes[j], xysums[n] );
		}
	}
	image_count++;
}

//The corresponding compute function is also just a slight extension of 
//the compute function for the variances we saw earlier.
//   note that 'variance' is sigma^2
//
void computeCoariance(
	cv::Mat& covariance
	// a six-channel array, channels are the
	// rr, rg, rb, gg, gb, and bb elements of Sigma_xy
) {
	double one_by_N = 1.0 / image_count;
	
	// reuse the xysum arrays as storage for individual entries
	//
	int n = 0;
	for( int i=0; i<3; i++ ) {
		// "row" of Sigma
		for( int j=i; j<3; j++ ) {
			// "column" of Sigma
			n++;
			xysums[n] = (one_by_N * xysums[n])
			- ((one_by_N * one_by_N) * sums[i].mul(sums[j]));
		}
	}
	
	// reassemble the six individual elements into a six-channel array
	//
	cv::merge( xysums, covariance );
}

////////////////////////////////////////////////////////////////////////
/////////////Utilities to run///////////////////////////////////////////

void help(char** argv ) {
	cout << "\n"
	<< "Compute mean and std on <#frames to train on> frames of an incoming video, then run the model\n"
	<< argv[0] <<" <#frames to train on> <avi_path/filename>\n"
	<< "For example:\n"
	<< argv[0] << " 50 ../tree.avi\n"
	<< "'a' to adjust thresholds, esc, 'q' or 'Q' to quit"
	<< endl;
}
	
//////////////  Borrowed code from example_15-02  //////////////////////

// Global storage
//
// Float, 3-channel images
//
cv::Mat image; // movie frame
cv::Mat IavgF, IdiffF, IhiF, IlowF; //threshold
cv::Mat tmp, mask; //scratch and our mask

// Float, 1-channel images
//
vector<cv::Mat> Igray(3); //scratch to split image
vector<cv::Mat> Ilow(3);//low per pixel thresh
vector<cv::Mat> Ihi(3); //high per pixel thresh

// Byte, 1-channel image
//
cv::Mat Imaskt; //Temp mask

// Thresholds
//
float high_thresh = 21.0;  //scaling the thesholds in backgroundDiff()
float low_thresh = 2.0;    //

// I is just a sample image for allocation purposes
// (passed in for sizing)
//
void AllocateImages( const cv::Mat& I ) {
	cv::Size sz = I.size();
	IavgF = cv::Mat::zeros(sz, CV_32FC3 ); 
	IdiffF = cv::Mat::zeros(sz, CV_32FC3 ); 
	IhiF = cv::Mat::zeros(sz, CV_32FC3 ); 
	IlowF = cv::Mat::zeros(sz, CV_32FC3 );
	tmp = cv::Mat::zeros( sz, CV_32FC3 ); 
	Imaskt = cv::Mat( sz, CV_32FC1 ); 
}


void setHighThreshold( float scale ) {
	IhiF = IavgF + (IdiffF * scale);
	cv::split( IhiF, Ihi );
}

void setLowThreshold( float scale ) {
	IlowF = IavgF - (IdiffF * scale);
	cv::split( IlowF, Ilow );
}

void createModelsfromStats() {
	//IavgF is already set;
	//IdiffF is the standard deviation image...
	
	// Make sure diff is always something
	//
	IdiffF += cv::Scalar( 0.1, 0.1, 0.1 );
	setHighThreshold( high_thresh);
	setLowThreshold( low_thresh);
}


// Create a binary: 0,255 mask where 255 (red) means foreground pixel
// I      Input image, 3-channel, 8u
// Imask  Mask image to be created, 1-channel 8u
//
void backgroundDiff(
		cv::Mat& I,
		cv::Mat& Imask) {
	
	I.convertTo( tmp, CV_32F ); // To float
	cv::split( tmp, Igray );
	
	// Channel 1
	//
	cv::inRange( Igray[0], Ilow[0], Ihi[0], Imask );

	// Channel 2
	//
	cv::inRange( Igray[1], Ilow[1], Ihi[1], Imaskt );
	Imask = cv::min( Imask, Imaskt );

	// Channel 3
	//
	cv::inRange( Igray[2], Ilow[2], Ihi[2], Imaskt );
	Imask = cv::min( Imask, Imaskt );

	// Finally, invert the results
	//
	Imask = 255 - Imask;
}


void showForgroundInRed( char** argv, const cv::Mat &img) {
		cv::Mat rawImage;
		cv::split( img, Igray );
		Igray[2] = cv::max( mask, Igray[2] );
		cv::merge( Igray, rawImage );
		cv::imshow( argv[0], rawImage );
		cv::imshow("Segmentation", mask);
}

void adjustThresholds(char** argv, cv::Mat &img) {
	int key = 1;
	while((key = cv::waitKey()) != 27 && key != 'Q' && key != 'q')  // Esc or Q or q to exit
	{
		if(key == 'L') { low_thresh += 0.2;}
		if(key == 'l') { low_thresh -= 0.2;}	
		if(key == 'H') { high_thresh += 0.2;}
		if(key == 'h') { high_thresh -= 0.2;}
		cout << "H or h, L or l, esq or q to quit;  high_thresh = " << high_thresh << ", " << "low_thresh = " << low_thresh << endl;
		setHighThreshold(high_thresh);
		setLowThreshold(low_thresh);
		backgroundDiff(img, mask);
		showForgroundInRed(argv, img);
	}
}

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

	// FIRST PROCESSING LOOP (TRAINING):
	//
	int image_count = 0;
	int key;
	bool first_frame = true;
	cout << "Total frames to train on = " << number_to_train_on << endl; //db
	while(1) {
		cout << "frame#: " << image_count << endl;
		cap >> image;
		if( !image.data ) exit(1); // Something went wrong, abort
		if(image_count == 0) AllocateImages( image ); 
		accumulateVariance(image);
		cv::imshow( argv[0], image );
		image_count++;
		if( (key = cv::waitKey(7)) == 27 || key == 'q' || key == 'Q' || image_count >= number_to_train_on) break; //Allow early exit on space, esc, q
	}

	// We have accumulated our training, now create the models
	//
	cout << "Creating the background model" << endl;
	computeAvg(IavgF);
	computeStdev(IdiffF);
	createModelsfromStats();
	cout << "Done!  Hit any key to continue into single step. Hit 'a' or 'A' to adjust thresholds, esq, 'q' or 'Q' to quit\n" << endl;

	// SECOND PROCESSING LOOP (TESTING):
	//
	cv::namedWindow("Segmentation", cv::WINDOW_AUTOSIZE ); //For the mask image
	while((key = cv::waitKey()) != 27 || key == 'q' || key == 'Q'  ) { // esc, 'q' or 'Q' to exit
		cap >> image;
		if( !image.data ) exit(0);
		cout <<  image_count++ << endl;
		backgroundDiff( image, mask );
		cv::imshow("Segmentation", mask);
		
		// A simple visualization is to write to the red channel
		//
		showForgroundInRed( argv, image);
		if(key == 'a') {
			cout << "In adjust thresholds, 'H' or 'h' == high thresh up or down; 'L' or 'l' for low thresh up or down." << endl;
			cout << " esq, 'q' or 'Q' to quit " << endl;
			adjustThresholds(argv, image);
			cout << "Done with adjustThreshold, back to frame stepping, esq, q or Q to quit." << endl;
		}
	}
	exit(0);
}

	
