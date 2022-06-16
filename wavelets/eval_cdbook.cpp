#include "missing_header.h"
#include "header.hpp"
#include <stdlib.h>
#include <bits/stdc++.h>

int** init_dd_img(int x, int y){
	int **dd_img = (int **) malloc(y*sizeof(int*));
	for (int i=0; i<y; i++)
		dd_img[i] = (int *) malloc(x*sizeof(int));

	return dd_img;
}

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
		if (term == "csv")
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
			if (alg <= NBANDS)
				break;
			n_band = n_band*10 + alg;
		}
		if (n_band >= NBANDS)
			cout<<"Wrong codebook! "<<code_file<<" will be ignored."<<endl;
		else{
			vect_list centroids = read_codebook(code_file, x_fr, y_fr);

			double D=0., aux=0.;
			double R= log2((double ) centroids.size()) / (double) test_files.size(); //log_2(#codebook)/N_files
			for (string file : test_files){
				int x=0,y=0;
				int *im_array = op_pgm(x,y,file);
				aux += 1./((double) x*y);
				int **dd_img = im_to_ddot(im_array, x, y);
				int **dd_dec = init_dd_img(x,y);

				avg_rem(dd_img, x, y);
				double *sIMG[YIMG];
				analysis(dd_img,dd_dec, sIMG, x, y);

				subband band = split_bands(sIMG,x,y,NSTAGES)[n_band];
				//quantizar a banda
				subband q_band = quantize_band(band, centroids, x_fr, y_fr);
				D += eq_dist2(band.img,q_band.img, x*y)/(x*y);//mse
			}
			R *= aux;
			D /= (double) test_files.size();

			ifstream infile(dirname + "/results.csv");
			if (infile){
				infile.close();
				ofstream outfile(dirname + "/results.csv", ios_base::app);
				outfile<<code_file + "," + to_string(D) + "," + to_string(R)<<endl;
				outfile.close();
			}
			else{
				ofstream outfile(dirname + "/results.csv");
				outfile<<"Codebook,D,R"<<endl;
				outfile<<code_file + "," + to_string(D) + "," + to_string(R)<<endl;
				outfile.close();
			}
		}
	}

	return 0;
}
