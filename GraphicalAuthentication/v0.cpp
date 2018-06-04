#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<fstream>
using namespace std;

struct Image {
   vector<string> det;
   int sco;
};

int k,m,sz;
vector<Image> all;
ofstream fout("test.txt");

vector<string> successfulLogin () {
   cout << 1 << endl; fflush(stdout);
   vector<string> vs(10*sz+2);
   for (int i=0; i<10*sz+2; i++)
       cin >> vs[i];
   return vs;
}

   

class GraphicalAuthentication {
   public:
      int shoulderSurfing(int K, int M, int SZ) {
         k=K; m=M; sz=SZ;
         vector<string> suc;
         int cnt=0;
         while (cnt<0) {
            suc=successfulLogin();
            fout << suc[0] << " " << suc[1] << endl;
            for (int i=2; i<10*sz+2; i++) {
                for (int j=0; j<suc[i].length(); j++)
                    if (suc[i][j]=='1') fout << '#';
                       else fout << '.';
                fout << endl;
                }
            cnt++;
            }
         return 1;
         }
      vector <int> loginAttempt(vector <string> screen) {
         vector<int> ret;
         ret.push_back(90); ret.push_back(90);
         return ret;
         }
};

int main () {
    GraphicalAuthentication GA;
    int t1,t2,t3;
    cin >> t1 >> t2 >> t3;
    t1=GA.shoulderSurfing(t1,t2,t3);
    cout << 0 << endl; fflush(stdout);
    while (true) {
       vector<string> vs(10*sz);
       for (int i=0; i<10*sz; i++)
           cin >> vs[i];
       vector<int> res;
       res=GA.loginAttempt(vs);
       cout << res[0] << endl;
       cout << res[1] << endl;
       fflush(stdout);
       }
}
