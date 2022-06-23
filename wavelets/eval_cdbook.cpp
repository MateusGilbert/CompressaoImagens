#include "missing_header.h"
#include "header.hpp"
#include <stdlib.h>
#include <bits/stdc++.h>

//int** init_dd_img(int x, int y){
//	int **dd_img = (int **) malloc(y*sizeof(int*));
//	for (int i=0; i<y; i++)
//		dd_img[i] = (int *) malloc(x*sizeof(int));
//
//	return dd_img;
//}

int
main(int argc, char *argv[]){
	if (argc < 3){
		cout<<"Usage: "<<argv[0]<<" [codebooks] [pgm files]"<<endl;
		cout<<"\tObs.: codebooks and pgm can be inserted at random."<<endl;
		return 1;
	}

	vector< string > cli_input;
	vector< string > codebooks;
	vector< string > test_files;
	for (int i=1; i<argc; i++)
		cli_input.push_back(argv[i]);

	for (auto inp: cli_input){
		string term = inp.substr(inp.find_last_of(".") + 1);
		if (term == "dat")
			codebooks.push_back(inp);
		else if (term == "pgm")
			test_files.push_back(inp);
		else
			cout<<"Wrong file format!!! "<<inp<<" will be ignored."<<endl;
	}

	for (auto code_file: codebooks){
		string dirname = code_file.substr(0, code_file.find_last_of('/'));
		int x_fr=0, y_fr=0;
		int len = code_file.length(), n_band=0;
		for (int i=1; i<len; i++){
			int alg = (int) code_file[i] - '0';
			if ((alg >= NBANDS) or (alg < 0))
				break;
			n_band = n_band*10 + alg;
		}
		if (n_band >= NBANDS)
			cout<<"Wrong codebook! "<<code_file<<" will be ignored."<<endl;
		else{
			vect_list centroids = read_codebook(code_file, x_fr, y_fr);

			double D=0.;
			double R = log2((double) centroids.size()) / (double) (x_fr*y_fr); //log_2(#codebook)/(tam. bloco)
			for (string file : test_files){
				int x=0,y=0,pad_x=0,pad_y=0;
				int *im_array = op_pgm(x,y,file);
				vector< string > v_aux;
				v_aux.push_back(code_file);
				//check if padding is needed; adds it if it is the case
				chk_pad(v_aux, im_array, x, y, pad_x, pad_y);

				int **dd_img = im_to_ddot(im_array, x, y);
				int **dd_dec = init_dd_img(x,y);

				avg_rem(dd_img, x, y);
				double *sIMG[YIMG];
				analysis(dd_img,dd_dec, sIMG, x, y);

				//tentar compactar
				subbands aux = split_bands(sIMG,x,y,NSTAGES);
				subband band = aux[n_band];
				//quantizar a banda
				subband q_band = quantize_band(band, centroids, x_fr, y_fr);
				x = band.x; y = band.y;
				D += eq_dist2(band.img,q_band.img, x*y)/(x*y);//mse
			}
			D /= test_files.size();

			string filename = dirname + "/results.dat";
			ifstream infile(filename);
			if (!infile){
				ofstream outfile(filename);
				outfile<<"# D R Codebook";
				outfile.close();
			}
			FILE *outfile = fopen(filename.c_str(),"ab");
			char e_line = '\n';
			fwrite(&e_line,sizeof(char), 1, outfile);
			fwrite(&D, sizeof(double), 1, outfile);
			fwrite(&R, sizeof(double), 1, outfile);
			fwrite(code_file.c_str(), sizeof(char), code_file.length(), outfile);
			fclose(outfile);
		}
	}

	return 0;
}
