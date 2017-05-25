//Example 9-3. Using a trackbar to create a “switch” that the user can turn on and off;
//this program plays a video and uses the switch to create a pause functionality
//
// An example program in which the user can draw boxes on the screen.
//
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
//
// Using a trackbar to create a "switch" that the user can turn on and off.
// We make this value global so everyone can see it.
//
int g_switch_value = 1;
void switch_off_function() { cout << "Pause\n"; }; //YOU COULD DO MORE
void switch_on_function()  { cout << "Run\n"; };

// This will be the callback that we give to the trackbar.
//
void switch_callback( int position, void* ) {
  if( position == 0 ) {
    switch_off_function();
  } else {
    switch_on_function();
  }
}

void help(char ** argv) {
    cout << "Call: " << argv[0] << " <my.avi>" << endl;
    cout << "Shows putting a pause button in a video; Esc to quit" << endl;
}

int main( int argc, char** argv ) {
  cv::Mat frame; // To hold movie images
  cv::VideoCapture g_capture;
  help(argv);
  if( argc < 2 || !g_capture.open( argv[1] ) ){
    cout << "Failed to open " << argv[1] << " video file\n" << endl;
    return -1;
  }

  // Name the main window
  //
  cv::namedWindow( "Example", 1 );

  // Create the trackbar. We give it a name,
  // and tell it the name of the parent window.
  //
  cv::createTrackbar(
    "Switch",
    "Example",
    &g_switch_value,
    1,
    switch_callback
  );

  // This will cause OpenCV to idle until
  // someone hits the Esc key.
  //
  for(;;) {
    if( g_switch_value ) {
          g_capture >> frame;
          if( frame.empty() ) break;
          cv::imshow( "Example", frame);
      }
      if( cv::waitKey(10)==27 ) break; //esc
  }
  return 0;

}
