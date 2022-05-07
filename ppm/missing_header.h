#ifndef MISSING_FUNCS_H
#define MISSING_FUNCS_H

#include <stdio.h>
#include "arithmetic_coding.h"

void start_model(void);           /*adaptive_model.c*/
void update_model(int);           /*adaptive_model.c*/
void start_encoding(void);        /*arithmetic_encode.c*/
//void encode_symbol(int,int[]);    /*arithmetic_encode.c*/
void encode_symbol(int,int*);    /*arithmetic_encode.c*/
void done_encoding(void);         /*arithmetic_encode.c*/
//static void bit_plus_follow(int); /*arithmetic_encode.c*/
void start_decoding(FILE*);        /*arithmetic_decode.c*/
int decode_symbol(int*,FILE*);          /*arithmetic_decode.c*/
void start_inputing_bits(void);   /*bit_input.c*/
int input_bit(FILE*);              /*bit_input.c*/
void start_outputing_bits(void);  /*bit_output.c*/
void output_bit(int);             /*bit_output.c*/
void done_outputing_bits(void);   /*bit_output.c*/

#endif
