// Example 7-1. Using the default random number generator to generate a pair of integers
// and a pair of floating-point numbers

//#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  cout << "\nExample 7-1. Using the default random number generator"
       << "\nto generate a pair of integers and a pair of"
       << "\n floating-point numbers"
       << "\n\nCall:\n" << argv[0] << "\n" << endl;

  cv::RNG rng = cv::theRNG();
  cout << "An integer: "      << (int)rng   << endl;
  cout << "Another integer: " << int(rng)   << endl;
  cout << "A float: "         << (float)rng << endl;
  cout << "Another float: "   << float(rng) << endl;

  return 0;

}
