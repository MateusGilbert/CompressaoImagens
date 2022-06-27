#include "header.hpp"
#include "subdefs2.h"
#include <cstdio>
#include <algorithm>

vector<int> get_cumfreq(string cum_freqs, string codebook){
	vector< int > cum_freq;
	string dirname = cum_freqs.substr(0, cum_freqs.find_last_of('/'));
	int n_dirname = dirname.length();
	ifstream file(cum_freqs);

	string line;
	bool found=false;
	while (getline(file,line)){
		if (line.substr(0, n_dirname) == dirname){
			string aux = dirname;;
			int n_max = codebook.length();
			for (int i=n_dirname; i<n_max; i++)
				aux.push_back(line[i]);

			if (aux == codebook){
				ofstream tmp("tmp.dat");
				tmp<<line.substr(aux.length());
				do{
					getline(file,line);
					if (line.substr(0, n_dirname) == dirname)
						break;
					tmp<<endl<<line;
				}while(true);

				tmp.close();
				file.close();
				found=true;
				break;
			}
		}
	}

	if (!found)
		return cum_freq;

	FILE *f_tmp = fopen("tmp.dat", "rb");
	int num;
	while(fread(&num, sizeof(int), 1, f_tmp))
		cum_freq.push_back(num);
	cum_freq.push_back(0);

	fclose(f_tmp);
	remove("tmp.dat");

	return cum_freq;
}

string get_cdbk_name(int id, string trk){
	ifstream file(trk);
	string line;
	int i=0;
	do getline(file,line);
	while(i++ < id);
	file.close();

	return line;
}

int get_cdbk_id(string codebook, string trk){
	ifstream file(trk);
	string line;
	int i=0, id=-1;
	while (getline(file,line)){
		if (line == codebook){
			id = i;
			break;
		}
		i++;
	}
	file.close();

	return id;
}

void write_header(FILE *f, vector< string > codebooks, cod_subbs bands, int x, int y, int pad_x, int pad_y, double avg, string trk){
	sort(codebooks.begin(), codebooks.end());

	fwrite(&x, sizeof(int), 1, f);
	fwrite(&y, sizeof(int), 1, f);
	fwrite(&pad_x, sizeof(int), 1, f);
	fwrite(&pad_y, sizeof(int), 1, f);
	fwrite(&avg, sizeof(double), 1, f);
	for (int i=0; i<NBANDS; i++){
		int bx = bands[i].x;
		int by = bands[i].y;
		string dirname = codebooks[i].substr(0, codebooks[i].find_last_of('/')+1);
		int id = get_cdbk_id(codebooks[i], dirname + trk);
		fwrite(&id, sizeof(int), 1, f);
		fwrite(&bx, sizeof(int), 1, f);
		fwrite(&by, sizeof(int), 1, f);
	}
	return;
}

void read_header(FILE *f, vector< string > &codebooks, subbands &bands, int &x, int &y, int pad_x, int pad_y, double &avg, string trk){
	fread(&x, sizeof(int), 1, f);
	fread(&y, sizeof(int), 1, f);
	fwrite(&pad_x, sizeof(int), 1, f);
	fwrite(&pad_y, sizeof(int), 1, f);
	fread(&avg, sizeof(double), 1, f);
	for (int i=0; i<NBANDS; i++){
		int bx, by, id;
		fread(&id, sizeof(int), 1, f);
		string dirname = "B" + to_string(i) + "_Codebooks/";
		codebooks.push_back(get_cdbk_name(id, dirname + trk));

		fread(&bx, sizeof(int), 1, f);
		fread(&by, sizeof(int), 1, f);
		subband aux = new subband;
		aux.x = bx;
		aux.y = by;
		bands.push_back(aux)
	}
	return;
}

//int main(void){
//	vector< string > codebooks = {
//		"B0_Codebooks/codebook_2_4_32.dat",
//		"B1_Codebooks/codebook_1_2_32.dat",
//		"B4_Codebooks/codebook_4_2_32.dat",
//		"B2_Codebooks/codebook_2_1_128.dat",
//		"B5_Codebooks/codebook_1_1_1024.dat",
//		"B7_Codebooks/codebook_2_4_32.dat",
//		"B8_Codebooks/codebook_2_4_32.dat",
//		"B9_Codebooks/codebook_2_4_32.dat",
//		"B3_Codebooks/codebook_4_4_32.dat",
//		"B6_Codebooks/codebook_2_4_32.dat"
//	};
//
//	for (auto cdbk: codebooks)
//		cout<<cdbk<<endl;
//
//	FILE *out = fopen("aux.dat", "wb");
//	write_header(out, codebooks, 760, 240, 0, 0, 120.2);
//	fclose(out);
//
//	FILE *in = fopen("aux.dat", "rb");
//	int x, y, pad_x, pad_y;
//	double avg;
//	vector< string > aux;
//	read_header(in, aux, x, y, pad_x, pad_y, avg);
//	fclose(in);
//
//	cout<<"X = "<<x<<" Y = "<<y<<endl;
//	cout<<"padding (x) = "<<pad_x<<"padding (y) = "<<pad_y<<endl;
//	cout<<"Avg = "<<avg<<endl;
//	for (auto i: aux)
//		cout<<i<<endl;
//
////	int code = get_cdbk_id("B0_Codebooks/codebook_2_4_32.dat");
////	cout<<code<<endl;
////	string codebook = get_cdbk_name(code, "B0_Codebooks/tracker.txt");//"B0_Codebooks/codebook_1_1_256.dat";
////	cout<<codebook<<endl;
////	string cum_freqs = "B0_Codebooks/cummulative_freqs.dat";
////
////	vector< int > cum_freq = get_cumfreq(cum_freqs,codebook);
////	for (auto i: cum_freq)
////		cout<<i<<endl;
//
//		return 0;
//	}
