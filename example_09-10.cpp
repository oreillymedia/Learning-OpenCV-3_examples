//Example 9-10. The WxMoviePlayer object source file WxMoviePlayer.cpp
//This file is incomplete
#include "WxMoviePlayer.hpp"
BEGIN_EVENT_TABLE( WxMoviePlayer, wxWindow )
  EVT_PAINT( WxMoviePlayer::OnPaint )
  EVT_TIMER( TIMER_ID, WxMoviePlayer::OnTimer )
  EVT_CHAR( WxMoviePlayer::OnKey )
END_EVENT_TABLE()
The first thing we do is to set up the callbacks that will be associated with individual
events. We do this through macros provided by the wxWidgets framework.17
WxMoviePlayer::WxMoviePlayer(
  wxWindow*      parent,
  const wxPoint& pos,
  const wxSize&  size
) : wxWindow( parent, -1, pos, size, wxSIMPLE_BORDER ) {
  m_timer         = NULL;
  m_parent        = parent;
}
