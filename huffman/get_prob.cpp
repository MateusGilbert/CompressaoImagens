#include "header.hpp"

bool cmp(prob_pair a, prob_pair b){
	return a.second < b.second;
}

bool l_cmp(prob_pair a, prob_pair b){
	return a.first.length() < b.first.length();
}

probs_dict get_prob(string filename){
	accumulator acc = {{"chars", 1}, {"EOL", 0}, {"EOF", 1}};
	probs_dict p;

	ifstream file(filename);
	string line;
	while (getline(file, line)){
		unsigned n = line.length();
		for (unsigned i=0; i < n; i++)
			if (acc.find(string (1,line[i])) == acc.end())
				acc[string (1,line[i])] = 1;
			else
				acc[string (1,line[i])]++;
		acc["chars"] += n+1;
		acc["EOL"]++;
	}
	file.close();

	for (accumulator::iterator i = acc.begin(); i != acc.end(); i++)
		p[i->first] = static_cast< double >(i->second)/static_cast< double >(acc["chars"]);

	p.erase("chars");
	return p;
}

prob_vector sort_prob(probs_dict p){
	prob_vector vect(p.begin(), p.end());
	sort(vect.begin(), vect.end(), cmp);

	return vect;
}

prob_vector comb_and_sort(prob_vector p, string sep){
	unsigned n = p.size();
	if (n < 3)
		return p;

	prob_vector vect(n-1);
	string new_entry = p[1].first + sep +  p[0].first;
	double new_p = p[1].second + p[0].second;

	vect[0] = {new_entry, new_p};
	for (unsigned i = 1; i < n-1; i++)
		vect[i] = p[i+1];

	sort(vect.begin(), vect.end(), cmp);
	bool pos_change = true;
	n--;
	while (pos_change){
		prob_vector aux = vect;
		for (unsigned i = 0; i < n-1; i++)
			if (aux[i] == aux[i+1])
				sort(aux.begin()+i, aux.begin()+(i+1),l_cmp);

		if (aux == vect)
			pos_change = false;
		else
			vect = aux;
	}

	return vect;
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
//	cout<<"--------------------------"<<endl;
//	cout<<"    Unsorted Probs"<<endl;
//	cout<<"--------------------------"<<endl;
//	for (probs_dict::iterator i = p.begin(); i != p.end(); i++)
//		cout<<i->first + ": "<<i->second<<endl;
//
//	cout<<"--------------------------"<<endl;
//	cout<<"    Sorted Probs"<<endl;
//	cout<<"--------------------------"<<endl;
//	unsigned n = sorted.size();
//	for (unsigned i = 0; i < n; i++)
//		cout<<sorted[i].first + ": "<<sorted[i].second<<endl;
//
//	for (unsigned i = 0; i < n; i++){
//		cout<<"--------------------------"<<endl;
//		cout<<"    New Probs"<<endl;
//		cout<<"--------------------------"<<endl;
//
//		sorted = comb_and_sort(sorted);
//		unsigned m = sorted.size();
//		for (unsigned i = 0; i < m; i++)
//			cout<<sorted[i].first + ": "<<sorted[i].second<<endl;
//	}
//
//	return 0;
//}
