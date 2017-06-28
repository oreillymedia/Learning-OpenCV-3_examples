//Exercises_9_1-2-10-11-12-15-16.cpp Exercises at end of Chapter 8
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

 
void help(const char **argv) {
	cout << "\n\n"
		<< "This program solves the Exercise 1、2、10、11、12、15 and 16 at the end of Chapter 9 \n"
		<< "Call:\n"
		<< argv[0] << " <path/image_name>\n\n"
		<< "For example: ./" << argv[0] << " ../lena.jpg\n"
		<< endl;
}

int main( int argc, const char** argv )
{
	help(argv);
	if(argc < 2) {
		cout << "\nERROR: You had too few parameters.\n" << endl;
		return -1;
	}
	Mat src ;
	Mat gray;
	Mat mask;
	Mat temp;
	Mat temp2;
	/************************************************************************/
	/* 1.  Load an image with interesting textures. Smooth the image in several ways using
	cv::smooth() with smoothtype=cv::GAUSSIAN.
	a.  Use a symmetric 3 × 3, 5 × 5, 9 × 9, and 11 × 11 smoothing window size and
	display the results.
	b.  Are the output results nearly the same by smoothing the image twice with a
	5 × 5 Gaussian filter as when you smooth once with two 11 × 11 filters? Why
	or why not?                                                                     */
	/************************************************************************/	
	src = imread(argv[1]);
	if (src.empty())
	{
		cout << "\nERROR: parameters is not a image name.\n" << endl;
		return -1;
	}
	double minPixelValue, maxPixelValue;
	//a
	Mat smooth33;Mat smooth55;Mat smooth99;Mat smooth111;
	GaussianBlur(src,smooth33,cv::Size(3,3),0);
	GaussianBlur(src,smooth55,cv::Size(5,5),0);
	GaussianBlur(src,smooth99,cv::Size(9,9),0);
	GaussianBlur(src,smooth111,cv::Size(11,11),0);
	//b
	GaussianBlur(smooth55,smooth55,cv::Size(5,5),0);
	temp = smooth55 - smooth111;
	cv::minMaxIdx(temp, &minPixelValue, &maxPixelValue);
	// maxPixelVaule  = 19 ,the result is " 5 × 5 Gaussian filter twice" is much like "11 × 11 filters"

	/************************************************************************/
	/* 2.  Create a 100 × 100 single-channel image. Set all pixels to 0. Finally, set the center
	pixel equal to 255.
	a.  Smooth this image with a 5 × 5 Gaussian filter and display the results. What
	did you find?
	b.  Do this again but with a 9 × 9 Gaussian filter.
	c.  What does it look like if you start over and smooth the image twice with the 5
	× 5 filter? Compare this with the 9 × 9 results. Are they nearly the same? Why
	or why not?
	*/
	/************************************************************************/
	Mat singleChanel100 = Mat(100,100,CV_8U,Scalar(0));
	singleChanel100.at<uchar>(50,50) = 255;
	
	//a
	GaussianBlur(singleChanel100,temp,cv::Size(5,5),0);
	imshow("5 × 5 Gaussian filter",temp);
	//b
	GaussianBlur(singleChanel100,temp,cv::Size(9,9),0);
	imshow("9 × 9Gaussian filter",temp);
	//c
	GaussianBlur(singleChanel100,temp,cv::Size(5,5),0);
	GaussianBlur(temp,temp,cv::Size(5,5),0);
	GaussianBlur(singleChanel100,temp2,cv::Size(9,9),0);
	absdiff(temp,temp2,temp2);
	cv::minMaxIdx(temp2, &minPixelValue, &maxPixelValue);
	//maxPixelVaule = 5,the result are nearly the same
	/************************************************************************/
	/* 10.  Create  a  low-variance  random  image  (use  a  random  number  call  such  that  the
	numbers don’t differ by much more than three and most numbers are near zero).
	Load  the  image  into  a  drawing  program  such  as  PowerPoint,  and  then  draw  a
	wheel  of  lines  meeting  at  a  single  point.  Use  bilateral  filtering  on  the  resulting
	image and explain the results.                                                                     */
	/************************************************************************/
	Mat matLowVariance  = Mat(512,512,CV_8U,Scalar(0));
	RNG arng = cv::theRNG();
	arng.fill(matLowVariance,RNG::UNIFORM,0,30);
	//draw a wheel of lines meeting at the center
	line(matLowVariance,Point(256,256),Point(256,256-100),Scalar(255),1);
	line(matLowVariance,Point(256,256),Point(256+100,256+100),Scalar(255),1);
	line(matLowVariance,Point(256,256),Point(256+100,256),Scalar(255),1);
	line(matLowVariance,Point(256,256),Point(256+100,256-100),Scalar(255),1);
	line(matLowVariance,Point(256,256),Point(256,256+100),Scalar(255),1);
	line(matLowVariance,Point(256,256),Point(256-100,256-100),Scalar(255),1);
	line(matLowVariance,Point(256,256),Point(256-100,256),Scalar(255),1);
	line(matLowVariance,Point(256,256),Point(256-100,256+100),Scalar(255),1);
	imshow("a wheel  of  lines  meeting  at  a  single  point",matLowVariance);
	bilateralFilter(matLowVariance,temp,5,10.0,2.0); 
	imshow("bilateralFilter",temp);
	/************************************************************************/
	/* 11.  Load an image of a scene and convert it to grayscale.
	a.  Run  the  morphological  Top  Hat  operation  on  your  image  and  display  theresults.
	b.  Convert the resulting image into an 8-bit mask.
	c.  Copy a grayscale value into the original image where the Top Hat mask (from
	Part b of this exercise) is nonzero. Display the results.                                                                     */
	/************************************************************************/
	cvtColor(src,gray,COLOR_BGR2GRAY);
	//a 
	morphologyEx(gray,temp,CV_MOP_TOPHAT,Mat());
	imshow(" morphological  Top  Hat",temp);
	//b
	temp.convertTo(mask,CV_8UC1);
	//c
	cvtColor(gray,gray,COLOR_GRAY2BGR);
	gray.copyTo(src,mask);
	imshow("execrise 11 result",src);
	/************************************************************************/
	/* 12.  Load an image with many details.
	a.  Use resize() to reduce the image by a factor of 2 in each dimension (hence
	the image will be reduced by a factor of 4). Do this three times and display the
	results.
	b.  Now take the original image and use cv::pyrDown() to reduce it three times,
	and then display the results.
	c.  How are the two results different? Why are the approaches different?
	*/
	/************************************************************************/
	//a
	Mat matResize;
	resize(src,matResize,cv::Size(0,0),0.5,0.5);
	resize(matResize,matResize,cv::Size(0,0),0.5,0.5);
	resize(matResize,matResize,cv::Size(0,0),0.5,0.5);
	imshow("resize 3 times",matResize);
	//b
	Mat	matPyrDown;
	pyrDown(src,matPyrDown);
	pyrDown(matPyrDown,matPyrDown);
	pyrDown(matPyrDown,matPyrDown);
	imshow("pyrDown 3 times",matPyrDown);
	//c
	absdiff(matResize,matPyrDown,temp);
	imshow("two results of resize and pyDown diff",temp);
	/************************************************************************/
	/* 15.  Use  cv::filter2D()  to  create  a  filter  that  detects  only  60-degree  lines  in  an
	image. Display the results on a sufficiently interesting image scene.                                                                     */
	/************************************************************************/
	Mat matWithLines = Mat(512,512,CV_8UC1,Scalar(0));
	// create 9 lines
	for (int i=0;i<9;i++)
	{
		line(matWithLines,Point(arng.uniform(0,512),arng.uniform(0,521)),Point(arng.uniform(0,512),arng.uniform(0,521)),Scalar(255),1);
	}
	//45 degree line 
	line(matWithLines,Point(0,512),Point(512,0),Scalar(255),1);
	matWithLines.convertTo(matWithLines,CV_32FC1,1.0/255);
	// detects  only  45-degree lines
	Mat matKernel = Mat(3,3,CV_32FC1,Scalar(0));
    matKernel.at<float>(0,0) =  0 ;
	matKernel.at<float>(0,1) =  0 ;
	matKernel.at<float>(0,2) =  1.0/3 ;
	matKernel.at<float>(1,0) =  0 ;
	matKernel.at<float>(1,1) =  1.0/3 ;
	matKernel.at<float>(1,2) =  0 ;
	matKernel.at<float>(2,0) =  1.0/3 ;
	matKernel.at<float>(2,1) = 0;
	matKernel.at<float>(2,2) = 0;
	filter2D(matWithLines,temp,CV_32FC1,matKernel);
	threshold(temp,temp,0.99,1,CV_THRESH_BINARY);
	/************************************************************************/
	/* 16.  Separable kernels: create a 3 × 3 Gaussian kernel using rows [(1/16, 2/16, 1/16),
	(2/16, 4/16, 2/16), (1/16, 2/16, 1/16)] and with anchor point in the middle.
	a.  Run this kernel on an image and display the results.
	b.  Now  create  two  one-dimensional  kernels  with  anchors  in  the  center:  one
	going  “across”  (1/4,  2/4,  1/4),  and  one  going  down  (1/4,  2/4,  1/4).  Load  the
	same  original  image  and  use  cv::filter2D()  to  convolve  the  image  twice,
	once with the first 1D kernel and once with the second 1D kernel. Describe
	the results.
	c.  Describe  the  order  of  complexity  (number  of  operations)  for  the  kernel  in
	part a and for the kernels in part b. The difference is the advantage of being
	able  to  use  separable  kernels  and  the  entire  Gaussian  class  of  filters—or  any
	linearly  decomposable  filter  that  is  separable,  since  convolution  is  a  linear
	operation.                                                                     */
	/************************************************************************/
	Mat matGaussianKernel = Mat(3,3,CV_32FC1,Scalar(0));
	matGaussianKernel.at<float>(0,0) = 1.0/16;
	matGaussianKernel.at<float>(0,1) = 2.0/16;
	matGaussianKernel.at<float>(0,2) = 1.0/16;
	matGaussianKernel.at<float>(1,0) = 2.0/16;
	matGaussianKernel.at<float>(1,1) = 4.0/16;
	matGaussianKernel.at<float>(1,2) = 2.0/16;
	matGaussianKernel.at<float>(2,0) = 1.0/16;
	matGaussianKernel.at<float>(2,1) = 2.0/16;
	matGaussianKernel.at<float>(2,2) = 1.0/16;
	//a
	src.convertTo(temp,CV_32F,1.0/255);
	filter2D(temp,temp,CV_32F,matGaussianKernel);
	imshow("a 3 × 3 Gaussian kernel",temp);
	//b
	Mat matKernel1 = Mat(1,3,CV_32FC1,Scalar(0));
	Mat matKernel2 = Mat(3,1,CV_32FC1,Scalar(0));
	matKernel1.at<float>(0,0) = 1.0/4;
	matKernel1.at<float>(0,1) = 2.0/4;
	matKernel1.at<float>(0,2) = 1.0/4;
	matKernel2.at<float>(0,0) = 1.0/4;
	matKernel2.at<float>(1,0) = 2.0/4;
	matKernel2.at<float>(2,0) = 1.0/4;
	filter2D(temp,temp2,CV_32F,matKernel1);
	filter2D(temp2,temp2,CV_32F,matKernel2);
	absdiff(temp,temp2,temp2);
	//temp and temp2 is just the same mat,maxPixelValue is very small,nearly ZERO
	cv::minMaxIdx(temp2, &minPixelValue, &maxPixelValue);
	//c the order is no matter
	waitKey();
	return 0;
}
