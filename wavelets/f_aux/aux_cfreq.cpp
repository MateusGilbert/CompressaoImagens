#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>

using namespace std;

int main()
{
    FILE *t = fopen("a.txt", "rb");
    int n;
    while (fread(&n, sizeof(int), 1, t))
        cout<<n<<endl;
    fclose(t);
    /*remove("tmp.dat");*/
    
    return 0;
}
