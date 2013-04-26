//
//  jpgStream.cpp
//  JpgStream
//
//  Created by Kris Meeusen on 25/04/13.
//
//

#include "JpgStream.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "cinder/app/AppBasic.h"
#include "cinder/ImageIo.h"

#include <iostream>
#include <istream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/thread/thread.hpp>


using namespace ci;
using namespace std;
using boost::asio::ip::tcp;

unsigned int JpgStream::getContentLength(std::istream& header)
{
	//Content-Length: 3789
	
	int length = 0;
	std::string contentLength = "Content-Length: ";
	std::string line;
	while (std::getline(header, line) && line != "\r\n")
	{
		std::string name = line.substr(0, contentLength.size());
		if (name == contentLength){
			vector<string> res;
			boost::algorithm::split(res,line, boost::algorithm::is_any_of(":"));
			
			std::cout << res[1];
			length = atoi(res[1].c_str());
			// atoi(split(line, contentLength)[0].c_str());
		}
	}
	return length;
}


void JpgStream::setup(std::string url,std::string path,std::string port){
	mUrl = url;
	mPath = path;
	mPort = port;
}


void JpgStream::startStream(){
	try
	{
		
		isDataReady = false;
		
		ContentStreamMode mode = MODE_HEADER;
		
		boost::asio::io_service io_service;
		
		// Get a list of endpoints corresponding to the server name.
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(mUrl, mPort);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	
	
		
		// Try each endpoint until we successfully establish a connection.
		tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_iterator);
		
		// Form the request. We specify the "Connection: close" header so that the
		// server will close the socket after transmitting the response. This will
		// allow us to treat all data up until the EOF as the content.
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET " << mPath << " HTTP/1.1\r\n";
		request_stream << "Host: " << mUrl << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";
		
		// Send the request.
		boost::asio::write(socket, request);
		
		
		boost::system::error_code error;
		
		
		boost::asio::streambuf response;
				
		
		
		std::istream response_stream(&response);
		
		char* cBuf = new char[512000];
		
		int c=0;
		bool resetBuffer ;
		string boundary = "--boundary";
		
		while(boost::asio::read_until(socket, response, EOI)){
			//	while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) {
			//output << &response;
			
			//float time = ci::app::getElapsedSeconds();


			//boost::this_thread::sleep(boost::posix_time::seconds(1));
			
//			boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error);
			
		//	cout << ci::app::getElapsedSeconds()  << "\n";
			
			response_stream.get(cBuf[c]);
			
			resetBuffer = false;
			
			if(c > 0) {
				if(mode == MODE_HEADER && cBuf[c-1] == '\r' && cBuf[c] == '\n') {
					if(c > 1) {
						cBuf[c-1] = 0; // NULL terminator
						string line(cBuf);
						
						//cout << "line // " << line << std::endl;
						if(boost::iequals("--boundary",line)) {
							//std::cout << "new marker\n";
							
							mode = MODE_HEADER;
						}
						
					}
					resetBuffer = true; // reset upon new line
					
				}else
					
					if(cBuf[c-1] == JFF){
						if(cBuf[c] == SOI ) {
							mode = MODE_JPEG;
							
							//std::cout << "start new frame";
						}else if(cBuf[c] == EOI ) {
							//std::cout << "end old frame \n";
							mode = MODE_HEADER;
							resetBuffer = true;
							
							
							Buffer buffer( cBuf, c );
							pixels = Surface( loadImage( DataSourceBuffer::create(buffer), ImageSource::Options(), "jpg" ) );
							
//							std::string fileName = "/Users/lab101/Pictures//test/"+ toString(ci::app::getElapsedSeconds())+".jpg";
//							std::ofstream file(fileName.c_str(),ios::binary);
//							file.write(cBuf,c);
//							file .close();
							
							isDataReady = true;
						}
					}
				
				
			}
			
			if(resetBuffer) {
				c = 0;
			} else {
				c++;
			}
			
			
			
		}
		
		if (error != boost::asio::error::eof)
			throw boost::system::system_error(error);
		
		// clean up the buffer
		delete[] cBuf;
		cBuf = NULL;

	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}
	

	
	//return 0;
}
