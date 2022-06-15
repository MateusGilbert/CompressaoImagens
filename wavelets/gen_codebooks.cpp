#include "missing_header.h"
#include "header.hpp"
#include <stdlib.h>
#include "lbg_vq/header.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int** init_dd_img(int x, int y){
	int **dd_img = (int **) malloc(y*sizeof(int*));
	for (int i=0; i<y; i++)
		dd_img[i] = (int *) malloc(x*sizeof(int));

	return dd_img;
}

inline void save_codebook(vect_list centroids, int x, int y, string saveat="codebook"){
	int N = centroids.size();
	saveat += "_" + to_string(x) + "_" + to_string(y) + "_" + to_string(N) + ".csv";
	ofstream outfile(saveat);

	outfile<<"# x "<<x<<" y "<<y<<" N "<<N<<endl;
	for (int *y_i : centroids){
		for (int i=0; i<x*y; i++)
			if (i == 0)
				outfile<<y_i[0];
			else
				outfile<<","<<y_i[i];
		outfile<<endl;
	}

	outfile.close();

	return;
}

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

	vector< string > dir_names;
	//gerar os diretorios do niveis se não existirem
	for (int i=0; i<NBANDS; i++){
		string dest_dir= "B" + to_string(i) + "_Codebooks";
		dir_names.push_back(dest_dir);
		struct stat st = {0};
		if (stat(dest_dir.c_str(), &st) == -1)
			mkdir(dest_dir.c_str(), 0700);
	}

	band_vects tr_vects(NBANDS);
	//generate training vector for all bands
	for (string file : files){
		int x=0,y=0;
		int *im_array = op_pgm(x,y,file);
		int **dd_img = im_to_ddot(im_array, x, y);
		int **dd_dec = init_dd_img(x,y);

		avg_rem(dd_img, x, y);
		double *sIMG[YIMG];
		analysis(dd_img,dd_dec, sIMG, x, y);

		subbands bands = split_bands(sIMG,x,y,NSTAGES);
		/*int n_bands = bands.size();*/
		for (int i=0; i<NBANDS; i++){
			int x = bands[i].x, y = bands[i].y;
			vect_list aux = vectorize(bands[i].img,x,y,x_fr,y_fr);
			tr_vects[i].insert(tr_vects[i].end(), aux.begin(), aux.end());
			cout<<tr_vects[i].size()<<endl;
		}
	}

	//acrescentar o lbg por banda
	for (int i=0; i<NBANDS; i++){
		vect_list codebook = lbg(tr_vects[i], n_cent, x_fr*y_fr, 1e-6);
		save_codebook(codebook,x_fr,y_fr,dir_names[i] + "/codebook");
	}

	return 0;
}
