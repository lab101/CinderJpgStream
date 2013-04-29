#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "cinder/gl/Texture.h"

#include <iostream>
#include <istream>
#include <fstream>

#include <boost/thread.hpp>
#include "AsyncStream.h"

//#include <boost/date_time.hpp>


#include "JpgStream.h"


#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <string>

boost::asio::io_service io_service;
boost::asio::ip::tcp::resolver resolver(io_service);
boost::asio::ip::tcp::socket sock(io_service);
boost::array<char, 4096> buffer;

using namespace ci;
using namespace ci::app;
using namespace std;



class JpgStreamApp : public AppBasic {
  public:

    //boost::asio::io_service io_service;
    AsyncStream asyncStream;

	gl::Texture texture;
	
	void setup();
	void mouseDown( MouseEvent event );
	void update();
	void draw();
};





//http://148.61.142.228/axis-cgi/mjpg/video.cgi


//


void handler(const boost::system::error_code &ec)
{
    std::cout << "5 s." << std::endl;
}



void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred)
{
    if (!ec)
    {
      //  std::cout << std::string(buffer.data(), bytes_transferred) << std::endl;
        sock.async_read_some(boost::asio::buffer(buffer), read_handler);
    }
}

void connect_handler(const boost::system::error_code &ec)
{
    if (!ec)
    {
        
        std::string mPath = "/axis-cgi/mjpg/video.cgi";
        std::string mUrl = "212.142.228.68";
        
        boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET " << mPath << " HTTP/1.1\r\n";
		request_stream << "Host: " << mUrl << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: keep-alive\r\n\r\n";

      //  boost::asio::write(sock, boost::asio::buffer("GET /axis-cgi/mjpg/video.cgi HTTP 1.1\r\nHost: 212.142.228.68\r\n\r\n"));
        
        boost::asio::write(sock, request);

        sock.async_read_some(boost::asio::buffer(buffer), read_handler);
    }
}

void resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it)
{
    if (!ec)
    {
        sock.async_connect(*it, connect_handler);
    } 
}

void run1()
{
    io_service.run();
}


void JpgStreamApp::setup()
{
	
//	stream1.setup("212.142.228.68","/axis-cgi/mjpg/video.cgi","80");
//	stream1.setup("148.61.142.228","/axis-cgi/mjpg/video.cgi","80");
//	stream1.setup("76.10.86.11","/axis-cgi/mjpg/video.cgi?resolution=4CIF&camera=3","80");
//	stream1.setup("10.200.10.113","/","8080");
//	stream1.startStream();
	
   // boost::thread t(&JpgStream::startStream, &stream1);
    
    
    
    
    try
    {
        
        
//        if (argc != 3)
//        {
//            std::cout << "Usage: async_client <server> <path>\n";
//            std::cout << "Example:\n";
//            std::cout << "  async_client www.boost.org /LICENSE_1_0.txt\n";
//            return 1;
//        }
        
     //   boost::asio::io_service io_service;
    //    client c(io_service, "212.142.228.68", "/axis-cgi/mjpg/video.cgi");
//         c = new client(io_service, "www.boost.org", "/LICENSE_1_0.txt");



       
//        boost::asio::deadline_timer timer(io_service, boost::posix_time::seconds(15));
//        timer.async_wait(handler);
//        
//        boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
//        
//        t.join();

//
  //      io_service.run();
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }
    

}

void JpgStreamApp::mouseDown( MouseEvent event )
{
}

void JpgStreamApp::update()
{
	if(asyncStream.isDataReady){
		texture =  gl::Texture(asyncStream.pixels);
		asyncStream.isDataReady = false;
		//cout << "dataready";
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
