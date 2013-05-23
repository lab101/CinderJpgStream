#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "cinder/gl/Texture.h"

#include "AsyncStream.h"

#include <string>

using namespace ci;
using namespace ci::app;
using namespace std;


class JpgStreamApp : public AppBasic {
  public:

    AsyncStream asyncStream;
    AsyncStream asyncStream2;

	void setup();
	void mouseDown( MouseEvent event );
	void update();
	void draw();

};


void JpgStreamApp::setup()
{

	setWindowSize(640*2, 500);
	
	asyncStream.setUrl("http://212.142.228.68/axis-cgi/mjpg/video.cgi?resolution=640x480");
	asyncStream.start();

	asyncStream2.setUrl("http://134.29.208.43/axis-cgi/mjpg/video.cgi?resolution=640x480");
	asyncStream2.start();
	

}

void JpgStreamApp::mouseDown( MouseEvent event )
{
	
	// not working
	// can only stop thread no restarting yet
	
//	if(asyncStream.isRunning)
//		asyncStream.stop();
//	else{
//		asyncStream.start();
//	}
}

void JpgStreamApp::update()
{
	asyncStream.update();
	asyncStream2.update();

}

void JpgStreamApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	if(asyncStream.texture) gl::draw(asyncStream.texture,Rectf(0,0,640,480));
	if(asyncStream2.texture) gl::draw(asyncStream2.texture,Rectf(640,0,640*2,480));
}

CINDER_APP_BASIC( JpgStreamApp, RendererGl )
