// alphablend <imageA> <image B> <x> <y> <width> <height> <alpha> <beta>
//
//#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char** argv) {

  // Using the first two arguments, open up the image to be copied onto
  // (src1), and the image that will be copied from (src2).
  //
  cv::Mat src1 = cv::imread(argv[1],1);
  cv::Mat src2 = cv::imread(argv[2],1);

  if( argc==9 && !src1.empty() && !src2.empty() ) {

    // Four more arguments tell where in src1 to paste the chunk taken from
    // src2. Note that the width and height also specify what portion of
    // src2 to actually use.
    //
    int x = atoi(argv[3]);
    int y = atoi(argv[4]);
    int w = atoi(argv[5]);
    int h = atoi(argv[6]);

    // Two more arguments set the blending coefficients.
    //
    double alpha = (double)atof(argv[7]);
    double beta  = (double)atof(argv[8]);

    cv::Mat roi1( src1, cv::Rect(x,y,w,h) );
    cv::Mat roi2( src2, cv::Rect(0,0,w,h) );

    // Blend together the image src2 onto the image src1
    // at the specified location.
    //
    cv::addWeighted( roi1, alpha, roi2, beta, 0.0, roi2 );

    // Create a window to shoow the result and show it.
    //
    cv::namedWindow( "Alpha Blend", 1 );
    cv::imshow( "Alpha Blend", src2 );

    // Leave the window up and runnnig until the user hits a key
    //
    cv::waitKey( 0 );

  }

  return 0;

}
