#include "header.hpp"
#include "ppm_header.hpp"

int char_to_index[No_of_chars];
unsigned char index_to_char[No_of_symbols+1];
/*bool exclusion_aux[No_of_symbols+1];*/

//start_model -- size vector == n_ctx + 2 (first slot is the -1 table)
vector< ctx_node* > start_model(int n_ctx){
	vector< ctx_node* > contexts(n_ctx + 2);

	for (int i=0; i<n_ctx+2; i++){
		contexts[i] = new ctx_node;
		for (int j=0; j<No_of_chars; j++)
			contexts[i]->next[j] = (ctx_node *) NULL;
	}
	for (int i=0; i<No_of_symbols+1; i++){
		contexts[0]->freq[i] = 1;
		contexts[1]->freq[i] = 0;
		contexts[0]->cum_freq[i] = No_of_symbols-i;
		contexts[1]->cum_freq[i] = 0;
	}
	contexts[1]->cum_freq[0] = 1;

	//starting converter vectors (same as original model)
	for (int i=0; i<No_of_chars; i++){
		char_to_index[i] = i+2;		//skip first position and escape
		index_to_char[i+2] = i;
	}

	return contexts;
}

int* get_cum_freq(ctx_node* ctx, string ctx_str, bool get_freq){
	size_t ctx_l = ctx_str.length();

	ctx_node *cur = ctx; //skeep -1 table
	string aux_str = ctx_str;

	if (ctx_l == 0){
		if (get_freq)
			return ctx->freq;
		return ctx->cum_freq;
	}

	string str = aux_str;

	while (aux_str.length() > 0){
		if (!cur->next[(int) (unsigned char) aux_str[0]])
			break;
		else
			cur = cur->next[(int) (unsigned char) aux_str[0]];
		aux_str.erase(0,1);
	}

	if (get_freq)
		return cur->freq;

	return cur->cum_freq;
}

inline void up_cum_freq(int *cum_freq, int change, int *freq){//bool up_esc){
	if (freq[change] == 1){		//a new char observation
		freq[ESC_symbol]++;
		freq[0]++;
		cum_freq[ESC_symbol]++;
		cum_freq[0]++;
	}

	if (cum_freq[0] == Max_frequency){
		int cum=0;
		for (int i=No_of_symbols; i>=0; i--){
			if (freq[i] > 0)
				freq[i] = (freq[i]+1)/2;
			cum_freq[i] = cum;
			cum += freq[i];
		}
	}

	for (int i=change-1; i>=0; i--)
		cum_freq[i]++;

	return;
}

void update_model(vector< ctx_node* > ctxs, string ctx_str, int cur_char){
	size_t ctx_l = ctx_str.length();

	int ctx_counter = (int) ctx_l + 1; //+1, so we skip -1 context table
	ctx_node *cur = ctxs[ctx_counter];
	string aux_str = ctx_str;

	size_t i = 0;
	while(ctx_counter>1){//cxt_counter == 0 is the -1 context table, which shall be kept unchanged
								//ctx_counter == 1 is the 0 context table, there is no need to peform a search
		string str = aux_str;
		bool found=true;

		while (aux_str.length() > 0){
			int index = (int) (unsigned char) aux_str[0];
			if (!cur->next[index]){
				found = false;
				cur->next[index] = new ctx_node;
				cur = cur->next[index];
				/*cur->val = index;*/
				for (int j=0; j<No_of_chars; j++)
					cur->next[j] = (ctx_node *) NULL;
			}
			else
				cur = cur->next[index];
			aux_str.erase(0,1);
		}

		if ((!found) or (ctx_str.length() < 1)){
			for (int j=1; j<No_of_symbols+1; j++){
				cur->freq[j]=0;
				cur->cum_freq[j]=0;
			}
			cur->cum_freq[0] = 1;
			if (ctx_str.length() == 0)
				break;
		}

		if (cur_char != ESC_symbol){//It is actually a char from the alphabet; update counts
			cur->freq[0]++;		//We trak the frequency explicitly because of the constant the limits the maximum cummulative frequency
			cur->freq[cur_char]++;
			up_cum_freq(cur->cum_freq, cur_char, cur->freq);//up_esc);
		}
		else//if cur_char is ESC_symbol, we are creating a new (empty) context, in order to use the ESC_symbol
			if (found)
				break;

		//change context
		i++;
		if (i<ctx_l)
			aux_str = ctx_str.substr(i, ctx_l);//copy ctx_str starting from the
		else											//i-th char; droping to the next context
			aux_str = "";
		cur = ctxs[--ctx_counter];
	}

	if ((ctx_counter > 0) and (cur_char != ESC_symbol)){
		cur->freq[0]++;
		cur->freq[cur_char]++;
		up_cum_freq(cur->cum_freq, cur_char, cur->freq);
	}

	return;
}
