// interessting asio info from
// http://en.highscore.de/cpp/boost/asio.html


// image processing based on
// https://github.com/bakercp/ofxIpVideoGrabber


#include "AsyncStream.h"
#include <fstream>
#include <ostream>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "cinder/Utilities.h"

using namespace boost::asio::ip;
using namespace ci;


const int BUFFER_SIZE = 512000;

void AsyncStream::connect_handler(const boost::system::error_code &ec)
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
                
        boost::asio::write(*sock, request);

//        boost::asio::async_read(*sock, boost::asio::buffer(readBuffer),
  //                              boost::bind(&AsyncStream::read_handler, this,boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        
        sock->async_read_some(boost::asio::buffer(readBuffer), boost::bind(&AsyncStream::read_handler, this,boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
//        
    }
}

void AsyncStream::resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it)
{
    if (!ec)
    {
        sock->async_connect(*it,boost::bind(&AsyncStream::connect_handler, this, boost::asio::placeholders::error));
    }
}

void AsyncStream::read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred)
{
    std::cout << "read\n";
    if (!ec)
    {
        
        for (int i=0; i < bytes_transferred; ++i) {
            bool resetBuffer = false;

            imageBuffer[imageBufferIndex] = readBuffer.data()[i];
            
                  if(imageBufferIndex > 0) {                                          
                            if(mode == MODE_HEADER && imageBuffer[imageBufferIndex-1] == '\r' && imageBuffer[imageBufferIndex] == '\n') {
                                    
                                    //std::cout << "newline \n\n";
               					//if(imageBufferIndex > 1) {
//               						imageBuffer[imageBufferIndex-1] = 0; // NULL terminator
//                                    std::string line(imageBuffer);
//               
//               						if(boost::iequals("--boundary",line)) {
//               							std::cout << "new marker\n";
//               
//               							mode = MODE_HEADER;
//               						}
//               
//               					}
                                    resetBuffer = true; // reset upon new line
                                    mode = MODE_HEADER;
               
               				}else{
             
             					if(imageBuffer[imageBufferIndex-1] == JFF){
                                   // std::cout << " JFF \n";

               						if(imageBuffer[imageBufferIndex] == SOI ) {
               							mode = MODE_JPEG;
               
               							//std::cout << "\n\nstart new frame\n";
               						}else if(imageBuffer[imageBufferIndex] == EOI ) {
                                        
               							Buffer buffer( imageBuffer, imageBufferIndex );
               							pixels = Surface( loadImage( DataSourceBuffer::create(buffer), ImageSource::Options(), "jpg" ) );
                                        isDataReady = true;

//                                           							std::string fileName = "/Users/kris/test/" + toString(++testIndex) +  ".jpg";
//                                        std::ofstream file(fileName.c_str(),std::ios::binary);
//                                           							file.write(imageBuffer,imageBufferIndex);
//                                           							file .close();
               							
                                        
               							mode = MODE_HEADER;
               							resetBuffer = true;                                        

               						}
               					}
                            }
               				
               			}
               			
               			if(resetBuffer) {
               				imageBufferIndex = 0;                    
               			} else {
               				imageBufferIndex++;
                            
                            if(imageBufferIndex >= BUFFER_SIZE) imageBufferIndex=0;
               			}
               			
            
        }
     //   boost::asio::async_read(*sock, boost::asio::buffer(readBuffer),
       //                         boost::bind(&AsyncStream::read_handler, this,boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

        
        
        sock->async_read_some(boost::asio::buffer(readBuffer), boost::bind(&AsyncStream::read_handler, this,boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
}

AsyncStream::AsyncStream(){
 
    
    isDataReady = false;
    mode = MODE_HEADER;
    imageBuffer = new char[BUFFER_SIZE];
    imageBufferIndex = 0;
    
    testIndex = 0;
    
    sock = new tcp::socket(io_service);
    resolver = new tcp::resolver(io_service);
    
    
    boost::asio::ip::tcp::resolver::query query("212.142.228.68", "80");
    resolver->async_resolve(query, boost::bind(&AsyncStream::resolve_handler, this,
                                               boost::asio::placeholders::error,
                                               boost::asio::placeholders::iterator));

    boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
    

}