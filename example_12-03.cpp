// Example 12-3. Using GrabCut for background removal

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using std::cout;
using std::cerr;
using std::endl;

cv::Mat img_preview;
cv::Mat img;
cv::Mat markers;

bool finished;

void displayResult() {
    int rows = img.rows;
    int cols = img.cols;
    cv::Vec3b blackClr(0, 0, 0);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (markers.at<uchar>(i, j) != cv::GC_FGD && markers.at<uchar>(i, j) != cv::GC_PR_FGD) {
                img.at<cv::Vec3b>(i, j) = blackClr;
            }
        }
    }
    cv::imshow("image", img);
    finished = true;
}

static void onMouseClick(int event, int x, int y, int, void*) {
    if (finished) {
        return;
    }

    if (event == cv::EVENT_LBUTTONDOWN) {
        cv::ellipse(markers, cv::Point(x, y), cv::Size(1, 1),
            0, 0, 360, cv::GC_FGD, 3);
        cv::ellipse(img_preview, cv::Point(x, y), cv::Size(1, 1),
            0, 0, 360, cv::Scalar(0, 0, 255), 3);
        cv::imshow("image", img_preview);
        return;
    }

    if (event == cv::EVENT_RBUTTONDOWN) {
        cv::Mat bg;
        cv::Mat fg;
        cv::grabCut(img, markers, cv::Rect(0, 0, img.cols - 1, img.rows - 1),
            bg, fg, 5, cv::GC_EVAL);
        displayResult();
        return;
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "\nExample 12-3. Using GrabCut for background removal"
             << "\n- Use left click on the image to select foreground point"
             << "\n- Use right clock on the image to perform GrabCut"
             << "\n- Press any key to terminate program"
             << "\nUsage: "
             << argv[0] << " <path/imagename>\n" << endl;
        return -1;
    }

    img = cv::imread(std::string(argv[1]), CV_LOAD_IMAGE_COLOR);
    if (img.channels() != 3) {
        cerr << "Input image should have 3 channels" << endl;
        exit(1);
    }

    markers = cv::Mat(img.size(), CV_8UC1);
    markers.setTo(cv::GC_PR_BGD);

    img_preview = img.clone();

    finished = false;

    cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("image", onMouseClick, 0);

    cv::imshow("image", img_preview);

    cv::waitKey(0);

    return 0;
}
