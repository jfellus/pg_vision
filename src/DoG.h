/*
 * DoG.h
 *
 *  Created on: 12 avr. 2015
 *      Author: jfellus
 */

#ifndef DOG_H_
#define DOG_H_

#include <pg.h>
#include <image.h>
#include "imgproc/mask.h"

class DoG {
public:
	float theta1, theta2;
	int radius;

	Image mask;
	OUTPUT(Image, mask)

	DoG() {
		theta1 = 5;
		theta2 = 2;
		radius = 10;
	}

	void init() {
		mask = create_DoG_mask(radius, theta2, theta1);
	}

	void process() {}
};



#endif /* DOG_H_ */
