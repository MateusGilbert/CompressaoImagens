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

		ignore_header(res);
		while (!close_file){
			cand_cdbk="";
			fread(&D, sizeof(double), 1, res);
			fread(&R, sizeof(double), 1, res);
			char aux;
			bool ignore=false;
			do{
				size_t s = fread(&aux, sizeof(char), 1, res);
				if (aux == '#')
					ignore=true;
				if ((aux == '\n') or (s != 1)){
					if (s != 1)
						close_file = true;
					break;
				}
				cand_cdbk.push_back(aux);
			}while(true);
			if ((!ignore) and ((R + D/lambda) < best)){
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
	if (argc < NBANDS+3){
		cout<<"Usage: "<<argv[0]<<" -l [lambda] -d [codebook-directories] [files]"<<endl;
		return 1;
	}

	vector< string > cli_input;
	vector< string > directories;
	vector< string > files;
	for (int i=1; i<argc; i++)
		cli_input.push_back(argv[i]);

	bool n_lamb = false;
	int lambda;
	for (auto inp: cli_input)
		if (n_lamb){
			n_lamb = false;
			lambda = stoi(inp);
		}
		else{
			string term = inp.substr(inp.find_last_of(".") + 1);
			if (term == "pgm")
				files.push_back(inp);
			else if (inp == "-l")
				n_lamb = true;
			else if (inp[0] == 'B')
				directories.push_back(inp);//gambiarra
			else
				cout<<"Wrong file format!!! "<<inp<<" will be ignored."<<endl;
		}

	vector< string > codebooks = get_codebooks(lambda,directories);
	sort(codebooks.begin(), codebooks.end());
	for (auto codebook: codebooks)
		cerr<<codebook<<endl;

	//get cum_freq for the arithmetic compressor
	vector< int* > cum_freqs;
	for (auto codebook: codebooks){
		string dirname = codebook.substr(0, codebook.find_last_of('/')+1);
		vector<int> aux = get_cumfreq(dirname + "cummulative_freqs.dat", codebook);
		int n = aux.size();
		int *cf = new int[n];
		for (int i=0; i<n; i++)
			cf[i] = aux[i];
		cum_freqs.push_back(cf);
	}

	/*vect_list tr_vects;*/
	for (auto file : files){
		cout<<"Encoding "<<file<<"..."<<endl;
		int x=0,y=0,pad_x=0,pad_y=0;
		int *im_array = op_pgm(x,y,file);
		//check if padding is needed; adds it if it is the case
		chk_pad(codebooks, im_array, x, y, pad_x, pad_y);

		int **dd_img = im_to_ddot(im_array, x, y);
		int **dd_dec = init_dd_img(x,y);

		double avg = avg_rem(dd_img, x, y);
		double *sIMG[YIMG];
		analysis(dd_img,dd_dec, sIMG, x, y);

		subbands bands = split_bands(sIMG,x,y,NSTAGES);
		cod_subbs v_bands(NBANDS);

		//quantizar bandas
		for (auto codebook : codebooks){
			vect_list cd_vects;
			string dirname = codebook.substr(0, codebook.find_last_of('/'));
			int x_fr=0, y_fr=0;
			int len = dirname.length(), n_band=0;
			for (int i=1; i<len; i++){
				int alg = (int) dirname[i] - '0';
				if ((alg >= NBANDS) or (alg < 0))
					break;
				n_band = n_band*10 + alg;
			}
			if (n_band >= NBANDS)
				cout<<"Wrong codebook! "<<codebook<<" will be ignored."<<endl;
			else
				cd_vects = read_codebook(codebook, x_fr, y_fr);

			v_bands[n_band] = quantize_band2(bands[n_band], cd_vects, x_fr, y_fr);
		}

		string outfile = file.substr(0, file.find_last_of('.')) + "_l" + to_string(lambda) + ".dat";
		FILE *cmp_file = fopen(outfile.c_str(), "wb");
		write_header(cmp_file, codebooks, v_bands, x, y, pad_x, pad_y, avg);

		start_outputing_bits();
		start_encoding();
		for (int i=0; i<NBANDS; i++){
			cout<<"... #"<<i<<" band..."<<endl;
			int *v_img = v_bands[i].vects;
			int n_vects = (v_bands[i].x / v_bands[i].x_fr)*(v_bands[i].y / v_bands[i].y_fr);

			for (int j=0; j<n_vects; j++)
				encode_symbol(v_img[j]+1, cum_freqs[i], cmp_file);
		}

		done_encoding(cmp_file);
		done_outputing_bits(cmp_file);
		fclose(cmp_file);
		cout<<"... done."<<endl;
	}

	return 0;
}
