#include "missing_header.h"
#include "header.hpp"
#include <stdlib.h>

int** init_dd_img(int x, int y){
	int **dd_img = (int **) malloc(y*sizeof(int*));
	for (int i=0; i<y; i++)
		dd_img[i] = (int *) malloc(x*sizeof(int));

	return dd_img;
}

int
main(int argc, char *argv[]){
	if (argc < 2)
		return 1;

	int shift=1;
	vector< string > files;
	while(shift < argc)
		files.push_back(argv[shift++]);

	vect_list tr_vects;
	for (string file : files){
		int x=0,y=0;
		int *im_array = op_pgm(x,y,file);
		int **dd_img = im_to_ddot(im_array, x, y);
		int **dd_out = init_dd_img(x,y);
		int **dd_dec = init_dd_img(x,y);
		/*sub(dd_img, dd_out, dd_dec, x, y);*/
		/*int **dd_enc = init_dd_img(x,y);*/

		/*int avg = avg_rem(dd_img, x, y);*/
		double *sIMG[YIMG];
		/*wav_analysis(dd_img,sIMG, x, y);*/
		analysis(dd_img,dd_dec, sIMG, x, y);
		int *d_img = ddot_to_im(dd_dec, x, y);
		save_csv(d_img, x, y, file + "d.csv");

		synthesis(sIMG, dd_out, x, y);
		/*avg_add(dd_out, x, y, avg);*/
		int *r_img = ddot_to_im(dd_out, x, y);
		save_csv(r_img, x, y, file + ".csv");
	}

	return 0;
}
