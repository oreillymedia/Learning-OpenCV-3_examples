// Example 14-4. Using the shape context distance extractor

#include <algorithm>
#include <iostream>
#include <string>

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

static vector<Point> sampleContour( const Mat& image, int n=300 ) {

  vector<vector<Point> > _contours;
  vector<Point> all_points;
  findContours(image, _contours, RETR_LIST, CHAIN_APPROX_NONE);
  for (size_t i=0; i <_contours.size(); i++) {
    for (size_t j=0; j <_contours[i].size(); j++)
      all_points.push_back( _contours[i][j] );

    // If too little points, replicate them
    //
    int dummy=0;
    for (int add=(int)all_points.size(); add<n; add++)
      all_points.push_back(all_points[dummy++]);

    // Sample uniformly
    random_shuffle(all_points.begin(), all_points.end());
    vector<Point> sampled;
    for (int i=0; i<n; i++)
      sampled.push_back(all_points[i]);
    return sampled;
  }
}

int main(int argc, char** argv) {

  string path = "../data/shape_sample/";
  int indexQuery = 1;

  Ptr<ShapeContextDistanceExtractor> mysc = createShapeContextDistanceExtractor();

  Size sz2Sh(300,300);
  Mat img1=imread(argv[1], IMREAD_GRAYSCALE);
  Mat img2=imread(argv[2], IMREAD_GRAYSCALE);
  vector<Point> c1 = sampleContour(img1);
  vector<Point> c2 = sampleContour(img2);
  float dis = mysc->computeDistance( c1, c2 );
  cout << "shape context distance between " <<
    argv[1] << " and " << argv[2] << " is: " << dis << endl;

  return 0;

}
