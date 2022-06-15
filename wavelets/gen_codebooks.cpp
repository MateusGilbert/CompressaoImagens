#include "missing_header.h"
#include "header.hpp"
#include <stdlib.h>
#include "lbg_vq/header.hpp"

int** init_dd_img(int x, int y){
	int **dd_img = (int **) malloc(y*sizeof(int*));
	for (int i=0; i<y; i++)
		dd_img[i] = (int *) malloc(x*sizeof(int));

	return dd_img;
}

int
main(int argc, char *argv[]){
	if (argc < 6){
		cout<<"Usage: "<<argv[0]<<" -n/N [n. of centroids] -d/D [x] [y] -f/F [pgm files]"<<endl;
		cout<<"   Obs.: Order can be interchanged."<<endl;
		return 1;
	}
	vector< string > files;

	//ler o num de niveis
	//gerar os diretorios do niveis se não existirem

	int shift = 0;
	int n_cent=0, x_fr=0, y_fr=0;
	read_cli:
		if (shift < argc-1)
			if (argv[1+shift][0] == '-'){
				switch (argv[1+shift][1]){
					case 'n':;
					case 'N':
						n_cent= stoi(argv[2 + shift]);
						shift += 2;
						goto read_cli;
					case 'd':;
					case 'D':
						x_fr = stoi(argv[2 + shift]);
						y_fr = stoi(argv[3 + shift]);
						shift += 3;
						goto read_cli;
					case 'f':;
					case 'F':
						while ((shift+2 < argc) and (argv[2 + shift][0] != '-'))
							files.push_back(argv[2 + shift++]);
						goto read_cli;
				}
			}

	int shift=1;
	vector< string > files;
	while(shift < argc)
		files.push_back(argv[shift++]);

	for (string file : files){
		int x=0,y=0;
		int *im_array = op_pgm(x,y,file);
		int **dd_img = im_to_ddot(im_array, x, y);
		int **dd_dec = init_dd_img(x,y);

		int avg = avg_rem(dd_img, x, y);
		double *sIMG[YIMG];
		analysis(dd_img,dd_dec, sIMG, x, y);

		//acrescentar o vetorizador
		band_vects tr_vects[NSTAGES*3+1];
		subbands bands = split_bands(sIMG,x,y,NSTAGES);
		int n_bands = bands.size()
		for (int i=0; i<n_bands; i++){
			int x = bands[i]->x, y = bands[i]->y;
			vect_list aux = vectorize(bands[i]->band_img,x,y,x_fr,y_fr);
			tr_vects[i].insert(tr_vects[i].end(), aux.begin(), aux.end());
		}

		//acrescentar o lbg por banda
		for (int i=0; i<n_bands; i++){
			vect_list codebook = lbg(tr_vects[i], n_cent, x_fr*y_fr, 1e-6);
			save_codebook(codebook,x_fr,y_fr,"B_" + to_string(i) + "/codebook");
		}

//		synthesis(sIMG, dd_out, x, y);
//		avg_add(dd_out, x, y, avg);
//		int *r_img = ddot_to_im(dd_out, x, y);
//		save_csv(r_img, x, y, file + ".csv");
	}

	return 0;
}
