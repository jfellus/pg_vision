/*
 * convolution.h
 *
 *  Created on: 12 avr. 2015
 *      Author: jfellus
 */

#ifndef CONVOLUTION_H_
#define CONVOLUTION_H_



/////////////////
// CONVOLUTION //
/////////////////



float convolution(const Image& in, const Image& mask, Image& out) {
	out = 0;
	float maxval = -99999999;
	int hh = mask.h/2;
	int ww = mask.w/2;

	#pragma omp parallel for reduction (max:maxval)
	for(uint y=hh; y<out.h-hh; y++) {
		int offset_y = y*out.w;
		float* pout = &out[offset_y];
		for(uint x=ww; x<out.w-ww; x++, pout++) {
			int offset = offset_y + x;
			for(uint i=0; i<mask.h; i++) {
				const float* pmask = mask;
				const float* pin = &in[offset + (i-hh)*in.w - ww];
				for(uint j=0; j<mask.w; j++, pmask++, pin++) *pout += *pin * *pmask;
			}
			if(*pout>maxval) maxval = *pout;
		}
	}


#pragma omp parallel for
	for(uint i=0; i<out.n; i++) out[i] /= maxval;

	return maxval;
}

float convolution(const Image& in, const Image& mask, Image& out, float threshold) {
	if(threshold==0) return convolution(in, mask, out);
	float maxval = -99999999;

//	#pragma omp parallel for reduction (max:maxval)
	for(uint y=mask.h/2; y<out.h-mask.h/2; y++) {
		int offset_y = y*out.w;
		float* pout = &out[offset_y];
		for(uint x=mask.w/2; x<out.w-mask.h/2; x++, pout++) {
			int offset = offset_y + x;
			*pout = 0;
			for(uint i=0; i<mask.h; i++) {
				const float* pmask = mask;
				const float* pin = &in[offset - (i-mask.h/2)*in.w];
				for(uint j=0; j<mask.w; j++, pmask++, pin++) if(*pin>threshold) *pout += *pin * *pmask;
			}
			if(*pout>maxval) maxval = *pout;
		}
	}


//#pragma omp parallel for
	for(uint i=0; i<out.n; i++) out[i] /= maxval;

	return maxval;
}






#endif /* CONVOLUTION_H_ */
