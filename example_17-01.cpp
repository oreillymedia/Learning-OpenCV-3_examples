// Example 17-1. Kalman filter example code

#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <opencv2/opencv.hpp>

using std::cout;
using std::endl;

#define phi2xy(mat) \
    cv::Point(cvRound(img.cols / 2 + img.cols / 3 * cos(mat.at<float>(0))), \
              cvRound(img.rows / 2 - img.cols / 3 * sin(mat.at<float>(0))))


void help(char** argv ) {
    cout << "\n"
         << "Example 17-1: code for using cv::KalmanFilter\n"
         << argv[0] << "\n\n"
         << "For example:\n"
         << argv[0] <<"\n\n"
         << "Esc to quit\n"
         << endl;
}

int main(int argc, char** argv) {
    help(argv);

    // Initialize, create Kalman filter object, window, random number
    // generator etc.
    //
    cv::Mat img(500, 500, CV_8UC3);
    cv::KalmanFilter kalman(2, 1, 0);

    // state is (phi, delta_phi) - angle and angular velocity
    // Initialize with random guess.
    //
    cv::Mat x_k(2, 1, CV_32F);
    randn(x_k, 0.0, 0.1);

    // process noise
    //
    cv::Mat w_k(2, 1, CV_32F);

    // measurements, only one parameter for angle
    //
    cv::Mat z_k = cv::Mat::zeros(1, 1, CV_32F);

    // Transition matrix 'F' describes relationship between
    // model parameters at step k and at step k+1 (this is
    // the "dynamics" in our model.
    //
    float F[] = {1, 1, 0, 1};
    kalman.transitionMatrix = cv::Mat(2, 2, CV_32F, F).clone();

    // Initialize other Kalman filter parameters.
    //
    cv::setIdentity(kalman.measurementMatrix, cv::Scalar(1));
    cv::setIdentity(kalman.processNoiseCov, cv::Scalar(1e-5));
    cv::setIdentity(kalman.measurementNoiseCov, cv::Scalar(1e-1));
    cv::setIdentity(kalman.errorCovPost, cv::Scalar(1));

    // choose random initial state
    //
    randn(kalman.statePost, 0.0, 0.1);

    for (;;) {
        // predict point position
        //
        cv::Mat y_k = kalman.predict();

        // generate measurement (z_k)
        //
        cv::randn(z_k, 0.0,
            sqrt(static_cast<double>(kalman.measurementNoiseCov.at<float>(0, 0))));
        z_k = kalman.measurementMatrix*x_k + z_k;

        // plot points (e.g., convert
        //
        img = cv::Scalar::all(0);
        cv::circle(img, phi2xy(z_k), 4, cv::Scalar(128, 255, 255));  // observed
        cv::circle(img, phi2xy(y_k), 4, cv::Scalar(255, 255, 255), 2);  // predicted
        cv::circle(img, phi2xy(x_k), 4, cv::Scalar(0, 0, 255));  // actual to
                                                                 // planar co-ordinates and draw

        cv::imshow("Kalman", img);

        // adjust Kalman filter state
        //
        kalman.correct(z_k);

        // Apply the transition matrix 'F' (e.g., step time forward)
        // and also apply the "process" noise w_k
        //
        cv::randn(w_k, 0.0, sqrt(static_cast<double>(kalman.processNoiseCov.at<float>(0, 0))));
        x_k = kalman.transitionMatrix*x_k + w_k;

        // exit if user hits 'Esc'
        if ((cv::waitKey(100) & 255) == 27) {
            break;
        }
    }

    return 0;
}
