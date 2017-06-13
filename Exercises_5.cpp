//Exercises at end of Chapter 5
// 1-6
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void help(const char **argv) {
  cout << "\n\n"
   << "This program solves the Exercises at the end of Chapter 5\n"
   << "Call:\n"
   << argv[0] << " <path/image_name>\n\n"
   << "For example: ./" << argv[0] << " ../faces.png\n"
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
	/* 1.  This  exercise  will  accustom  you  to  the  idea  of  many  functions  taking  matrix
	types.  Create  a  two-dimensional  matrix  with  three  channels  of  type  byte  with
	data size 100 × 100. Set all the values to 0.
	a.  Draw a circle in the matrix using void cv::circle(InputOutputArray img,
	cv::point  center,  intradius,  cv::Scalar  color,  int  thickness=1,
	int line_type=8, int shift=0).
	b.  Display this image using methods described in Chapter 2.                                                                     */
	/************************************************************************/
	Mat m1 = Mat(100,100,CV_8U,Scalar(0));
	// a
	cv::circle(m1,Point(m1.cols/2,m1.rows/2),40,Scalar(255));
	// b
	cv::imshow("execrise 1",m1);

	/************************************************************************/
	/* 2.  Create a two-dimensional matrix with three channels of type byte with data size
	100 × 100, and set all the values to 0. Use the cv::Mat element access functions to
	modify the pixels. Draw a green rectangle between (20, 5) and (40, 20).                                                                     */
	/************************************************************************/
	Mat m2 = Mat(100,100,CV_8UC3,Scalar(0));
	for (int i=0;i<m2.rows;i++)
	{
		for (int j=0;j<m2.cols;j++)
		{
			if (j>=20&&j<=40&&i>=5&&i<=20)
			{
				m2.at<Vec3b>(i,j)[0]=0;    //b
				m2.at<Vec3b>(i,j)[1]=255;  //g
				m2.at<Vec3b>(i,j)[2]=0;    //r
			}
		}
	}
	cv::imshow("execrise 2",m2);

	/************************************************************************/
	/* 3.  Create a three-channel RGB image of size 100 × 100. Clear it. Use pointer arith‐
	metic to draw a green square between (20, 5) and (40, 20).                                                                     */
	/************************************************************************/
	Mat m3 = Mat(100,100,CV_8UC3,Scalar(0));
	for(int i=0;i<m3.rows;i++)
	{       
		uchar* outData=m3.ptr<uchar>(i);     
		for(int j=0;j<m3.cols;j++)
		{
			if (j>=20&&j<=40&&i>=5&&i<=20)
			{
				outData[j*3+1] = 255;
			}
		}
	}
	cv::imshow("execrise 3",m3);

	/************************************************************************/
	/* 4.  Practice using region of interest (ROI). Create a 210 × 210 single-channel byte
	image and zero it. Within the image, build a pyramid of increasing values using  
	ROI and cv::Mat::setTo(). That is: the outer border should be 0, the next inner
	border should be 20, the next inner border should be 40, and so on until the final
	innermost square is set to value 200; all borders should be 10 pixels wide. Display
	the image.
	/************************************************************************/
	Mat m4 = Mat(210,210,CV_8U,Scalar(0));
	for (int i=0;i<210/2;i=i+10)
	{
		Mat roi = m4(cv::Rect(i,i,210-i*2,210-i*2));
		roi.setTo(i*2);// roi = i*2;
		
	}
	cv::imshow("execrise 4",m4);

	/************************************************************************/
	/* 5.  Use  multiple  headers  for  one  image.  Load  an  image  that  is  at  least  100  ×  100.
	Create two additional headers that are ROIs where width = 20 and the height =
	30.  Their  origins  should  be  at  (5,  10)  and  (50,  60),  respectively.  Pass  these  new
	image  subheaders  to  cv::bitwise_not().  Display  the  loaded  image,  which
	should have two inverted rectangles within the larger image.                                                                     */
	/************************************************************************/
	Mat m5 = Mat(100,100,CV_8U,Scalar(0));
	Mat roi1 = m5(Rect(5,10,20,30));
	Mat roi2 = m5(Rect(50,60,20,30));
	bitwise_not(roi1,roi1);
	bitwise_not(roi2,roi2);
	cv::imshow("execrise 5",m5);

	/************************************************************************/
	/* 6.  Create a mask using cv::compare(). Load a real image. Use cv::split() to split
	the image into red, green, and blue images.
	a.  Find and display the green image.
	b.  Clone this green plane image twice (call these clone1 and clone2).
	c.  Find the green plane’s minimum and maximum value.
	d.  Set  clone1’s  values  to  thresh = (unsigned char)((maximum - minimum)/
	2.0).
	e.  Set  clone2  to  0  and  use  cv::compare  (green_image,  clone1,  clone2,
	cv::CMP_GE).  Now  clone2  will  have  a  mask  of  where  the  value  exceeds
	thresh in the green image.
	f.  Finally,     use     cv::subtract   (green_image,thresh/2,   green_image,
	clone2) and display the results.                                                                     */
	/************************************************************************/
	Mat clone1,clone2;
	vector<cv::Mat> bgr_planes;
	Mat src = cv::imread(argv[1],1);
	split(src, bgr_planes );
	// a
	Mat green = bgr_planes[1];
	imshow("green",green);
	// b
	clone1 = green.clone();
	clone2 = green.clone();
	// c
	double minPixelValue, maxPixelValue;
	int minPixelID,maxPixelID;
	cv::minMaxIdx(green, &minPixelValue, &maxPixelValue,&minPixelID,&maxPixelID);
	// d
	double thresh= (unsigned char)((maxPixelValue - minPixelValue)/2.0);
	int ithresh = (int)thresh;
	clone1 = Mat(clone1.size(),clone1.type(),Scalar(ithresh));
	// e
	clone2 = Mat(clone2.size(),clone2.type(),Scalar(0));
	compare(green,clone1,clone2,cv::CMP_GE);
	// f
	cv::subtract(green,thresh/2,green,clone2);
	imshow("execrise 6",clone2);
	cout << "6" << endl;
	waitKey(-1); //Wait here until any key pressed
	return 0;
}
