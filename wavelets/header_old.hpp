#ifndef WAV_HEADER_HPP
#define WAV_HEADER_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <regex>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <math.h>
/*#include "header.h"*/

using namespace std;

typedef vector< int* > vect_list;
typedef vector< vect_list > band_vects;

//hand_img
void save_csv(int*, int=0, int=0, string="out.csv");
int* op_pgm(int&, int&, string="");
int** im_to_ddot(int*, int, int);
int* ddot_to_im(int**, int, int);
int* ddot_to_bands(int**, int, int);
double avg_rem(int**,int,int);
void avg_add(int**,int,int,double);

int** init_dd_img(int, int);

//hand_bands
typedef struct band_node{
	int x, y;
	int *img;
	/*struct band_node next = (band_node *) NULL;*/
} subband;
typedef vector< subband > subbands;
subbands split_bands(double*[],int,int,int);
subband quantize_band(subband,vect_list,int,int);

//quantize
vect_list read_codebook(string, int&, int&);
double eq_dist2(int*,int*,int);
int* v_encode(int*,vect_list,int,int,int,int);

//encode
vector< string > get_codebooks(int, vector< string >, string="results.csv");

//from lbg_vq
vect_list vectorize(int*, int, int, int, int);
vect_list lbg(vect_list, int, int, float=.1,bool=true);
void print_centroids(vect_list, int);

#endif
