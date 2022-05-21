#include "header.hpp"

inline void char_2_int(int &num, char digit){
	if (num > 0)
		num *= 10;
	num += digit - '0';
}

int* op_pgm(int &x, int &y, string filename=){
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
	while(infile.get(val))//tentei colocar mais compacto, deu erro
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

	return;
}

//int main(int argc, char *argv[]){
//	if (argc < 2){
//		cerr<<"Usage: "<<argv[0]<<"<pgm file>"<<endl;
//		return 1;
//	}
//
//	int x=0, y=0;
//	int *im_array = op_pgm(x,y,argv[1]);
//	cout<<x<<" "<<y<<endl;
//	save_csv(im_array,x,y);
//
//	return 0;
//}
