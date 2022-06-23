#include "header.hpp"
#include "subdefs2.h"

inline void char_2_int(int &num, char digit){
	if (num > 0)
		num *= 10;
	num += digit - '0';
}

int* op_pgm(int &x, int &y, string filename){
	x = y = 0;
	if (filename.length() == 0)
		return (int *) NULL;

	ifstream infile(filename.c_str());
	string line;

	getline(infile,line);
	if (line != "P5")
		cerr<<"Wrong file format! Leaving."<<endl;

	regex comm("^#");
	getline(infile,line);
	while(regex_search(line,comm))
		getline(infile,line);

	int len = line.length();

	{
		int aux=0;
		for (int i=0; i<len; i++){
			char c = line[i];
			if (c == ' '){
				x = aux;			//width
				aux = 0;
			}
			else
				char_2_int(aux, c);
		}
		y = aux;				//height
	}

	getline(infile,line);
	int max=0;
	len = line.length();
	for (int i=0; i<len; i++){
		char c = line[i];
		char_2_int(max,c);
	}

	int *im_array = new int[x*y];
	char val;
	int count=0;
	while(infile.get(val))
		if ( ((int) val) < 0)
			im_array[count++] = val + max;
		else
			im_array[count++] = val;

	infile.close();

	return im_array;
}

int* add_padding(int *og_img, int x, int y, int pad_x, int pad_y){
	int* pad_img = new int[(x + pad_x)*(y + pad_y)];
	int** mat_pad = im_to_ddot(pad_img, x+pad_x, y+pad_y);
	int** mat_img = im_to_ddot(og_img, x, y);

	for (int i=0; i<y; i++){
		for (int j=0; j<x; j++)
			mat_pad[i][j] = mat_img[i][j];
		for (int j=0; j<pad_x; j++)
			mat_pad[i][x + j] = mat_img[i][x-1];
	}

	for (int i=0; i<pad_y; i++)
		for(int j=0; j<x+pad_x; j++)
			mat_pad[i+y][j] = mat_pad[y-1][j];

	return ddot_to_im(mat_pad,x+pad_x,y+pad_y);
}

int* rem_padding(int *pad_img, int x, int y, int pad_x, int pad_y){
	int* og_img = new int[(x - pad_x)*(y - pad_y)];
	int** mat_pad = im_to_ddot(pad_img, x, y);
	int** mat_img = im_to_ddot(og_img, x-pad_x, y-pad_y);

	for (int i=0; i<y-pad_y; i++)//{
		for (int j=0; j<x-pad_x; j++)
			mat_img[i][j] = mat_pad[i][j];

	return ddot_to_im(mat_img,x-pad_x,y-pad_y);
}

void save_csv(int *img, int x, int y, string filename){
	ofstream outfile(filename);

	for (int i=0; i<y; i++){
		for (int j=0; j<x; j++){
			outfile<<img[i*x+j];
			if (j < x-1)
				outfile<<',';
		}
		outfile<<endl;
	}

	outfile.close();

	return;
}

inline void read_header(string filename, int &x, int &y){
	char a;
	string header = "";
	int count=0;
	int N;

	FILE *file = fopen(filename.c_str(), "rb");

	do{
		fread(&a, sizeof(char), 1, file);
		if (a == '\n')
			break;
		header.push_back(a);
		count++;
	}while(true);

	x = y = N = 0;
	for (int i=0; i<count; i++){
		switch (header[i]){
			case 'x':
				i++;
				while (header[++i] != ' ')
					x = x*10 + (int) (header[i] - '0');
				break;
			case 'y':
				i++;
				while (header[++i] != ' ')
					y = y*10 + (int) (header[i] - '0');
				break;
			case 'N':
				i++;
				while (header[++i] != '\0')
					N = N*10 + (int) (header[i] - '0');
				break;
		}
	}

	fclose(file);
	return;
}

void chk_pad(vector< string > codebooks, int *im_array, int &x, int &y, int &pad_x, int &pad_y){
	for (auto codebook: codebooks){
		int len = codebook.length(), n_band=0;
		for (int i=1; i<len; i++){
			int alg = (int) codebook[i] - '0';
			if ((alg >= NBANDS) or (alg < 0))
				break;
			n_band = n_band*10 + alg;
		}
		if (n_band >= NBANDS)
			cout<<"Wrong codebook! "<<codebook<<" will be ignored."<<endl;
		else{
			int p;
			if (n_band == 0)
				p = (int) pow(2,NSTAGES);
			else
				p = (int) pow(2,NSTAGES - ((int) (n_band-1) / 3));

			int x_fr=0, y_fr=0;
			read_header(codebook, x_fr, y_fr);

			pad_x = (x / p) % x_fr;
			if (pad_x != 0)
				pad_x = (x_fr - pad_x)*p;
			pad_y = (y / p) % y_fr;
			if (pad_y != 0)
				pad_y = (y_fr - pad_y)*p;
			if ((pad_x != 0) or (pad_y != 0)){
				im_array = add_padding(im_array, x, y, pad_x, pad_y);
				x += pad_x;
				y += pad_y;
			}
		}
	}
}

int** im_to_ddot(int *img, int x, int y){
	int **dd_img = init_dd_img(x,y);

	for (int i=0; i<y; i++)
		for (int j=0; j<x; j++)
			dd_img[i][j] = img[i*x+j];

	return dd_img;
}

int* ddot_to_im(int **dd_img, int x, int y){
	int *img = new int[x*y];

	for (int i=0; i<y; i++)
		for (int j=0; j<x; j++)
			img[i*x+j] = dd_img[i][j];

	return img;
}

int* ddot_to_bands(int **dd_bands, int x, int y){
	int *img = new int[x*y];

	for (int i=0; i<y; i++)
		for (int j=0; j<x; j++)
			img[i+j*y] = dd_bands[i][j];

	return img;
}

double avg_rem(int **img, int x, int y){
	double avg = 0;
	for (int i=0; i<x; i++)
		for (int j=0; j<y; j++)
			avg += (double) img[j][i];
	avg /= (double) x*y;

	for (int i=0; i<x; i++)
		for (int j=0; j<y; j++)
			img[j][i] -= (int) subd_round(avg);

	return avg;
}

void avg_add(int **img, int x, int y, double avg){
	for (int i=0; i<x; i++)
		for (int j=0; j<y; j++)
			img[j][i] += (int) subd_round(avg);//mudei

	return;
}

int** init_dd_img(int x, int y){
	int **dd_img = (int **) malloc(y*sizeof(int*));
	for (int i=0; i<y; i++)
		dd_img[i] = (int *) malloc(x*sizeof(int));

	return dd_img;
}
