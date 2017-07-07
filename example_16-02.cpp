// Example 16-2. 2D Feature detectors and 2D Extra Features framework
//
// Note, while this code is free to use commercially, not all the algorithms are. For example
// sift is patented. If you are going to use this commercially, check out the non-free 
// algorithms and secure license to use them.
//

#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/ocl.hpp>

using std::cout;
using std::cerr;
using std::vector;
using std::string;

using cv::Mat;
using cv::Point2f;
using cv::KeyPoint;
using cv::Scalar;
using cv::Ptr;

using cv::FastFeatureDetector;
using cv::SimpleBlobDetector;

using cv::DMatch;
using cv::BFMatcher;
using cv::DrawMatchesFlags;
using cv::Feature2D;
using cv::ORB;
using cv::BRISK;
using cv::AKAZE;
using cv::KAZE;

using cv::xfeatures2d::BriefDescriptorExtractor;
using cv::xfeatures2d::SURF;
using cv::xfeatures2d::SIFT;
using cv::xfeatures2d::DAISY;
using cv::xfeatures2d::FREAK;

const double kDistanceCoef = 4.0;
const int kMaxMatchingSize = 50;

inline void detect_and_compute(string type, Mat& img, vector<KeyPoint>& kpts, Mat& desc) {
    if (type.find("fast") == 0) {
        type = type.substr(4);
        Ptr<FastFeatureDetector> detector = FastFeatureDetector::create(10, true);
        detector->detect(img, kpts);
    }
    if (type.find("blob") == 0) {
        type = type.substr(4);
        Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create();
        detector->detect(img, kpts);
    }
    if (type == "surf") {
        Ptr<Feature2D> surf = SURF::create(800.0);
        surf->detectAndCompute(img, Mat(), kpts, desc);
    }
    if (type == "sift") {
        Ptr<Feature2D> sift = SIFT::create();
        sift->detectAndCompute(img, Mat(), kpts, desc);
    }
    if (type == "orb") {
        Ptr<ORB> orb = ORB::create();
        orb->detectAndCompute(img, Mat(), kpts, desc);
    }
    if (type == "brisk") {
        Ptr<BRISK> brisk = BRISK::create();
        brisk->detectAndCompute(img, Mat(), kpts, desc);
    }
    if (type == "kaze") {
        Ptr<KAZE> kaze = KAZE::create();
        kaze->detectAndCompute(img, Mat(), kpts, desc);
    }
    if (type == "akaze") {
        Ptr<AKAZE> akaze = AKAZE::create();
        akaze->detectAndCompute(img, Mat(), kpts, desc);
    }
    if (type == "freak") {
        Ptr<FREAK> freak = FREAK::create();
        freak->compute(img, kpts, desc);
    }
    if (type == "daisy") {
        Ptr<DAISY> daisy = DAISY::create();
        daisy->compute(img, kpts, desc);
    }
    if (type == "brief") {
        Ptr<BriefDescriptorExtractor> brief = BriefDescriptorExtractor::create(64);
        brief->compute(img, kpts, desc);
    }
}

inline void match(string type, Mat& desc1, Mat& desc2, vector<DMatch>& matches) {
    matches.clear();
    if (type == "bf") {
        BFMatcher desc_matcher(cv::NORM_L2, true);
        desc_matcher.match(desc1, desc2, matches, Mat());
    }
    if (type == "knn") {
        BFMatcher desc_matcher(cv::NORM_L2, true);
        vector< vector<DMatch> > vmatches;
        desc_matcher.knnMatch(desc1, desc2, vmatches, 1);
        for (int i = 0; i < static_cast<int>(vmatches.size()); ++i) {
            if (!vmatches[i].size()) {
                continue;
            }
            matches.push_back(vmatches[i][0]);
        }
    }
    std::sort(matches.begin(), matches.end());
    while (matches.front().distance * kDistanceCoef < matches.back().distance) {
        matches.pop_back();
    }
    while (matches.size() > kMaxMatchingSize) {
        matches.pop_back();
    }
}

inline void findKeyPointsHomography(vector<KeyPoint>& kpts1, vector<KeyPoint>& kpts2,
        vector<DMatch>& matches, vector<char>& match_mask) {
    if (static_cast<int>(match_mask.size()) < 3) {
        return;
    }
    vector<Point2f> pts1;
    vector<Point2f> pts2;
    for (int i = 0; i < static_cast<int>(matches.size()); ++i) {
        pts1.push_back(kpts1[matches[i].queryIdx].pt);
        pts2.push_back(kpts2[matches[i].trainIdx].pt);
    }
    findHomography(pts1, pts2, cv::RANSAC, 4, match_mask);
}

int main(int argc, char** argv) {
    // Program expects at least four arguments:
    //   - descriptors type ("surf", "sift", "orb", "brisk",
    //          "kaze", "akaze", "freak", "daisy", "brief").
    //          For "brief", "freak" and "daisy" you also need a prefix
    //          that is either "blob" or "fast" (e.g. "fastbrief", "blobdaisy")
    //   - match algorithm ("bf", "knn")
    //   - path to the object image file
    //   - path to the scene image file
    //
    if (argc != 5) {
        cerr << "\nError: wrong (you had: " << argc << ") number of arguments (should be 5).\n";
        cerr    << "\nExample 16-2. 2D Feature detectors and 2D Extra Features framework\n\n"
                << "Use:\n" << argv[0] << " <descriptors_type> <matching_algirthm> "
                << "<path/image_file1> <path/image_file2>\n"
                << "To run this demo\n\n"
                << "Program expects at least four arguments:\n"
                << "  - descriptors type (\"surf\", \"sink\", \"orb\", \"brisk\",\n"
                << "       \"kaze\", \"akaze\", \"freak\", \"daisy\", \"brief\").\n"
                << "       For \"brief\", \"freak\" and \"daisy\" you also need a prefix\n"
                << "       that is either \"blob\" or \"fast\" (e.g. \"fastbrief\", "
                << "\"blobdaisy\")\n"
                << "  - match algorithm (\"bf\", \"knn\")\n"
                << "  - path to the object image file\n"
                << "  - path to the scene image file\n\n"
                << "Examples:\n"
                << argv[0] << " surf knn ../box.png ../box_in_scene.png\n"
                << argv[0] << " fastfreak bf ../box.png ../box_in_scene.png\n"
                << "\nNOTE: Not all of these methods are free, check licensing conditions!\n\n"
                << std::endl;
        exit(1);
    }

    string desc_type(argv[1]);
    string match_type(argv[2]);

    string img_file1(argv[3]);
    string img_file2(argv[4]);

    Mat img1 = cv::imread(img_file1, CV_LOAD_IMAGE_COLOR);
    Mat img2 = cv::imread(img_file2, CV_LOAD_IMAGE_COLOR);

    if (img1.channels() != 1) {
        cvtColor(img1, img1, cv::COLOR_RGB2GRAY);
    }

    if (img2.channels() != 1) {
        cvtColor(img2, img2, cv::COLOR_RGB2GRAY);
    }

    vector<KeyPoint> kpts1;
    vector<KeyPoint> kpts2;

    Mat desc1;
    Mat desc2;

    vector<DMatch> matches;

    detect_and_compute(desc_type, img1, kpts1, desc1);
    detect_and_compute(desc_type, img2, kpts2, desc2);

    match(match_type, desc1, desc2, matches);

    vector<char> match_mask(matches.size(), 1);
    findKeyPointsHomography(kpts1, kpts2, matches, match_mask);

    Mat res;
    cv::drawMatches(img1, kpts1, img2, kpts2, matches, res, Scalar::all(-1),
        Scalar::all(-1), match_mask, DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    cv::imshow("result", res);
    cv::waitKey(0);

    return 0;
}
