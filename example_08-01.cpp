// alphablend <imageA> <image B> <x> <y> <width> <height> <alpha> <beta>
//
//#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {

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
