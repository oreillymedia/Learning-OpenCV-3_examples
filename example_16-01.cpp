// Pyramid L-K optical flow example
//
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

static const int MAX_CORNERS = 1000;

void help( char** argv ) {
  cout << "Call: " <<argv[0] <<" [image1] [image2]" << endl;
  cout << "Demonstrates Pyramid Lucas-Kanade optical flow." << endl;
}

int main(int argc, char** argv) {

  if( argc != 3 ) { help( argv ); exit( -1 ); }

  // Initialize, load two images from the file system, and
  // allocate the images and other structures we will need for
  // results.
  //
  cv::Mat imgA = cv::imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
  cv::Mat imgB = cv::imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE );
  cv::Size img_sz = imgA.size();

  int win_size = 10;
  cv::Mat imgC = cv::imread( argv[2], CV_LOAD_IMAGE_UNCHANGED );

  // The first thing we need to do is get the features
  // we want to track.
  //
  vector< cv::Point2f > cornersA, cornersB;
  const int MAX_CORNERS = 500;
  cv::goodFeaturesToTrack(
    imgA,                         // Image to track
    cornersA,                     // Vector of detected corners (output)
    MAX_CORNERS,                  // Keep up to this many corners
    0.01,                         // Quality level (percent of maximum)
    5,                            // Min distance between corners
    cv::noArray(),                // Mask
    3,                            // Block size
    false,                        // true: Harris, false: Shi-Tomasi
    0.04                          // method specific parameter
  );

  cv::cornerSubPix(
    imgA,                         // Input image
    cornersA,                     // Vector of corners (input and output)
    cv::Size(win_size, win_size), // Half side length of search window
    cv::Size(-1,-1),              // Half side length of dead zone (-1=none)
    cv::TermCriteria(
      cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
      20,                         // Maximum number of iterations
      0.03                        // Minimum change per iteration
    )
  );

  // Call the Lucas Kanade algorithm
  //
  vector<uchar> features_found;
  cv::calcOpticalFlowPyrLK(
    imgA,                         // Previous image
    imgB,                         // Next image
    cornersA,                     // Previous set of corners (from imgA)
    cornersB,                     // Next set of corners (from imgB)
    features_found,               // Output vector, each is 1 for tracked
    cv::noArray(),                // Output vector, lists errors (optional)
    cv::Size( win_size*2+1, win_size*2+1 ), // Search window size
    5,                            // Maximum pyramid level to construct
    cv::TermCriteria(
      cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
      20,                         // Maximum number of iterations
      0.3                         // Minimum change per iteration
    )
  );

  // Now make some image of what we are looking at:
  // Note that if you want to track cornersB further, i.e.
  // pass them as input to the next calcOpticalFlowPyrLK,
  // you would need to "compress" the vector, i.e., exclude points for which
  // features_found[i] == false.
  for( int i = 0; i < (int)cornersA.size(); i++ ) {
    if( !features_found[i] )
      continue;
    line(
      imgC,                        // Draw onto this image
      cornersA[i],                 // Starting here
      cornersB[i],                 // Ending here
      cv::Scalar(0,255,0),         // This color
      2,                           // This many pixels wide
      cv::LINE_AA                  // Draw line in this style
    );
  }

  cv::imshow( "ImageA", imgA );
  cv::imshow( "ImageB", imgB );
  cv::imshow( "LK Optical Flow Example", imgC );
  cv::waitKey(0);

  return 0;
}
