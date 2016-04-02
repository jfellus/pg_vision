/*
 * Image2Matrix.h
 *
 *  Created on: 30 mai 2015
 *      Author: jfellus
 */

#ifndef IMAGE2MATRIX_H_
#define IMAGE2MATRIX_H_

#include <pg.h>
#include <matrix.h>
#include <image.h>

class Image2Matrix {
public:
	Matrix m;
	OUTPUT(Matrix, m);

public:
	Image2Matrix() {}

	void init() {}

	void process(Image& img) {
		m.data = img.data;
		m.w = img.w;
		m.h = img.h;
		m.n = img.n;
	}
};


#endif /* IMAGE2MATRIX_H_ */
