// Example 9-10. The WxMoviePlayer object source file WxMoviePlayer.cpp
#include <opencv2/opencv.hpp>

using namespace std;

int main( int argc, char** argv ) {
  // Create a named window with the name of the file
  //
  cv::namedWindow( argv[1], 1 );
  // Load the image from the given filename
  //
  cv::Mat = cv::imread( argv[1] );
  // Show the image in the named window
  //
  cv::imshow( argv[1], img );
  // Idle until the user hits the Esc key
  //
  while( true ) {
    if( cv::waitKey( 100 /* milliseconds */ ) == 27 ) break;
  }
  // Clean up and don't be piggies
  //
  cv::destroyWindow( argv[1] );
  exit(0);
}
