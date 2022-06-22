#include "missing_header.h"
#include "header.hpp"
#include <stdlib.h>
#include "lbg_vq/header.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

inline void save_codebook(vect_list centroids, int x, int y, string saveat="codebook"){
	int N = centroids.size();
	saveat += "_" + to_string(x) + "_" + to_string(y) + "_" + to_string(N) + ".dat";
	/*ofstream outfile(saveat);*/
	FILE *codebook = fopen(saveat.c_str(),"wb");

	string header = "# x "+ to_string(x) + " y "  + to_string(y) + " N " + to_string(N) + "\n";
	fwrite(header.c_str(), sizeof(char), header.length(), codebook);
	char e_line = '\n';
	int v_len = x*y;

	for (auto y_i : centroids){
		fwrite(&y_i[0], sizeof(float)*v_len, 1, codebook);
//		for (int i=0; i<x*y; i++)
//			fwrite(&y_i[0], sizeof(float), 1, codebook);
		fwrite(&e_line, sizeof(char), 1, codebook);
//			if (i == 0)
//				outfile<<y_i[0];
//			else
//				outfile<<","<<y_i[i];
		/*outfile<<endl;*/
	}

	/*outfile.close();*/
	fclose(codebook);

	return;
}

inline void add_null_vec(vect_list vectors, int size){
	bool found=true;

	for (auto v : vectors){
		found=true;
		for (int i=0; i<size; i++)
			if (v[i] != (float) 0){
				found=false;
				break;
			}
	}

	if (!found){
		float vect[size] = {};
		vectors.push_back(vect);
	}
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
			tr_vects[i].insert(tr_vects[i].end(), aux.begin(), aux.end());
		}
	}

	//acrescentar o lbg por banda
	for (int i=0; i<NBANDS; i++){
		vect_list codebook = lbg(tr_vects[i], n_cent, x_fr*y_fr, 1e-6);
		add_null_vec(codebook, x_fr*y_fr);
		save_codebook(codebook,x_fr,y_fr,dir_names[i] + "/codebook");
	}

	return 0;
}
