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
#include <image.h>
#include <matrix.h>
#include "imgproc/rho_theta.h"



class LogPolar {
public:
	uint side; // side of a Logpolar descriptor (dimension is always <side>x<side> )
	uint nbMaxDescriptors;

	Matrix out; // Output descriptors (one per row)

	float r, R; // Inner & outer radiuses

	OUTPUT(Matrix, out)
	OUTPUT(uint, out.h)

private:
	uint dim;
	uint **lookup;

public:
	LogPolar() {
		side = 16;
		r = 5;
		R = 21;
		nbMaxDescriptors = 200;

		dim = 0; lookup = 0;
	}

	void init() {
		dim = side*side;
	}

	void process(const Image& in, const Matrix& focuspoints) {
		if(!out) {
			lookup = init_lookup(in.w, in.h, r, R);
			out.init(nbMaxDescriptors, dim);
		}

		out.h = focuspoints.h;

//		#pragma omp parallel for
		for(uint i=0; i<focuspoints.h; i++) {
			const float* focuspoint = &focuspoints[i*focuspoints.w];
			compute_log_polar(in, &out[out.w*i], (int)focuspoint[0], (int)focuspoint[1], focuspoint[2]);
		}
	}


	void compute_log_polar(const Image& in, float* out, int x, int y, float v) {
		int offset_center = x + y * in.w;
		for (uint i = 0; i < dim; i++) {
			float sum = 0;
			for (uint k = 0; k < lookup[i][0]; k++) {
				int offset = lookup[i][k + 1] + offset_center;
				if (offset >= 0 && offset < (int)in.n)  sum += in[offset];
			}
			out[i] = sum / lookup[i][0];
		}
	}

	uint** init_lookup(uint w, uint h, float r, float R) {
		return init_lookup_rho_theta(w,h,side,dim,r,R);
	}
};

