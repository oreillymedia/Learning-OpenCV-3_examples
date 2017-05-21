// Log-polar transform example.
// This demonstrates the forward and backward (inverse) log-polar
// transform.

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  if(argc != 3) {
    cout << "LogPolar\nUsage: " <<argv[0] <<" <imagename> <M value>\n"
	 <<"<M value>~30 is usually good enough\n";
    return -1;
  }

  cv::Mat src = cv::imread(argv[1],1);

  if( src.empty() ) { cout << "Can not load " << argv[1] << endl; return -1; }

  double M = atof(argv[2]);
  cv::Mat dst(src.size(), src.type()), src2(src.size(), src.type());

  cv::logPolar(
    src,
    dst,
    cv::Point2f(src.cols*0.5f, src.rows*0.5f),
    M,
    cv::INTER_LINEAR | cv::WARP_FILL_OUTLIERS
  );
  cv::logPolar(
    dst,
    src2,
    cv::Point2f(src.cols*0.5f, src.rows*0.5f),
    M,
    cv::INTER_LINEAR | cv::WARP_INVERSE_MAP
  );
  cv::imshow( "log-polar", dst );
  cv::imshow( "inverse log-polar", src2 );

  cv::waitKey();

  return 0;
}
