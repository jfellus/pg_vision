/*
 * Grayscale.h
 *
 *  Created on: 10 avr. 2015
 *      Author: jfellus
 */

#ifndef GRAYSCALE_H_
#define GRAYSCALE_H_

#include <pg.h>
#include <image.h>

class Grayscale {
public:
	Image out;

	OUTPUT(Image, out)
public:
	void init() {}

	void process(const ImageRGB& in) {
		if(!out) out.init(in.w, in.h);
		for(uint i = out.n-1; i--; ) out[i] = (in[i*3]+in[i*3+1]+in[i*3+2])/(255*3.0);
	}
};



#endif /* GRAYSCALE_H_ */
