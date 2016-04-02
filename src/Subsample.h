/*
 * Subsample.h
 *
 *  Created on: 1 mai 2015
 *      Author: jfellus
 */

#ifndef SUBSAMPLE_H_
#define SUBSAMPLE_H_


class Subsample {
public:
	uint factor;

	ImageRGB out;
	OUTPUT(ImageRGB, out)

public:

	Subsample() {
		factor = 2;
	}

	void init() {}

	void process(const ImageRGB& in) {
		if(!out) out.init(in.w/factor, in.h/factor);
		for(uint y=0; y<out.h; y++) {
			for(uint x=0; x<out.w; x++) {
				float r=0,g=0,b=0;
				for(uint i=0; i<factor; i++) {
					for(uint j=0; j<factor; j++) {
						r += in(x*factor+j,y*factor+i)[0];
						g += in(x*factor+j,y*factor+i)[1];
						b += in(x*factor+j,y*factor+i)[2];
					}
				}
				out(x,y)[0] = r/(factor*factor);
				out(x,y)[1] = g/(factor*factor);
				out(x,y)[2] = b/(factor*factor);
			}
		}
	}


};

class SubsampleImage {
public:
	uint factor;

	Image out;
	OUTPUT(Image, out)

public:

	SubsampleImage() {
		factor = 2;
	}

	void init() {}

	void process(const Image& in) {
		if(!out) out.init(in.w/factor, in.h/factor);
		for(uint y=0; y<out.h; y++) {
			for(uint x=0; x<out.w; x++) {
				float r=0,g=0,b=0;
				for(uint i=0; i<factor; i++) {
					for(uint j=0; j<factor; j++) {
						r += in(x*factor+j,y*factor+i);
					}
				}
				out(x,y) = r/(factor*factor);
			}
		}
	}


};



class Supersample {
public:
	uint factor;

	ImageRGB out;
	OUTPUT(ImageRGB, out)

public:

	Supersample() {
		factor = 2;
	}

	void init() {}

	void process(const ImageRGB& in) {
		if(!out) out.init(in.w*factor, in.h*factor);
		for(uint y=0; y<out.h; y++) {
			for(uint x=0; x<out.w; x++) {
				int xx = x/factor;
				int yy = y/factor;
				out(x,y)[0] = in(xx,yy)[0];
				out(x,y)[1] = in(xx,yy)[1];
				out(x,y)[2] = in(xx,yy)[2];
			}
		}
	}


};


class SupersampleImage {
public:
	uint factor;

	Image out;
	OUTPUT(Image, out)

public:

	SupersampleImage() {
		factor = 2;
	}

	void init() {}

	void process(const Image& in) {
		if(!out) out.init(in.w*factor, in.h*factor);
		for(uint y=0; y<out.h; y++) {
			for(uint x=0; x<out.w; x++) {
				int xx = x/factor;
				int yy = y/factor;
				out(x,y) = in(xx,yy);
			}
		}
	}


};


#endif /* SUBSAMPLE_H_ */
