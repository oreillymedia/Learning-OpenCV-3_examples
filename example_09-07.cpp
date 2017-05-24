//Example 9-7. The QMoviePlayer object source file: QMoviePlayer.cpp
//
#include "QMoviePlayer.hpp"
#include <QTimer>
#include <QPainter>
QMoviePlayer::QMoviePlayer( QWidget *parent )
 : QWidget( parent )
{
  ui.setupUi( this );
}
