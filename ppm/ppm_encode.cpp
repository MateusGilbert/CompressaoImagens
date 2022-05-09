#include "header.hpp"
#include "ppm_header.hpp"

inline void exclusion_imp(int *freq, bool *exc_aux, int cur_char){
	int cum_freq[No_of_symbols+1];
	int cum_count=0, esc_count=0;

	for (int i=No_of_symbols; i>=ESC_symbol; i--){
		cum_freq[i] = cum_count;
		if ((!exc_aux[i]) and (i != ESC_symbol) and (freq[i] > 0)){
			cum_count += freq[i];
			esc_count++;
		}
	}
	cum_freq[0] = cum_count + (esc_count ? (esc_count) : 1);

	encode_symbol(cur_char,cum_freq);
}

void ppm_encode(vector< ctx_node* > ctxs, string ctx_str, int cur_char, bool exclusion){
	size_t ctx_l = ctx_str.length();
	bool exclusion_aux[No_of_symbols+1];
	bool table_1=false;

	if (exclusion)
		for (int i=0; i<No_of_symbols+1; i++)
			exclusion_aux[i] = false;

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
			if (exclusion)
				exclusion_imp(cur->freq, exclusion_aux, ESC_symbol);
			else
				encode_symbol(ESC_symbol,cur->cum_freq);

			if (exclusion)				//exclusion principle
				for (int i=ESC_symbol+1; i<No_of_symbols+1; i++)
					if (cur->freq[i] > 0)
						exclusion_aux[i] = true;

			aux_str = str;
			aux_str.erase(0,1);
			ctx_l = aux_str.length();
			cur = ctxs[ctx_l+1];	//gets the next lower context
		}
		else
			break;
	}

	if (cur->freq[cur_char] == 0){
		if (exclusion)
			exclusion_imp(cur->freq, exclusion_aux, ESC_symbol);
		else
			encode_symbol(ESC_symbol,cur->cum_freq);
		cur = ctxs[0];					//move to the -1 table*/
		table_1=true;
	}
	//interestingly, if something went wrong and we are stuck
	//in a high context where cur_char has not occurred, the program will
	//go to the lowest context possible.

	if ((exclusion) and (!table_1))
		exclusion_imp(cur->freq, exclusion_aux, cur_char);
	else
		encode_symbol(cur_char, cur->cum_freq);

	return;
}
