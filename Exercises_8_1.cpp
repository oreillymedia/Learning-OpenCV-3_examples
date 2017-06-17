//Exercises at end of Chapter 8
//1
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


void help(const char **argv) {
	cout << "\n\n"
		<< "This program solves the Exercise 1 at the end of Chapter 8 \n"
		<< "Call:\n"
		<< argv[0] << " <path/video_name>\n\n"
		<< "For example: ./" << argv[0] << " ../bike.avi\n"
		<< endl;
}

int main( int argc, const char** argv )
{
	help(argv);
	if(argc < 2) {
		cout << "\nERROR: You had too few parameters.\n" << endl;
		return -1;
	}
	/************************************************************************/
	/* 1.  Create a program that 
	(1) reads frames from a video, 
	(2) turns the result to gray‐scale,  and 
	(3) performs  Canny  edge  detection  on  the  image.  
	Display  all  three stages of processing in three different windows, with each window appropriately
	named for its function.
	a.  Display  all  three  stages  of  processing  in  one  image.  (Hint:  create  another
	image of the same height but three times the width as the video frame. Copy
	the  images  into  this,  either  by  using  pointers  or  (more  cleverly)  by  creating
	three new image headers that point to the beginning of and to one-third and
	two-thirds of the way into the imageData. Then use Mat::copyTo().)
	b.  Write  appropriate  text  labels  describing  the  processing  in  each  of  the  three
	slots.*/
	/************************************************************************/
	VideoCapture capture(argv[1]); 
	if(!capture.isOpened())
		return 1;
	double rate=capture.get(CV_CAP_PROP_FPS);
	bool stop(false);
	Mat MatFrame;
	Mat MatGray;
	Mat MatCanny;
	int delay=1000/rate;
	while(!stop)
	{
		if(!capture.read(MatFrame))
			break;
		//(1)
		imshow("Raw Video",MatFrame);
		//(2)
		cvtColor(MatFrame,MatGray,COLOR_BGR2GRAY);
		imshow("Gray Video",MatGray);
		//(3)
		Canny(MatGray,MatCanny,100,255);
		imshow("Canny Video",MatCanny);
		//question a
		Mat MatAll(MatFrame.rows,3* MatFrame.cols  ,CV_8UC3,Scalar::all(0));
		cvtColor(MatGray,MatGray,COLOR_GRAY2BGR);
		cvtColor(MatCanny,MatCanny,COLOR_GRAY2BGR);
		Mat MatSub = MatAll.colRange(0, MatFrame.cols);
		MatFrame.copyTo(MatSub);
		MatSub = MatAll.colRange( MatFrame.cols,2*MatFrame.cols);
		MatGray.copyTo(MatSub);
		MatSub = MatAll.colRange(2*MatFrame.cols,3*MatFrame.cols);
		MatCanny.copyTo(MatSub);
		//question b
		Scalar color = CV_RGB(255,0,0);
		putText(MatAll,"raw video",Point(50,30),CV_FONT_HERSHEY_DUPLEX,1.0f,color);
		putText(MatAll,"gray video",Point(50+MatFrame.cols,30),CV_FONT_HERSHEY_DUPLEX,1.0f,color);
		putText(MatAll,"canny video",Point(50+2*MatFrame.cols,30),CV_FONT_HERSHEY_DUPLEX,1.0f,color);
		imshow("all Video",MatAll);
		if(waitKey(delay)>=0)
			stop=true;
	}
	capture.release();
	waitKey();
	getchar();
	return 0;

}
