// Example 9-9. The WxMoviePlayer object header file WxMoviePlayer.hpp
//
#include "opencv2/opencv.hpp"
#include "wx/wx.h"
#include <string>
#define TIMER_ID 0
using namespace std;
class WxMoviePlayer : public wxWindow {
  public:
    WxMoviePlayer(
      wxWindow*      parent,
      const wxPoint& pos,
      const wxSize&  size
    );
    virtual ~WxMoviePlayer() {};
    bool open( wxString file );
  private:
    cv::VideoCapture m_cap;
    cv::Mat          m_cv_img;
    wxImage          m_wx_img;
    wxBitmap         m_wx_bmp;
    wxTimer*         m_timer;
    wxWindow*        m_parent;
    void _copyImage( void );
 void OnPaint( wxPaintEvent& e );
    void OnTimer( wxTimerEvent& e );
    void OnKey(   wxKeyEvent&   e );
  protected:
    DECLARE_EVENT_TABLE();
};
