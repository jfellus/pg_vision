/*
 * gradient.cpp
 *
 *  Created on: 10 avr. 2015
 *      Author: jfellus
 */

#include "gradient.h"
#include <stdlib.h>
#include <pg.h>


int mem_my_sqrt[TAILLE_SQRT];


// Gaussian filtering
void filtre_ghv(float *image, float alpha, int nx, int ny) {
	int ix, iy;
	float y, yy, yyy, x, xx;
	float a,a2,b,v1,v2,v3;
	float *p;
	float *pnt;

	a = exp(-alpha);
	a2=a*a;
	b=2*a;

	v1= (1 - a) * (1 - a);
	v2= (1 - a2);
	v3 = (1 - a) / (1 + a);

	for (iy = 0; iy < ny; iy++)
	{
		p = iy * nx + image;
		y = yy = yyy = 0.;
		x = xx = 0.;
		for (ix = 0; ix < nx; ix++)
		{
			pnt = p + ix;
			xx = x;
			x = *(pnt) ;/*x = image[pnt]; */
			yyy = yy;
			yy = y;
			y = v1 * (x + xx) + b * yy - a2 * yyy;
			*(pnt) = y; /*image[pnt] = y;*/
		}

		y = yy = yyy = 0.;
		x = xx = 0.;
		for (ix = nx-1 ; ix--;  )
		{
			pnt = p + ix;
			xx = x;
			x = *(pnt); /*x = image[pnt];*/
			yyy = yy;
			yy = y;
			y = v2 * x + (b * yy - a2 * yyy);
			*(pnt) = y;/*image[pnt] = y;*/
		}
	}

	for (ix = 0; ix < nx; ix++)
	{
		p = ix + image;
		y = yy = yyy = 0.;
		x = xx = 0.;
		for (iy = 0; iy < ny; iy++)
		{
			pnt = iy * nx + p;
			xx = x;
			x = *(pnt); /*x = image[pnt];*/
			yyy = yy;
			yy = y;
			y = v1 * (x + xx) + b * yy - a2 * yyy;
			*(pnt) = y;/*image[pnt] = y;*/
		}

		y = yy = yyy = 0.;
		x = xx = 0.;
		for (iy = ny -1;  iy--;  )
		{
			pnt = iy * nx + p;
			xx = x;
			x = *(pnt); /*x = image[pnt];*/
			yyy = yy;
			yy = y;
			y = v2 * x + (b * yy - a2 * yyy);
			*(pnt) = (y / 4) * v3;/*image[pnt] = (y / 4) * v3;*/
		}
	}
}

// Horizontal gradient
void fh_ghv(float *image, float alpha, int nx, int ny)
{
	int ix, iy;
	float y, yy, yyy, x, xx, xxx;
	float a,a2,b,v1,v2;
	float *p;
	float *pnt;

	a = exp(-alpha);
	a2=a*a;
	b=2*a;

	v1= (1 - a) * (1 - a); /*1+a2-b*/
	v2= (1 - a2);
	for (iy = 0; iy < ny; iy++)
	{
		y = yy = yyy = 0.;
		x = xx = 0.;
		p=iy * nx + image;
		for (ix = 0; ix<nx; ix++ )
		{
			pnt = p + ix;
			xx = x;
			x= *(pnt); /* x = image[pnt];*/
			yyy = yy;
			yy = y;
			y = v1 * xx + b * yy - a2 * yyy;
			*(pnt) =y; /* image[pnt] = y; */
		}

		y = yy = yyy = 0.;
		x = xx = xxx = 0.;
		for (ix = nx-1 ; ix --; )
		{
			pnt = p + ix; /* pnt = iy * nx + ix; */
			xxx = xx;
			xx = x;
			x= *(pnt);  /*x = image[pnt];*/
			yyy = yy;
			yy = y;
			y = v2 * (xxx - x) + (b* yy - a2 * yyy); /*y = (1 - a * a) * (xxx - x) + (2 * a * yy - a * a * yyy);*/
			*(pnt) =y; /* image[pnt] = y; */
		}
	}
}

// Vertical gradient
void fv_ghv(float *image, float alpha, int nx, int ny)
{
	int ix, iy;
	float y, yy, yyy, x, xx, xxx;
	float a,a2,b,v1,v2;
	float *p;
	float *pnt = NULL;

	a = exp(-alpha);
	a2=a*a;
	b=2*a;

	v1= (1 - a) * (1 - a);
	v2= (1 - a2);

	for (ix = 0; ix < nx; ix++)
	{
		y = yy = yyy = 0.;
		x = xx = 0.;
		p=ix + image ;
		for (iy = 0; iy < ny; iy++)
		{
			pnt = iy * nx + p;
			xx = x;
			x= *( pnt); /*x = image[pnt];*/
			yyy = yy;
			yy = y;
			y = v1 * xx + b* yy - a2 * yyy;
			*( pnt) =y;
		}

		y = yy = yyy = 0.;
		x = xx = xxx = 0.;
		for (iy = ny-1 ;  iy--; )
		{
			pnt = iy * nx + p; /* pnt = iy * nx + ix; */
			xxx = xx;
			xx = x;
			x= *( pnt); /* x = image[pnt]; */
			yyy = yy;
			yy = y;
			y = v2 * (xxx - x) + (b * yy - a2 * yyy);
			*( pnt) =y; /* image[pnt] = y; */
		}
	}
}

// Non-maximums suppression
void mamphiJ(float *ik, float *in, unsigned Nx, unsigned Ny)
{
	int sign;
	unsigned indice, indice1, indice2, indice3, indice4;
	unsigned kn;
	//register int i, j;
	unsigned int i,j;
	float *a;
	float angle;
	float g1, g2, u, ux, uy;
	float v1,v2;

	kn = Nx * Ny;
	if ((a = (float *) calloc(kn, sizeof(float))) == NULL) ERROR("Allocation error");

	/*
	 * calcul de l'amplitude
	 */
	for (i = kn-1; i--; )
	{
		v1=ik[i] ;v2= in[i];
		a[i] = my_sqrt(v1 * v1 + v2 * v2);
	}
	/*
	 * suppression de non maximum
	 */
	for (i = 1; i <= Ny - 2; ++i)
		for (j = 1; j <= Nx - 2; ++j)
		{
			indice = i * Nx + j;
			if (is_diff(ik[indice], 0))  angle = in[indice] / ik[indice];
			else                        angle = 2.;

			sign = 1;
			if (angle < 0) sign = -1;

			ux = ik[indice];
			if (ux < 0)   ux = -ux;
			uy = in[indice];

			if (uy < 0)   uy = -uy;
			ik[indice] = 0;

			switch (sign)
			{
			case -1:
				if (angle < -1)
				{
					u = a[indice] * uy;
					indice1 = (i - 1) * Nx + j + 1;
					indice2 = indice1 - 1;
					g1 = ux * a[indice1] + (uy - ux) * a[indice2];
					if (u < g1)
					{
						ik[indice] = 0;
						continue;
					}
					indice3 = (i + 1) * Nx + j - 1;
					indice4 = indice3 + 1;
					g2 = ux * a[indice3] + (uy - ux) * a[indice4];
					if (u <= g2)
					{
						ik[indice] = 0;
						continue;
					}
				}
				else
				{
					u = a[indice] * ux;
					indice1 = (i - 1) * Nx + j + 1;
					indice2 = indice1 + Nx;
					g1 = uy * a[indice1] + (ux - uy) * a[indice2];
					if (u < g1)
					{
						ik[indice] = 0;
						continue;
					}
					indice3 = (i + 1) * Nx + j - 1;
					indice4 = indice3 - Nx;
					g2 = uy * a[indice3] + (ux - uy) * a[indice4];
					if (u <= g2)
					{
						ik[indice] = 0;
						continue;
					}
				}
				break;
			case 1:
				if (angle >= 1)
				{
					u = a[indice] * uy;
					indice1 = (i + 1) * Nx + j + 1;
					indice2 = indice1 - 1;
					g1 = ux * a[indice1] + (uy - ux) * a[indice2];
					if (u < g1)
					{
						ik[indice] = 0;
						continue;
					}
					indice3 = (i - 1) * Nx + j - 1;
					indice4 = indice3 + 1;
					g2 = ux * a[indice3] + (uy - ux) * a[indice4];
					if (u <= g2)
					{
						ik[indice] = 0;
						continue;
					}
				}
				else
				{
					u = a[indice] * ux;
					indice1 = (i - 1) * Nx + j - 1;
					indice2 = indice1 + Nx;
					g1 = uy * a[indice1] + (ux - uy) * a[indice2];
					if (u < g1)
					{
						ik[indice] = 0;
						continue;
					}
					indice3 = (i + 1) * Nx + j + 1;
					indice4 = indice3 - Nx;
					g2 = uy * a[indice3] + (ux - uy) * a[indice4];
					if (u <= g2)
					{
						ik[indice] = 0;
						continue;
					}
				}
				break;
			}                   /*  fin du switch */

			ik[indice] = a[indice];
		}                       /* fin du for */

	for (j = 0; j < Nx; ++j)
	{
		ik[j] = 0;
		ik[Nx + j] = 0;
		indice = (Ny - 1) * Nx + j;
		ik[indice] = 0;
		indice -= Nx;
		ik[indice] = 0;
	}
	for (i = 0; i < Ny; ++i)
	{
		indice = i * Nx;
		ik[indice] = 0;
		++indice;
		ik[indice] = 0;
		indice = (i + 1) * Nx - 1;
		ik[indice] = 0;
		--indice;
		ik[indice] = 0;
	}
	for (i = 0; i <= ((Ny - 1) * (Nx + 1)); ++i)     in[i] = a[i];

	free(a);
	return;
}
