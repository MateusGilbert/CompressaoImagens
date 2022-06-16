#include "header.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

vect_list read_codebook(string code_file, int &x, int &y){
	ifstream file(code_file);
	string line;
	getline(file,line);
	vect_list centroids;

	int i=0, n=line.length();
	string num;
	for (int j=0; j<n; j++){
		num="";
		if (line[j] == 'x'){
			j+=2;
			while (line[j] != ' ')
				num.push_back(line[j++]);
			x = stoi(num);
		}
		else if (line[j] == 'y'){
			j+=2;
			while (line[j] != ' ')
				num.push_back(line[j++]);
			y = stoi(num);
		}
	}

	int N = x*y;
	while(getline(file,line)){
		int *y_i = new int[N];
		line.push_back(',');
		i=0;
		n=line.length();
		num="";
		for (int j=0; j<n; j++){
			while (line[j] != ',')
				num.push_back(line[j++]);
			y_i[i++] = stoi(num);
			num="";
		}
		centroids.push_back(y_i);
	}

	return centroids;
}

double eq_dist2(int *x, int *y, int size){
	double cum=0;
	for (int i=0; i<size; i++)
		cum += (double) pow(x[i] - y[i], 2);

	return cum;
}

inline int match_vect(int *vect, vect_list centroids, int size){
	int id=0, i=-1;
	double dist=3.4e+38;
	for (int *y_i : centroids){
		i++;
		double aux = eq_dist2(vect,y_i,size);
		if (aux < dist){
			dist = aux;
			id = i;
		}
	}

	return id;
}

int* v_encode(int *in_im, vect_list centroids, int x, int y, int x_fr, int y_fr){
	int *out_im = new int[x*y];

	for (int i=0; i<=y-y_fr; i += y_fr)
		for (int j=0; j<=x-x_fr; j += x_fr){
			int *v = new int[x_fr*y_fr];
			int count=0;
			for (int ii=0; ii<y_fr; ii++)
				for (int jj=0; jj<x_fr; jj++)
					v[count++] = in_im[(i+ii)*x + j + jj];
			int id = match_vect(v,centroids,x_fr*y_fr);
			int *c = centroids[id];
			count=0;
			for (int ii=0; ii<y_fr; ii++)
				for (int jj=0; jj<x_fr; jj++)
					out_im[(i+ii)*x + j + jj] = c[count++];
		}

	return out_im;
}
