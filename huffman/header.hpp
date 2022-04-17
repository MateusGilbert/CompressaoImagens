#ifndef GET_FUNCS_H
#define GET_FUNCS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>		//sort function
#include <tuple>

using namespace std;

typedef unsigned char byte;
typedef unsigned short word;
typedef pair<string, double> prob_pair;
typedef vector< prob_pair > prob_vector;
typedef map<string, long unsigned> accumulator;
typedef map<string, double> probs_dict;
typedef map<string, string> codebook;
typedef struct node {
	string val = "";
	struct node *left = (struct node *) NULL, *right = (struct node *) NULL;
} huff_node;

probs_dict get_prob(string);
prob_vector sort_prob(probs_dict);
prob_vector comb_and_sort(prob_vector,string="~s~");
codebook gen_huff_code(prob_vector,string="~s~");
bool cmp(prob_pair,prob_pair);
bool l_cmp(prob_pair,prob_pair);
vector< string > str2strvec(string,string="~s~");
int write_header(string, codebook, string="~s~", string="\\end\\");
tuple<huff_node*, unsigned> read_header(string, string="~s~", string="\\end\\");
huff_node* gen_dec_tree(vector< string >);
int huff_encode(string, string, codebook);
int huff_decode(string, string);
int print_tree(huff_node*,string="");

class buffer{
	buffer(void);

	public:
		buffer(string);
		string flush_to;
		void flush(void);
		void push(bool);
		void change_dest(string);

	private:
		const int b_size=sizeof(word)*8;		//sizeof will return sizeof(char*2)
		int n_entries=0;
		word buff;
};

#endif
