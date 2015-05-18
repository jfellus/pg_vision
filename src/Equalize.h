/*
 * Equalize.h
 *
 *  Created on: 30 avr. 2015
 *      Author: jfellus
 */

#ifndef EQUALIZE_H_
#define EQUALIZE_H_


#include <pg.h>
#include <image.h>

class Equalize {
public:
	Image* out;
	OUTPUT(Image, *out)
public:
	void init() {}

	void process(Image& in) {
		out = &in;
		uint n = in.w*in.h;
	    float mean = 0;
		float sqr = 0;
		for (size_t i=0;i<n;i++) {
			float x = in[i];
			mean += x;
			sqr += x*x;
		}

		mean /= n;
		sqr = 2*sqrt(sqr/n - mean*mean);
		float min = mean - sqr;
		float max = mean + sqr;

		for (size_t i=0;i<n;i++) {
			float x = in[i];
			out->data[i] = ( (x-min)/(max-min) );
			if(out->data[i]<0) out->data[i] = 0;
			if(out->data[i]>1) out->data[i] = 1;
		}
	}
};



#endif /* EQUALIZE_H_ */
