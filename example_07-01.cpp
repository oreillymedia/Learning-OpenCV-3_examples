// alphablend <imageA> <image B> <x> <y> <width> <height> <alpha> <beta>
//
//#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {

  cv::RNG rng = cv::theRNG();
  cout << "An integer: "      << (int)rng   << endl;
  cout << "Another integer: " << int(rng)   << endl;
  cout << "A float: "         << (float)rng << endl;
  cout << "Another float: "   << float(rng) << endl;

  return 0;

}
