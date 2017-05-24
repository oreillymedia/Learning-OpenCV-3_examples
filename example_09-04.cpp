//Example 9-4. Slightly modified code from the OpenCV documentation that draws a
//cube every frame; this modified version uses the global variables rotx and roty that are
//connected to the sliders in Figure 9-6

#include <opencv2/opencv.hpp>
using namespace std;

void on_opengl( void* param ) {
  glMatrixModel( GL_MODELVIEW );
  glLoadIdentity();
  glTranslated( 0.0, 0.0, -1.0 );
  glRotatef( rotx, 1, 0, 0 );
  glRotatef( roty, 0, 1, 0 );
  glRotatef( 0, 0, 0, 1 );
  static const int coords[6][4][3] = {
    { { +1, -1, -1 }, { -1, -1, -1 }, { -1, +1, -1 }, { +1, +1, -1 } },
    { { +1, +1, -1 }, { -1, +1, -1 }, { -1, +1, +1 }, { +1, +1, +1 } },
    { { +1, -1, +1 }, { +1, -1, -1 }, { +1, +1, -1 }, { +1, +1, +1 } },
    { { -1, -1, -1 }, { -1, -1, +1 }, { -1, +1, +1 }, { -1, +1, -1 } },
    { { +1, -1, +1 }, { -1, -1, +1 }, { -1, -1, -1 }, { +1, -1, -1 } },
    { { -1, -1, +1 }, { +1, -1, +1 }, { +1, +1, +1 }, { -1, +1, +1 } }
  };
  for (int i = 0; i < 6; ++i) {
    glColor3ub( i*20, 100+i*10, i*42 );
    glBegin(GL_QUADS);
    for (int j = 0; j < 4; ++j) {
      glVertex3d(
        0.2 * coords[i][j][0],
        0.2 * coords[i][j][1],
        0.2 * coords[i][j][2]
      );
    }
    glEnd();
  }
}
