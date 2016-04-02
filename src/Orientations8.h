/*
 * Orientations8.h
 *
 *  Created on: 30 avr. 2015
 *      Author: jfellus
 */

#ifndef ORIENTATIONS8_H_
#define ORIENTATIONS8_H_

#include <pg.h>
#include <image.h>

class Orientations8 {
public:
	Image* out; // One image per orientation bin
	OUTPUT(Image*, out)
	OUTPUT(Image, mag)

public:
	Image mag;

public:

	void init() {}

	void process(const Image& gx, const Image& gy) {
		if(!mag) {
			out = new Image[8];
			for(int ori=0; ori<8; ori++) out[ori].init(gx.w,gx.h);
			mag.init(gx.w, gx.h);
		}


		// Compute orientation bins
		for(uint ori=0; ori<8; ori++) out[ori] = 0;

#pragma omp parallel for
		for(uint i=0; i<mag.w*mag.h; i++) {
			// Compute gradient_magnitude
			mag[i] = sqrt(gx[i]*gx[i]+gy[i]*gy[i]);
			if(mag[i]<1e-20) continue;

			float angle = atan2f_mod2pi(gy[i], gx[i]) * 4 * M_1_PI; // Rescale angle in [0,8]

			int ori = ((int)floorf(angle) % 8);
			float rf = angle - ori;

			// Proportionally distribute energy in the two nearest orientation bins
			out[ori][i] = (1-rf) * mag[i];
			out[(ori+1)%8][i] = rf  * mag[i];
		}
	}


private:
	float mod2pi(float x) { while(x > 2*M_PI) x -= 2.*M_PI; while(x < 0) x += 2.*M_PI; return x;}
	float atan2f_mod2pi(float gy, float gx) {return mod2pi(atan2f(gy,gx));}
};


class OrientationBin {
public:
	int bin;

	Image out;
	OUTPUT(Image, out)

public:
	OrientationBin() { bin = 0;  }

	void init() {}

	void process(const Orientations8& o) {
		if(!out) out.init(o.out[bin].w, o.out[bin].h);
		for(uint i=o.out[bin].w*o.out[bin].h; i--;) out[i] = o.out[bin].data[i];
	}
};

#endif /* ORIENTATIONS8_H_ */
