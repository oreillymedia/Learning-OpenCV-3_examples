// Example 13-3. Template matching

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

void help( char** argv ){
  cout << "\n"
  <<"Example of using matchTemplate(). The call is:\n"
  <<"\n"
  <<argv[0] <<" template image_to_be_searched\n"
  <<"\n"
  <<" This routine will search using all methods:\n"
  <<" cv::TM_SQDIFF 0\n"
  <<" cv::TM_SQDIFF_NORMED 1\n"
  <<" cv::TM_CCORR 2\n"
  <<" cv::TM_CCORR_NORMED 3\n"
  <<" cv::TM_CCOEFF 4\n"
  <<" cv::TM_CCOEFF_NORMED 5\n"
  <<"\n";
}

// Display the results of the matches
//
int main( int argc, char** argv ) {

  if( argc != 3) {
    help( argv );
    return -1;
  }

  cv::Mat src, templ, ftmp[6];   // ftmp is what to display on

  // Read in the template to be used for matching:
  //
  if((templ=cv::imread(argv[1], 1)).empty()) {
    cout << "Error on reading template " << argv[1] << endl;
    help( argv );return -1;
  }

  // Read in the source image to be searched:
  //
  if((src=cv::imread(argv[2], 1)).empty()) {
    cout << "Error on reading src image " << argv[2] << endl;
    help( argv );return -1;
  }

  // Do the matching of the template with the image
  for(int i=0; i<6; ++i){
    cv::matchTemplate( src, templ, ftmp[i], i);
    cv::normalize(ftmp[i],ftmp[i],1,0,cv::NORM_MINMAX);
  }

  // Display
  //
  cv::imshow( "Template", templ );
  cv::imshow( "Image", src );
  cv::imshow("SQDIFF", ftmp[0] );
  cv::imshow("SQDIFF_NORMED", ftmp[1] );
  cv::imshow("CCORR", ftmp[2] );
  cv::imshow("CCORR_NORMED", ftmp[3] );
  cv::imshow("CCOEFF", ftmp[4] );
  cv::imshow("CCOEFF_NORMED", ftmp[5] );

  // Let user view results:
  //
  cv::waitKey(0);
}

