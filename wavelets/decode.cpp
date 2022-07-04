#include "missing_header.h"
#include "header.hpp"
#include <stdlib.h>
int
main(int argc, char *argv[]){
	if (argc < NBANDS+2){
		cout<<"Usage: "<<argv[0]<<" [codebook-directories] [files]"<<endl;
		return 1;
	}

	vector< string > cli_input;
	vector< string > directories;
	vector< string > files;
	for (int i=1; i<argc; i++)
		cli_input.push_back(argv[i]);

	for (auto inp: cli_input){
		string term = inp.substr(inp.find_last_of(".") + 1);
		if (term == "dat")
			files.push_back(inp);
		else if (inp[0] == 'B')
			directories.push_back(inp);//gambiarra
		else
			cout<<"Wrong file format!!! "<<inp<<" will be ignored."<<endl;
	}

	for (auto file : files){
		cout<<"Decoding "<<file<<"..."<<endl;
		vector< string > codebooks;
		subbands bands;
		int x, y, pad_x, pad_y;
		double avg;

		FILE *f_in = fopen(file.c_str(),"rb");
		read_header(f_in, codebooks, bands, x, y, pad_x, pad_y, avg);
		start_inputing_bits();
		start_decoding(f_in);
		sort(codebooks.begin(),codebooks.end());

		for (auto codebook: codebooks)
			cerr<<codebook<<endl;

		int n = bands.size();
		cod_subbs v_bands(n);
		for (int i=0; i<n; i++){
			v_bands[i].x = bands[i].x;
			v_bands[i].y = bands[i].y;
		}

		//recover quantized bands
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

			cout<<"... #"<<n_band<<" band..."<<endl;
			v_bands[n_band].x_fr = x_fr;
			v_bands[n_band].y_fr = y_fr;
			vector<int> aux = get_cumfreq(dirname + "/cummulative_freqs.dat", codebook);
			int n = aux.size();
			int *cf = new int[n];
			for (int i=0; i<n; i++)
				cf[i] = aux[i];

			int n_vects = (v_bands[n_band].x / x_fr)*(v_bands[n_band].y / y_fr);
			v_bands[n_band].vects = new int[n_vects];

			//get vector indices
			for (int i=0; i<n_vects; i++)
				v_bands[n_band].vects[i] = decode_symbol(cf,f_in)-1;
			bands[n_band] = dec_vects(v_bands[n_band], cd_vects);
		}
		fclose(f_in);
		cout<<"...done."<<endl;

		//aggregate bands
		double *sIMG[YIMG];
		int **dd_out = init_dd_img(x,y);
		agg_bands(bands, sIMG, x, y, NSTAGES, true);

		synthesis(sIMG, dd_out, x, y);
		avg_add(dd_out, x, y, avg);

		int *r_img = ddot_to_im(dd_out, x, y);
		if ((pad_x != 0) or (pad_y != 0))
			r_img = rem_padding(r_img, x, y, pad_x, pad_y);
		string saveat = file.substr(0,file.find_last_of('.')+1);
		save_csv(r_img, x-pad_x, y-pad_y, saveat + "csv");
		save_pgm(r_img, x-pad_x, y-pad_y, saveat + "pgm");
	}

	return 0;
}
