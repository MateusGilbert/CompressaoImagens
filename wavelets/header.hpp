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

using namespace std;

typedef vector< float* > vect_list;//mudei
typedef vector< vect_list > band_vects;

//hand_img
void save_csv(int*, int=0, int=0, string="out.csv");
int* op_pgm(int&, int&, string="");
int** im_to_ddot(int*, int, int);
int* ddot_to_im(int**, int, int);
int* ddot_to_bands(int**, int, int);
double avg_rem(int**,int,int);//trocar para float?
void avg_add(int**,int,int,double);
int* add_padding(int*,int,int,int,int);

int** init_dd_img(int, int);

//hand_bands
typedef struct band_node{
	int x, y;
	float *img;
} subband;
typedef vector< subband > subbands;
subbands split_bands(double*[],int,int,int=NSTAGES);
subband quantize_band(subband,vect_list,int,int);
void agg_bands(subbands,double*[],int,int,int=NSTAGES,bool=false);

//quantize
vect_list read_codebook(string, int&, int&);
double eq_dist2(float*,float*,int);
float* v_encode(float*,vect_list,int,int,int,int);

//encode
vector< string > get_codebooks(int, vector< string >, string="results.dat");

//lbg_vq
vect_list vectorize(float*, int, int, int, int);
vect_list lbg(vect_list, int, int, float=.1);
void print_centroids(vect_list, int);

#endif
