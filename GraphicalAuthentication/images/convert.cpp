#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<sstream>
#include<fstream>
#include<set>
#include<queue>
#include<cmath>
#include<ctime>
#include<cstdlib>
using namespace std;

template <class INT> string i2s (INT I) { ostringstream OS; OS << I; return OS.str(); }

int main () {
    ofstream fout("pic.txt");
    for (int i=0; i<256; i++) {
        string fn=i2s(i);
        fn+=".pbm";
        //fout << fn << endl;
        ifstream fin(fn.c_str());
        string str;
        int cnt=0;
        while (cnt<3) { getline(fin,str); cnt++; }
        int pix; cnt=0;
        while (fin >> pix) {
           fout << pix;
           cnt++;
           if (cnt%18==0) fout << endl;
           }
        fin.close();
        }
}
