/*
 * Matrix2Image.h
 *
 *  Created on: 30 mai 2015
 *      Author: jfellus
 */

#ifndef Matrix2Image_h
#define Matrix2Image_h

#include <pg.h>
#include <matrix.h>
#include <image.h>

class Matrix2Image {
public:
	Image img;
	OUTPUT(Image, img);

public:
	Matrix2Image() {}

	void init() {}

	void process(Matrix& m) {
		img.data = m.data;
		img.w = m.w;
		img.h = m.h;
		img.n = m.n;
	}
};


#endif /* IMAGE2MATRIX_H_ */
