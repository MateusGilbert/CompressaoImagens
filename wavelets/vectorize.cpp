#include "header.hpp"

vect_list vectorize(float *im, int x, int y, int x_fr, int y_fr){
	vect_list vectors;

	if ((x <= 0) or (y <= 0) or (x % x_fr != 0 ) or (y % y_fr != 0)){
		vectors.push_back((float *) NULL);
		return vectors;
	}

	if (x_fr < 0)
		x_fr = 1;
	if (y_fr < 0)
		y_fr = 1;

	for (int i=0; i<=y-y_fr; i += y_fr)
		for (int j=0; j<=x-x_fr; j += x_fr){
			float *v = new float[x_fr*y_fr];
			int count=0;
			for (int ii=0; ii<y_fr; ii++)
				for (int jj=0; jj<x_fr; jj++)
					v[count++] = im[(i+ii)*x + j + jj];
			vectors.push_back(v);
		}

	return vectors;
}
