#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>

#define ALPHA_SIZE 256

using namespace std;

typedef struct node{
	char val;
	unsigned count=0;
	struct node *next[ALPHA_SIZE];
	char table[ALPHA_SIZE];
	unsigned count_table[ALPHA_SIZE];
} ctx_node;

inline void update_prob(ctx_node *top, string ctx, char cur_char){
	size_t n = ctx.length();
	ctx_node *cur = top;

	for (size_t i=0; i<n; i++)
		cur = cur->next[ctx[i]];
	cur->count++;
	cur->count_table[cur_char]++;
}

int get_ctx(ctx_node **top, char *msg, unsigned n_ctx=4){
	if (!top)
		return 1;

	size_t len = strlen(msg);
	size_t i = 0;
	string cont_str="";

	i = 0;
	unsigned aux_count = 0;
	while(i<len){
		char cur_char = msg[i++];
		/*cont_str.push_back(cur_char);*/
		if (cont_str.length() == n_ctx)
			cont_str.erase(0,1);
		string str;

		string cont_aux = cont_str;
		bool found;
		main_loop:
			if (cont_aux.length()>0)
				str = cont_aux;//.substr(0,cont_aux.length());
			else
				str = "";
			size_t len_ctx = cont_aux.length();
			ctx_node *cur = top[n_ctx-(len_ctx+1)];
			found=true;

			while (cont_aux.length() > 0){
				if (!cur->next[cont_aux[0]]){
					cur->next[cont_aux[0]] = new ctx_node;
					cur = cur->next[cont_aux[0]];
					cur->val = cont_aux[0];
					for (unsigned j=0; j<ALPHA_SIZE; j++)
						cur->next[j] = (ctx_node *) NULL;
					found = false;
				}
				else
					cur = cur->next[cont_aux[0]];
				cont_aux.erase(0,1);
			}
			if ((!found) or (str.length() == 0)){
				for (unsigned j=0; j<ALPHA_SIZE; j++)
					cur->count_table[j]=0;
			}
			if (cur->count_table[cur_char] == 0)
				found=false;
			cur->count_table[cur_char]++;
			cur->count++;

			if (len_ctx > 1){
				if (cont_str.length() < n_ctx){
					unsigned l_aux = cont_str.length();
					cont_aux = cont_str.substr(l_aux - (len_ctx-1));
				}
				else
					cont_aux = cont_str.substr(n_ctx - (len_ctx-1));
			}

			if (found){
				if (str.length()>0)
					cout<<str;
				cont_aux = str;
				do{
					cont_aux.erase(0,1);
					len_ctx = cont_aux.length();
					cur = top[n_ctx-(len_ctx+1)];
					update_prob(cur, cont_aux, cur_char);
					/*cont_aux.erase(0,1);*/
				}while(cont_aux.length()>0);
			}
			else{
				cout<<str<<"<ESC>";
				if (str.length() > 0)
					goto main_loop;
			}
		cout<<cur_char<<endl;
		/*cur->count_table[cur_char]++;*/
		cont_str.push_back(cur_char);
	}

	return 0;
}

int main(void){
	unsigned n_ctx = 5;
	ctx_node *top[n_ctx];
	string msg="This is a random string to test the ppm model. Starting sequence of repetitive characters to generate long contexts: abaabaaaabbabababababbabababbcbcbdbcbabcbdbc";

	cout<<'"'<<msg<<'"'<<endl;
	for (unsigned i=0; i<n_ctx; i++){
		top[i] = new ctx_node;
		for (unsigned j=0; j<ALPHA_SIZE; j++)
			top[i]->next[j] = (ctx_node *) NULL;
	}
	get_ctx(top,(char *) msg.c_str(), n_ctx);

	return 0;
}
