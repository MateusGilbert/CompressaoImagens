/* MAIN PROGRAM FOR ENCODING */

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

	int n_ctx = 0;//-1;

	if (argc >= 3)
		n_ctx = ((int) argv[2][0] - '0');

	if ((n_ctx > 10) or (n_ctx < 0))
		n_ctx = 0;//-1;  if one wants to use -1 context as default

	vector< ctx_node* > contexts = start_model(n_ctx);  	  /* Set up other modules		*/
	//start_model();					/* set up other modules.	*/
	start_outputing_bits();
	start_encoding();
	string ctx_str="";
	FILE *source = fopen(argv[1],"r");

	/*while (getline(file,line))*/
	int ch; int symbol;
	bool first=true;
	ch = getc(source);
	while(ch != EOF){
		symbol = char_to_index[ch];		 /* Translates to an index 		*/
		//update_model(symbol);		   /* Update the model		*/
		if ((n_ctx < 0) or (first)){
			encode_symbol(symbol,contexts[0]->cum_freq);		/* Encode that symbol.	*/
			first=false;
		}
		else
			ppm_encode(contexts, ctx_str, symbol);

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
	ppm_encode(contexts, ctx_str, EOF_symbol);
	done_encoding();				/* Send the last few bits		*/
	done_outputing_bits();

	return 0;
}
