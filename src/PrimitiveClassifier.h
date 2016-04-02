/*
 * PrimitiveClassifier.h
 *
 *  Created on: 3 juin 2015
 *      Author: jfellus
 */

#ifndef PRIMITIVECLASSIFIER_H_
#define PRIMITIVECLASSIFIER_H_

#include <pg.h>

class LinearPrimitiveClassifier {
public:

	Image out;

	float threshold;
	float eta;
	float lambda;

	int dim;
	Matrix mask;

	OUTPUT(Image, out);
	PARAM(float, threshold, {});
	PARAM(float, eta, {});
public:

	LinearPrimitiveClassifier(){
		threshold = 0.5;
		dim = 20;
		eta = 0.001;
		lambda = 0.001;
	}

	void init() {
		mask.init(dim,dim);
	}

	void process(const Image& in, const Image& saliency) {
		if(!out) out.init(in.w, in.h);
		return;

		if(eta>0.000001) {
			for(uint y=0; y<in.h; y++) {
				for(uint x=0; x<in.w; x++) {
					out(x,y) = saliency(x,y)>threshold;
				}
			}
			for(uint y=0; y<in.h; y++) {
				for(uint x=0; x<in.w; x++) {
					float label = saliency(x,y)>threshold ? 1 : -1;
					float d = 0;
					for(int yy=-dim/2; yy<dim/2; yy++) {
						for(int xx=-dim/2; xx<dim/2; xx++) {
							if(x+xx < 0 || x+xx >= in.w || y+yy<0 || y+yy>=in.h) continue;
							d += in(x+xx,y+yy)*mask(xx+dim/2,yy+dim/2);
						}
					}

					// Correct => regul only
					if(label*d>0) {
						for(uint i=mask.n; i--;) mask[i] -= eta*lambda*mask[i];
					}
					// Wrong => adapt
					else {
						for(int yy=-dim/2; yy<dim/2; yy++) {
							for(int xx=-dim/2; xx<dim/2; xx++) {
								if(x+xx < 0 || x+xx >= in.w || y+yy<0 || y+yy>in.h) continue;
								mask(xx+dim/2,yy+dim/2) -= eta*(lambda*mask(xx+dim/2,yy+dim/2)-label*in(x+xx,y+yy));
							}
						}
					}
				}
			}
		}
		else {
			for(uint y=0; y<in.h; y++) {
				for(uint x=0; x<in.w; x++) {
					float label = saliency(x,y)>threshold ? 1 : -1;
					float d = 0;
					for(int yy=-dim/2; yy<dim/2; yy++) {
						for(int xx=-dim/2; xx<dim/2; xx++) {
							if(x+xx < 0 || x+xx >= in.w || y+yy<0 || y+yy>=in.h) continue;
							d += in(x+xx,y+yy)*mask(xx+dim/2,yy+dim/2);
						}
					}
					out(x,y) = d>0;
				}
			}
		}



	}
};



class NNPrimitiveClassifier {
public:

	array<Image*> prototypes;
	bool* prototypes_labels;

	Image out;

	float threshold;
	uint maxPrototypes;
	uint size;

	uint dim;

	OUTPUT(Image, out);
	PARAM(float, threshold, {});
	PARAM(uint, maxPrototypes, {});

public:

	NNPrimitiveClassifier(){
		threshold = 0.99999;
		maxPrototypes = 10;
		size = 5;
		prototypes_labels = 0;
		dim = 0;
	}

	void init() {
		prototypes_labels = new bool[maxPrototypes];
		dim = size*2+1;
	}

	void process(const Image& in, const Image& saliency) {
		if(!out) out.init(in.w, in.h);
sleep(1);
		for(int y = size; y<out.h-size; y++) {
			for(int x = size; x<out.w-size; x++) {

				float min_dist = 9999999;
				int winner = -1;

				for(int k = 0; k<prototypes.size(); k++) {
					float d = dist(in, *prototypes[k], x-size, y-size);
					if(d<min_dist) { winner = k; min_dist = d;}
				}

				if(winner == -1 || min_dist >= threshold) {
					if(prototypes.size() < maxPrototypes) {
						learn(in, x-size, y-size, saliency(x,y)>0.5);
					}
					winner = prototypes.size()-1;
				}

				out(x,y) = prototypes_labels[winner];
			}
		}
	}

	void learn(const Image& in, uint x, uint y, bool label) {
		Image* p = new Image();
		p->init(in.w, in.h);
		for(uint i=0; i<dim; i++) {
			for(uint j=0; j<dim; j++) {
				(*p)(i,j) = in(x+j,y+i);
			}
		}
		prototypes_labels[prototypes.size()] = label;
		prototypes.add(p);
		DBG("LEARN " << prototypes.size());
	}

	float dist(const Image& a, const Image& b, uint X, uint Y) {
		float d = 0;
		for(int y=0; y<dim; y++) {
			for(int x=0; x<dim; x++) {
				float f = (a(x+X,y+Y)-b(x,y));
				d += f*f;
			}
		}
		return d/(dim*dim);
	}
};



typedef NNPrimitiveClassifier PrimitiveClassifier;




#endif /* PRIMITIVECLASSIFIER_H_ */
