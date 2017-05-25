//Example 9-6. The QMoviePlayer object header file QMoviePlayer.hpp
//
#include "ui_QMoviePlayer.h"
#include <opencv2/opencv.hpp>
#include <string>
using namespace std;
class QMoviePlayer : public QWidget {
  Q_OBJECT;
  public:
  QMoviePlayer( QWidget *parent = NULL );
  virtual ~QMoviePlayer() {;}
  bool open( string file );
  private:
  Ui::QMoviePlayer  ui;
  cv::VideoCapture m_cap;
  QImage  m_qt_img;
  cv::Mat m_cv_img;
  QTimer* m_timer;
  void paintEvent( QPaintEvent* q );
  void _copyImage( void );
  public slots:
  void nextFrame();
};
