/*
 * SaveImage.h
 *
 *  Created on: 4 avr. 2015
 *      Author: jfellus
 */

#ifndef LOADIMAGE_H_
#define LOADIMAGE_H_

#include <pg.h>
#include <image.h>
#include <jpeglib.h>
#include "imageio/jpg.h"




class LoadImage {
public:
	std::string filename;
	size_t i;

	ImageRGB out;
	OUTPUT(ImageRGB, out);
	OUTPUT(size_t, i);

public:

	LoadImage() {
		filename = "imgout/%06d.jpg";
		i = 0;
	}

	void init() {
		if(!str_ends_with(filename, ".jpg")) ERROR("Format not supported : " << filename);
	}

	void process() {
		char f[PATH_MAX]; sprintf(f, filename.c_str(), i++);

		JPEGFILE* jpg = open_jpg(f);
		if(!jpg) { DBG("No such image : " << f); i = 0; return; }

		if(jpg->w!=out.w || jpg->h!=out.h) { out.free(); out.init(jpg->w,jpg->h); }

		read_jpg(jpg, out);
		close_jpg(jpg);
	}


};


#endif /* LOADIMAGE_H_ */
