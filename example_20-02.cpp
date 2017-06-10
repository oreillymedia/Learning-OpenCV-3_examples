//Example 20-02. Using the Mahalanobis distance for classification
#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>
#include <algorithm>

using namespace std;

cv::Mat img(500, 500, CV_8UC3, cv::Scalar::all(0));
const int CLUSTER_COUNT = 5;
const int SAMPLE_COUNT = 500;
const cv::Scalar colorTab[] = {
    cv::Scalar( 0, 0, 255 ),
    cv::Scalar( 0, 255, 0 ),
    cv::Scalar( 255, 0, 0 ),
    cv::Scalar( 255, 0, 255 ),
    cv::Scalar( 0, 255, 255 )
};

void generatePoints(int clusterCount, int sampleCount, cv::Mat &points) {
    cv::RNG rng(time(NULL));
    points.create(sampleCount, 1, CV_32FC2);

    clusterCount = MIN(clusterCount, sampleCount);
    /* generate random sample from multigaussian distribution */
    for(int k = 0; k < clusterCount; k++) {
        cv::Point center;
        center.x = rng.uniform(0, img.cols);
        center.y = rng.uniform(0, img.rows);
        cv::Mat pointChunk = points.rowRange(
                    k*sampleCount/clusterCount,
                    k == clusterCount - 1 ? sampleCount : (k+1)*sampleCount/clusterCount
                                            );
        rng.fill(
                    pointChunk,
                    cv::RNG::NORMAL,
                    cv::Scalar(center.x, center.y),
                    cv::Scalar(img.cols*0.05, img.rows*0.05)
                    );
    }
    randShuffle(points, 1, &rng);
}

static void help(char* argv[]) {
    cout << "\nThis program demonstrates using the Mahalanobis distance for classification.\n"
            " It generates an image with random points, uses kmeans clustering.\n"
            " And then uses the Mahalanobis distance for classification.\n"
            "Usage:\n"
         << argv[0] << "\n" << endl;
}
int main(int argc, char** argv) {
    cv::Mat points;
    generatePoints(CLUSTER_COUNT, SAMPLE_COUNT, points);

    cv::Mat labels;
    kmeans(points, CLUSTER_COUNT, labels,
           cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT,
                            10, 1.0),
           3,
           cv::KMEANS_PP_CENTERS
           );

    vector<cv::Mat> clusters(CLUSTER_COUNT);

    for(int i = 0; i < SAMPLE_COUNT; i++) {
        int clusterIdx = labels.at<int>(i);

        cv::Point ipt = points.at<cv::Point2f>(i);

        cv::Mat sample(1, 2, CV_32FC1);
        sample.at<float>(0, 0) = ipt.x;
        sample.at<float>(0, 1) = ipt.y;
        clusters[clusterIdx].push_back(sample);
        cv::circle(img, ipt, 2, colorTab[clusterIdx], cv::FILLED, cv::LINE_AA);
    }
    cv::namedWindow("Example 20-02");
    cv::imshow("Example 20-02", img);

    vector<cv::Mat> covarMats(CLUSTER_COUNT);
    vector<cv::Mat> means(CLUSTER_COUNT);
    for(int i = 0; i < CLUSTER_COUNT; i++) {
        cv::calcCovarMatrix(clusters[i], covarMats[i], means[i],
                            CV_COVAR_NORMAL | CV_COVAR_ROWS, 5);

    }

    cout << "Press any button to classify the next point!\n"
         << "Press ESC to exit." << endl;

    cv::RNG rng(time(NULL));
    for(;;) {
        char key = (char)cv::waitKey();
        if( key == 27 ) break;

        cv::Mat newPoint(1, 2, CV_32FC1);
        newPoint.at<float>(0, 0) = rng.uniform(0, img.cols);
        newPoint.at<float>(0, 1) = rng.uniform(0, img.rows);
        vector<float> mahalanobisDistance(CLUSTER_COUNT);

        for(int i = 0; i < CLUSTER_COUNT; i++) {
             mahalanobisDistance[i] = cv::Mahalanobis(newPoint, means[i],
                                                      covarMats[i]);
        }
        int clusterIdx =  std::distance( mahalanobisDistance.begin(),
                                        min_element(mahalanobisDistance.begin(),
                                                    mahalanobisDistance.end()));

        cv::circle(img, newPoint.at<cv::Point2f>(0), 5, colorTab[clusterIdx],
                    cv::FILLED, cv::LINE_AA);
        cv::imshow("Example 20-02", img);
    }

    cv::destroyAllWindows();
    return 0;
}
