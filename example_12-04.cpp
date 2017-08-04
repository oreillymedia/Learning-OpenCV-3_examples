// Example 12-4. Using Watershed for image segmentation

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
    cv::imshow("image", img);
    finished = true;
}

const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};

void fillMarker(int x, int y, int marker_id) {
    if (x < 0 || y < 0 || x >= markers.rows || y >= markers.cols) {
        return;
    }

    if (markers.at<int>(x, y) != -1) {
        return;
    }

    markers.at<int>(x, y) = marker_id;

    for (int dir = 0; dir < 4; ++dir) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];
        fillMarker(nx, ny, marker_id);
    }
}

static void onMouseClick(int event, int x, int y, int, void*) {
    if (finished) {
        return;
    }

    if (event == cv::EVENT_LBUTTONDOWN) {
        cv::ellipse(markers, cv::Point(x, y), cv::Size(1, 1),
            0, 0, 360, -1, 3);
        cv::ellipse(img_preview, cv::Point(x, y), cv::Size(1, 1),
            0, 0, 360, cv::Scalar(0, 0, 255), 3);
        cv::imshow("image", img_preview);
        return;
    }

    if (event == cv::EVENT_RBUTTONDOWN) {
        int marker_id = 0;
        for (int x = 0; x < markers.rows; ++x) {
            for (int y = 0; y < markers.cols; ++y) {
                if (markers.at<int>(x, y) == -1) {
                    ++marker_id;
                    fillMarker(x, y, marker_id);
                }
            }
        }
        cv::watershed(img, markers);
        cv::Vec3b borderColor(0, 0, 255);
        for (int x = 0; x < img.rows; ++x) {
            for (int y = 0; y < img.cols; ++y) {
                if (markers.at<int>(x, y) == -1) {
                    img.at<cv::Vec3b>(x, y) = borderColor;
                    continue;
                }
                for (int dir = 0; dir < 4; ++dir) {
                    int nx = x + dx[dir];
                    int ny = y + dy[dir];
                    if (nx < 0 || ny < 0 || nx >= img.rows || ny >= img.cols) {
                        continue;
                    }
                    if (markers.at<int>(x, y) != markers.at<int>(nx, ny)) {
                        img.at<cv::Vec3b>(x, y) = borderColor;
                    }
                }
            }
        }

        displayResult();
        return;
    }
}

void help(char** argv) {
       cout << "\nExample 12-4. Using Watershed for image segmentation"
             << "\n- Use left click on the image to place marker for the new segment"
             << "\n- Use right clock on the image to perform Watershed"
             << "\n- Press any key to terminate program"
             << "\nUsage: "
             << argv[0] << " <path/imagename>\n" 
             << "\nExample:\n" << argv[0] << " ../stuff.jpg\n" << endl;	
}

int main(int argc, char** argv) {
	help(argv);
    if (argc != 2) {
         return -1;
    }

    img = cv::imread(std::string(argv[1]), CV_LOAD_IMAGE_COLOR);
    if (img.channels() != 3) {
        cerr << "Input image should have 3 channels" << endl;
        exit(1);
    }

    markers = cv::Mat(img.size(), CV_32SC1);
    markers.setTo(0);

    img_preview = img.clone();

    finished = false;

    cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("image", onMouseClick, 0);

    cv::imshow("image", img_preview);

    cv::waitKey(0);

    return 0;
}
