/*
 * ImageExtractChannel.h
 *
 *  Created on: 10 avr. 2015
 *      Author: jfellus
 */

#ifndef IMAGEEXTRACTCHANNEL_H_
#define IMAGEEXTRACTCHANNEL_H_

#include <pg.h>
#include <image.h>

class ImageExtractChannel {
public:
	char channel; // in {'R','G','B'}

	Image out;

	OUTPUT(Image, out)

private:
	int offset;

	ImageExtractChannel() { channel = 'R'; offset = 0; }

	void init() {
		if(channel == 'R') offset = 0;
		else if(channel == 'G') offset = 1;
		else offset = 2;
	}

	void process(const ImageRGB& in) {
		if(!out) out.init(in.w, in.h);
		for(uint i=out.n-1; i--; ) out[i] = in[i*3 + offset];
	}

	void process(const ImageRGB& in, int offset) {
		if(!out) out.init(in.w, in.h);
		for(uint i=out.n-1; i--; ) out[i] = in[i*3 + offset];
	}

};



class ImageExtractR {
public:
	Image out;
	OUTPUT(Image, out)

	ImageExtractR() {}
	void init() {}

	void process(const ImageRGB& in) {
		if(!out) out.init(in.w, in.h);
		for(uint i=out.n-1; i--; ) out[i] = in[i*3]/255.0;
	}
};

class ImageExtractG {
public:
	Image out;
	OUTPUT(Image, out)

	ImageExtractG() {}
	void init() {}

	void process(const ImageRGB& in) {
		if(!out) out.init(in.w, in.h);
		for(uint i=out.n-1; i--; ) out[i] = in[i*3+1]/255.0;
	}
};

class ImageExtractB {
public:
	Image out;
	OUTPUT(Image, out)

	ImageExtractB() {}
	void init() {}

	void process(const ImageRGB& in) {
		if(!out) out.init(in.w, in.h);
		for(uint i=out.n-1; i--; ) out[i] = in[i*3+2]/255.0;
	}
};


#endif /* IMAGEEXTRACTCHANNEL_H_ */
