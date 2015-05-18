/*
 * RGB2lum.h
 *
 *  Created on: 30 avr. 2015
 *      Author: jfellus
 */

#ifndef RGB2LUM_H_
#define RGB2LUM_H_


#include <pg.h>
#include <image.h>

class RGB2lum {
public:
	Image out;
	OUTPUT(Image, out);

public:
	void init() {}

	void process(const ImageRGB& in) {
		if(!out) out.init(in.w, in.h);
		uint n = in.w*in.h;
		for (int i=0;i<n;i++) {
			double r,g,b,Y,L;
			r = in[3*i+0]/255.0;
			g = in[3*i+1]/255.0;
			b = in[3*i+2]/255.0;
			Y = 0.212671 * r + 0.715160 * g + 0.072169 * b;
			L = (Y>0.008856)?1.16*pow(Y,1.0/3.0)-0.16:9.033*Y;
			out[i] = (float)(L);
		}
	}
};

#endif /* RGB2LUM_H_ */
