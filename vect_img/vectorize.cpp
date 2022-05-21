#include "header.hpp"

vector< int* >vectorize(int *im, int x, int y, int x_fr, int y_fr){
	vector< int *> vectors;

	if ((x <= 0) or (y <= 0) or (x % x_fr != 0 ) or (y % y_fr != 0)){
		vectors.push_back((int *) NULL);
		return vectors;
	}

	if (x_fr < 0)
		x_fr = 1;
	if (y_fr < 0)
		y_fr = 1;

	for (int i=0; i<=y-y_fr; i += y_fr)
		for (int j=0; j<=x-x_fr; j += x_fr){
			int *v = new int[x_fr*y_fr];
			int count=0;
			for (int ii=0; ii<y_fr; ii++)
				for (int jj=0; jj<x_fr; jj++)
					v[count++] = im[(i+ii)*x + j + jj];
			vectors.push_back(v);
		}

	return vectors;
}

//inline void print_im(int *im, int x=0, int y=0){
//	if ((x==0) or (y==0))
//		return;
//
//	cout<<"Image:"<<endl;
//	for (int i=0; i<y; i++){
//		for (int j=0; j<x; j++){
//			cout<<im[i*x+j];
//			if (j < x-1)
//				cout<<',';
//		}
//		cout<<endl<<endl;
//	}
//
//	return;
//}
//
//int main(void){
//	int x=6, y=4, x_fr=2, y_fr=2;
//	int im[x*y]= {
//		0,1,2,3,4,5,
//		6,7,8,9,10,11,
//		1,2,2,3,2,12,
//		1,2,2,3,4,5
//	};
//
//	print_im(im, x, y);
//
//	vector< int* > im_vects = vectorize(im, x, y, x_fr, y_fr);
//
//	for (int *it : im_vects){
//		cout<<"vect = [ ";
//		for (int i=0; i<x_fr*y_fr; i++)
//			cout<<it[i]<<" ";
//		cout<<"]"<<endl;
//	}
//
//	return 0;
//}
