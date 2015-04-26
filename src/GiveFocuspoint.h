/*
 * GiveFocuspoint.h
 *
 *  Created on: 12 avr. 2015
 *      Author: jfellus
 */

#ifndef GIVEFOCUSPOINT_H_
#define GIVEFOCUSPOINT_H_

#include <pg.h>
#include <image.h>
#include <list>
#include <vector>

class GiveFocuspoints {
public:

	float threshold;
	uint nbMaxFocuspoints;

	Matrix out;
	OUTPUT(Matrix, out);

public:
	GiveFocuspoints() {
		threshold = 0.001;
		nbMaxFocuspoints = 200;
	}

	void init() {
		out.init(nbMaxFocuspoints, 3);
	}

	void process(const Image& in) {
		out.h = 0;
		for(uint y = 0; y < in.h; y++) {
			for(uint x = 0; x < in.w; x++) {
				float v = in(x,y);
				if(v>threshold) {
					out[out.h*3] = x;
					out[out.h*3 +1] = y;
					out[out.h*3 +2] = v;
					if(out.h < nbMaxFocuspoints) out.h++;
				}
			}
		}
	}
};



class GiveFocuspoint {
public:
	bool end;
	Matrix focuspoint;

	OUTPUT(Matrix, focuspoint);
	OUTPUT(bool, end);

public:

	GiveFocuspoint() {
		end = false;
		focuspoint.init(3,1);
	}


	void init() {}

	void process(const Image& in, bool bNewImage) {
		if(bNewImage) analyse_image(in);
		if(end) return;

		focuspoint[0] = (*i_focuspoint).x;
		focuspoint[1] = (*i_focuspoint).y;
		focuspoint[2] = (*i_focuspoint).v;

		if(++i_focuspoint == focuspoints.end()) {end=true;}
	}


private:
	class Focuspoint {
	public:
		int x,y; float v;
		Focuspoint(int x,int y, float v) : x(x),y(y),v(v){}
		inline static bool cmp(const Focuspoint& i, const Focuspoint& j) { return i.v > j.v; }
	};
	std::vector<Focuspoint> focuspoints;

	std::vector<Focuspoint>::iterator i_focuspoint;
private:

	void analyse_image(const Image& in) {
		end = false;
		focuspoints.clear();
		const float* p = in;
		for(uint y=0;y<in.h;y++) {
			for(uint x=0;x<in.w;x++) {
				if(in(x,y)<0.0001) continue;
				focuspoints.push_back(Focuspoint(x,y,*p++));
			}
		}
		std::sort(focuspoints.begin(), focuspoints.end(), Focuspoint::cmp);
		i_focuspoint = focuspoints.begin();
	}
};



#endif /* GIVEFOCUSPOINT_H_ */
