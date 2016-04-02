/*
 * RedishBluish.h
 *
 *  Created on: 3 juin 2015
 *      Author: jfellus
 */

#ifndef REDISHBLUISH_H_
#define REDISHBLUISH_H_

#include <pg.h>

class RedishBluish {
public:

	bool bRedish;
	ImageRGB out;
	OUTPUT(ImageRGB, out);

public:

	RedishBluish() { bRedish = true;}
	void init() {}

	void process(const Image& in, const Image& redish) {
		if(!out) out.init(in.w, in.h);


		for(uint i=in.n; i--;) {
			if(redish[i]>0.1) {
				out[i*3] = 255*(bRedish+in[i])/2;
				out[i*3+1] = 255*(0+in[i])/2;
				out[i*3+2] = 255*(!bRedish+in[i])/2;
			} else {
				out[i*3] = 255*in[i];
				out[i*3+1] = 255*in[i];
				out[i*3+2] = 255*in[i];
			}
		}
	}

	void process(const Image& in, const Image& redish, const Image& bluish) {
		if(!out) out.init(in.w, in.h);

		for(uint i=in.n; i--;) {
			out[i*3] = 255*(in[i]+redish[i])/2;
			out[i*3+1] = 255*in[i]*redish[i]*bluish[i];
			out[i*3+2] = 255*(in[i]+bluish[i])/2;
		}
	}
};



#endif /* REDISHBLUISH_H_ */
