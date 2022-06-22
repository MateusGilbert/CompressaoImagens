#include "header.hpp"

int main(int argc, char *argv[]){
	if (argc < 2)
		return 1;

	string im_file = argv[1];
	cout<<argv[0]<<endl;
	int x=0,y=0;
	int *im_array = op_pgm(x,y,im_file);
	int *new_array = add_padding(im_array, x, y, 8, 4);
	save_csv(new_array,x+8,y+4,"boat_pad.csv");

	return 0;
}
