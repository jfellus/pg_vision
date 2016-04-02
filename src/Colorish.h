/*
 * Colorish.h
 *
 *  Created on: 3 juin 2015
 *      Author: jfellus
 */

#ifndef COLORISH_H_
#define COLORISH_H_

#include <pg.h>

class Colorish {
public:

	uint nbColors;

	ImageRGB out;
	OUTPUT(ImageRGB, out);

public:

	Colorish() { nbColors = 1; }
	void init() {}

	void process(const Image& in, const Matrix& points_colors) {
		if(!out) out.init(in.w, in.h);

		size_t jj = 0, j = 0;
		uint step = in.n/points_colors.h;
		for(uint i=0; i<points_colors.h; i++) {
			for(uint z=0; z<step; z++) {
				colorish(&out[jj+=3], in[j++], points_colors(i,0)/nbColors);
			}
		}

	}

	void process(const Image& in, const Matrix& points_colors, const Matrix& points_coordinates) {
		if(!out) out.init(in.w, in.h);
		out.clear();

#pragma omp parallel for
		for(uint i=0; i<points_colors.h; i++) {
			uint x = points_coordinates(i,0);
			uint y = points_coordinates(i,1);
			colorish(out(x,y), in(x,y), points_colors(i,0)/nbColors);
		}

	}

private:

	void colorish(unsigned char* out, float val, float hue) {
		hue_to_rgb(hue, out);
		out[0] = (unsigned char) (((255 * val) + out[0])*0.5);
		out[1] = (unsigned char) (((255 * val) + out[1])*0.5);
		out[2] = (unsigned char) (((255 * val) + out[2])*0.5);
	}



	void hue_to_rgb( float h,  unsigned char* rgb) {
		unsigned char* r = &rgb[0];
		unsigned char* g = &rgb[1];
		unsigned char* b = &rgb[2];
		h *= 6;			// sector 0 to 5
		int i = floor( h );
		float f = h - i;			// factorial part of h
		float p = 0;
		float q = (1 - f)*255;
		float t = f*255;
		switch( i ) {
			case 0:
				*r = 255;
				*g = t;
				*b = p;
				break;
			case 1:
				*r = q;
				*g = 255;
				*b = p;
				break;
			case 2:
				*r = p;
				*g = 255;
				*b = t;
				break;
			case 3:
				*r = p;
				*g = q;
				*b = 255;
				break;
			case 4:
				*r = t;
				*g = p;
				*b = 255;
				break;
			default:		// case 5:
				*r = 255;
				*g = p;
				*b = q;
				break;
		}
	}
};



#endif /* COLORISH_H_ */
