/*
 * Binarize.h
 *
 *  Created on: 10 avr. 2015
 *      Author: jfellus
 */

#ifndef BINARIZE_H_
#define BINARIZE_H_

#include <pg.h>


class Binarize {
public:

	float threshold;

	Image out;
	OUTPUT(Image, out)

	PARAM(float, threshold, {});

	Binarize() { threshold = 0.5; }

	void init() {}

	void process(const Matrix& in, float threshold) {
		if(!out) out.init(in.w, in.h);
		for(uint i=out.n; i--;) out[i] = (in[i] >= threshold);
	}

	void process(const Matrix& in) { process(in, threshold); }

	void process(const Image& in, float threshold) {
		if(!out) out.init(in.w, in.h);
		for(uint i=out.n; i--;) out[i] = (in[i] >= threshold);
	}

	void process(const Image& in) { process(in, threshold); }

};



#endif /* BINARIZE_H_ */
