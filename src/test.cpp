/*
 * test.cpp
 *
 *  Created on: 9 avr. 2015
 *      Author: jfellus
 */

#include "Camera.h"
#include "SaveImage.h"

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
