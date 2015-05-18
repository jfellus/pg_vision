/*
 * test.cpp
 *
 *  Created on: 9 avr. 2015
 *      Author: jfellus
 */

#include "Camera.h"
#include "SaveImage.h"
#include "Binarize.h"
#include "Grayscale.h"
#include "ImageExtractChannel.h"
#include "Ptcg.h"
#include "Convolution.h"
#include "DoG.h"
#include "GiveFocuspoint.h"
#include "LogPolar.h"
#include "ThetaPopulationCoding.h"
#include "Equalize.h"
#include "HoG.h"
#include "Gradient.h"
#include "Orientations8.h"
#include "Subsample.h"

int main(int argc, char **argv) {
	try {
	Camera c;
	c.path = "/dev/video1";
	SaveImage s;

	c.init();
	s.init();
	for(int i=0; i<100; i++) {
		c.process();
		s.process(c);
	}
	} catch(std::runtime_error& e) { PRINT_FATAL_ERROR(e.what());}
}
