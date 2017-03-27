// alphablend <imageA> <image B> <x> <y> <width> <height> <alpha> <beta>
//
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char** argv) {

  cv::FileStorage fs2("test.yml", cv::FileStorage::READ);

  // first method: use (type) operator on FileNode.
  //
  int frameCount = (int)fs2["frameCount"];

  // second method: use cv::FileNode::operator >>
  //
  std::string date;
  fs2["calibrationDate"] >> date;

  cv::Mat cameraMatrix2, distCoeffs2;
  fs2["cameraMatrix"] >> cameraMatrix2;
  fs2["distCoeffs"] >> distCoeffs2;

  cout << "frameCount: "        << frameCount    << endl
       << "calibration date: "  << date          << endl
       << "camera matrix: "     << cameraMatrix2 << endl
       << "distortion coeffs: " << distCoeffs2   << endl;

  cv::FileNode features = fs2["features"];
  cv::FileNodeIterator it = features.begin(), it_end = features.end();
  int idx = 0;
  std::vector<uchar> lbpval;

  // iterate through a sequence using FileNodeIterator
  for( ; it != it_end; ++it, idx++ ) {

    cout << "feature #" << idx << ": ";
    cout << "x="   << (int)(*it)["x"]
         << ", y=" << (int)(*it)["y"]
         << ", lbp: (";

    // ( Note: easily read numerical arrays using FileNode >> std::vector. )
    //
    (*it)["lbp"] >> lbpval;
    for( int i = 0; i < (int)lbpval.size(); i++ )
      cout << " " << (int)lbpval[i];
    cout << ")" << endl;

  }

  fs2.release();

}
