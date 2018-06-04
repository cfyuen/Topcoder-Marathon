#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<sstream>
#include<fstream>
using namespace std;

struct Image {
   vector<string> det;
   int sco,nr;
};

template <class STR> int s2i (STR S) { istringstream IS(S); int I; IS >> I; return I; }

int k,m,sz;
vector<Image> all;
vector<vector<vector<int> > > prev;
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
         
         while (true) {
            suc=successfulLogin();
            int sucx=s2i(suc[0]),sucy=s2i(suc[1]),gx=sucx/sz,gy=sucy/sz;
            /*
            fout << suc[0] << " " << suc[1] << endl;
            for (int i=2; i<10*sz+2; i++) {
                for (int j=0; j<suc[i].length(); j++)
                    if (suc[i][j]=='1') fout << '#';
                       else fout << '.';
                fout << endl;
                }
            */
            //process grid
            vector<vector<int> > cur;
            for (int m=0; m<10; m++) {
                vector<int> row;
                for (int n=0; n<10; n++) {
                    int empty=1;
                    for (int i=0; i<sz; i++)
                        for (int j=0; j<sz; j++)
                            if (suc[m*sz+i][n*sz+j]!='0') empty=0;
            
                    if (empty==0) {
                       int k;
                       for (k=0; k<all.size(); k++) {
                           int dif=0;
                           for (int i=0; i<sz; i++)
                               for (int j=0; j<sz; j++)
                                   if (all[k].det[i][j]!=suc[m*sz+i][n*sz+j]) dif++;
                           if (dif<=8) {
                              row.push_back(all[k].nr);
                              break;
                              }
                           }
                       if (k==all.size()) {
                          Image nim;
                          for (int i=0; i<sz; i++) {
                              string nrow="";
                              for (int j=0; j<sz; j++)
                                  nrow+=suc[m*sz+i][n*sz+j];
                              nim.det.push_back(nrow);
                              }
                          nim.sco=0;
                          nim.nr=all.size();
                          all.push_back(nim);
                          row.push_back(all[k].nr);
                          }
                       }
                       else row.push_back(-1);
                    }
                 cur.push_back(row);
                 for (int n=0; n<10; n++)
                     fout << row[n] << " ";
                 fout << endl;
                 }
            fout << endl;
            prev.push_back(cur);
            //calculate score
            
            //rubbish
            /*
            for (int i=0; i<sz; i++) {
                for (int j=0; j<sz; j++)
                    fout << cur[i][j] << " ";
                fout << endl;
                }
            */
            if (prev.size()==100) break;
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
