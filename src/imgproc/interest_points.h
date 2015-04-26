/*
 * interest_points.h
 *
 *  Created on: 11 avr. 2015
 *      Author: jfellus
 */

#ifndef INTEREST_POINTS_H_
#define INTEREST_POINTS_H_


/////////////////
// COMPETITION //
/////////////////

inline void compute_local_maxima(const Image& in, float *out, int neighborhood, float maxval) {
	float ratio = 1/maxval;
	for (uint y = neighborhood; y < in.h - neighborhood; y++) {
		int yoffset = y*in.w;
		for(uint x = neighborhood; x < in.w - neighborhood; x++) {
			int offset = yoffset + x;
			if(in[offset]<0 || out[offset]<0) out[offset] = 0;
			else {
				for (int j = -neighborhood/2; j < neighborhood/2; j++) {
					int neigh_yoffset = offset + in.w*j;
					for (int i = -neighborhood/2; i < neighborhood/2; i++) {
						if (i | j) {
							int neigh_offset = neigh_yoffset + i;
							if (in[neigh_offset] >= in[offset]) {
								out[offset] = 0;
								goto finie2;
							} else out[neigh_offset]=-1; /* un point dans le voisinage plus faible on le met a -1 pour accelerer la suite des calculs */
						}
					}
				}
				out[offset] = in[offset] * ratio;
			}
			finie2:;
		}
	}
}












#endif /* INTEREST_POINTS_H_ */
