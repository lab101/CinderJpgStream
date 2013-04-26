//
//  jpgStream.h
//  JpgStream
//
//  Created by Kris Meeusen on 25/04/13.
//
//

#ifndef __JpgStream__jpgStream__
#define __JpgStream__jpgStream__

#include <iostream>
#include "cinder/Surface.h"

static char JFF = 0xFF;
static char SOI = 0xD8;
static char EOI = 0xD9;


enum ContentStreamMode {
    MODE_HEADER = 0,
    MODE_JPEG = 1
};

class JpgStream{
private:
	unsigned int getContentLength(std::istream& header);
	std::string mUrl;
	std::string mPath;
	std::string mPort;
	
public:
	void startStream();
	ci::Surface pixels;
	bool isDataReady;
	void setup(std::string url,std::string path,std::string port);

};

#endif /* defined(__JpgStream__jpgStream__) */
