#include "header.hpp"

vector< string > str2strvec(string str, string sep){
	vector< string > str_vect;
	size_t pos;

	while ((pos = str.find(sep)) != string::npos){
		string aux = str.substr(0,pos);
		str_vect.push_back(aux);
		str.erase(0, pos + sep.length());
	}
	str_vect.push_back(str);

	return str_vect;
}

codebook gen_huff_code(prob_vector vect, string sep){
	unsigned n = vect.size();
	codebook huff_code;

	prob_vector sorted = vect;
	sort(sorted.begin(),sorted.end(),cmp);

	while (n > 1){
		for (unsigned i=0; i<2; i++){
			vector< string > words = str2strvec(sorted[i].first);
			unsigned m = words.size();
			for (unsigned j=0; j<m; j++){
				if (huff_code.find(words[j]) == huff_code.end())
					huff_code[words[j]] = to_string(i);
				else
					huff_code[words[j]] += to_string(i);
				}
		}
		sorted = comb_and_sort(sorted);
		n--;
	}

	codebook::iterator it;

	for (it = huff_code.begin(); it != huff_code.end(); it++)
		reverse(it->second.begin(), it->second.end());

	return huff_code;
}

//int main(int argc, char *argv[]){
//	if (argc != 2)
//		return 1;
//
//	string filename = argv[1];
//	cout<<filename<<endl;
//
//	probs_dict p = get_prob(filename);
//	prob_vector sorted = sort_prob(p);
//
//	codebook huff_code = gen_huff_code(sorted);
//	codebook::iterator it;
//
//	cout<<"--------------------------"<<endl;
//	cout<<"    Huffman Codebook"<<endl;
//	cout<<"--------------------------"<<endl;
//	for (it = huff_code.begin(); it != huff_code.end(); it++)
//		cout<<it->first<<": "<<it->second<<endl;
//
//	return 0;
//}
