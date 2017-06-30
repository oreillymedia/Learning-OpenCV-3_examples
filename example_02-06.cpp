// Example 2-6. Using cv::pyrDown() to create a new image that is half the width and
// height of the input image
#include <opencv2/opencv.hpp>

void help(char** argv ) {
	std::cout << "\n"
	<< "2-06: AUsing cv::pyrDown() to create a new image that is half the width and"
    << "      height of the input image\n"
	<< argv[0] <<" <path/image>\n"
	<< "For example:\n"
	<< argv[0] << " ../faces.png\n"
	<< std::endl;
}
	

int main( int argc, char** argv ) {
	
	if (argc != 2) {
		help(argv);
		return 0;
	}

  cv::Mat img1,img2;

  cv::namedWindow( "Example 2-6-in", cv::WINDOW_AUTOSIZE );
  cv::namedWindow( "Example 2-6-out", cv::WINDOW_AUTOSIZE );

  img1 = cv::imread( argv[1] );

  cv::imshow( "Example 2-6-in", img1 );
  cv::pyrDown( img1, img2);

  cv::imshow( "Example 2-6-out", img2 );
  cv::waitKey(0);

  return 0;

};
