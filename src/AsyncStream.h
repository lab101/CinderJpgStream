#pragma once


#include "cinder/ImageIo.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>
#include <string>



static char JFF = 0xFF;
static char SOI = 0xD8;
static char EOI = 0xD9;


enum ContentStreamMode {
    MODE_HEADER = 0,
    MODE_JPEG = 1
};



class AsyncStream {
    
    
private:
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::resolver* resolver;//(io_service);
    boost::asio::ip::tcp::socket* sock;//(io_service);
    boost::array<char, 4096> readBuffer;
	
	boost::shared_ptr<boost::thread> backgroundThread;

    char* imageBuffer;
    int imageBufferIndex;
	
	
	std::string mPath;
	std::string mUrl;
	
   
    ContentStreamMode mode;

public:
    AsyncStream();
	
	void setUrl(std::string url);
	void start();
	void stop();
	void update();
	
    
    void resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it);
    
    void connect_handler(const boost::system::error_code &ec);
    void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
    
    
    ci::Surface pixels;
	ci::gl::Texture texture;
	
	bool isDataReady;
	bool isRunning;



};