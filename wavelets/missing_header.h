#ifndef MISSING_FUNCS_H
#define MISSING_FUNCS_H

#include <stdio.h>
#include "arithmetic_coding.h"
#include "subdefs2.h"
#include <iostream>

/*using namespace std;*/

void start_model(void);           /*adaptive_model.c*/
void update_model(int);           /*adaptive_model.c*/
void start_encoding(void);        /*arithmetic_encode.c*/
//void encode_symbol(int,int[]);    /*arithmetic_encode.c*/
void encode_symbol(int,int*,FILE* = stdout);    /*arithmetic_encode.c*/
void done_encoding(FILE* = stdout);         /*arithmetic_encode.c*/
//static void bit_plus_follow(int); /*arithmetic_encode.c*/
void start_decoding(FILE*);        /*arithmetic_decode.c*/
int decode_symbol(int*,FILE*);          /*arithmetic_decode.c*/
void start_inputing_bits(void);   /*bit_input.c*/
int input_bit(FILE*);              /*bit_input.c*/
void start_outputing_bits(void);  /*bit_output.c*/
void output_bit(int, FILE* = stdout);             /*bit_output.c*/
void done_outputing_bits(FILE* = stdout);   /*bit_output.c*/

/*the funtions below belong to fqmf16A[A,S].c*/
int subanal(double*[], short int, short int, short int, short int, short int, short int, short int, short int, int);
int subsynt(double*[], short int, short int, short int, short int, short int, short int, short int, short int, int);
void sub(int**,int**,int**,int,int);
/*void sub_anal(int**,int**,int**,double**,int,int);*/
void analysis(int**,int**,double*[],int,int);
void synthesis(double*[],int**,int,int);

/*functions that sub.c needs*/
int sub4anal(double*[], int, int);
int sub4synt(double*[], int, int);

/*variables that sub.c needs*/
static int ximg, yimg, ylum, xlowsize, ylowsize, skip;
typedef struct{
	int xmin, xmax, ymin, ymax;
} frame;
static frame bband[NBANDS][NCOMPS];
static int ncomps = 1;

#endif
