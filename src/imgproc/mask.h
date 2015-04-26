/*
 * mask.h
 *
 *  Created on: 11 avr. 2015
 *      Author: jfellus
 */

#ifndef MASK_H_
#define MASK_H_

Image create_gaussian_mask(int l, float sigma) {
	uint size = l*2+1;
	Image mask(size, size);

	float a3 = (2. * sigma * sigma);
	float a1 = 1. / (2. * M_PI * sigma * sigma);

	float energy = 0;
	for (int j = -l ; j < l; j++) {
		for (int i = -l; i < l; i++) {
			float d = (i * i + j * j);
			float d1 = exp(-d / a3);
			energy += mask[(j+l) + (i+l)*mask.w] = (a1 * d1);
		}
	}

	mask /= energy;

	mask.dump();

	return mask;
}


Image create_DoG_mask(int l, float theta2, float theta1) {
	uint size = l*2+1;
	Image mask(size, size);

	float a3 = (2. * theta1 * theta1);
	float a4 = (2. * theta2 * theta2);
	float a1 = 1. / (2. * M_PI * theta1 * theta1);
	float a2 = 1. / (2. * M_PI * theta2 * theta2);

	float energy = 0;
	for (int j = -l ; j < l; j++) {
		for (int i = -l; i < l; i++) {
			float d = (i * i + j * j);
			float d1 = exp(-d / a3);
			float d2 = exp(-d / a4);
			float v = (a1 * d1 - a2 * d2);
			mask[(j+l) + (i+l)*mask.w] = v;
			energy += fabs(v);
		}
	}

	//mask /= energy;

	return mask;
}

float get_mask_max_out(const Image& mask) {
	float m = 0;
	for(uint i=mask.n; i--;) if(mask[i]>0) m+= mask[i];
	return m;
}

float get_mask_min_out(const Image& mask) {
	float m = 0;
	for(uint i=mask.n; i--;) if(mask[i]<0) m+= mask[i];
	return m;
}

#endif /* MASK_H_ */
