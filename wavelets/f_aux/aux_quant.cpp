#include "missing_header.h"
#include "header.hpp"
#include <stdlib.h>
#include "lbg_vq/header.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char *argv[]){
	if (argc < 6){
		cout<<"Usage: "<<argv[0]<<" -n/N [n. of centroids] -d/D [x] [y] -f/F [pgm files]"<<endl;
		cout<<"   Obs.: Order can be interchanged."<<endl;
		return 1;
	}
	vector< string > files;

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

//	vector< string > dir_names;
//	//gerar os diretorios do niveis se não existirem
//	for (int i=0; i<NBANDS; i++){
//		string dest_dir= "B" + to_string(i) + "_Codebooks";
//		dir_names.push_back(dest_dir);
//		struct stat st = {0};
//		if (stat(dest_dir.c_str(), &st) == -1)
//			mkdir(dest_dir.c_str(), 0700);
//	}

	band_vects tr_vects(NBANDS);
	//generate training vector for all bands
	for (string file : files){
		int x=0,y=0;
		int *im_array = op_pgm(x,y,file);

		//check if padding is needed; adds it if it is the case
		int p = (int) pow(2,NSTAGES);
		int pad_x = (x / p) % x_fr;
		if (pad_x != 0)
			pad_x = (x_fr - pad_x)*p;
		int pad_y = (y / p) % y_fr;
		if (pad_y != 0)
			pad_y = (y_fr - pad_y)*p;
		if ((pad_x != 0) or (pad_y != 0)){
			im_array = add_padding(im_array, x, y, pad_x, pad_y);
			x += pad_x;
			y += pad_y;
		}

		int **dd_img = im_to_ddot(im_array, x, y);
		int **dd_dec = init_dd_img(x,y);

		avg_rem(dd_img, x, y);
		double *sIMG[YIMG];
		analysis(dd_img,dd_dec, sIMG, x, y);

		subbands bands = split_bands(sIMG,x,y,NSTAGES);
		for (int i=0; i<NBANDS; i++){
			int x = bands[i].x, y = bands[i].y;
			vect_list aux = vectorize(bands[i].img,x,y,x_fr,y_fr);
			cout<<x<<" "<<y<<endl;
			cout<<x_fr<<" "<<y_fr<<endl;
			tr_vects[i].insert(tr_vects[i].end(), aux.begin(), aux.end());
		}
	}

//	//acrescentar o lbg por banda
	for (int i=0; i<NBANDS; i++){
		cout<<"i = "<<i<<endl;
		print_centroids(tr_vects[i], x_fr*y_fr);
		cout<<"Here we go!"<<endl;
		vect_list codebook = lbg(tr_vects[i], n_cent, x_fr*y_fr, 1e-6);
		/*add_null_vec(codebook, x_fr*y_fr);*/
//		save_codebook(codebook,x_fr,y_fr,dir_names[i] + "/codebook");
	}

	return 0;
}
