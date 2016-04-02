

#include <pg.h>
#include <image.h>
#include <matrix.h>



class ImageAtPoints {
public:

	Matrix out;
	OUTPUT(Matrix, out)


public:
	ImageAtPoints() {
	}

	void init() {
	}

	void process(const Image& in, const Matrix& points) {
		if(out.h != in.n) out.init(in.n, 1);
		out.clear();
		out.set_height(points.h);
		for(uint i=0; i<points.h; i++) {
			out[i] = in(points(i,0),points(i,1));
		}
	}
};

