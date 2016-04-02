/*
 * HoG.h
 *
 *  Created on: 30 avr. 2015
 *      Author: jfellus
 */

#ifndef HOG_H_
#define HOG_H_



#include <pg.h>
#include <matrix.h>
#include <image.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#define M_1_PI         (1.0/M_PI)
#endif




class HoG {
public:
	uint step;
	uint scale;
	float clamp;
	float threshold_energy;
	uint nbMax;
	bool bKeepNull;
	bool bKeepCoords;

	Matrix out;
	Matrix coordinates;
	OUTPUT(Matrix, out)
	OUTPUT(float, threshold_energy)

private:
	uint w,h;
	uint rwidth, rheight;

public:
	HoG() {
		threshold_energy = 1;
		bKeepCoords = false;
		bKeepNull = false;
		step = 2;
		scale = 4;
		clamp = 0.2f;

		nbMax = 0;
		rwidth = rheight = 0;
		w = h = 0;
	}

	void init() {}

	void process(Image orientations[8]) {
		if(!out) {
			w = orientations[0].w;
			h = orientations[0].h;
			if(!nbMax) nbMax = get_nb_hogs(w,h);
			out.init(nbMax, 128);
			if(bKeepCoords) coordinates.init(nbMax, 2);

			rwidth = w - 1 - scale*4;
			rheight = h - 1 - scale*4;
		}

		compute_orientation_integral(orientations);

		out.set_height(0);
		coordinates.set_height(0);
		if(bKeepNull) out.clear();

//#pragma omp parallel for
		for(uint y = 1 ; y < rheight ; y+=step) {
			for(uint x = 1 ; x < rwidth ; x+=step) {
				if(out.h<nbMax)
					if(extract_hog(x,y, orientations, out.get_row(out.h)) || bKeepNull) {
						out.set_height(out.h+1);
						if(bKeepCoords) {
							coordinates(coordinates.h, 0) = x + scale*2;
							coordinates(coordinates.h, 1) = y + scale*2;
							coordinates.set_height(coordinates.h+1);
						}
					}
			}
		}

	}


private:

	uint get_nb_hogs(uint w, uint h) {
		// width - 2*1px (border) - 1 HoG
		size_t nb_w = (w  -2 - 4*scale)/ step + 1;
		size_t nb_h = (h -2 - 4*scale)/ step + 1;
		if(h < 2 + 4*scale)	nb_h = 0;
		if(w < 2 + 4*scale) nb_w = 0;
		return nb_w*nb_h;
	}

	void compute_orientation_integral(Image orientations[8]) {
		// Integral image
		for(size_t o = 0 ; o < 8 ; o++) {
			float *ori = orientations[o];
			memset(ori, 0, w*sizeof(float));
			for(size_t l = 1 ; l < h ; l++) {
				ori[l*w] = 0;
				for(size_t c = 1 ; c < w ; c++) {
					double A = ori[(l-1)*w+c-1];
					double B = ori[(l-1)*w+c];
					double C = ori[l*w+c-1];
					double D = ori[l*w+c];
					ori[l*w+c] = (float)(D+B+C-A);
				}
			}
		}
	}

	bool extract_hog(int x, int y, Image orientations[8], float* out) {
		size_t di = y * w;
		size_t dj = di + x;

		// Integral image
		for(size_t o = 0 ; o < 8 ; o++) {
			float* ori = orientations[o];
			for(size_t sx = 0 ; sx < 4 ; sx++) {
				size_t dsx = dj + sx*scale*w;
				for(size_t sy = 0 ; sy < 4 ; sy++) {
					dsx = dj + sx*scale*w + sy*scale;
					double a = *(ori + dsx - w - 1);
					double b = *(ori + dsx + scale - w - 1);
					double c = *(ori + dsx + scale*w - w - 1);
					double d = *(ori + dsx + scale*w + scale - w - 1);
					out[sx*32+sy*8+o] = (float)(a+d-b-c);
				}
			}
		}

		// L2-normalize (cancel low-energy HoGs)
		float ndes = vec_n2(out, 128);
		if(ndes <= threshold_energy) return false;
		vec_div(out, ndes, 128);

		// Clamp
		for(size_t x = 0 ; x < 128 ; x++) if(out[x] > clamp) out[x] = clamp;

		// L2-normalize
		ndes = vec_n2(out, 128);
		vec_div(out, ndes, 128);
		return true;
	}




	// Vector operations

	float vec_n2p2(float* v, uint dim) {float sqr = 0;for(uint i=dim; i--;) sqr += v[i]*v[i]; return sqr;}
	float vec_n2(float* v, uint dim) {return sqrt(vec_n2p2(v,dim));}
	void vec_div(float* v, float f, uint dim) {	for(uint i=dim; i--;) v[i]/=f;	}
	void vec_sub(float* v1, float* v2, uint dim) { for(uint i=dim; i--;) v1[i]-=v2[i]; }


	// Utils

	float atan2f_mod2pi(float gx, float gy) { return atan2f(gx,gy); }
};




#endif /* HOG_H_ */
