/* MAIN PROGRAM FOR DECODING */

#include "header.hpp"
#include "ppm_header.hpp"

using namespace std;

/*inline int* add_exc(int *cum_freq, int *ref_cfreq, int *freq, bool *exc_aux){*/
inline int* add_exc(int *cum_freq, int *freq, bool *exc_aux){
	int cum_count=0, esc_count=0;

	for (int i=No_of_symbols; i>=ESC_symbol; i--){
		cum_freq[i] = cum_count;//ref_cfreq[i] - count;
		if ((!exc_aux[i]) and (i != ESC_symbol) and (freq[i] > 0)){
			exc_aux[i] = true;
			cum_count += freq[i];
			esc_count++;
		}
	}
	cum_freq[0] = cum_count + (esc_count ? (esc_count) : 1);

	return cum_freq;
}

int
main(int argc, char *argv[])
{
	if (argc < 2){
		help_exit:
			cerr<<"Usage: "<<argv[0]<<" -[options] [file] [n_contexts]"<<endl;
			cerr<<"Options:"<<endl;
			cerr<<"  -h, -H   Display this help and exit."<<endl;
			cerr<<"  -v, -V   Verbose, print a message each time the program process 10k chars."<<endl;
			cerr<<"  -e, -E   Encode using the exclusion principle."<<endl;
		return 1;
	}

	int *cum_freq;
	int n_ctx = 0;//-1;
	int shift = 0;//when flags passed as arguments, shifts the search for each ellement
	bool verbose=false, exclusion=false;

	while (argv[1+shift][0] == '-'){
		switch (argv[1+shift][1]){
			case 'h':;
			case 'H':
				goto help_exit;
			case 'v':;
			case 'V':
				verbose=true;
				break;
			case 'e':;
			case 'E':
				exclusion=true;
		}
		shift++;
	};

	if (argc >= 3+shift)
		n_ctx = ((int) argv[2+shift][0] - '0');

	if ((n_ctx > 10) or (n_ctx < 0))
		n_ctx = 0;

	vector< ctx_node* > contexts = start_model(n_ctx);  	  /* Set up other modules		*/
	start_inputing_bits();
	FILE *file = fopen(argv[1+shift], "r");
	string ctx_str="";
	start_decoding(file);
   
	bool table_1=true, exc_aux[No_of_symbols+1];
	unsigned long count=0;

	if (verbose)
		cerr<<"Max. context length: "<<n_ctx<<endl;
	for (;;) 								  /* Loop through characters	*/
	{
		if (exclusion)
			for (int i=0; i<No_of_symbols+1; i++)
				exc_aux[i] = false;

		if (verbose){
			count++;
			if (count % 10000 == 0)
				cerr<<"Stil decoding. "<<count/1000<<"k bytes already processed."<<endl;
		}

		size_t ctx_len = ctx_str.length();
		string str_aux = ctx_str;
		ctx_node *cur = contexts[ctx_len + 1];
		int ch; int symbol;
		int *aux = (int *) NULL;

		main_loop://when an esc is found, repeat process
			if ((table_1) or (n_ctx < 0)) //or (ctx_len > ctx_str.length())
				cum_freq = contexts[0]->cum_freq;
			else if (exclusion) {/*and (!first)*/ //and (ctx_len != ctx_str.length())){
				aux = new int[No_of_symbols+1];
				add_exc(aux, get_cum_freq(cur,str_aux,true), exc_aux);
				cum_freq = aux;
			}
			else
				cum_freq = get_cum_freq(cur,str_aux);


			symbol = decode_symbol(cum_freq,file);	  /* decode next symbol 		*/  
			if (aux){
				aux = (int *) NULL;
				delete cum_freq;
			}

			if (symbol==EOF_symbol) break;		  /* Exit loop if EOF symbol 	*/
			else if (symbol == ESC_symbol){
				str_aux.erase(0,1);		//delete first char from context string
				if (ctx_len == 0){
					cur = contexts[0];
					table_1=true;
				}
				else
					cur = contexts[ctx_len--];//notice that the context indices are shifted by one
												//(see cur definition above)
				goto main_loop;
			}
		ch = index_to_char[symbol];		  /* translate to a character 	*/
		putc(ch,stdout);				      /* write that character 		*/
		//update_model(symbol);			      /* Update the model 			*/
		update_model(contexts,ctx_str,symbol);

		//update context string
		ctx_str.push_back((char) ch);
		if (ctx_str.length() > (size_t) n_ctx)
			ctx_str.erase(0,1);
		if (ctx_str.length() > 0)
			update_model(contexts,ctx_str,ESC_symbol);//if new context does not exist, it will create it in order to use <esc>
		table_1=false;
	}

	fclose(file);

	return 0;
}
