// Example 17-2. Farneback optical flow example code

#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using std::cout;
using std::cerr;
using std::string;

// Argument values for calcOpticalFlowFarneback
//
const double pyr_scale = 0.85;  // Scale between pyramid levels (< 1.0)
const int levels = 7;           // Number of pyramid levels
const int winsize = 13;         // Size of window for pre-smoothing pass
const int iterations = 10;      // Iterations for each pyramid level
const int poly_n = 5;           // Area over which polynomial will be fit
const double poly_sigma = 1.1;  // Width of fit polygon

// Function returns cv::Mat object with optical flow visualization
//
cv::Mat get_optflow_image(cv::Mat& optflow, cv::Mat& img) {
    cv::Scalar arrow_color(0, 0, 255);
    cv::Mat res = img.clone();
    res /= 2;  // making image darker
    int rows = res.rows;
    int cols = res.cols;
    const int step = 12;
    for (int x = (step >> 1); x < rows; x += step)
        for (int y = (step >> 1); y < cols; y += step) {
            float vx = optflow.at<cv::Vec2f>(x, y)[0];
            float vy = optflow.at<cv::Vec2f>(x, y)[1];
            cv::Point pt1(y, x);
            cv::Point pt2(y + vx, x + vy);
            cv::arrowedLine(res, pt1, pt2, arrow_color, 1);
        }
    return res;
}

int main(int argc, char** argv) {
    // Program expects at least one argument that is path to video file
    //
    if (argc < 2) {
        cerr 	<< "\nExample 17-2: Farnback optical flow example\n"
				<< "Use:\n" << argv[0] << " <path/video_file>\n"
				<< "Example:\n" << argv[0] << " ../test.avi\n"
				<< std::endl;
        exit(1);
    }

    string file_name = string(argv[1]);
    cv::VideoCapture capture(file_name);

    if (!capture.isOpened()) {
        cerr << "Cannot open file \"" << file_name << "\"\n";
        exit(-1);
    }

    cv::Mat optflow;  // optical flow result
    cv::Mat optflow_image;  // optical flow visualization
    cv::Mat prev_frame;  // previous frame grayscale image
    cv::Mat frame;  // current frame grayscale image
    cv::Mat colored_frame;  // current frame RGB-image

    cv::namedWindow("video");

    // User can terminate program with hitting ESC
    //
    while ((cv::waitKey(10) & 255) != 27) {
        capture >> colored_frame;
        if (!colored_frame.rows || !colored_frame.cols) {
            break;
        }
        if (colored_frame.type() == CV_8UC3) {
            cvtColor(colored_frame, frame, CV_BGR2GRAY);
        }
        if (prev_frame.rows) {
            calcOpticalFlowFarneback(prev_frame, frame, optflow, pyr_scale, levels, winsize,
                iterations, poly_n, poly_sigma, cv::OPTFLOW_FARNEBACK_GAUSSIAN);
            optflow_image = get_optflow_image(optflow, colored_frame);
            cv::imshow("video", optflow_image);
        }
        prev_frame = frame.clone();
    }
    cv::destroyAllWindows();

    return 0;
}
