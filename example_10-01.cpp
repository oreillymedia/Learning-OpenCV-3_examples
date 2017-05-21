//Example 10-1. Using cv::threshold() to sum three channels of an image

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

void sum_rgb( const cv::Mat& src, cv::Mat& dst ) {

	// Split image onto the color planes.
	//
	vector< cv::Mat> planes;
	cv::split(src, planes);
	cv::Mat b = planes[0], g = planes[1], r = planes[2], s;

	// Add equally weighted rgb values.
	//
	cv::addWeighted( r, 1./3., g, 1./3., 0.0, s );
	cv::addWeighted( s, 1., b, 1./3., 0.0, s );

	// Truncate values above 100.
	//
	cv::threshold( s, dst, 100, 100, cv::THRESH_TRUNC );
}

void help() {
	cout << "Call: ./example_10-03 ../faces.jpg" << endl;
	cout << "Shows use of alpha blending (addWeighted) and threshold" << endl;
}

int main(int argc, char** argv) {
	help();
	if(argc < 2) { cout << "specify input image" << endl; return -1; }

	// Load the image from the given file name.
	//
	cv::Mat src = cv::imread( argv[1] ), dst;
	if( src.empty() ) { cout << "can not load " << argv[1] << endl; return -1; }
	sum_rgb( src, dst);

	// Create a named window with the name of the file and
	// show the image in the window
	//
	cv::imshow( argv[1], dst );

	// Idle until the user hits any key.
	//
	cv::waitKey(0);
	return 0;
}
