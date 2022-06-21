#include <iostream>
#include <vector>
#include <string>

using namespace std;

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

int main(){
	string header = "# x 2 y 2 N 2\n";
	float nums[8] = {1.2, -0.3, 3.4, -9.43, -8.41, 1.32, .03, -9.4};
	char endline = '\n';

	FILE *aux = fopen("aux.dat", "wb");
	fwrite(header.c_str(), sizeof(char), header.length(), aux);
	for (int i = 0; i<8; i++){
		fwrite(&nums[i], sizeof(float), 1, aux);
		if (i % 4 == 3)
			fwrite(&endline, sizeof(char), 1, aux);
	}
	fclose(aux);

	FILE *r = fopen("aux.dat", "rb");
	vector< float* > values;

	int x,y,N;
	read_header(r,x,y,N);

	cout<<"x = "<<x<<"; y = "<<y<<"; N = "<<N<<endl;
	for (int i=0; i<N; i++){
		float *v = new float[x*y];
		float f[x*y];

		//strange but necessary; c/c++ reasons...
		fread(&f, sizeof(float), x*y, r);
		for (int i = 0; i<x*y; i++)
			v[i] = f[i];
		values.push_back(v);

		char a;
		fread(&a, sizeof(char), 1, r);
	}
	fclose(r);


	for (auto f : values){
		cout<<"[";
		for (int i = 0; i<x*y-1; i++)
			cout<<f[i]<<", ";
		cout<<f[x*y-1]<<"]"<<endl;
	}
}
