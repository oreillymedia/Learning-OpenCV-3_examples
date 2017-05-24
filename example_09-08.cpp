//Example 9-8. An example program which takes a single argument
//indicating a video file; that video file will be replayed inside of a wxWidgets object that
//we will define, called WxMoviePlayer
//
#include "wx/wx.h"
#include "WxMoviePlayer.hpp"
// Application class, the top level object in wxWidgets
//
class MyApp : public wxApp {
  public:
    virtual bool OnInit();
};
// Behind the scenes stuff to create a main() function and attach MyApp
//
DECLARE_APP( MyApp );
IMPLEMENT_APP( MyApp );
// When MyApp is initialized, do these things.
//
bool MyApp::OnInit() {
  wxFrame* frame = new wxFrame( NULL, wxID_ANY, wxT("ch4_wx") );
  frame->Show( true );
  WxMoviePlayer* mp = new WxMoviePlayer(
    frame,
    wxPoint( -1, -1 ),
    wxSize( 640, 480 )
  );
  mp->open( wxString(argv[1]) );
  mp->Show( true );
  return true;
}
