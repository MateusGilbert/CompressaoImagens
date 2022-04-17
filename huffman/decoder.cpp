#include "header.hpp"

int main(int argc, char *argv[]){
	if (argc != 2){
		cout<<"Usage: "<<argv[0]<<" <encoded_file>"<<endl;
		return 1;
	}

	string filename = argv[1];
	if (filename.substr(filename.find_last_of('.') + 1) != "dat"){
		cout<<"Unsupported file. Insert a file name ending in dat."<<endl;
		return 1;
	}

	string decode_name = filename.substr(0,filename.length()-8) + "_dec.txt";

	cout<<"---------------------------"<<endl;
	cout<<" Starting Huffman Decoder"<<endl;
	cout<<"---------------------------"<<endl;
	cout<<"Decompressing "<<filename<<"..."<<endl;
	huff_decode(decode_name, filename);
	cout<<"...done! Saved @ "<<decode_name<<endl;
	cout<<"---------------------------"<<endl;

	return 0;
}
