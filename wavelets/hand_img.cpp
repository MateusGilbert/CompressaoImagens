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
			img[j][i] = (int) subd_round(img[j][i] + avg);//mudei

	return;
}

int** init_dd_img(int x, int y){
	int **dd_img = (int **) malloc(y*sizeof(int*));
	for (int i=0; i<y; i++)
		dd_img[i] = (int *) malloc(x*sizeof(int));

	return dd_img;
}
