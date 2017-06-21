// Example 19-2. Computing the fundamental matrix using RANSAC
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
void help(char *argv[]) { 
	cout 	<< "\nExample 19-2, Computing the fundamental matrix using RANSAC relating 2 images. Show the camera a checkerboard "
			<< "\nCall"
			<< "\n./example_19-2 <1:board_w> <2:board_h> <3:# of boards> <4:delay capture this many ms between frames> <5:scale the images 0-1>"
			<< "\n\nExample call:"
			<< "\n./example_19-2 9 6 20 500 0.5"
			<< "\n\n -- use the checkerboard9x6.png provided"
			<< "\n" 
			<< endl;
}


// args: [board_w] [board_h] [number_of_boards] [delay]? [scale]?
//
int main(int argc, char *argv[]) {
  int n_boards = 0;
  float image_sf = 0.5f;
  float delay = 1.f;
  int board_w = 0;
  int board_h = 0;

  // Will be set by input list
  if (argc != 6) {
    cout << "\nERROR: Wrong number of input parameters, need 5, got " << argc - 1 << "\n";
    help(argv);
    return -1;
  }
  board_w = atoi(argv[1]);
  board_h = atoi(argv[2]);
  n_boards = atoi(argv[3]);
  delay = atof(argv[4]);
  image_sf = atof(argv[5]);
  int board_n = board_w * board_h;
  cv::Size board_sz = cv::Size(board_w, board_h);
  cv::VideoCapture capture(0);

  if (!capture.isOpened()) {
    cout << "\nCouldn't open the camera\n";
    help(argv);
    return -1;
  }
  // Allocate Storage
  //
  vector<vector<cv::Point2f> > image_points;
  vector<vector<cv::Point3f> > object_points;
  // Capture corner views; loop until we've got n_boards number of
  // successful captures (meaning: all corners on each
  // board are found).
  //
  double last_captured_timestamp = 0;
  cv::Size image_size;
  while (image_points.size() < (size_t)n_boards) {
    cv::Mat image0, image;
    capture >> image0;
    image_size = image0.size();
    resize(image0, image, cv::Size(), image_sf, image_sf, cv::INTER_LINEAR);
    // Find the board
    //
    vector<cv::Point2f> corners;
    bool found = cv::findChessboardCorners(image, board_sz, corners);
    // Draw it
    //
    cv::drawChessboardCorners(image, board_sz, corners, found);
    // If we got a good board, add it to our data
    //
    double timestamp = (double)clock() / CLOCKS_PER_SEC;
    if (found && timestamp - last_captured_timestamp > 1) {
      last_captured_timestamp = timestamp;
      image ^= cv::Scalar::all(255);

      cv::Mat mcorners(corners);
      // do not copy the data
      mcorners *= (1. / image_sf);
      // scale corner coordinates
      image_points.push_back(corners);
      object_points.push_back(vector<cv::Point3f>());
      vector<cv::Point3f> &opts = object_points.back();
      opts.resize(board_n);
      for (int j = 0; j < board_n; j++) {
        opts[j] = cv::Point3f((float)(j / board_w), (float)(j % board_w), 0.f);
      }
      cout << "Collected our " << (int)image_points.size() << " of " << n_boards
           << " needed chessboard images\n" << endl;
    }
    // in color if we did collect the image
    //
    cv::imshow("Calibration", image);
    if ((cv::waitKey(30) & 255) == 27)
      return -1;
  }
  // end collection while() loop.
  cv::destroyWindow("Calibration");
  cout << "\n\n*** CALIBRATING THE CAMERA...\n" << endl;
  // Calibrate the camera!
  //
  cv::Mat intrinsic_matrix, distortion_coeffs;
  double err = cv::calibrateCamera(
      object_points,     // Vector of vectors of points
                         // from the calibration pattern
      image_points,      // Vector of vectors of projected
                         // locations (on images)
      image_size,        // Size of images used
      intrinsic_matrix,  // Output camera matrix
      distortion_coeffs, // Output distortion coefficients
      cv::noArray(),     // We'll pass on the rotation vectors...
      cv::noArray(),     // ...and the translation vectors
      cv::CALIB_ZERO_TANGENT_DIST | cv::CALIB_FIX_PRINCIPAL_POINT);

  // Save the intrinsics and distortions
  cout << " *** DONE!\n\nReprojection error is " << err
       << "\nStoring Intrinsics.xml and Distortions.xml files\n\n";
  cv::FileStorage fs("intrinsics.xml", cv::FileStorage::WRITE);
  fs << "image_width" << image_size.width << "image_height" << image_size.height
     << "camera_matrix" << intrinsic_matrix << "distortion_coefficients"
     << distortion_coeffs;
  fs.release();

  // Example of loading these matrices back in:
  //
  fs.open("intrinsics.xml", cv::FileStorage::READ);
  cout << "\nimage width: " << (int)fs["image_width"];
  cout << "\nimage height: " << (int)fs["image_height"];
  cv::Mat intrinsic_matrix_loaded, distortion_coeffs_loaded;
  fs["camera_matrix"] >> intrinsic_matrix_loaded;
  fs["distortion_coefficients"] >> distortion_coeffs_loaded;
  cout << "\nintrinsic matrix:" << intrinsic_matrix_loaded;
  cout << "\ndistortion coefficients: " << distortion_coeffs_loaded << endl;

  // Compute Fundamental Matrix Between the first
  // and the second frames:
  //
  cv::undistortPoints(
      image_points[0],   // Observed point coordinates (from frame 0)
      image_points[0],   // undistorted coordinates (in this case,
                         // the same array as above)
      intrinsic_matrix,  // Intrinsics, from cv::calibrateCamera()
      distortion_coeffs, // Distortion coefficients, also
                         // from cv::calibrateCamera()
      cv::Mat(),         // Rectification transformation (but
                         // here, we don't need this)
      intrinsic_matrix   // New camera matrix
      );

  cv::undistortPoints(
      image_points[1],   // Observed point coordinates (from frame 1)
      image_points[1],   // undistorted coordinates (in this case,
                         // the same array as above)
      intrinsic_matrix,  // Intrinsics, from cv::calibrateCamera()
      distortion_coeffs, // Distortion coefficients, also
                         // from cv::calibrateCamera()
      cv::Mat(),         // Rectification transformation (but
                         // here, we don't need this)
      intrinsic_matrix   // New camera matrix
      );

  // Since all the found chessboard corners are inliers, i.e., they
  // must satisfy epipolar constraints, here we are using the
  // fastest, and the most accurate (in this case) 8-point algorithm.
  //
  cv::Mat F = cv::findFundamentalMat( // Return computed matrix
      image_points[0],                // Points from frame 0
      image_points[1],                // Points from frame 1
      cv::FM_8POINT                   // Use the 8-point algorithm
      );
  cout << "Fundamental matrix: " << F << endl;

  // Build the undistort map which we will use for all
  // subsequent frames.
  //
  cv::Mat map1, map2;
  cv::initUndistortRectifyMap(
      intrinsic_matrix_loaded,  // Our camera matrix
      distortion_coeffs_loaded, // Our distortion coefficients
      cv::Mat(),                // (Optional) Rectification, don't
                                // need.
      intrinsic_matrix_loaded,  // "New" matrix, here it's the same
                                // as the first argument.
      image_size,               // Size of undistorted image we want
      CV_16SC2,                 // Specifies the format of map to use
      map1,                     // Integerized coordinates
      map2                      // Fixed-point offsets for
                                // elements of map1
      );

  // Just run the camera to the screen, now showing the raw and
  // the undistorted image.
  //
  for (;;) {
    cv::Mat image, image0;
    capture >> image0;
    if (image0.empty())
      break;
    cv::remap(image0, // Input image
              image,  // Output image
              map1,   // Integer part of map
              map2,   // Fixed point part of map
              cv::INTER_LINEAR, cv::BORDER_CONSTANT,
              cv::Scalar() // Set border values to black
              );
    cv::imshow("Undistorted", image);
    if ((cv::waitKey(30) & 255) == 27)
      break;
  }
  return 1;
}
