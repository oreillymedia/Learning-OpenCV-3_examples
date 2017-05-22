// Example 13-2. Creating signatures from histograms for EMD; note that this code is the
// source of the data in Table 13-1, in which the hand histogram is compared in different
// lighting conditions

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;

void help( char** argv ){
  cout << "\nCall is:\n"
  << argv[0] <<" modelImage0 testImage1 testImage2 badImage3\n\n"
  << "for example: " << argv[0]
  << " HandIndoorColor.jpg HandOutdoorColor.jpg "
  << "HandOutdoorSunColor.jpg fruits.jpg\n"
  << "\n";
}

// Compare 3 images' histograms
int main( int argc, char** argv ) {
  if( argc != 5 ) { help( argv ); return -1; }
  vector<cv::Mat> src(5);
  cv::Mat tmp;
  int i;

  tmp = cv::imread( argv[1], 1);
  if( tmp.empty() ) {
    cerr << "Error on reading image 1," << argv[1] << "\n" << endl;
    help( argv );
    return(-1);
  }

  // Parse the first image into two image halves divided halfway on y
  //
  cv::Size size = tmp.size();
  int width = size.width;
  int height = size.height;
  int halfheight = height >> 1;

  cout <<"Getting size [[" <<tmp.cols <<"] [" <<tmp.rows <<"]]\n" <<endl;
  cout <<"Got size (w,h): (" <<size.width <<"," <<size.height <<")" <<endl;

  src[0] = cv::Mat(cv::Size(width,halfheight), CV_8UC3);
  src[1] = cv::Mat(cv::Size(width,halfheight), CV_8UC3);

  // Divide the first image into top and bottom halves into src[0] and src[1]
  //
  cv::Mat_<cv::Vec3b>::iterator tmpit = tmp.begin<cv::Vec3b>();

  // top half
  //
  cv::Mat_<cv::Vec3b>::iterator s0it = src[0].begin<cv::Vec3b>();
  for(i = 0; i < width*halfheight; ++i, ++tmpit, ++s0it) *s0it = *tmpit;

  // Bottom half
  //
  cv::Mat_<cv::Vec3b>::iterator s1it = src[1].begin<cv::Vec3b>();
  for(i = 0; i < width*halfheight; ++i, ++tmpit, ++s1it) *s1it = *tmpit;

  // Load the other three images
  //
  for(i = 2; i<5; ++i){
    src[i] = cv::imread(argv[i], 1);
    if(src[i].empty()) {
      cerr << "Error on reading image " << i << ": " << argv[i] << "\n" << endl;
      help( argv );
      return(-1);
    }
  }

  // Compute the HSV image, and decompose it into separate planes.
  //
  vector<cv::Mat> hsv(5), hist(5), hist_img(5);
  int h_bins = 8;
  int s_bins = 8;
  int hist_size[] = { h_bins, s_bins }, ch[] = {0, 1};
  float h_ranges[] = { 0, 180 }; // hue range is [0,180]
  float s_ranges[] = { 0, 255 };
  const float* ranges[] = { h_ranges, s_ranges };
  int scale = 10;

  for(i = 0; i<5; ++i) {
    cv::cvtColor( src[i], hsv[i], cv::COLOR_BGR2HSV );
    cv::calcHist( &hsv[i], 1, ch, cv::noArray(), hist[i], 2, hist_size, ranges, true );
    cv::normalize( hist[i], hist[i], 0, 255, cv::NORM_MINMAX );
    hist_img[i] = cv::Mat::zeros( hist_size[0]*scale, hist_size[1]*scale, CV_8UC3 );

    // Draw our histogram For the 5 images
    //
    for( int h = 0; h < hist_size[0]; h++ )
      for( int s = 0; s < hist_size[1]; s++ ) {
	float hval = hist[i].at<float>(h, s);
	cv::rectangle(
	  hist_img[i],
	  cv::Rect(h*scale, s*scale, scale, scale),
	  cv::Scalar::all(hval),
	  -1
	);
      }
  }

  // Display
  //
  cv::namedWindow( "Source0", 1 );cv::imshow( "Source0", src[0] );
  cv::namedWindow( "HS Histogram0", 1 );cv::imshow( "HS Histogram0", hist_img[0] );

  cv::namedWindow( "Source1", 1 );cv::imshow( "Source1", src[1] );
  cv::namedWindow( "HS Histogram1", 1 ); cv::imshow( "HS Histogram1", hist_img[1] );

  cv::namedWindow( "Source2", 1 ); cv::imshow( "Source2", src[2] );
  cv::namedWindow( "HS Histogram2", 1 ); cv::imshow( "HS Histogram2", hist_img[2] );

  cv::namedWindow( "Source3", 1 ); cv::imshow( "Source3", src[3] );
  cv::namedWindow( "HS Histogram3", 1 ); cv::imshow( "HS Histogram3", hist_img[3] );

  cv::namedWindow( "Source4", 1 ); cv::imshow( "Source4", src[4] );
  cv::namedWindow( "HS Histogram4", 1 ); cv::imshow( "HS Histogram4", hist_img[4] );

  // Compare the histogram src0 vs 1, vs 2, vs 3, vs 4
  cout << "Comparison:\n"
  << "Corr                Chi                Intersect                Bhat\n"<< endl;
  
  for(i=1; i<5; ++i) { // For each histogram
    cout << "Hist[0] vs Hist[" << i << "]: " << endl;;
    for(int j=0; j<4; ++j) { // For each comparison type
      cout << "method[" << j << "]: " << cv::compareHist(hist[0],hist[i],j) << " ";
    }
    cout << endl;
  }

  //Do EMD and report
  //
  vector<cv::Mat> sig(5);
  cout << "\nEMD: " << endl;

  // Oi Vey, parse histograms to earth movers signatures
  //
  for( i=0; i<5; ++i) {

    vector<cv::Vec3f> sigv;

    // (re)normalize histogram to make the bin weights sum to 1.
    //
    cv::normalize(hist[i], hist[i], 1, 0, cv::NORM_L1);
    for( int h = 0; h < h_bins; h++ )
      for( int s = 0; s < s_bins; s++ ) {
	float bin_val = hist[i].at<float>(h, s);
	if( bin_val != 0 )
	  sigv.push_back( cv::Vec3f(bin_val, (float)h, (float)s));
      }

    // make Nx3 32fC1 matrix, where N is the number of nonzero histogram bins
    //
    sig[i] = cv::Mat(sigv).clone().reshape(1);
    if( i > 0 )
      cout << "Hist[0] vs Hist[" << i << "]: "
	   << EMD(sig[0], sig[i], cv::DIST_L2) << endl;
  }

  cv::waitKey(0);

}
