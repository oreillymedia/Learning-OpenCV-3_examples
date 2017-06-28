//Exercises at end of Chapter 11
// 1、2、5、6 and 7
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void help(const char **argv) {
	cout << "\n\n"
		<< "This program solves the Exercises at the end of Chapter 11(without 3、4 and 8)\n"
		<< "Call:\n"
		<< argv[0] << " <path/image_name>\n\n"
		<< "For example: " << argv[0] << " /AverageMaleFace.jpg\n"
		<< endl;
}

// Get the coordinates of the points after the rotation
Point2f getPointAffinedPos(const Point2f src, const Point2f center, double angle)  
{  
	Point dst;  
	int x = src.x - center.x;  
	int y = src.y - center.y;  

	dst.x = x * cos(angle) + y * sin(angle) + center.x;  
	dst.y = -x * sin(angle) + y * cos(angle) + center.y;  
	return dst;  
}  

int main( int argc, const char** argv )
{
	help(argv);
	if(argc < 2) {
		cout << "\nERROR: You had too few parameters.\n" << endl;
		return -1;
	}
	/************************************************************************/
	/* 1.  Find  and  load  a  picture  of  a  face  where  the  face  is  frontal,  has  eyes  open,  and
	takes up most or all of the image area. Write code to find the pupils of the eyes.                                                                     */
	/************************************************************************/
	Mat matFrontFace = imread(argv[1]);
	Mat gray;Mat temp;
	double minPixelValue, maxPixelValue;
	Point minPixelPoint,maxPixelPoint;
	cvtColor(matFrontFace,gray,COLOR_BGR2GRAY);
	// Laplacian pyramid
	pyrDown(gray,temp);
	pyrUp(temp,temp);
	temp = gray - temp;
	// find and circle the result
	minMaxLoc(temp,&minPixelValue,&maxPixelValue,&minPixelPoint,&maxPixelPoint);
	circle(matFrontFace,maxPixelPoint,10,Scalar(255,255,255),2);
	/************************************************************************/
	/* 2.  Look  at  the  diagrams  of  how  the  log-polar  function  transforms  a  square  into  a
	wavy line.
	a.  Draw the log-polar results if the log-polar center point were sitting on one of
	the corners of the square.
	b.  What would a circle look like in a log-polar transform if the center point were
	inside the circle and close to the edge?
	c.  Draw what the transform would look like if the center point were sitting just
	outside of the circle.                                                                     */
	/************************************************************************/
	// draw a circle,white on black.
	Mat matLogPolar = Mat(512,512,CV_8U,Scalar(0));
	circle(matLogPolar,Point(256,256),100,Scalar(255),3);
	//a
	logPolar(matLogPolar,temp,Point(0,0),40,INTER_CUBIC);
	//b
	logPolar(matLogPolar,temp,Point(256-101,256),40,INTER_CUBIC);
	//c the result like two rings
	logPolar(matLogPolar,temp,Point(256-101-3,256),40,INTER_CUBIC);
	/************************************************************************/
	/* 5.  Load an image, take a perspective transform, and then rotate it. Can this trans‐
	form be done in one step?                                            */
	/************************************************************************/
	Mat matE5_1 = imread(argv[1]);
	Mat matE5_2 = imread(argv[1]);
	double angle = 45;
	//perspective matrix
	Point2f src_vertices[4];
	src_vertices[0] = Point(0, 0);
	src_vertices[1] = Point(matE5_1.cols, 0);
	src_vertices[2] = Point(matE5_1.cols, matE5_1.rows);
	src_vertices[3] = Point(0, matE5_1.rows);
	Point2f dst_vertices[4];
	dst_vertices[0] = Point(100, 0);
	dst_vertices[1] = Point(matE5_1.cols - 100, 0);
	dst_vertices[2] = Point(matE5_1.cols, matE5_1.rows);
	dst_vertices[3] = Point(0, matE5_1.rows);
	Mat perspectiveMatrix = getPerspectiveTransform(src_vertices, dst_vertices);
	//roate matrix
	Mat affineMatrix = getRotationMatrix2D(Point(matE5_1.cols/2,matE5_1.rows/2), angle, 1.0 );
	// perspective -> rotate
	warpPerspective(
		matE5_1, 
		matE5_1, 
		perspectiveMatrix,
		matE5_1.size(), 
		INTER_LINEAR, 
		BORDER_CONSTANT,
		Scalar());
	warpAffine(
		matE5_1,
		matE5_1,
		affineMatrix,
		matE5_1.size(),
		INTER_LINEAR,
		BORDER_CONSTANT,
		Scalar()
		);
	// do it in one step
	Point center = Point(matE5_2.cols/2,matE5_2.rows/2);
	dst_vertices[0] = getPointAffinedPos(dst_vertices[0],center,angle);
	dst_vertices[1] = getPointAffinedPos(dst_vertices[1],center,angle);
	dst_vertices[2] = getPointAffinedPos(dst_vertices[2],center,angle);
	dst_vertices[3] = getPointAffinedPos(dst_vertices[3],center,angle);
	perspectiveMatrix = getPerspectiveTransform(src_vertices, dst_vertices);
	warpPerspective(
		matE5_2, 
		matE5_2, 
		perspectiveMatrix,
		matE5_2.size(), 
		INTER_LINEAR, 
		BORDER_CONSTANT,
		Scalar());
	// the result is almost the same
	/************************************************************************/
	/* 6.  Inpainting works pretty well for the repair of writing over textured regions. What
	would happen if the writing obscured a real object edge in a picture? Try it.                                                                     */
	/************************************************************************/
	Mat matInpaint = imread(argv[1]);
	Mat matInpaintMask = Mat(matInpaint.size(),CV_8UC1,Scalar(0));//the same size,all in black(oh yeah!)
	//draw the same circle on the matInpaint and matInpaintMask
	circle(matInpaint,Point(255,255),100,Scalar(255),10);
	circle(matInpaintMask,Point(255,255),100,Scalar(255),10);
	imshow("befor inpaint",matInpaint);
	inpaint(matInpaint,matInpaintMask,matInpaint,10,CV_INPAINT_TELEA);
	imshow("after inpaint",matInpaint);
	//the result is :if "the writing obscured a real object edge in a picture",the result is bad
	/************************************************************************/
	/* 7.  Practice  histogram  equalization  on  images  that  you  load  in,  and  report  the
	results.                                                                     */
	/************************************************************************/
	Mat matSrc = imread(argv[1]);
	vector<Mat> planes;
	split(matSrc,planes);
	imshow("b",planes[0]);
	imshow("g",planes[1]);
	imshow("r",planes[2]);
	cv::equalizeHist(planes[0],planes[0]);
	cv::equalizeHist(planes[1],planes[1]);
	cv::equalizeHist(planes[2],planes[2]);
	imshow("bh",planes[0]);
	imshow("gh",planes[1]);
	imshow("rh",planes[2]);
	waitKey();
	return 0;

}
