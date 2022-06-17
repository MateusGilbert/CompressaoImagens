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
