/*
 * SaveImage.h
 *
 *  Created on: 4 avr. 2015
 *      Author: jfellus
 */

#ifndef SAVEIMAGE_H_
#define SAVEIMAGE_H_

#include <pg.h>
#include <image.h>
#include <jpeglib.h>
#include "imageio/jpg.h"




class SaveImage {
public:
	std::string filename;
	size_t i;

public:

	SaveImage() {
		filename = "imgout/%06d.jpg";
		i = 0;
	}

	void init() {
		if(!str_ends_with(filename, ".jpg")) ERROR("Format not supported : " << filename);
		mkdir(str_dirname(filename));
	}

	void process(ImageRGB& img) {
		char f[PATH_MAX]; sprintf(f, filename.c_str(), i++);
		save_jpg(img, img.w, img.h, f);
	}

	void process(Image& img) {
		char f[PATH_MAX]; sprintf(f, filename.c_str(), i++);
		save_jpg(img, img.w, img.h, f);
	}

};


#endif /* SAVEIMAGE_H_ */
