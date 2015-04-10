/*
 * gradient.h
 *
 *  Created on: 10 avr. 2015
 *      Author: jfellus
 */

#ifndef GRADIENT_H_
#define GRADIENT_H_

#include <math.h>

// My sqrt
#define TAILLE_SQRT 2*2000*2000
extern int mem_my_sqrt[TAILLE_SQRT];
inline void init_my_sqrt() {for(int x=0;x<TAILLE_SQRT;x++) mem_my_sqrt[x]=sqrt((float) x);}
inline int my_sqrt(float x) {	return mem_my_sqrt[(int) x]; }


// Gaussian filtering
void filtre_ghv(float *image, float alpha, int nx, int ny);

// Horizontal gradient
void fh_ghv(float *image, float alpha, int nx, int ny);

// Vertical gradient
void fv_ghv(float *image, float alpha, int nx, int ny);

// Non-maximums suppression
void mamphiJ(float *ik, float *in, unsigned Nx, unsigned Ny);





#endif /* GRADIENT_H_ */
