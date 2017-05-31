//Example 9-10. The WxMoviePlayer object source file WxMoviePlayer.cpp
//
#include "WxMoviePlayer.hpp"
BEGIN_EVENT_TABLE( WxMoviePlayer, wxWindow )
  EVT_PAINT( WxMoviePlayer::OnPaint )
  EVT_TIMER( TIMER_ID, WxMoviePlayer::OnTimer )
  EVT_CHAR( WxMoviePlayer::OnKey )
END_EVENT_TABLE()

// The first thing we do is to set up the callbacks that will be associated with individual
// events. We do this through macros provided by the wxWidgets framework.
WxMoviePlayer::WxMoviePlayer(
  wxWindow*      parent,
  const wxPoint& pos,
  const wxSize&  size
) : wxWindow( parent, -1, pos, size, wxSIMPLE_BORDER ) {
  m_timer         = NULL;
  m_parent        = parent;
}

//We will need to
//know which frame is the parent when it comes time to close the application in
//response to the Esc key.
void WxMoviePlayer::OnPaint( wxPaintEvent& event ) {
	wxPaintDC dc( this );
	if( !dc.Ok() ) return;
	int x,y,w,h;
	dc.BeginDrawing();
	dc.GetClippingBox( &x, &y, &w, &h );
	dc.DrawBitmap( m_wx_bmp, x, y );
	dc.EndDrawing();
	return;
}

//The WxMoviePlayer::_copyImage() method will get called whenever a new image is
//read from the cv::VideoCapture object.
void WxMoviePlayer::_copyImage( void ) {
	m_wx_bmp = wxBitmap( m_wx_img );
	Refresh( FALSE ); // indicate that the object is dirty
	Update();
}

//The WxMoviePlayer::open() method also does several important things. The first is
//to actually open the cv::VideoCapture object, but there is a lot more to be done.
//Next, an image is read off of the player and is used to create a wxImage object that
//“points at” the OpenCV cv::Mat image. This is the opposite philosophy to the one
//we used in the Qt example: in this case, it turns out to be a little more convenient to
//create the cv::Mat first and have it own the data, and then to create the GUI toolkit’s
//image object

bool WxMoviePlayer::open( wxString file ) {
	
	if( !m_cap.open( std::string( file.mb_str() ) )) {
	return false;
	}

	// If we opened the file, set up everything now:
	//
	m_cap.read( m_cv_img );
	m_wx_img = wxImage(
	m_cv_img.cols,
	m_cv_img.rows,
	m_cv_img.data,
	TRUE // static data, do not free on delete()
	);
	_copyImage();
	m_timer = new wxTimer( this, TIMER_ID );
	m_timer->Start( 1000. / m_cap.get( cv::CAP_PROP_FPS ) );
	return true;
}

//The following handler doesn’t do too much; primarily it just reads a new frame from the video,
//converts that frame from BGR to RGB for display, and then calls our WxMovie
//Player::_copyImage() , which makes the next bitmap for us.
void WxMoviePlayer::OnTimer( wxTimerEvent& event ) {
	if( !m_cap.isOpened() ) return;
	m_cap.read( m_cv_img );
	cv::cvtColor( m_cv_img, m_cv_img, cv::BGR2RGB );
	_copyImage();
}

//Handler for keypresses 
void WxMoviePlayer::OnKey( wxKeyEvent& e ) {
	if( e.GetKeyCode() == WXK_ESCAPE ) m_parent->Close();
}
