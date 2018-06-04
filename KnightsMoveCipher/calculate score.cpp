#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
using namespace std;

int main () {
    freopen("s3 1000 test.txt","r",stdin);
    freopen("score.txt","w",stdout);
    string str;
    double tot=0;
    while (getline(cin,str)) {
       if (str.find("Score =")!=string::npos) {
          for (int i=0; i<8; i++)
              str.erase(str.begin());
          printf("%s\n",str.c_str());
          }
       double sco=0;
       sscanf(str.c_str(),"%lf",&sco);
       //cout << sco << endl;
       tot+=sco;
       }
    fflush(stdout);
    printf("%0.4lf\n",tot);
    return 0;
}
