/* MAIN PROGRAM FOR DECODING */

#include "header.hpp"
#include "ppm_header.hpp"

using namespace std;


int
main(int argc, char *argv[])
{  
	if (argc < 2){
		cout<<"Usage: "<<argv[0]<<" <file> [<n_contexts>]"<<endl;
		return 1;
	}

	int *cum_freq;//[No_of_symbols+1];
	int n_ctx = 0;

	if (argc >= 3)
		n_ctx = ((int) argv[2][0] - '0');

	if ((n_ctx > 10) or (n_ctx < 0))
		n_ctx = 0;

	vector< ctx_node* > contexts = start_model(n_ctx);  	  /* Set up other modules		*/
	start_inputing_bits();
	FILE *file = fopen(argv[1], "r");
	string ctx_str="";
	start_decoding(file);
   
	bool first=true;
	for (;;) 								  /* Loop through characters	*/
	{					          
		size_t ctx_len = ctx_str.length();
		string str_aux = ctx_str;
		ctx_node *cur = contexts[0 ? (first) : ctx_len + 1];
		first=false;
		int ch; int symbol;

		main_loop://esta c/ algum erro, talvez os indices!
			cum_freq = get_cum_freq(cur,str_aux);
			symbol = decode_symbol(cum_freq,file);	  /* decode next symbol 		*/  
			if (symbol==EOF_symbol) break;		  /* Exit loop if EOF symbol 	*/
			else if (symbol == ESC_symbol){
				str_aux.erase(0,1);		//delete first char from context string
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
		update_model(contexts,ctx_str, ESC_symbol);//if new context does not exist, it will create it in order to use <esc>
	}

	fclose(file);

	return 0;
}
