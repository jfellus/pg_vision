/*
 * Camera.h
 *
 *  Created on: 4 avr. 2015
 *      Author: jfellus
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <pg.h>
#include <image.h>
#include "v4l2/V4LIn.h"

class Camera {
public:
	std::string path;
	ImageRGB frame;

	V4LIn* v4lin;

	OUTPUT(ImageRGB, frame);

public:

	Camera() {
		v4lin = 0;
		path = "/dev/video0";
	}

	~Camera() {
		if(v4lin) {
			v4lin->stop();
			delete v4lin;
		}
	}

	void init() {
		v4lin = new V4LIn(path.c_str());
		frame.w = v4lin->w;
		frame.h = v4lin->h;
		frame.data = v4lin->data;
		v4lin->start();
	}

	void process() {
		v4lin->readFrame();
		frame.data = v4lin->data;
	}


};


#endif /* CAMERA_H_ */
