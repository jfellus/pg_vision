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

#include "imgproc/mask.h"
#include "imgproc/interest_points.h"
#include "imgproc/convolution.h"
#include <pg.h>
#include <image.h>

#define DEFAULT_THETA1		3.0
#define DEFAULT_THETA2		2.0
#define DEFAULT_MASK_SIZE	10


class Ptcg {
public:
	float theta1;
	float theta2;
	int   mask_radius;
	int competition_radius;
	float   threshold;
	uint   exclusion_radius;
	bool   bModePano;
	bool   bOcclusion;
	float  min_max;

	Image out;
	OUTPUT(Image, out)

private:
	float min_global_after_filtering;
	float max_global_after_filtering;

	Image tmp;
	Image DoG;

public:

	Ptcg() {
		theta2          = DEFAULT_THETA2;
		theta1          = DEFAULT_THETA1;
		threshold       = 0;
		mask_radius      = DEFAULT_MASK_SIZE;
		competition_radius = DEFAULT_MASK_SIZE;
		exclusion_radius     = 0;
		bModePano       = false;
		min_max     = -1.;
		bOcclusion  	= false;
		min_global_after_filtering = max_global_after_filtering = 0;
	}

	void init() {
		DoG = create_DoG_mask(mask_radius, theta2, theta1);
		min_global_after_filtering = get_mask_min_out(DoG);
		max_global_after_filtering = get_mask_max_out(DoG);
	}

	void process(const Image& in) {
		if(!out) {
			out.init(in.w, in.h);
			tmp.init(in.w, in.h);
		}

		float maxval = convolution(in, DoG, tmp, threshold);

		/*mode_minmax inactif : renormalisation par le max courant*/
		if(min_max<=0) compute_local_maxima(tmp, out, competition_radius, maxval);
		/*mode_minmax actif : le point le plus fort doit etre au moins de (vmax-vmin) / minMax */
		else {
			if(maxval<(max_global_after_filtering - min_global_after_filtering) / min_max) out = 0;
			compute_local_maxima(tmp, out, competition_radius, maxval);
		}

		out.trim(0,1);
		compute_exclusion(in);

		for(uint i=0; i<out.n-mask_radius*in.w-mask_radius; i++) out[i] = out[i-mask_radius+in.w];
	}

private:

	void compute_exclusion(const Image& in) {
		if (2 * exclusion_radius >= MIN(in.w,in.h)) ERROR("Exclusion radius too large => there will be no focus point !");

		/* Left & Right borders exclusion */
		for (uint i = exclusion_radius; i < in.h - exclusion_radius; i++)	{
			for (uint j = exclusion_radius;j--;) out(j,i) = 0;
			for (uint j = in.w - exclusion_radius; j < in.w; j++) out(j,i) = 0;
		}

		if (!bModePano) {
			/* Top & Bottom borders exclusion */
			for (uint i = exclusion_radius * in.w; i--;) out[i] = 0;
			for (uint i = ((int)in.h - exclusion_radius) * (int)in.w; i < in.n; i++) out[i] = 0;
		} else {
			for (uint j = in.w; j--;) {
				for (uint i = exclusion_radius + (int) ((45. / 240.) * in.h); i--;) {
					if (i < exclusion_radius + (int) ((10. / 240.) * in.h)) out(j,i) = 0;
					else if ((j > (300. / 1540.) * in.w) && (j < ((545. / 1540.) * in.w))) out(j,i) = 0;
				}

				for (uint i = in.h - exclusion_radius - (int) ((40. / 240.) * in.h); i < in.h; i++) out(j,i) = 0;
			}
		}

		if (bOcclusion) {
			for (uint i = exclusion_radius;i < exclusion_radius + (in.w - 2 * exclusion_radius) * 0.5; i++)
				for (uint j = 0; j < in.h; j++) out(i,j) = 0;
		}
	}

};
