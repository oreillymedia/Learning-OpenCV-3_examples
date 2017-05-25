// Example 10-2. Alternative method to combine and threshold image planes

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

void sum_rgb( const cv::Mat& src, cv::Mat& dst ) {

	// Split image onto the color planes.
	//
	vector<cv::Mat> planes;
	cv::split(src, planes);
	cv::Mat b = planes[0], g = planes[1], r = planes[2];

	// Accumulate separate planes, combine and threshold.
	//
	cv::Mat s = cv::Mat::zeros(b.size(), CV_32F);
	cv::accumulate(b, s);
	cv::accumulate(g, s);
	cv::accumulate(r, s);

	// Truncate values above 100 and rescale into dst.
	//
	cv::threshold( s, s, 100, 100, cv::THRESH_TRUNC );
	s.convertTo(dst, b.type());
}

void help() {
	cout << "Call: ./example_10-02 ../faces.jpg" << endl;
	cout << "Shows an alternative use of alpha blending and threshold" << endl;
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
