#include "header.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

inline void read_header(FILE* file, int &x, int &y, int &N){
	char a;
	string header = "";
	int count=0;

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

	return;
}

vect_list read_codebook(string code_file, int &x, int &y){
	FILE *codebook = fopen(code_file.c_str(), "rb");
	vect_list centroids;

	int N;
	read_header(codebook,x,y,N);

	for (int i=0; i<N; i++){
		float *v = new float[x*y];
		float f[x*y];

		//strange but necessary; c/c++ reasons...
		fread(&f, sizeof(float), x*y, codebook);
		for (int i = 0; i<x*y; i++)
			v[i] = f[i];
		centroids.push_back(v);

		char a;
		fread(&a, sizeof(char), 1, codebook);
	}
	fclose(codebook);

	return centroids;
}

double eq_dist2(float *x, float *y, int size){
	double cum=0;
	for (int i=0; i<size; i++)
		cum += (double) pow(x[i] - y[i], 2);

	return cum;
}

inline int match_vect(float *vect, vect_list centroids, int size){
	int id=0, i=-1;
	double dist=3.4e+38;
	for (float *y_i : centroids){
		i++;
		double aux = eq_dist2(vect,y_i,size);
		if (aux < dist){
			dist = aux;
			id = i;
		}
	}

	return id;
}

float* v_encode(float *in_im, vect_list centroids, int x, int y, int x_fr, int y_fr){
	float *out_im = new float[x*y];

	for (int i=0; i<=y-y_fr; i += y_fr)
		for (int j=0; j<=x-x_fr; j += x_fr){
			float *v = new float[x_fr*y_fr];
			int count=0;
			for (int ii=0; ii<y_fr; ii++)
				for (int jj=0; jj<x_fr; jj++)
					v[count++] = in_im[(i+ii)*x + j + jj];
			int id = match_vect(v,centroids,x_fr*y_fr);
			float *c = centroids[id];
			count=0;
			for (int ii=0; ii<y_fr; ii++)
				for (int jj=0; jj<x_fr; jj++)
					out_im[(i+ii)*x + j + jj] = c[count++];
		}

	return out_im;
}

int* v_count(float *in_im, vect_list centroids, int x, int y, int x_fr, int y_fr){
	int n = centroids.size();
	int *cum_count = new int[n];
	for (int i=0; i<n; i++)
		cum_count[i] = 0;

	for (int i=0; i<=y-y_fr; i += y_fr)
		for (int j=0; j<=x-x_fr; j += x_fr){
			float *v = new float[x_fr*y_fr];
			int count=0;
			for (int ii=0; ii<y_fr; ii++)
				for (int jj=0; jj<x_fr; jj++)
					v[count++] = in_im[(i+ii)*x + j + jj];
			int id = match_vect(v,centroids,x_fr*y_fr);
			for (int i=id; i>=0; i--)
				cum_count[i] += 1;
		}

	return cum_count;
}

int* v_enc(float *in_im, vect_list centroids, int x, int y, int x_fr, int y_fr){
	/*int *v_img = new int[(x / x_fr)*(y / y_fr)];*/
	int*  v_im = new int[(x / x_fr)*(y / y_fr)];

	int i_v = 0;
	for (int i=0; i<=y-y_fr; i += y_fr)
		for (int j=0; j<=x-x_fr; j += x_fr){
			float *v = new float[x_fr*y_fr];
			int count=0;
			for (int ii=0; ii<y_fr; ii++)
				for (int jj=0; jj<x_fr; jj++)
					v[count++] = in_im[(i+ii)*x + j + jj];
			v_im[i_v++] = match_vect(v,centroids,x_fr*y_fr);
			/*int id = match_vect(v,centroids,x_fr*y_fr);*/
		}

	return v_im;
}


float* v_dec(int *v_im, vect_list centroids, int x, int y, int x_fr, int y_fr){
	float *out_im = new float[x*y];

	int i_vects = 0;
	for (int i=0; i<=y-y_fr; i += y_fr)
		for (int j=0; j<=x-x_fr; j += x_fr){
			/*float *v = new float[x_fr*y_fr];*/
			/*int count=0;*/
//			for (int ii=0; ii<y_fr; ii++)
//				for (int jj=0; jj<x_fr; jj++)
//					v[count++] = in_im[(i+ii)*x + j + jj];
//			int id = match_vect(v,centroids,x_fr*y_fr);
			int id = v_im[i_vects++];
			float *c = centroids[id];
			int count=0;
			for (int ii=0; ii<y_fr; ii++)
				for (int jj=0; jj<x_fr; jj++)
					out_im[(i+ii)*x + j + jj] = c[count++];
		}

	return out_im;
}
