//Exercises_9_4.cpp Exercises at end of Chapter 9

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

	/************************************************************************/
	/* 1.In  this  exercise,  we  learn  to  experiment  with  parameters  by  setting  good  low
	Thresh  and  highThresh  values  in  cv::Canny().  Load  an  image  with  suitably
	interesting line structures. We’ll use three different high:low threshold settings of
	1.5:1, 2.75:1, and 4:1.
	a.  Report what you see with a high setting of less than 50.
	b.  Report what you see with high settings between 50 and 100.
	c.  Report what you see with high settings between 100 and 150.
	d.  Report what you see with high settings between 150 and 200.
	e.  Report what you see with high settings between 200 and 250.
	f.  Summarize your results and explain what happens as best you can.                                                                     */
	/************************************************************************/
  Mat src = imread(argv[1],IMREAD_GRAYSCALE);
	Mat dst;
	
	int iHighThresh = 50;
	Canny(src,dst,static_cast<int>(iHighThresh/1.5),iHighThresh);
	imshow("iHighThresh is 50 and  high:low is 1.5:1",dst);
	Canny(src,dst,static_cast<int>(iHighThresh/2.75),iHighThresh);
	imshow("iHighThresh is 50 and  high:low is 2.75:1",dst);
	Canny(src,dst,static_cast<int>(iHighThresh/4),iHighThresh);
	imshow("iHighThresh is 50 and  high:low is 4:1",dst);

	iHighThresh = (50+100)/2;
	Canny(src,dst,static_cast<int>(iHighThresh/1.5),iHighThresh);
	imshow("iHighThresh is (50+100)/2 and  high:low is 1.5:1",dst);
	Canny(src,dst,static_cast<int>(iHighThresh/2.75),iHighThresh);
	imshow("iHighThresh is (50+100)/2 and  high:low is 2.75:1",dst);
	Canny(src,dst,static_cast<int>(iHighThresh/4),iHighThresh);
	imshow("iHighThresh is (50+100)/2 and  high:low is 4:1",dst);

	iHighThresh = (100+150)/2;
	Canny(src,dst,static_cast<int>(iHighThresh/1.5),iHighThresh);
	imshow("iHighThresh is (100+150)/2 and  high:low is 1.5:1",dst);
	Canny(src,dst,static_cast<int>(iHighThresh/2.75),iHighThresh);
	imshow("iHighThresh is (100+150)/2 and  high:low is 2.75:1",dst);
	Canny(src,dst,static_cast<int>(iHighThresh/4),iHighThresh);
	imshow("iHighThresh is (100+150)/2 and  high:low is 4:1",dst);

	iHighThresh = (150+200)/2;
	Canny(src,dst,static_cast<int>(iHighThresh/1.5),iHighThresh);
	imshow("iHighThresh is (150+200)/2 and  high:low is 1.5:1",dst);
	Canny(src,dst,static_cast<int>(iHighThresh/2.75),iHighThresh);
	imshow("iHighThresh is (150+200)/2 and  high:low is 2.75:1",dst);
	Canny(src,dst,static_cast<int>(iHighThresh/4),iHighThresh);
	imshow("iHighThresh is (150+200)/2 and  high:low is 4:1",dst);

	iHighThresh = (200+250)/2;
	Canny(src,dst,static_cast<int>(iHighThresh/1.5),iHighThresh);
	imshow("iHighThresh is (200+250)/2 and  high:low is 1.5:1",dst);
	Canny(src,dst,static_cast<int>(iHighThresh/2.75),iHighThresh);
	imshow("iHighThresh is (200+250)/2 and  high:low is 2.75:1",dst);
	Canny(src,dst,static_cast<int>(iHighThresh/4),iHighThresh);
	imshow("iHighThresh is (200+250)/2 and  high:low is 4:1",dst);

	/************************************************************************/
	/* 2.  Load an image containing clear lines and circles such as a side view of a bicycle.
	Use  the  Hough  line  and  Hough  circle  calls  and  see  how  they  respond  to  your
	image.                                                                     */
	/************************************************************************/
	Mat src = imread(argv[1],IMREAD_GRAYSCALE);//a bike in gray
	GaussianBlur( src, src, Size(9, 9), 2, 2 );
	Mat temp;
	vector<Vec4i> linesP;
	vector<Vec3f> circles;
	//first find the canny edge
	Canny(src,temp,50,200);
	//find lines
	HoughLinesP(temp,linesP,1,CV_PI/180,80,50,10);
	//find circles
	HoughCircles( src, circles, CV_HOUGH_GRADIENT, 1, src.rows/8, 200, 100, 0, 0 );
	//draw lines and circles in the source image
	for (int i = 0;i<linesP.size();i++)
	{
		Vec4i l = linesP[i];
		line(src,Point(l[0],l[1]),Point(l[2],l[3]),Scalar(255));
	}

	for( size_t i = 0; i < circles.size(); i++ )
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle( src, center, 3, Scalar(255,255,255), -1, 8, 0 );
		// circle outline
		circle( src, center, radius, Scalar(255,255,255), 1, 8, 0 );
	}
	
	/*11.  Create an image with a 20 × 20 square in it. Rotate it to an arbitrary angle. Take a
	distance transform of this image. Create a 20 × 20 square shape. Use the distance
	transform image to algorithmically overlay the shape onto the rotated square in
	the image you made.*/
	// create a 20 × 20 square
	Mat matBoard = Mat(500,500,CV_8UC3,Scalar(0));
	rectangle(matBoard,Rect(250,250,20,20),Scalar(255,255,255),-1);
	// create an arbitary angle
	cv::RNG rng = cv::theRNG();
	float fDegree = rng.uniform(0,360);
	Mat rotMatS = getRotationMatrix2D(Point(250,250), fDegree, 1.0);
	warpAffine(matBoard, matBoard, rotMatS, matBoard.size(), 1, 0, Scalar(0,0,0));
	//solove the problem
	Mat matDistance;
	cvtColor(matBoard,gray,COLOR_BGR2GRAY);
	threshold(gray,gray,100,255,CV_THRESH_BINARY); 
	distanceTransform(gray,matDistance,CV_DIST_L2,3);  
       //find the center corners
	double minVal; 
	double maxVal2; 
	Point minLoc,maxLoc;
	minMaxLoc( matDistance, &minVal, &maxVal2, &minLoc, &maxLoc, matDistance>0); 
	//draw the result
	circle(matBoard,maxLoc,1,Scalar(0),1);
	circle(matBoard,minLoc,2,Scalar(255),2);
	rectangle(matBoard,Rect(maxLoc.x - 10,maxLoc.y - 10,20,20),Scalar(0,0,255),-1);
	return 0;
}
 
