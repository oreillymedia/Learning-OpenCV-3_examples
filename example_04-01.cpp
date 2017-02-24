#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

// Summation of a multidimensional array, done plane by plane
//
int main( int argc, char** argv ) {

  const int n_mat_size = 5;
  const int n_mat_sz[] = { n_mat_size, n_mat_size, n_mat_size };
  cv::Mat n_mat( 3, n_mat_sz, CV_32FC1 );

  cv::RNG rng;
  rng.fill( n_mat, cv::RNG::UNIFORM, 0.f, 1.f );

  const cv::Mat* arrays[] = { &n_mat, 0 };
  cv::Mat my_planes[1];
  cv::NAryMatIterator it( arrays, my_planes );

  // On each iteration, it.planes[i] will be the current plane of the
  // i-th array from 'arrays'.
  //
  float s = 0.f; // Total sum over all planes
  int   n = 0;   // Total number of planes
  for (int p = 0; p < it.nplanes; p++, ++it) {
    s += cv::sum(it.planes[0])[0];
    n++;
  }

  cout <<"Total across entire volume: " <<s <<endl;

}
