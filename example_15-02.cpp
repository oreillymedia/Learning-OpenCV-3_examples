// Example 15-2. Learning a background model to identify foreground pixels
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

// Global storage
//
// Float, 3-channel images
//
cv::Mat image; //, rawImage;
cv::Mat IavgF, IdiffF, IprevF, IhiF, IlowF;
cv::Mat tmp, tmp2, mask;

// Float, 1-channel images
//
vector<cv::Mat> Igray(3);
vector<cv::Mat> Ilow(3);
vector<cv::Mat> Ihi(3);

// Byte, 1-channel image
//
cv::Mat Imaskt;

// Thresholds
//
float high_thresh = 15.0;  //scaling the thesholds in backgroundDiff()
float low_thresh = 13.0;

// Counts number of images learned for averaging later
//
float Icount;

// I is just a sample image for allocation purposes
// (passed in for sizing)
//
void AllocateImages( const cv::Mat& I ) {
	cv::Size sz = I.size();
	IavgF = cv::Mat::zeros(sz, CV_32FC3 );
	IdiffF = cv::Mat::zeros(sz, CV_32FC3 );
	IprevF = cv::Mat::zeros(sz, CV_32FC3 );
	IhiF = cv::Mat::zeros(sz, CV_32FC3 );
	IlowF = cv::Mat::zeros(sz, CV_32FC3 );
	Icount = 0.00001; // Protect against divide by zero
	tmp = cv::Mat::zeros( sz, CV_32FC3 );
	tmp2 = cv::Mat::zeros( sz, CV_32FC3 );
	Imaskt = cv::Mat( sz, CV_32FC1 );
}

// Learn the background statistics for one more frame
// I is a color sample of the background, 3-channel, 8u
//
void accumulateBackground( cv::Mat& I ){
	static int first = 1; // nb. Not thread safe
	I.convertTo( tmp, CV_32F ); // convert to float
	if( !first ){
		IavgF += tmp;
		cv::absdiff( tmp, IprevF, tmp2 );
		IdiffF += tmp2;
		Icount += 1.0;
	}
	first = 0;
	IprevF = tmp;
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
	IavgF *= (1.0/Icount);
	IdiffF *= (1.0/Icount);
	
	// Make sure diff is always something
	//
	IdiffF += cv::Scalar( 1.0, 1.0, 1.0 );
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

///////////////////
void help(char** argv ) {
	cout << "\n"
	<< "Train a background model on  the first <#frames to train on> frames of an incoming video, then run the model\n"
	<< argv[0] <<" <#frames to train on> <avi_path/filename>\n"
	<< "For example:\n"
	<< argv[0] << " 50 ../tree.avi\n"
	<< "'A' or 'a' to adjust thresholds, esc, 'q' or 'Q' to quit"
	<< endl;
}

void showForgroundInRed( char** argv, const cv::Mat &img) {
		cv::Mat rawImage;
		cv::split( img, Igray );
		Igray[2] = cv::max( mask, Igray[2] );
		cv::merge( Igray, rawImage );
		cv::imshow( argv[0], rawImage );
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

////////////////////////////////////////////////////////////////
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
	int frame_count = 0;
	int key;
	bool first_frame = true;
	cout << "Total frames to train on = " << number_to_train_on << endl; //db
	while(1) {
		cout << "frame#: " << frame_count << endl;
		cap >> image;
		if(frame_count == 0) { AllocateImages(image);}
		if( !image.data ) exit(1); // Something went wrong, abort
		accumulateBackground( image );
		cv::imshow( argv[0], image );
		frame_count++;
		if( (key = cv::waitKey(7)) == 27 || key == 'q' || key == 'Q' || frame_count >= number_to_train_on) break; //Allow early exit on space, esc, q
	}

	// We have accumulated our training, now create the models
	//
	cout << "Creating the background model" << endl;
	createModelsfromStats();
	cout << "Done!  Hit any key to continue into single step. Hit 'a' or 'A' to adjust thresholds, esq, 'q' or 'Q' to quit\n" << endl;
	
	// SECOND PROCESSING LOOP (TESTING):
	//
	while((key = cv::waitKey()) != 27 || key == 'q' || key == 'Q'  ) { // esc, 'q' or 'Q' to exit
		cap >> image;
		if( !image.data ) exit(0);
		cout <<  frame_count++ << endl;
		backgroundDiff( image, mask );

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
