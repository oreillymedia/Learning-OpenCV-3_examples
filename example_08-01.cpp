// Example 8-1. Unpacking a four-character code to identify a video codec
//
//#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
void help(char** argv ) {
	cout << "\n"
	<< "Example 8-1. Unpacking a four-character code to identify a video codec"
  << "\nCall:\n"
	<< argv[0] <<" <path/video>\n"
	<< "For example:\n"
	<< argv[0] << " ../tree.avi\n"
	<< endl;
}
	

int main( int argc, char** argv ) {
	
	if (argc != 2) {
		help(argv);
		return 0;
	}


  cv::VideoCapture cap( argv[1] );

  unsigned f = (unsigned)cap.get( cv::CAP_PROP_FOURCC );

  char fourcc[] = {
    (char) f, // First character is lowest bits
    (char)(f >> 8), // Next character is bits 8-15
    (char)(f >> 16), // Next character is bits 16-23
    (char)(f >> 24), // Last character is bits 24-31
    '\0' // and don't forget to terminate
  };

  cout <<"FourCC for this video was: " <<fourcc <<endl;




}
