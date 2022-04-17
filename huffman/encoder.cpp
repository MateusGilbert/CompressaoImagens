#include "header.hpp"

int main(int argc, char *argv[]){
	if (argc != 2){
		cout<<"Usage: "<<argv[0]<<" <file>"<<endl;
		return 1;
	}

	string filename = argv[1];
	string huff_name = filename.substr(0,filename.length()-4) + "_huf.dat";

	probs_dict p = get_prob(filename);
	prob_vector sorted = sort_prob(p);

	cout<<"---------------------------"<<endl;
	cout<<" Starting Huffman Encoder"<<endl;
	cout<<"---------------------------"<<endl;
	codebook huff_code = gen_huff_code(sorted);
	write_header(huff_name, huff_code);
	cout<<"Compressing "<<filename<<"..."<<endl;
	huff_encode(huff_name, filename, huff_code);
	cout<<"...done! Saved @ "<<huff_name<<endl;
	cout<<"---------------------------"<<endl;

	return 0;
}
