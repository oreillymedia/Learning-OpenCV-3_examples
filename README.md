# Learning OpenCV 3

## INTRO

This is the example code that accompanies Learning OpenCV 3 by Adrian Kaehler and Gary Bradski ([9781491937990](http:*shop.oreilly.com/product/0636920044765.do)).

Click the Download Zip button to the right to download example code.

Visit the catalog page [here](http:*shop.oreilly.com/product/0636920044765.do).

See an error? Report it [here](http:*oreilly.com/catalog/errata.csp?isbn=0636920044765), or simply fork and send us a pull request


## NOTES

For default suggestions of how the run the code, it assumes you put your build directory under `Learning-OpenCV-3_examples` directory. 

Thus, from the `Learning-OpenCV-3_examples` directory:

```	
  mkdir build
  cd build
  cmake ..
  make -j
```

#### Docker
For your interest, included here is an Ubuntu _Docker_ file that
* Shares a directory with the host operating system
* Shares the first camera between both systems
* Loads Ubuntu 16.04 
* Loads all dependencies for OpenCV 3.2 and opencv_contrib
* Loads and builds OpenCV 3.2 and opencv_contrib into a build directory 
  * executable files end up in `opencv-3.2.0/build/bin`
* Next, it `git clones` the code (and Docker file) for Learning OpenCV 3 and builds it
  * executable files end up in `Learning_OpenCV-3_examples/build`
* To get to the top level directory, just type: `cd`


## CONTENTS:

### SPECIAL FILES:

* README.md       -- this readme file
* Dockerfile      -- complete self contained opencv environment using Ubuntu 16-04
* CMakeLists.txt  -- how to buld everything here 

### EXERCISES:

* Exercises at end of Chapter 5
* Exercises at end of Chapter 7
* Exercises_8_1.cpp Exercises at end of Chapter 8
* Exercises_9_1-2-10-11-12-15-16.cpp Exercises at end of Chapter 8
* Exercises_9_4.cpp Exercises at end of Chapter 9
* Exercises_9_5.cpp Exercises at end of Chapter 9
* Exercises at end of Chapter 11
* Exercises_13_1-2-11.cpp	Exercises for Chapter 13
* Exercises_13_9.cpp	

### EXAMPLES:

* Example 2-1. A simple OpenCV program that loads an image from disk and displays it
* Example 2-2. Same as Example 2-1 but employing the “using namespace” directive
* Example 2-3. A simple OpenCV program for playing a video file from disk
* Example 2-4. Adding a trackbar slider to the basic viewer window for moving around
* Example 2-5. Loading and then smoothing an image before it is displayed on the screen
* Example 2-6. Using cv::pyrDown() to create a new image that is half the width and
* Example 2-7. The Canny edge detector writes its output to a single-channel (grayscale) image
* Example 2-8. Combining the pyramid down operator (twice) and the Canny
* Example 2-9. Getting and setting pixels in Example 2-8
* Example 2-10. The same object can load videos from a camera or a file
* Example 2-11. A complete program to read in a color video and write out the log-polar-
* Example 4-1. Summation of a multidimensional array, done plane by plane
* Example 4-2. Summation of two arrays using the N-ary operator
* Example 4-3. Printing all of the nonzero elements of a sparse array
* Example 4-4. A better way to print a matrix
* Example 5-1. Complete program to alpha-blend the ROI starting at (0,0) in src2 with the ROI starting at (x,y) in src1
* Example 7-1. Using the default random number generator to generate a pair of integers
* Example 8-1. Unpacking a four-character code to identify a video codec
* Example 8-2. Using cv::FileStorage to create a .yml data file
* Example 8-3. Using cv::FileStorage to read a .yml file
* Example 9-1. Creating a window and displaying an image in that window
* Example 9-2. Toy program for using a mouse to draw boxes on the screen
* Example 9-3. Using a trackbar to create a “switch” tha t the user can turn on and off;
* Example 9-4. Slightly modified code from the OpenCV documentation that draws a
* Example 9-5. An example program ch4_qt.cpp, which takes a single argument
* Example 9-6. The QMoviePlayer object header file QMoviePlayer.hpp
* Example 9-7. The QMoviePlayer object source file: QMoviePlayer.cpp
* Example 9-8. An example program which takes a single argument
* Example 9-9. The WxMoviePlayer object header file WxMoviePlayer.hpp
* Example 9-10. The WxMoviePlayer object source file WxMoviePlayer.cpp
* Example 9-11. An example header file for our custom View class
* Example 10-1. Using cv::threshold() to sum three channels of an image
* Example 10-2. Alternative method to combine and threshold image planes
* Example 10-3. Threshold versus adaptive threshold
* Example 11-1. An affine transformation.
* Example 11-2. Code for perspective transformation
* Example 11-3. Log-polar transform example
* Example 12-1. Using cv::dft() and cv::idft() to accelerate the computation of
* Example 12-2. Using cv::HoughCircles() to return a sequence of circles found in a
* **EXTRA** Example 12-3. Using GrabCut for background removal
* **EXTRA** Example 12-4. Using GrabCut for background removal
* Example 13-1. Histogram computation and display
* Example 13-2. Creating signatures from histograms for EMD; note that this code is the
* Example 13-3. Template matching
* Example 14-1. Finding contours based on a trackbar’s location; the contours are
* Example 14-2. Finding and drawing contours on an input image
* Example 14-3. Drawing labeled connected components
* Example 14-4. Using the shape context distance extractor
* Example 15-1. Reading out the RGB values of all pixels in one row of a video and
* Example 15-2. Learning a background model to identify foreground pixels
* Example 15-3. Computing the on and off-diagonal elements of a variance/covariance model
* Example 15-4. Codebook algorithm implementation
* Example 15-5. Cleanup using connected components
* **EXTRA** Example 15-6, using OpenCV's background subtractor class.  Modified by Gary Bradski, 6/4/2017
* Example 16-1. Pyramid L-K optical flow
* **EXTRA** Example 16-2. 2D Feature detectors and 2D Extra Features framework
* Example 17-1. Kalman filter example code
* Example 17-2. Farneback optical flow example code
* Example 18-1. Reading a chessboard’s width and height, reading them and calibrating 
* **EXTRA** Example 18-1. From disk. Reading a chessboard’s width and height, reading them and calibrating 
* Example 19-1. Bird’s - eye view
* Example 19-2. Computing the fundamental matrix using RANSAC
* Example 19-3. Stereo calibration, rectification, and correspondence
* Example 19-4. Two-dimensional line fitting
* Example 20-01. Using K-means
* Example 20-02. Using the Mahalanobis distance for classification
* Example 21-1. Creating and training a decision tree
* Example 22-1. Detecting and drawing faces

### IMAGES:

* box.png
* box_in_scene.png
* checkerboard9x6.png
* example_16-01-imgA.png
* example_16-01-imgB.png
* faces.png
* BlueCup.jpg
* HandIndoorColor.jpg
* HandOutdoorColor.jpg
* HandOutdoorSunColor.jpg
* adrian.jpg
* faceScene.jpg
* faceTemplate.jpg
* fruits.jpg
* stuff.jpg

### MOVIES:

* test.avi
* tree.avi

### CLASSIFIERS:

* haarcascade_frontalcatface.xml           #Cat faces!
* haarcascade_frontalcatface_extended.xml
* haarcascade_frontalface_alt.xml

### DIRECTORIES:

* birdseye     -- where the images are of checkerboards on the floor
* build        -- you will make and build things in this directory
* calibration  -- checkerboard images to calibrate on
* muchroom     -- machine learning database
* shape_sample -- silhoette shapes to recognize
* stereoData   -- left, right image pairs of checkboards to calibrate and view on


## LINKS:
Click the Download Zip button to the right to download example code.

Visit the catalog page [here](http:*shop.oreilly.com/product/0636920044765.do).

See an error? Report it [here](http:*oreilly.com/catalog/errata.csp?isbn=0636920044765), or simply fork and send us a pull request
