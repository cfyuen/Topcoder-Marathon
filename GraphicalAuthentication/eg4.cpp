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

int cross (int x1,int y1,int x2,int y2) {
    return x1*y2-y1*x2;
}

class GraphicalAuthentication {
   public:
      int shoulderSurfing(int K, int M, int SZ) {
         sz=SZ;
         cout << K << " " << M << endl;
         while (true) {
            cout << "RESTART " << callnr << endl;
            vector<string> suc(Spy::successfulLogin());
            sucx.push_back(s2i(suc[1]));
            sucy.push_back(s2i(suc[0]));
            int gx=sucx[sucx.size()-1]/sz,gy=sucy[sucy.size()-1]/sz;
            cout << sucx[sucx.size()-1] << " " << sucy[sucy.size()-1] << " " << gx << " " << gy << endl;
            /*
            cout << suc[0] << " " << suc[1] << endl;
            for (int i=2; i<10*sz+2; i++) {
                for (int j=0; j<suc[i].length(); j++)
                    if (suc[i][j]=='1') cout << '#';
                       else cout << '.';
                cout << endl;
                }
            */
            //process grid
            for (int m=0; m<10; m++) {
                for (int n=0; n<10; n++) {
                    int empty=1;
                    for (int i=0; i<sz; i++)
                        for (int j=0; j<sz; j++) {
                            //if (suc[m*sz+i+2][n*sz+j]!='0' && empty==1) cout << m*sz+i << " " << n*sz+j << endl << endl;
                            if (suc[m*sz+i+2][n*sz+j]!='0') empty=0;
                            }
                    if (empty==0) {
                       int k;
                       for (k=0; k<allsco.size(); k++) {
                           int dif=0;
                           //cout << k << " " << all[k].det[0][0] << endl<<flush;
                           for (int i=0; i<sz; i++)
                               for (int j=0; j<sz; j++) {
                                   if (alldet[k][i][j]!=suc[m*sz+i+2][n*sz+j]) dif++;
                                   }
                           if (dif<=8) {
                              prev[callnr][m][n]=k;
                              break;
                              }
                           }
                       //cout << k << endl << flush;
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
                     cout << row[n] << " ";
                 cout << endl;
                 cout << flush;
                 */
                 }
            //cout << endl;
            callnr++;
            cout << "point: " << prev[callnr-1][gx][gy] << endl;
            //calculate score
            hull.clear();
            while (!pq.empty()) pq.pop();
            //cout << pq.size() << endl;
            for (int i=0; i<allsco.size(); i++) {
                pq.push(make_pair(allsco[i],i));
                //cout << i << "  sco: " << all[i].sco << endl;
                }
            for (int i=0; i<M; i++)
                if (!pq.empty() && pq.top().first>0) {
                   hull.insert(pq.top().second);
                   //cout << pq.top().second << "  sco: " << pq.top().first << endl;
                   pq.pop();
                   }
            //cout << hull.size() << endl;
            if (hull.empty()) {
               if (prev[callnr-1][gx][gy]==-1) continue;
               allsco[prev[callnr-1][gx][gy]]=10;
               //cout << prev[callnr-1][gx][gy] << endl;
               continue;
               }
            //cout << "BEGIN SEARCH" << endl;
            //**************************************************************************************************************
            int allin=1;
            for (int k=callnr-1; k>=0; k--) {
                cout << "BEGIN SEARCH" << endl;
                hull.clear();
                
                while (!pq.empty()) pq.pop();
                for (int i=0; i<allsco.size(); i++) {
                    //cout << i << " " << allsco[i] << endl;
                    //cout << pqi.size() << endl;
                    pq.push(make_pair(allsco[i],i));
                    }
                
                for (int i=0; i<M; i++)
                    if (!pq.empty() && pq.top().first>0) {
                       hull.insert(pq.top().second);
                       cout << pq.top().second << "  sco: " << pq.top().first << endl;
                       pq.pop();
                       }
                
                /*
                for (set<int>::iterator it=hull.begin(); it!=hull.end(); it++)
                    cout << *it << " ";
                cout << endl;
                */
                /*
                for (int i=0; i<10; i++) {
                    for (int j=0; j<10; j++)
                        if (i==sucx[k]/sz && j==sucy[k]/sz) cout << "#";
                           else if (hull.find(prev[k][i][j])!=hull.end()) cout << "*";
                           else cout << ".";
                    cout << endl;
                    }
                */
                //make convex hull
                used.clear();
                coor.clear(); vex.clear();
                int pnr,inhull=1,cx=0,cy=0,vnr;
                for (int i=0; i<10; i++)
                    for (int j=0; j<10; j++) {
                        //cout << i << " " << j << " " << prev[k][i][j] << " " << cur[i][j] << endl;
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
            cout << flush;
            //if sucx,sucy is in corner (the triangle), corner+=999999;
            if (callnr>99 || (allin==1 && callnr>M*10-1)) break;
            
            //cout << endl << endl;
            }
         while (!pq.empty()) pq.pop();
         for (int i=0; i<allsco.size(); i++)
             if (allsco[i]>0) pq.push(make_pair(allsco[i],i));
         for (int i=0; i<M; i++)
             if (!pq.empty()) {
                cout << pq.top().second << "  sco: " << pq.top().first << endl;
                
                for (int j=0; j<sz; j++) {
                    for (int k=0; k<sz; k++)  
                        if (alldet[pq.top().second][j][k]=='1') cout << "#";
                           else cout << ".";
                    cout << endl;
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
                 if (scr[i][j]=='1') cout << "#"; else cout << ".";
             cout << endl;
             }
         */
         /*
         for (int i=0; i<final.size(); i++)
             cout << final[i] << " ";
         cout << endl;
         cout << ret.size() << endl;
         */
         cout << "New" << endl;
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
                           //cout << m << " " << n << endl << flush;
                           ret[0]=(n*sz+9); ret[1]=(m*sz+9);
                           prior=k;
                           //cout << k << " dif: " << dif << endl;
                           }
                        }
                    }
                 }
             }
         cout << ret[0] << " " << ret[1] << endl << flush;
         return ret;
         }
};
