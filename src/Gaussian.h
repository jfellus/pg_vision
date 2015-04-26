/*
 * Gaussian.h
 *
 *  Created on: 12 avr. 2015
 *      Author: jfellus
 */

#ifndef GAUSSIAN_H_
#define GAUSSIAN_H_

#include <pg.h>
#include <image.h>
#include "imgproc/mask.h"

class Gaussian {
public:
	float sigma;
	int radius;

	Image mask;
	OUTPUT(Image, mask)

	Gaussian() {
		sigma = 5;
		radius = 10;
	}

	void init() {
		mask = create_gaussian_mask(radius, sigma);
	}

	void process() {}
};



#endif /* GAUSSIAN_H_ */
