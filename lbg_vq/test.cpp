#include "header.hpp"

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

	for (string file : files){
		int im_array = op_pgm(x,y,file);
		//fazer a conversão
	}

	return 0;
}
