/*
 * Patches.h
 *
 *  Created on: 3 juin 2015
 *      Author: jfellus
 */

#ifndef PATCHES_H_
#define PATCHES_H_


#include <pg.h>


class Patches {
public:

	int w,h;
	int step;

	Matrix out;

	OUTPUT(Matrix,out);

public:

	Patches() {
		w = h = 28;
		step = 20;
	}

	void init() {}

	void process(const ImageRGB& in) {
		if(!out) out.init(in.w/step*in.h/step, w*h*3);

		size_t i = 0;
#pragma omp parallel for
		for(uint y = 0; y<in.h; y+=step) {
			for(uint x = 0; x<in.w; x+=step) {
				for(uint yy = 0; yy<h; yy++) {
					for(uint xx = 0; xx<w; xx++) {
						if(x+xx<0 || x+xx>=in.w || y+yy<0 || y+yy>=in.h) {
							out.get_row(i)[(yy*w+xx)*3] = 0;
							out.get_row(i)[(yy*w+xx)*3+1] = 0;
							out.get_row(i)[(yy*w+xx)*3+2] = 0;
						}
						const unsigned char* c = in(x+xx,y+yy);
						out.get_row(i)[(yy*w+xx)*3] = c[0]/255.0;
						out.get_row(i)[(yy*w+xx)*3+1] = c[1]/255.0;
						out.get_row(i)[(yy*w+xx)*3+2] = c[2]/255.0;
					}
				}
				i++;
			}
		}

	}
};


#endif /* PATCHES_H_ */
