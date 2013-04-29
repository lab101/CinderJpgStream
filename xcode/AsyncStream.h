#pragma once


#include "cinder/ImageIo.h"
#include "cinder/Surface.h"

#include <boost/asio.hpp>
#include <boost/array.hpp>

#include <iostream>
#include <string>



static char JFF = 0xFF;
static char SOI = 0xD8;
static char EOI = 0xD9;


enum ContentStreamMode {
    MODE_HEADER = 0,
    MODE_JPEG = 1
};



class AsyncStream{
    
    
private:
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::resolver* resolver;//(io_service);
    boost::asio::ip::tcp::socket* sock;//(io_service);
    boost::array<char, 4096> readBuffer;

    // replace this with boost buffer later on
    char* imageBuffer;
    
    int imageBufferIndex;
    int testIndex;
    
    
    
    ContentStreamMode mode;

public:
    AsyncStream();
    
    void resolve_handler(const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator it);
    
    void connect_handler(const boost::system::error_code &ec);
    void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
    
    
    ci::Surface pixels;
	bool isDataReady;


};