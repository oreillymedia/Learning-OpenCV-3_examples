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
cv::Mat drawRect;
int x_0 = -1;
int y_0 = -1; 
int x_1, y_1;
int drawr = 0;

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

    if (event == cv::EVENT_LBUTTONDOWN && drawr == 0) {
		if(x_0 < 0) {
			x_0 = x;
			y_0 = y;
			cv::ellipse(markers, cv::Point(x, y), cv::Size(1, 1),
            0, 0, 360, cv::GC_FGD, 3);
			cv::ellipse(drawRect, cv::Point(x, y), cv::Size(1, 1),
            0, 0, 360, cv::Scalar(0, 0, 255), 3);
            drawr = 1;
		} 
 
        cv::addWeighted(img,0.7,drawRect,0.3, 0, img_preview);
    
        cv::imshow("image", img_preview);
        return;
    }
    if( event == cv::EVENT_LBUTTONUP) {
		drawr = 2;
	}
    if(drawr == 1) { //Just moving
			drawRect.setTo(0);
			cv::rectangle(drawRect, cv::Point(x_0,y_0), cv::Point(x,y), cv::Scalar(0,0,255), -1);
 
			cv::addWeighted(img,0.7,drawRect,0.3, 0, img_preview);
			x_1 = x; y_1 = y;    
			cv::imshow("image", img_preview);
			return;
	}

    if (drawr == 2) {
        cv::Mat bg;
        cv::Mat fg;
        cv::rectangle(markers, cv::Point(x_0,y_0), cv::Point(x_1,y_1), cv::GC_PR_FGD, -1);
        cv::grabCut(img, markers, cv::Rect(0, 0, img.cols - 1, img.rows - 1),
            bg, fg, 5, cv::GC_EVAL);
        displayResult();
        return;
    }
}

void help(char** argv) {
        cout << "\nExample 12-3. Using GrabCut for background removal"
             << "\n- Use left mouse to drag a rectangle over the object"
             << "\n- On release of left mouse button, we will perform GrabCut"
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

    markers = cv::Mat(img.size(), CV_8UC1);
    markers.setTo(cv::GC_PR_BGD);

    img_preview = img.clone();
    drawRect = img.clone();

    finished = false;

    cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("image", onMouseClick, 0);

    cv::imshow("image", img_preview);

    cv::waitKey(0);

    return 0;
}
