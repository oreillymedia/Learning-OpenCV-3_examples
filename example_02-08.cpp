#include <opencv2/opencv.hpp>

int main( int argc, char** argv ) {

  cv::Mat img_rgb, img_gry, img_cny, img_pyr, img_pyr2;

  cv::namedWindow( "Example Gray", cv::WINDOW_AUTOSIZE );
  cv::namedWindow( "Example Canny", cv::WINDOW_AUTOSIZE );

  img_rgb = cv::imread( argv[1] );

  cv::cvtColor( img_rgb, img_gry, cv::COLOR_BGR2GRAY);

  cv::pyrDown( img_gry, img_pyr );
  cv::pyrDown( img_pyr, img_pyr2 );

  cv::Canny( img_pyr2, img_cny, 10, 100, 3, true );
  
  cv::imshow( "Example Gray", img_gry );

  cv::imshow( "Example Canny", img_cny );

  cv::waitKey(0);

}
