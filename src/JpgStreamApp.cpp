#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "cinder/gl/Texture.h"

#include <iostream>
#include <istream>
#include <fstream>
#include <regex.h>

#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>


#include "JpgStream.h"


using namespace ci;
using namespace ci::app;
using namespace std;



class JpgStreamApp : public AppBasic {
  public:

	JpgStream stream1;
	

	gl::Texture texture;
	
	void setup();
	void mouseDown( MouseEvent event );
	void update();
	void draw();
};








//http://148.61.142.228/axis-cgi/mjpg/video.cgi

void JpgStreamApp::setup()
{
	
	stream1.setup("212.142.228.68","/axis-cgi/mjpg/video.cgi","80");
//	stream1.setup("10.200.10.113","/","8080");
//	stream1.startStream();
	
	boost::thread* thr = new boost::thread(boost::bind(&JpgStream::startStream, &stream1));

}

void JpgStreamApp::mouseDown( MouseEvent event )
{
}

void JpgStreamApp::update()
{
	if(stream1.isDataReady){
		texture =  gl::Texture(stream1.pixels);
		stream1.isDataReady = false;
		//cout << "dataready";
	}
}

void JpgStreamApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	if(texture)
		gl::draw(texture);
}

CINDER_APP_BASIC( JpgStreamApp, RendererGl )
