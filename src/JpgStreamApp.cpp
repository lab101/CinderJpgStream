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

	gl::Texture texture;
	
	void setup();
	void mouseDown( MouseEvent event );
	void update();
	void draw();
};


void JpgStreamApp::setup()
{
}

void JpgStreamApp::mouseDown( MouseEvent event )
{
}

void JpgStreamApp::update()
{
	if(asyncStream.isDataReady){
		texture =  gl::Texture(asyncStream.pixels);
		asyncStream.isDataReady = false;
	}
}

void JpgStreamApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	if(texture)
		gl::draw(texture,Rectf(0,0,getWindowWidth(),getWindowHeight()));
}

CINDER_APP_BASIC( JpgStreamApp, RendererGl )
