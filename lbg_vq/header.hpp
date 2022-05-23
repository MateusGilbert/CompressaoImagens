#ifndef PPM_HEADER_HPP
#define PPM_HEADER_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <regex>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

using namespace std;

typedef vector< int* > vect_list;
//typedef struct{
//	int N;
//	vector< int* > vectors;
//	vector< int* > codebook;
//} lbg_code;

void save_csv(int*, int=0, int=0, string="out.csv");
int* op_pgm(int&, int&, string="");
vect_list vectorize(int*, int, int, int, int);
vect_list lbg(vect_list, int, int, float=.1);

#endif
