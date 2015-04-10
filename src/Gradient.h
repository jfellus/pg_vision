/*
Copyright  ETIS — ENSEA, Université de Cergy-Pontoise, CNRS (1991-2014)
promethe@ensea.fr

Authors: P. Andry, J.C. Baccon, D. Bailly, A. Blanchard, S. Boucena, A. Chatty, N. Cuperlier, P. Delarboulas, P. Gaussier, 
C. Giovannangeli, C. Grand, L. Hafemeister, C. Hasson, S.K. Hasnain, S. Hanoune, J. Hirel, A. Jauffret, C. Joulain, A. Karaouzène,  
M. Lagarde, S. Leprêtre, M. Maillard, B. Miramond, S. Moga, G. Mostafaoui, A. Pitti, K. Prepin, M. Quoy, A. de Rengervé, A. Revel ...

See more details and updates in the file AUTHORS 

This software is a computer program whose purpose is to simulate neural networks and control robots or simulations.
This software is governed by the CeCILL v2.1 license under French law and abiding by the rules of distribution of free software. 
You can use, modify and/ or redistribute the software under the terms of the CeCILL v2.1 license as circulated by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
As a counterpart to the access to the source code and  rights to copy, modify and redistribute granted by the license, 
users are provided only with a limited warranty and the software's author, the holder of the economic rights,  and the successive licensors have only limited liability. 
In this respect, the user's attention is drawn to the risks associated with loading, using, modifying and/or developing or reproducing the software by the user in light of its specific status of free software, 
that may mean  that it is complicated to manipulate, and that also therefore means that it is reserved for developers and experienced professionals having in-depth computer knowledge. 
Users are therefore encouraged to load and test the software's suitability as regards their requirements in conditions enabling the security of their systems and/or data to be ensured 
and, more generally, to use and operate it in the same conditions as regards security. 
The fact that you are presently reading this means that you have had knowledge of the CeCILL v2.1 license and that you accept its terms.
 */

#include <pg.h>
#include <math.h>
#include "imgproc/gradient.h"


class Gradient {
public:

	bool bSmoothing;
	float alpha; // in R+ : Gaussian smoothing diffusion factor

	char postProcessingMode; // in {'I','C','M'}
	float threshold;

	Image out;

	OUTPUT(Image, out)

private:

	Image lh,lv;

public:

	Gradient() {
		bSmoothing = true;
		postProcessingMode = 'I';
		alpha = 1;
		threshold = 0;
	}

	void init() {
		init_my_sqrt();
	}

	void process(const Image& in) {
		if(bSmoothing) compute_smoothed_gradient(in);
		else compute_gradient(in);
		post_processing(in);
	}

protected:

	Image tmp;
	inline void compute_smoothed_gradient(const Image& in) {
		tmp = in;

		filtre_ghv(tmp, alpha, in.w, in.h);

		float D0 = 0, D1 = 0, S0 = 0, S1 = 0;
		if(!lh) lh.init(in.w, in.h);
		if(!lv) lv.init(in.w, in.h);
		for (uint j = 1; j < in.n - in.w; j++) {
			D1 = tmp[j + in.w] - tmp[j] ;
			S1 = tmp[j + in.w] + tmp[j] ;
			S1 /= 2.0;
			lh[j - 1] = (D0 + D1)/2.0;
			lv[j - 1] = (S1 - S0);
			S0 = S1;
			D0 = D1;
		}
	}

	inline void compute_gradient(const Image& in) {
		lv = lh = in;
		fh_ghv(lv, alpha, in.w, in.h);
		fv_ghv(lh, alpha, in.w, in.h);
	}

	inline void post_processing(const Image& in) {
		if(!out) out.init(in.w, in.h);

		// Mode I : trimmed analog magnitude
		if (postProcessingMode == 'I') {
			for (uint i = in.n-1; i--; ) out[i] = (fabs(lh[i]) + fabs(lv[i]))/2;
			if(threshold>0) for (uint i = in.n-1; i--; ) out[i] = out[i] < threshold ? 0 : out[i];
		}
		// Mode C : binarized magnitude
		else if (postProcessingMode == 'C') {
			float th = threshold*2;
			for (uint i = in.n-1; i--; ) {
				out[i] = fabs(lh[i])+ fabs(lv[i]);
				out[i] = out [i] < th ? 0 : 1;
			}
		}
		// Mode M : analog vertical gradient
		else if (postProcessingMode == 'M') out = lv;
	}
};
