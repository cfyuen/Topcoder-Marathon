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
using namespace std;

template <class STR> int s2i (STR S) { istringstream IS(S); int I; IS >> I; return I; }

int sz,callnr=0;
vector<int> allsco;
vector<pair<int,int> > coor,vex;
priority_queue<pair<int,int> > pq;
char alldet[300][20][20];
int prev[110][15][15];
vector<int> sucx,sucy,final;
set<int> hull,used;
ofstream fout("test.txt");

vector<string> successfulLogin () {
   cout << 1 << endl; fflush(stdout);
   vector<string> vs(10*sz+2);
   for (int i=0; i<10*sz+2; i++)
       cin >> vs[i];
   return vs;
}

int cross (int x1,int y1,int x2,int y2) {
    return x1*y2-y1*x2;
}

class GraphicalAuthentication {
   public:
      int shoulderSurfing(int K, int M, int SZ) {
         sz=SZ;
         fout << K << " " << M << endl;
         while (true) {
            fout << "RESTART " << callnr << endl;
            vector<string> suc(successfulLogin());
            sucx.push_back(s2i(suc[1]));
            sucy.push_back(s2i(suc[0]));
            int gx=sucx[sucx.size()-1]/sz,gy=sucy[sucy.size()-1]/sz;
            fout << sucx[sucx.size()-1] << " " << sucy[sucy.size()-1] << " " << gx << " " << gy << endl;
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
            for (int m=0; m<10; m++) {
                for (int n=0; n<10; n++) {
                    int empty=1;
                    for (int i=0; i<sz; i++)
                        for (int j=0; j<sz; j++) {
                            //if (suc[m*sz+i+2][n*sz+j]!='0' && empty==1) fout << m*sz+i << " " << n*sz+j << endl << endl;
                            if (suc[m*sz+i+2][n*sz+j]!='0') empty=0;
                            }
                    if (empty==0) {
                       int k;
                       for (k=0; k<allsco.size(); k++) {
                           int dif=0;
                           //fout << k << " " << all[k].det[0][0] << endl<<flush;
                           for (int i=0; i<sz; i++)
                               for (int j=0; j<sz; j++) {
                                   if (alldet[k][i][j]!=suc[m*sz+i+2][n*sz+j]) dif++;
                                   }
                           if (dif<=8) {
                              prev[callnr][m][n]=k;
                              break;
                              }
                           }
                       //fout << k << endl << flush;
                       if (k==allsco.size()) {
                          allsco.push_back(0);
                          for (int i=0; i<sz; i++) {
                              for (int j=0; j<sz; j++)
                                  alldet[k][i][j]=suc[m*sz+i+2][n*sz+j];
                              }
                          prev[callnr][m][n]=k;
                          }
                       }
                       else prev[callnr][m][n]=-1;
                    }
                 /*
                 for (int n=0; n<10; n++)
                     fout << row[n] << " ";
                 fout << endl;
                 fout << flush;
                 */
                 }
            //fout << endl;
            callnr++;
            fout << "point: " << prev[callnr-1][gx][gy] << endl;
            //calculate score
            hull.clear();
            while (!pq.empty()) pq.pop();
            //fout << pq.size() << endl;
            for (int i=0; i<allsco.size(); i++) {
                pq.push(make_pair(allsco[i],i));
                //fout << i << "  sco: " << all[i].sco << endl;
                }
            for (int i=0; i<M; i++)
                if (!pq.empty() && pq.top().first>0) {
                   hull.insert(pq.top().second);
                   //fout << pq.top().second << "  sco: " << pq.top().first << endl;
                   pq.pop();
                   }
            //fout << hull.size() << endl;
            if (hull.empty()) {
               if (prev[callnr-1][gx][gy]==-1) continue;
               allsco[prev[callnr-1][gx][gy]]=10;
               //fout << prev[callnr-1][gx][gy] << endl;
               continue;
               }
            //fout << "BEGIN SEARCH" << endl;
            //**************************************************************************************************************
            int allin=1;
            for (int k=callnr-1; k>=0; k--) {
                fout << "BEGIN SEARCH" << endl;
                hull.clear();
                
                while (!pq.empty()) pq.pop();
                for (int i=0; i<allsco.size(); i++) {
                    //fout << i << " " << allsco[i] << endl;
                    //fout << pqi.size() << endl;
                    pq.push(make_pair(allsco[i],i));
                    }
                
                for (int i=0; i<M; i++)
                    if (!pq.empty() && pq.top().first>0) {
                       hull.insert(pq.top().second);
                       fout << pq.top().second << "  sco: " << pq.top().first << endl;
                       pq.pop();
                       }
                
                /*
                for (set<int>::iterator it=hull.begin(); it!=hull.end(); it++)
                    fout << *it << " ";
                fout << endl;
                */
                /*
                for (int i=0; i<10; i++) {
                    for (int j=0; j<10; j++)
                        if (i==sucx[k]/sz && j==sucy[k]/sz) fout << "#";
                           else if (hull.find(prev[k][i][j])!=hull.end()) fout << "*";
                           else fout << ".";
                    fout << endl;
                    }
                */
                //make convex hull
                used.clear();
                coor.clear(); vex.clear();
                int pnr,inhull=1,cx=0,cy=0,vnr;
                for (int i=0; i<10; i++)
                    for (int j=0; j<10; j++) {
                        //fout << i << " " << j << " " << prev[k][i][j] << " " << cur[i][j] << endl;
                        if (hull.find(prev[k][i][j])!=hull.end()) {
                           coor.push_back(make_pair(i*sz,j*sz));
                           coor.push_back(make_pair(i*sz+sz-1,j*sz));
                           coor.push_back(make_pair(i*sz,j*sz+sz-1));
                           coor.push_back(make_pair(i*sz+sz-1,j*sz+sz-1));
                           used.insert(prev[k][i][j]);
                           }
                        }
                if (used.find(prev[k][sucx[k]/sz][sucy[k]/sz])!=used.end() && k==callnr-1) {
                   allsco[prev[k][sucx[k]/sz][sucy[k]/sz]]+=10; continue;
                   }
                
                }
            fout << flush;
            //if sucx,sucy is in corner (the triangle), corner+=999999;
            if (callnr>99 || (allin==1 && callnr>M*10-1)) break;
            
            //fout << endl << endl;
            }
         while (!pq.empty()) pq.pop();
         for (int i=0; i<allsco.size(); i++)
             if (allsco[i]>0) pq.push(make_pair(allsco[i],i));
         for (int i=0; i<M; i++)
             if (!pq.empty()) {
                fout << pq.top().second << "  sco: " << pq.top().first << endl;
                
                for (int j=0; j<sz; j++) {
                    for (int k=0; k<sz; k++)  
                        if (alldet[pq.top().second][j][k]=='1') fout << "#";
                           else fout << ".";
                    fout << endl;
                    }
                
                final.push_back(pq.top().second);
                pq.pop();
                }
         return 1;
         }
      vector <int> loginAttempt(vector <string> scr) {
         vector<int> ret;
         ret.push_back(90); ret.push_back(90);
         /*
         for (int i=0; i<scr.size(); i++) {
             for (int j=0; j<scr[0].length(); j++)
                 if (scr[i][j]=='1') fout << "#"; else fout << ".";
             fout << endl;
             }
         */
         /*
         for (int i=0; i<final.size(); i++)
             fout << final[i] << " ";
         fout << endl;
         fout << ret.size() << endl;
         */
         fout << "New" << endl;
         int prior=1000;
         for (int m=0; m<10; m++) {
             for (int n=0; n<10; n++) {
                 int empty=1;
                 for (int i=0; i<sz; i++)
                     for (int j=0; j<sz; j++) {
                         if (scr[m*sz+i][n*sz+j]!='0') empty=0;
                         }
                 if (empty==0) {
                    for (int k=0; k<final.size(); k++) {
                        int dif=0;
                        for (int i=0; i<sz; i++)
                            for (int j=0; j<sz; j++)
                                if (alldet[final[k]][i][j]!=scr[m*sz+i][n*sz+j]) dif++;
                        if (dif<=8 && k<prior) {
                           //fout << m << " " << n << endl << flush;
                           ret[0]=(n*sz+9); ret[1]=(m*sz+9);
                           prior=k;
                           //fout << k << " dif: " << dif << endl;
                           }
                        }
                    }
                 }
             }
         fout << ret[0] << " " << ret[1] << endl << flush;
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
