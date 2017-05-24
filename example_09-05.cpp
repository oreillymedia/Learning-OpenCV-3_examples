//Example 9-5. An example program ch4_qt.cpp, which takes a single argument
//indicating a video file; that video file will be replayed inside of a Qt object that we will
//define, called QMoviePlayer
//
#include <QApplication>
#include <QLabel>
#include <QMoviePlayer.hpp>
int main( int argc, char* argv[] ) {
  QApplication app( argc, argv );
  QMoviePlayer mp;
  mp.open( argv[1] );
  mp.show();
  return app.exec();
}
