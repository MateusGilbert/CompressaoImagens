#include <iostream>
#include <iomanip>
#include <fstream>
#include <regex>
#include <vector>

void save_csv(int*, int=0, int=0, string="out.csv");
int* op_pgm(int&, int&, string="");
vector< int* >vectorize(int*, int, int, int, int);

using namespace std;
