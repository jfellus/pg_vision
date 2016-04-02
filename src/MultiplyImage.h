/*
 * MultiplyImage.h
 *
 *  Created on: 31 mai 2015
 *      Author: jfellus
 */

#ifndef MULTIPLYIMAGE_H_
#define MULTIPLYIMAGE_H_

#include <pg.h>

class MultiplyImageRGB {
public:
	ImageRGB out;
	OUTPUT(ImageRGB, out);

public:

	void init() {}

	void process(const ImageRGB& img, const Image& mask) {
		if(!out) out.init(img.w, img.h);
		for(uint i=mask.n; i--;) {
			out[i*3] = img[i*3]*mask[i];
			out[i*3+1] = img[i*3+1]*mask[i];
			out[i*3+2] = img[i*3+2]*mask[i];
		}
	}
};


#endif /* MULTIPLYIMAGE_H_ */
