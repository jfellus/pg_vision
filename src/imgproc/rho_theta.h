/*
 * rho_theta.h
 *
 *  Created on: 14 avr. 2015
 *      Author: jfellus
 */

#ifndef RHO_THETA_H_
#define RHO_THETA_H_


#include <math.h>


int* create_lookup(float r, float R, uint w, uint descw, uint desch) {
	float rho1 = descw / log(1. + R - r);
	float theta1 = desch / (2. * M_PI);

	int* lookup = new int[descw*desch];
	int* p = lookup;
	for (uint i = 0; i < desch; i++) {
		for (uint j = 0; j < descw; j++) {
			float dist = exp(j / rho1) - 1 + r;
			int x = dist * cos(i / theta1);
			int y = dist * sin(i / theta1);
			*p++ = y * w + x;
		}
	}
	return lookup;
}


/**
 *
 * Explication des différents tableaux:
 * xmax,ymax: coordonnee max de l'image d'entree
 * xmax2,ymax2 : coordonnee max de la sortie (groupe de neurone)
 *
 * 2 tableaux codent la projection dans les 2 sens :
 * lookup[xmax2*ymax2]: coordonnee du point dans l'image d'entrée code par le ieme neurone du groupe
 * other_pt[xmax,ymax]: index du neurone de sortie qui code le mieux le point donne en index
 *
 * 2 tableaux comptent le nombre de point (reel et normalise) represente par un neurone. Cette info est utilise pour moyenner l'activite du neurone
 * compteur[xmax2*ymax2]: nombre de points contenu dans les tableaux other_pt et lookup qui pointent vers le même neurone de sortie
 * compteur_egal[xmax2*ymax2]: normalisation: un même point dans l'image dorigine peut être code par plusieurs neurones en sortie.
 *                             chacun de ces neurones doit avoir la meme valeur de compteur -> propagation de la valeur max.
 *
 *
 * finalement la fonction renvoie un tableau nomme real_lookup:
 * real_lookup [i][0]=compteur [i]
 * real_lookup [i][1]=lookup[i]
 * real_lookup [i][k]=les coordonnees des (k-2) points de l'image d'entree tels que other_pt=i
 */
uint **init_lookup_rho_theta(int w, int h, uint side, uint dim, float r, float R) {
	uint descw = side;
	uint desch = side;
	int* lookup = create_lookup(r, R, w, descw, desch);

	/* je compte le nbre de point qui sont aussi dans la couronne et j'alloue un tablau de cette taille ou chaque point
      est represente par une case.
      le contenu de chaque case represente l'indice du neurone dont ce point est le plus pret.	 */
	int cpt = 0;
	for(int y = -h / 2; y < h / 2; y++) {
		for(int x = -w / 2; x < w / 2; x++) {
			float dist = sqrt(x * x + y * y);
			if (dist < R && dist > r) cpt++;
		}
	}

	int *other_pt = new int[w*h];
	for (int y = -h / 2; y < h / 2; y++) {
		for (int x = -w / 2; x < w / 2; x++) {
			other_pt[x + w / 2 + (y + h / 2) * w] = -1;
		}
	}

	for (int y = -h / 2; y < h / 2; y++){
		for (int x = -w / 2; x < w / 2; x++) {
			int ind_min = -1;
			float dist = sqrt(x * x + y * y);
			int p = x + w / 2 + (y + h / 2) * w;
			if (dist <= R && dist >= r){
				/*pour chaque point, on va chercher le point de lookup le plus proche   */
				float min = 9999999.;
				for (uint i = 0; i < dim; i++) {
					int z = (lookup[i] + w/2 + h/2*w);
					int xl = z % w;
					int yl = z / w;
					float dist = (x + w/2 - xl) * (x + w/2 - xl) + (y + h/2 - yl) * (y + h/2 - yl);
					if (dist <= min) { min = dist; ind_min = i; }
				}
				other_pt[p] = ind_min;
			}
		}
	}



	/*ensuite, je compte pour chaque neurone le nbre de point de other_pt qui senvoie sur le neurone, y compris le pt de lookup.
      Je met ca dans compteur et compteur_egal.
      Comme certain neurone utilise le meme point, il faut que tout le neurone utilisant le meme point utilise le meme nombre de pt en global
      compteur_egal contient pour chaque neurone le nombre de point precis a utilser. Alors que compteur donne juste le nombre de pt donne par lookup et other_pt */

	int *compteur =  new int[dim];
	int *compteur_egal =  new int[dim];


	for (uint i = 0; i < dim; i++) {compteur[i] = compteur_egal[i] = 1;}

	for (uint i = 0; i < desch; i++)  {
		for (uint j = 0; j < descw; j++) {
			for (int y = -h/2; y < h/2; y++) {
				for (int x = -w/2; x < w/2; x++) {
					if (other_pt[x + w/2 + (y + h/2) * w] == (int)(j + i * descw)
							&& x + w/2 + (y + h/2) * w != lookup[j + i * descw])  {
						compteur[j + i * descw]++;
						compteur_egal[j + i * descw]++;
					}
				}
			}
		}
	}

	for (uint i = 0; i < dim; i++) {
		float max = compteur_egal[i];
		for (uint j = 0; j < dim; j++) {
			if (lookup[i] == lookup[j])
				if (compteur_egal[j] > max) {
					compteur_egal[i] = compteur_egal[j];
					max = compteur_egal[j];
				}
		}
	}


	uint** real_lookup = new uint* [dim];
	for (uint i = 0; i < dim; i++) {
		real_lookup[i] = new uint[compteur_egal[i] + 1];
		real_lookup[i][0] = compteur[i];
		real_lookup[i][1] = lookup[i];
		uint* p = &real_lookup[i][2];
		for (int y = -h/2; y < h/2; y++) {
			for (int x = -w/2; x < w/2; x++){
				if (other_pt[x + w/2 + (y + h/2) * w] == (int)i) *p++ = x + y * w;
			}
		}
	}

	for (uint i = 0; i < dim; i++) {
		uint max = real_lookup[i][0];
		for (uint j = 0; j < dim; j++) {
			if (real_lookup[i][1] == real_lookup[j][1] && real_lookup[j][0] > max) {
				max = real_lookup[j][0];
				for (uint k = 0; k <= (uint)real_lookup[j][0]; k++) real_lookup[i][k] = real_lookup[j][k];
			}
		}
	}

	return real_lookup;
}




#endif /* RHO_THETA_H_ */
