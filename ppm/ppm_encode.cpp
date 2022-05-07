#include "header.hpp"
#include "ppm_header.hpp"

void ppm_encode(vector< ctx_node* > ctxs, string ctx_str, int cur_char){//conferir esse char
	size_t ctx_l = ctx_str.length();

	ctx_node *cur = ctxs[ctx_l+1]; //skip -1 table
	string aux_str = ctx_str;

	while(ctx_l>0){
		string str = aux_str;
		bool found=true;

		while (aux_str.length() > 0){
			if (!cur->next[(int) (unsigned char) aux_str[0]]){
				found = false;
				break;
			}
			else
				cur = cur->next[(int) (unsigned char) aux_str[0]];
			aux_str.erase(0,1);
		}
		if ((!found) or (cur->freq[cur_char] == 0)){ //second condtion is fired when context exists but
																	//but it is the first time char is met
				encode_symbol(ESC_symbol,cur->cum_freq);
			aux_str = str;
			aux_str.erase(0,1);
			ctx_l = aux_str.length();
			cur = ctxs[ctx_l+1];	//gets the next lower context
		}
		else
			break;
	}

	if (cur->freq[cur_char] == 0){// conferir isso*/
		encode_symbol(ESC_symbol,cur->cum_freq);
		cur = ctxs[0];					//move to the -1 table*/
	}
	//interestingly, if something went wrong and we are stuck
	//in a high context where cur_char has not occurred, the program will
	//go to the lowest context possible.
	encode_symbol(cur_char, cur->cum_freq);

	return;
}
