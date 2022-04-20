#include "header.hpp"

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))		//macro to check if bit @ pos is 1 or 0

int write_header(string filename, codebook header, string sep, string end_str){
	if(not (header.size() > 0))
		return 1;

	size_t sep_size = sizeof(char)*sep.length();

	ofstream outfile;
	outfile.open(filename, ios::binary | ios::out);

	codebook::iterator it;
	for (it = header.begin(); it != header.end(); it++){
		outfile.write(it->first.c_str(), sizeof(char)*it->first.length());
		outfile.write(sep.c_str(), sep_size);
		outfile.write(it->second.c_str(), sizeof(char)*it->second.length());
		outfile.write(sep.c_str(), sep_size);
	}
	outfile.write(end_str.c_str(),sizeof(char)*end_str.length());
	outfile.close();

	return 0;
}

tuple<huff_node*, unsigned> read_header(string filename, string sep, string end_str){
	size_t buff_size = 100;
	char buffer[buff_size];
	string header = "";
	bool stop = false;

	ifstream infile;
	infile.open(filename, ios::binary | ios::in);

	do{
		infile.read(buffer, buff_size);
		header += buffer;
		if (header.find(end_str) != string::npos)
			stop = true;
	}while (not stop);

	infile.close();
	unsigned end_pos = header.find(end_str);
	header.erase(end_pos - sep.length());
	end_pos += end_str.length();
	vector< string> sliced_vect = str2strvec(header, sep=sep);
	huff_node *root_node = gen_dec_tree(sliced_vect);

	return make_tuple(root_node,end_pos);
}

huff_node * gen_dec_tree(vector< string > header){
	huff_node *root = new huff_node();

	root->val = "root";
	root->left = root->right = (huff_node *) NULL;
	size_t n = header.size();
	for (size_t i = 0; i<n; i+=2){
		string val = header[i];
		string code = header[i+1];
		huff_node *cur_node = root;
		unsigned m = code.length();
		for (unsigned j=0; j<m; j++){
			if (code[j] == '0'){
				if (not cur_node->left){
					cur_node->left = new huff_node();
				}
				cur_node = cur_node->left;
			}
			else{
				if (not cur_node->right)
					cur_node->right = new huff_node();
				cur_node = cur_node->right;
			}
		}
		cur_node->val = val;
	}

	return root;
}

buffer::buffer(void){
	buff = 0;
	flush_to = "";
	return;
}

buffer::buffer(string filename){
	buff = 0;
	flush_to = filename;
	return;
}

void buffer::change_dest(string filename){
	flush_to = filename;
	return;
}

void buffer::push(bool alg){
	if (n_entries == b_size){
		flush();
		n_entries = 0;
	}

	buff = (buff << 1) | (word) alg;
	n_entries++;

	return;
}

void buffer::flush(void){
	if (n_entries == 0)
		return;

	FILE *file = fopen(flush_to.c_str(),"ab");

	while (b_size > n_entries++)
		buff <<= 1;
	fwrite(&buff,b_size/8,1,file);

	fclose(file);

	n_entries = 0;
	buff = 0;

	return;
}

int huff_encode(string filename, string og_file, codebook huff_code){
	if ((filename == "") or (og_file == "") or (huff_code.empty()))
		return 1;

	buffer huff_buff(filename);

	ifstream file(og_file, ios::in);
	string line;

	while (getline(file,line)){
		unsigned n = line.length();
		for (unsigned i=0; i<n; i++){
			string code = huff_code[string(1,line[i])];
			unsigned m = code.length();
			for (unsigned j=0; j<m; j++)
				huff_buff.push((code[j] == '1'));
		}

		string code = huff_code["EOL"];
		unsigned m = code.length();
		for (unsigned j=0; j<m; j++)
			huff_buff.push((code[j] == '1'));
	}
	file.close();

	string code = huff_code["EOF"];
	unsigned m = code.length();
	for (unsigned j=0; j<m; j++)
		huff_buff.push((code[j] == '1'));
	huff_buff.flush();

	return 0;
}

int huff_decode(string filename, string enc_file){//; huff_node *huff_code){
	if ((filename == "") or (enc_file == ""))
		return 1;

	word dec_buff;
	unsigned dec_size = sizeof(word)*8;		//sizeof will return sizeof(char*2)
	unsigned buff_size=100;
	string w_buff = "";
	huff_node *dec_tree, *cur_pos;
	unsigned st_pos;
	bool w_op = true;

	tie(dec_tree,st_pos) = read_header(enc_file);

	FILE *infile = fopen(enc_file.c_str(),"rb");
	fseek(infile, st_pos, SEEK_SET);

	cur_pos = dec_tree;
	string end_str = "EOF";
	bool leave=false;
	while ( not leave ) {
		fread(&dec_buff, dec_size/8, 1, infile);
		for (unsigned i=1; i<=dec_size; i++){
			string val = "";
			if (CHECK_BIT(dec_buff, dec_size-i))
				if (cur_pos->right)
					cur_pos = cur_pos->right;
				else{
					val = cur_pos->val;
					cur_pos = dec_tree->right;
				}
			else
				if (cur_pos->left)
					cur_pos = cur_pos->left;
				else{
					val = cur_pos->val;
					cur_pos = dec_tree->left;
				}

			if (val != ""){
				if (val == "EOL")
					val = '\n';
				else if (val == "EOF"){
					leave = true;
				}
				if (not leave)
					w_buff += val;
			}

			if (w_buff.length() == buff_size){
				ofstream outfile;
				if (w_op){
					ifstream aux(filename);
					if (not aux.fail()){
						aux.close();
						remove(filename.c_str());
					}
					w_op = false;
				}
				outfile.open(filename, ios::app);
				outfile.write(w_buff.c_str(), sizeof(char)*w_buff.length());
				outfile.close();
				w_buff = "";
			}
		}
	}
	fclose(infile);

	if (w_buff != ""){
		ofstream outfile(filename, ios::app);
		outfile.write(w_buff.c_str(), sizeof(char)*w_buff.length());
		outfile.close();
	}

	return 0;
}

int print_tree(huff_node *node, string code){
	if (not node)
		return 1;

	if (node->val != "")
		cout<<node->val<<" -- "<<code<<endl;

	if (node->left)
		print_tree(node->left, code+"0");

	if (node->right)
		print_tree(node->right, code+"1");

	return 0;
}

//int main(int argc, char *argv[]){
//	if (argc != 2)
//		return 1;
//
//	string filename = argv[1];
//	string huff_name = filename.substr(0,filename.length()-4) + "_huf.dat";
//	string decode_name = filename.substr(0,filename.length()-4) + "_dec.txt";
//
//	probs_dict p = get_prob(filename);
//	prob_vector sorted = sort_prob(p);
//
//	codebook huff_code = gen_huff_code(sorted);
//	write_header(huff_name, huff_code);
//	cout<<"Compressing "<<filename<<"..."<<endl;
//	huff_encode(huff_name, filename, huff_code);
//	cout<<"...done! Saved @ "<<huff_name<<endl;
//
//	cout<<"Decompressing "<<huff_name<<"..."<<endl;
//	huff_decode(decode_name, huff_name);
//	cout<<"...done! Saved @ "<<decode_name<<endl;
//
//	return 0;
//}
