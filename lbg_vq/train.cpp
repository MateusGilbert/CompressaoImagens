#include "header.hpp"

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

int main(int argc, char *argv[]){
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

	vect_list tr_vects;
	for (string file : files){
		int x=0,y=0;
		int *im_array = op_pgm(x,y,file);
		vect_list aux = vectorize(im_array,x,y,x_fr,y_fr);
		tr_vects.insert(tr_vects.end(), aux.begin(), aux.end());
	}
	vect_list codebook = lbg(tr_vects, n_cent, x_fr*y_fr, 1e-6);

	save_codebook(codebook, x_fr, y_fr);
	/*print_centroids(codebook,x_fr*y_fr);*/

	return 0;
}
