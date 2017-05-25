
// Example 9-1. Creating a window and displaying an image in that window
//
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;


void help(char** argv ) {
	cout << "\n"
		<< "Create a window and display an image\nCall:\n"
		<< argv[0] <<" image\n"
		<< endl;
}

int main(int argc, char** argv) {
	// Document the interface
	help(argv);
	if(argc != 2) { cout << "You need to supply an image path/name" << endl;; exit(0);}

  // Create a named window with the name of the file
  //
  cv::namedWindow( argv[1], 1 );

  // Load the image from the given filename
  //
  cv::Mat img = cv::imread( argv[1] );

  // Show the image in the named window
  //
  cv::imshow( argv[1], img );

  // Idle until the user hits the Esc key
  //
  cv::waitKey(); //Any key will end the program

  // Clean up and don't be piggies
  //
  cv::destroyWindow( argv[1] );
  exit(0);
}
