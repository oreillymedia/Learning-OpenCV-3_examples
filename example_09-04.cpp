//Example 9-4. Slightly modified code from the OpenCV documentation that draws a
//cube every frame; this modified version uses the global variables rotx and roty that are
//connected to the sliders in Figure 9-6
// Note: This example needs OpenGL installed on your system. It doesn't build if 
//       the OpenGL libraries cannot be found.
#include <GL/gl.h>
#include <GL/glu.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/opengl.hpp>
#include <iostream>

using namespace std;

int rotx = 55, roty = 40;

void on_opengl(void* param) {
    cv::ogl::Texture2D* backgroundTex = (cv::ogl::Texture2D*)param;
    glEnable( GL_TEXTURE_2D );
    backgroundTex->bind();
    cv::ogl::render(*backgroundTex);
    glDisable( GL_TEXTURE_2D );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef(0, 0, -0.5);
    glRotatef( rotx, 1, 0, 0 );
    glRotatef( roty, 0, 1, 0 );
    glRotatef( 0, 0, 0, 1 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );
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
        glBegin( GL_QUADS );
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

void on_trackbar( int, void* ) {
    cv::updateWindow( "Example 9-4" );
}

void help(char ** argv) {
	
	cout << "\n//Example 9-4. Slightly modified code from the OpenCV documentation that draws a"
		<< "\n//cube every frame; this modified version uses the global variables rotx and roty that are"
		<< "\n//connected to the sliders in Figure 9-6"
		<< "\n// Note: This example needs OpenGL installed on your system. It doesn't build if" 
		<< "\n//       the OpenGL libraries cannot be found.\n\/"
		<< "\nCall: " << argv[0] << " <image>\n\n"
		<< "\nHere OpenGL is used to render a cube on top of an image.\n"
        << "\nUser can rotate the cube with the sliders\n" <<endl;
}

int main(int argc, char* argv[])
{
    if(argc != 2) {
        help (argv);
        return -1;
    }

    cv::Mat img = cv::imread(argv[1]);
    if( img.empty() ) {
        cout << "Cannot load " << argv[1] << endl;
        return -1;
    }

    cv::namedWindow( "Example 9-4", CV_WINDOW_OPENGL );
    cv::resizeWindow("Example 9-4", img.cols, img.rows);
    cv::createTrackbar( "X-rotation", "Example 9-4", &rotx, 360, on_trackbar);
    cv::createTrackbar( "Y-rotation", "Example 9-4", &roty, 360, on_trackbar);

    cv::ogl::Texture2D backgroundTex(img);
    cv::setOpenGlDrawCallback("Example 9-4", on_opengl, &backgroundTex);
    cv::updateWindow("Example 9-4");

    cv::waitKey(0);

    cv::setOpenGlDrawCallback("Example 9-4", 0, 0);
    cv::destroyAllWindows();

    return 0;
}
