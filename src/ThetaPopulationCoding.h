/*
 * ThetaPopulationCoding.h
 *
 *  Created on: 15 avr. 2015
 *      Author: jfellus
 */

#ifndef THETAPOPULATIONCODING_H_
#define THETAPOPULATIONCODING_H_

#include <pg.h>
#include <matrix.h>

// NOTE : Logitisic lateral interaction (  da(x)/dx = theta*a(x)(1-a(x))  )
class ThetaPopulationCoding {
public:
	uint nbNeurons;
	uint nbMaxObservations;
	float theta;
	float min, max;

	Matrix out;
	OUTPUT(Matrix, out)

public:

	ThetaPopulationCoding() {nbNeurons = 64; nbMaxObservations = 200; theta = 0.4; min = 0; max = 640;}

	void init() {}

	void process(const Matrix& focuspoints) {
		if(!out) out.init(nbMaxObservations, nbNeurons);
		out.set_height(MIN(nbMaxObservations,focuspoints.h));

//		#pragma omp parallel for
		for(uint i=0; i<focuspoints.h; i++) {
			if(i>=nbMaxObservations) continue;
			int location = (int) ((focuspoints(i,0)-min)/(max-min) * nbNeurons);
			float peak = focuspoints(i,2);
			float damp = .1;
			for(uint j=location; j<nbNeurons; j++) {
				out(i,j) = peak-damp;
				damp += theta*damp*(peak-damp);
			}
			damp = .1;
			for(uint j=location; j--;) {
				out(i,j) = peak-damp;
				damp += theta*damp*(peak-damp);
			}
		}
	}

};


#endif /* THETAPOPULATIONCODING_H_ */
