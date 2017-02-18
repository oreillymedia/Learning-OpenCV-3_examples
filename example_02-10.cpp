#include <opencv2/opencv.hpp>
#include <iostream>

int main( int argc, char** argv ) {

  cv::namedWindow( "Example 2-10", cv::WINDOW_AUTOSIZE );
  cv::VideoCapture cap;

  if (argc==1) {
    cap.open(0); // open the first camera
  } else {
    cap.open(argv[1]);
  }

  if( !cap.isOpened() ) { // check if we succeeded
    std::cerr << "Couldn't open capture." << std::endl;
    return -1;
  }

  cv::Mat frame;

  for(;;) {

    cap >> frame;

    if( frame.empty() ) break; // Ran out of film

    cv::imshow( "Example 2-10", frame );

    if( (char) cv::waitKey(33) >= 0 ) break;

  }

  return 0;

}
