#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<sstream>
#include<fstream>
#include<set>
#include<queue>
using namespace std;

struct Image {
   vector<string> det;
   int sco;
};

template <class STR> int s2i (STR S) { istringstream IS(S); int I; IS >> I; return I; }

int k,m,sz;
vector<Image> all;
vector<vector<vector<int> > > prev;
set<int> hull;
ofstream fout("test.txt");

vector<string> successfulLogin () {
   cout << 1 << endl; fflush(stdout);
   vector<string> vs(10*sz+2);
   for (int i=0; i<10*sz+2; i++)
       cin >> vs[i];
   return vs;
}

bool cmp (Image a,Image b) {
     return a.sco>b.sco;
}

int cross (int x1,int y1,int x2,int y2) {
    return x1*y2-y1*x2;
}

class GraphicalAuthentication {
   public:
      int shoulderSurfing(int K, int M, int SZ) {
         k=K; sz=SZ;
         fout << k << " " << M << endl;
         vector<string> suc;
         
         while (true) {
            suc=successfulLogin();
            int sucx=s2i(suc[1]),sucy=s2i(suc[0]),gx=sucx/sz,gy=sucy/sz;
            fout << sucx << " " << sucy << " " << gx << " " << gy << endl;
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
                        for (int j=0; j<sz; j++) {
                            //if (suc[m*sz+i+2][n*sz+j]!='0' && empty==1) fout << m*sz+i << " " << n*sz+j << endl << endl;
                            if (suc[m*sz+i+2][n*sz+j]!='0') empty=0;
                            }
            
                    if (empty==0) {
                       int k;
                       for (k=0; k<all.size(); k++) {
                           int dif=0;
                           for (int i=0; i<sz; i++)
                               for (int j=0; j<sz; j++)
                                   if (all[k].det[i][j]!=suc[m*sz+i+2][n*sz+j]) dif++;
                           if (dif<=8) {
                              row.push_back(k);
                              break;
                              }
                           }
                       if (k==all.size()) {
                          Image nim;
                          for (int i=0; i<sz; i++) {
                              string nrow="";
                              for (int j=0; j<sz; j++)
                                  nrow+=suc[m*sz+i+2][n*sz+j];
                              nim.det.push_back(nrow);
                              }
                          nim.sco=0;
                          all.push_back(nim);
                          row.push_back(k);
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
            fout << "point: " << cur[gx][gy] << endl;
            //calculate score
            hull.clear();
            priority_queue<pair<int,int> > pq;
            for (int i=0; i<all.size(); i++)
                if (all[i].sco!=0) pq.push(make_pair(all[i].sco,i));
            for (int i=0; i<M; i++)
                if (!pq.empty()) {
                   hull.insert(pq.top().second);
                   fout << pq.top().second << "  sco: " << pq.top().first << endl;
                   pq.pop();
                   }
            if (hull.empty()) {
               if (cur[gx][gy]==-1) continue;
               all[cur[gx][gy]].sco=10;
               //fout << cur[gx][gy] << endl;
               continue;
               }
            fout << "HI" << endl;
            for (int k=prev.size()-1; k>=0; k--) {
                for (set<int>::iterator it=hull.begin(); it!=hull.end(); it++)
                    fout << *it << " ";
                fout << endl;
                if (hull.find(cur[gx][gy])!=hull.end()) {
                   all[cur[gx][gy]].sco+=10; continue;
                   }
                //make convex hull
                vector<pair<int,int> > coor,vex;
                for (int i=0; i<10; i++)
                    for (int j=0; j<10; j++) {
                        //fout << i << " " << j << " " << prev[k][i][j] << " " << cur[i][j] << endl;
                        if (hull.find(prev[k][i][j])!=hull.end()) {
                           coor.push_back(make_pair(i*sz,j*sz));
                           coor.push_back(make_pair(i*sz+sz-1,j*sz));
                           coor.push_back(make_pair(i*sz,j*sz+sz-1));
                           coor.push_back(make_pair(i*sz+sz-1,j*sz+sz-1));
                           }
                        }
                int pnr=coor.size();
                fout << pnr << endl;
                if (pnr==0) continue;
                //core part of convex hull
                int st=0,now=0;
                for (int i=0; i<pnr; i++)
                    if (coor[i]<coor[now]) now=i;
                st=now;
                do {
                    vex.push_back(make_pair(coor[now].first,coor[now].second));
                    int tpt=-1;
                    for (int i=0; i<pnr; i++)
                        if (i!=now) {
                           if (tpt==-1) tpt=i;
                           int crs=cross(coor[i].first-coor[now].first,coor[i].second-coor[now].second,coor[tpt].first-coor[now].first,coor[tpt].second-coor[now].second);
                           if (crs<0) tpt=i;
                           }
                    now=tpt;
                    } while (now!=st);
                for (int i=0; i<vex.size(); i++)
                    fout << vex[i].first << " " << vex[i].second << endl;
                
                //if convex hull has too many points
                //   all points-, regenerate
                int inhull=1,cx=0,cy=0,vnr=vex.size();
                for (int i=0; i<vnr; i++) {
                    cx+=vex[i].first; cy+=vex[i].second;
                    }
                cx/=vnr; cy/=vnr;
                for (int i=0; i<vnr; i++) {
                    int cpc=cross(vex[(i+1)%vnr].first-vex[i].first,vex[(i+1)%vnr].second-vex[i].second,cx-vex[i].first,cy-vex[i].second);
                    int cps=cross(vex[(i+1)%vnr].first-vex[i].first,vex[(i+1)%vnr].second-vex[i].second,sucx-vex[i].first,sucy-vex[i].second);
                    if (cpc*cps<0) inhull=0;
                    }
                if (inhull==1) {
                   //if size of convex hull is small, all points in convex hull+ (according to size)
                   }
                   else {
                   int mx=0;
                   for (set<int>::iterator it=hull.begin(); it!=hull.end(); it++)
                       if (all[*it].sco>mx) mx=all[*it].sco;
                   all[cur[gx][gy]].sco=mx+1;
                   }
                next:
                break;
                }
            //if sucx,sucy is in corner (the triangle), corner+=999999;
            //if sucx,sucy is in side(or +1), hull point nearer to side+
            //rubbish
            /*
            for (int i=0; i<sz; i++) {
                for (int j=0; j<sz; j++)
                    fout << cur[i][j] << " ";
                fout << endl;
                }
            */
            if (prev.size()==100) break;
            fout << endl << endl;
            }
         priority_queue<pair<int,int> > pq;
         for (int i=0; i<all.size(); i++)
             if (all[i].sco!=0) pq.push(make_pair(-all[i].sco,i));
         for (int i=0; i<M; i++)
             if (!pq.empty()) {
                fout << pq.top().second << "  sco: " << pq.top().first << endl;
                
                for (int j=0; j<sz; j++) {
                    for (int k=0; k<sz; k++)  
                        if (all[pq.top().second].det[j][k]=='1') fout << "#";
                           else fout << ".";
                    fout << endl;
                    }
                
                pq.pop();
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
