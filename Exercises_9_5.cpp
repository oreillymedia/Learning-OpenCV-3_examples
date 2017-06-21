//Exercises_9_5.cpp Exercises at end of Chapter 9
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

 
void help(const char **argv) {
	cout << "\n\n"
		<< "This program solves the Exercise 4、5 at the end of Chapter 9 \n"
		<< "Call:\n"
		<< argv[0] << " <path/image_name1>" << " <path/image_name2>\n\n"
		<< "For example: ./" << argv[0] << " ../left.jpg "<< " ../left.jpg\n"
		<< endl;
}

int main( int argc, const char** argv )
{
	help(argv);
	if(argc < 3) {
		cout << "\nERROR: You had too few parameters.\n" << endl;
		return -1;
	}
	Mat temp;
	Mat temp2;
 /************************************************************************/
	/* 5.  Create an outline of an object. Take a picture of a scene. Then, without moving
	the camera, put a coffee cup in the scene and take a second picture. Load these
	images and convert both to 8-bit grayscale images.
	a.  Take  the  absolute  value  of  their  difference.  Display  the  result,  which  should
	look like a noisy mask of a coffee mug.
	b.  Do a binary threshold of the resulting image using a level that preserves most
	of the coffee mug but removes some of the noise. Display the result. The “on”
	values should be set to 255.
	c.  Do a cv::MOP_OPEN on the image to further clean up noise.
	d.  Using  the  erosion  operator  and  logical  XOR  function,  turn  the  mask  of  the
	coffee  cup  image  into  an  outline  of  the  coffee  cup  (only  the  edge  pixels
	remaining).                                                                     */
	/************************************************************************/
	Mat matMug = imread(argv[1],IMREAD_GRAYSCALE);
	Mat matNoMug = imread(argv[2],IMREAD_GRAYSCALE);
	if (matMug.empty() || matNoMug.empty())
	{
		cout << "\nERROR: parameters is not a image name.\n" << endl;
		return -1;
	}
	//a
	absdiff(matMug,matNoMug,temp);
	imshow("absolute  value  of  mug ",temp);
	//b
	threshold(temp,temp,100,255,THRESH_OTSU);
	imshow("a binary threshold",temp);
	//c
	morphologyEx(temp,temp,cv::MORPH_OPEN,Mat());
	imshow("MORPH_OPEN",temp);
	//d
	erode(temp,temp,Mat());
	matMug.copyTo(temp2,temp);
	imshow("an outline of the coffee cup",temp2);
	waitKey();
	return 0;

}
