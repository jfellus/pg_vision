/*
 * GaussianSupersample.h
 *
 *  Created on: 3 juin 2015
 *      Author: jfellus
 */

#ifndef GAUSSIANSUPERSAMPLE_H_
#define GAUSSIANSUPERSAMPLE_H_

#include <pg.h>

class GaussianSupersample {
public:

	float sigma;
	int maskSize;
	int factor;

	Image out;
	OUTPUT(Image, out);
	PARAM(float, sigma, {});

public:

	GaussianSupersample() {
		factor = 2;
		maskSize = 10;
		sigma = 4;
	}

	void init() {

	}

//#define ENERGY_DECAY(e,sigma)  e -= sigma*(1.001-e)*(e+0.001);
#define ENERGY_DECAY(e,sigma)  e *= sigma;
#define TRIM_ENERGY(e)  if(e<0) e = 0; if(e>1) e=1

	void process(const Image& in) {
		if(!out) out.init(in.w*factor, in.h*factor);
		out = 0;

		// 1) Copy
		for(uint y = 0; y<in.h; y++) {
			for(uint x = 0; x<in.w; x++) {
				out(x*factor,y*factor) = in(x,y);
			}
		}

		// 2) Blur X
		for(uint y = 0; y<out.h; y++) {
			float energy = 0;
			for(uint x = 0; x<out.w; x++) {
				energy += in(x/factor,y/factor);
				TRIM_ENERGY(energy);
				out(x,y) = energy;
				ENERGY_DECAY(energy, sigma);
			}
		}
		for(uint y = 0; y<out.h; y++) {
			float energy = 0;
			for(uint x = out.w; x--; ) {
				energy += out(x,y);
				TRIM_ENERGY(energy);
				out(x,y) = energy;
				ENERGY_DECAY(energy, sigma);
			}
		}


		// 3) Blur Y
		for(uint x = 0; x<out.w; x++) {
			float energy = 0;
			for(uint y = 0; y<out.h; y++) {
				energy += out(x,y);
				TRIM_ENERGY(energy);
				out(x,y) = energy;
				ENERGY_DECAY(energy ,sigma*1.2);
			}
		}
		for(uint x = 0; x<out.w; x++) {
			float energy = 0;
			for(uint y = out.h; y--; ) {
				energy += out(x,y);
				TRIM_ENERGY(energy);
				out(x,y) = energy;
				ENERGY_DECAY(energy, sigma*1.2);
			}
		}

	}

};




#endif /* GAUSSIANSUPERSAMPLE_H_ */
