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
	Mat temp;

	/************************************************************************/
	/* 4.  Use a camera to take two pictures of the same scene while moving the camera as
	little as possible. Load these images into the computer as src1 and src1.
	a.  Take  the  absolute  value  of  src1  minus  src1  (subtract  the  images);  call  it
	diff12 and display. If this were done perfectly, diff12 would be black. Why
	isn’t it?
	b.  Create cleandiff by using cv::erode() and then cv::dilate() on diff12.
	Display the results.
	c.  Create  dirtydiff  by  using  cv::dilate()  and  then  cv::erode()  on  diff12
	and then display.
	d.  Explain the difference between cleandiff and dirtydiff.                                                                     */
	/************************************************************************/
	Mat src1 = imread(argv[1],IMREAD_GRAYSCALE);
	Mat src2 = imread(argv[2],IMREAD_GRAYSCALE);
	if (src1.empty() || src2.empty())
	{
		cout << "\nERROR: parameters is not a image name.\n" << endl;
		return -1;
	}
	//a
	Mat diff12 =  src1 - src2;
	imshow("diff12",diff12);
	//b
	Mat cleandiff;
	erode(diff12,cleandiff,Mat());
	dilate(cleandiff,cleandiff,Mat());
	imshow("cleandiff",cleandiff);
	//c
	Mat dirtydiff;
	dilate(diff12,dirtydiff,Mat());
	erode(dirtydiff,dirtydiff,Mat());
	imshow("dirtydiff",dirtydiff);
	//d
	absdiff(cleandiff,dirtydiff,temp) ;
	imshow("absdiff",temp);
	//the difference between cleandiff and dirtydiff is the difference between of "open" and "close"
	waitKey();
	return 0;

}
