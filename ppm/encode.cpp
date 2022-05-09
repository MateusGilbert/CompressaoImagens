/* MAIN PROGRAM FOR ENCODING */

#include "header.hpp"
#include "ppm_header.hpp"

using namespace std;


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
		n_ctx = 0;//-1;  if one wants to use -1 context as default

	vector< ctx_node* > contexts = start_model(n_ctx);  	  /* Set up other modules		*/
	//start_model();					/* set up other modules.	*/
	start_outputing_bits();
	start_encoding();
	string ctx_str="";
	FILE *source = fopen(argv[1+shift],"r");

	int ch; int symbol;
	unsigned long count=0;
	bool first=true;

	ch = getc(source);
	if (verbose)
		cerr<<"Max. context length: "<<n_ctx<<endl;
	while(ch != EOF){
		if (verbose){
			count++;
			if (count % 10000 == 0)
				cerr<<"Stil encoding. "<<count/1000<<"k chars already processed."<<endl;
		}
		symbol = char_to_index[ch];		 /* Translates to an index 		*/
		//update_model(symbol);		   /* Update the model		*/
		if ((n_ctx < 0) or (first)){
			encode_symbol(symbol,contexts[0]->cum_freq);		/* Encode that symbol.	*/
			first=false;
		}
		else
			ppm_encode(contexts, ctx_str, symbol, exclusion);

		if (n_ctx>=0)
			update_model(contexts, ctx_str, symbol);

		//update context string
		if (n_ctx > 0){
			ctx_str.push_back(ch);
			if (ctx_str.length() > (size_t) n_ctx)
				ctx_str.erase(0,1);
			update_model(contexts,ctx_str, ESC_symbol);//if the next context does not exist, it will create it in order to use <esc>
		}
		ch = getc(source);
	}
	//encode_symbol(EOF_symbol,cum_freq);		/* Encodes the EOF symbol 		*/
	ppm_encode(contexts, ctx_str, EOF_symbol, exclusion);
	done_encoding();				/* Send the last few bits		*/
	done_outputing_bits();
	if (verbose)
		cerr<<"Finished encoding."<<endl;

	return 0;
}
