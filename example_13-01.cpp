// Example 13-1. Histogram computation and display

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

int main( int argc, char** argv ){
  if(argc != 2) {
    cout << "Computer Color Histogram\nUsage: " <<argv[0] <<" <imagename>" << endl;
    return -1;
  }

  cv::Mat src = cv::imread( argv[1],1 );
  if( src.empty() ) { cout << "Cannot load " << argv[1] << endl; return -1; }

  // Compute the HSV image, and decompose it into separate planes.
  //
  cv::Mat hsv;
  cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

  float h_ranges[] = {0, 180}; // hue is [0, 180]
  float s_ranges[] = {0, 256};
  const float* ranges[] = {h_ranges, s_ranges};
  int histSize[] = {30, 32}, ch[] = {0, 1};

  cv::Mat hist;

  // Compute the histogram
  //
  cv::calcHist(&hsv, 1, ch, cv::noArray(), hist, 2, histSize, ranges, true);
  cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);

  int scale = 10;
  cv::Mat hist_img(histSize[0]*scale, histSize[1]*scale, CV_8UC3);

  // Draw our histogram.
  //
  for( int h = 0; h < histSize[0]; h++ ) {
    for( int s = 0; s < histSize[1]; s++ ){
      float hval = hist.at<float>(h, s);
      cv::rectangle(
	hist_img,
	cv::Rect(h*scale,s*scale,scale,scale),
	cv::Scalar::all(hval),
	-1
      );
    }
  }

  cv::imshow("image", src);
  cv::imshow("H-S histogram", hist_img);
  cv::waitKey();
  return 0;
}
