#include "missing_header.h"
#include "header.hpp"
#include <stdlib.h>
inline void ignore_header(FILE *i_file){
	char aux;
	do{
		fread(&aux, sizeof(char), 1, i_file);
	}while(aux != '\n');
}

vector< string > get_codebooks(int lambda, vector< string > directories, string outfile){
	vector< string > codebooks;

	for (auto dir: directories){
		string filename = dir + "/" + outfile;
		string codebook, cand_cdbk;
		double D,R,best=1.7e308;
		bool close_file=false;
		FILE *res = fopen(filename.c_str(), "rb");

		/*getline(file,line);			//ignore header*/
		ignore_header(res);
		while (!close_file){
			cand_cdbk="";
			fread(&D, sizeof(double), 1, res);
			fread(&R, sizeof(double), 1, res);
			char aux;
			do{
				size_t s = fread(&aux, sizeof(char), 1, res);
				if ((aux == '\n') or (s != 1)){
					if (s != 1)
						close_file = true;
					break;
				}
				cand_cdbk.push_back(aux);
			}while(true);
			if ((R + D/lambda) < best){
				best = R + D/lambda; //conferir este D
				codebook = cand_cdbk;
			}
		}
		fclose(res);
		codebooks.push_back(codebook);
	}

	return codebooks;
}

int
main(int argc, char *argv[]){
	if (argc < NBANDS+2){//ajeitar
		cout<<"Usage: "<<argv[0]<<" -l [lambda] -d [codebook-directories] -f [files]"<<endl;
		return 1;
	}

	int lambda = stoi(argv[1]);
	int shift=2; //ajeitar e acrescentar switch
	vector< string > directories;
	while(shift < argc)
		directories.push_back(argv[shift++]);

	vector< string > codebooks = get_codebooks(lambda,directories);
	for (auto codebook: codebooks)
		cout<<codebook<<endl;

	vect_list tr_vects;
	for (string file : files){
		int x=0,y=0;
		int *im_array = op_pgm(x,y,file);
		int **dd_img = im_to_ddot(im_array, x, y);
		int **dd_out = init_dd_img(x,y);
		int **dd_dec = init_dd_img(x,y);

		//acrescentar remover padding
		//check if padding is needed; adds it if it is the case
//		int p = (int) pow(2,NSTAGES);
//		int pad_x = (x / p) % x_fr;
//		if (pad_x != 0)
//			pad_x = (x_fr - pad_x)*p;
//		int pad_y = (y / p) % y_fr;
//		if (pad_y != 0)
//			pad_y = (y_fr - pad_y)*p;
//		if ((pad_x != 0) or (pad_y != 0)){
//			im_array = add_padding(im_array, x, y, pad_x, pad_y);
//			x += pad_x;
//			y += pad_y;
//		}

		double *sIMG[YIMG];
		analysis(dd_img,dd_dec, sIMG, x, y);
		int *d_img = ddot_to_im(dd_dec, x, y);
		save_csv(d_img, x, y, file + "d.csv");

		subbands bands = split_bands(sIMG,x,y,NSTAGES);
		subbands q_bands;
		for (auto band : bands)//quantizar bandas separadamente
		//agregar bandas

		synthesis(sIMG, dd_out, x, y);
		int *r_img = ddot_to_im(dd_out, x, y);
		save_csv(r_img, x, y, file + ".csv");
	}

	return 0;
}
