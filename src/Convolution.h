/*
 * Convolution.h
 *
 *  Created on: 12 avr. 2015
 *      Author: jfellus
 */

#ifndef PG_CONVOLUTION_H_
#define PG_CONVOLUTION_H_

#include <pg.h>
#include <image.h>
#include "imgproc/convolution.h"

class Convolution {
public:
	Image out;
	OUTPUT(Image, out)

public:

	void init() {}

	void process(const Image& in, const Image& mask) {
		if(!out) out.init(in.w, in.h);
		convolution(in, mask, out);
	}

};



#endif /* PG_CONVOLUTION_H_ */
