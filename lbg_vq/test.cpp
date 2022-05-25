#include "header.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/*#include <experimental/filesystem>*/

/*namespace fs = experimental::filesystem;*/

inline vect_list read_codebook(string code_file, int &x, int &y){
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

inline float eq_dist2(int *x, int *y, int size){
	float cum=0;
	for (int i=0; i<size; i++)
		cum += (float) pow(x[i] - y[i], 2);

	return cum;
}

inline int match_vect(int *vect, vect_list centroids, int size){
	int id=0, i=-1;
	float dist=3.4e+38;
	for (int *y_i : centroids){
		i++;
		float aux = eq_dist2(vect,y_i,size);
		if (aux < dist){
			dist = aux;
			id = i;
		}
	}

	return id;
}

int* v_encode(int *in_im, vect_list centroids, int x, int y, int x_fr, int y_fr){
	int *out_im = new int[x*y];

	for (int i=0; i<=y-y_fr; i += y_fr)//{
		for (int j=0; j<=x-x_fr; j += x_fr){
			int *v = new int[x_fr*y_fr];
			int count=0;
			for (int ii=0; ii<y_fr; ii++)
				for (int jj=0; jj<x_fr; jj++)
					v[count++] = in_im[(i+ii)*x + j + jj];
			//vectors.push_back(v);
			int id = match_vect(v,centroids,x_fr*y_fr);
			/*cout<<"oi"<<id<<endl;*/
			int *c = centroids[id];
			count=0;
			for (int ii=0; ii<y_fr; ii++)
				for (int jj=0; jj<x_fr; jj++)
					out_im[(i+ii)*x + j + jj] = c[count++];// in_im[(i+ii)*x + j + jj];
		}

	return out_im;
}

int main(int argc, char *argv[]){
	if (argc < 3){
		cout<<"Usage: "<<argv[0]<<" [codebook] [pgm files]"<<endl;
		return 1;
	}

	string code_file = argv[1];
	vector< string > files;
	for (int i=2; i<argc; i++)
		files.push_back(argv[i]);

	int x_fr=0, y_fr=0;
	vect_list centroids = read_codebook(code_file, x_fr, y_fr);
//	cout<<x_fr<<" "<<y_fr<<endl;
//	print_centroids(centroids, x_fr*y_fr);

	bool save_og = false;
	string dest_dir= to_string(centroids.size()) + "_" + to_string(x_fr) + "_" + to_string(y_fr);
	/*fs::create_directory(dest_dir);*/
	struct stat st = {0};
	if (stat(dest_dir.c_str(), &st) == -1)
		mkdir(dest_dir.c_str(), 0700);

	for (string file : files){
		int x=0,y=0;
		int *im_array = op_pgm(x,y,file);
		file.resize(file.size() - 3);
		file += "csv";
		if (save_og)
			save_csv(im_array, x, y, file);
		size_t pos = file.find_last_of('/');
		if (pos != string::npos)
			file = file.substr(pos);
		file = dest_dir + file;
		im_array = v_encode(im_array, centroids, x, y, x_fr, y_fr);
		save_csv(im_array, x, y, file);
	}

	return 0;
}
