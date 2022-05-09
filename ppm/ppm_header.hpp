#ifndef PPM_H
#define PPM_H

#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include "model.h"
#include "arithmetic_coding.h"

/*#define ALPHA_SIZE 256*/

using namespace std;

typedef struct node{
	/*char val;*/
	struct node *next[No_of_chars];//[No_of_symbols];
	char table[No_of_symbols];  //char_to_index
	unsigned char index_to_char[No_of_symbols+1];
	int freq[No_of_symbols+1];
	int cum_freq[No_of_symbols+1];
} ctx_node;

vector< ctx_node* > start_model(int=0);
void ppm_encode(vector< ctx_node* >, string, int, bool=false);
void update_model(vector< ctx_node* > ctxs, string, int);
int* get_cum_freq(ctx_node*,string="",bool=false);

#endif
