// Example 12-2. Using cv::HoughCircles() to return a sequence of circles found in a
// grayscale image

#include <math.h>

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

using std::cout;
using std::endl;
using std::vector;

void help(char** argv) {
        cout  << "\nExample 12-1. Using cv::dft() and cv::idft() to accelerate the computation of convolutions"  
              << "\nHough Circle detect\nUsage: " << argv[0] <<" <path/imagename>\n" 
              << "Example:\n" << argv[0] << " ../stuff.jpg\n" << endl;
}

int main(int argc, char** argv) {
	help(argv);
    if (argc != 2) {
        return -1;
    }

    cv::Mat src, image;

    src = cv::imread(argv[1], 1);
    if (src.empty()) {
        cout << "Cannot load " << argv[1] << endl;
        return -1;
    }

    cv::cvtColor(src, image, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(image, image, cv::Size(5, 5), 0, 0);

    vector<cv::Vec3f> circles;
    cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT, 2, image.cols/4);

    for (size_t i = 0; i < circles.size(); ++i) {
        cv::circle(src,
            cv::Point(cvRound(circles[i][0]), cvRound(circles[i][1])),
            cvRound(circles[i][2]),
            cv::Scalar(0, 0, 255),
            2,
            cv::LINE_AA);
    }

    cv::imshow("Hough Circles", src);
    cv::waitKey(0);

    return 0;
}
