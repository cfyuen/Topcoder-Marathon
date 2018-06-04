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

struct Image {
   vector<string> det;
   int sco;
};

template <class STR> int s2i (STR S) { istringstream IS(S); int I; IS >> I; return I; }

int sz;
vector<Image> all;
vector<vector<vector<int> > > prev;
vector<int> sucx,sucy,final;
set<int> hull,used;

bool cmp (Image a,Image b) {
     return a.sco>b.sco;
}

int cross (int x1,int y1,int x2,int y2) {
    return x1*y2-y1*x2;
}

class GraphicalAuthentication {
   public:
      int shoulderSurfing(int K, int M, int SZ) {
         sz=SZ;
         cout << K << " " << M << endl;
         
         
         while (true) {
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
            vector<vector<int> > cur;
            for (int m=0; m<10; m++) {
                vector<int> row;
                for (int n=0; n<10; n++) {
                    int empty=1;
                    for (int i=0; i<sz; i++)
                        for (int j=0; j<sz; j++) {
                            //if (suc[m*sz+i+2][n*sz+j]!='0' && empty==1) cout << m*sz+i << " " << n*sz+j << endl << endl;
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
                 /*
                 for (int n=0; n<10; n++)
                     cout << row[n] << " ";
                 cout << endl;
                 */
                 }
            //cout << endl;
            prev.push_back(cur);
            cout << "point: " << cur[gx][gy] << endl;
            //calculate score
            hull.clear();
            priority_queue<pair<int,int> > pq;
            for (int i=0; i<all.size(); i++) {
                if (all[i].sco>0) pq.push(make_pair(all[i].sco,i));
                //cout << i << " " << all[i].sco << endl;
                }
            for (int i=0; i<M; i++)
                if (!pq.empty()) {
                   hull.insert(pq.top().second);
                   //cout << pq.top().second << "  sco: " << pq.top().first << endl;
                   pq.pop();
                   }
            //cout << hull.size() << endl;
            if (hull.empty()) {
               if (cur[gx][gy]==-1) continue;
               all[cur[gx][gy]].sco=10;
               //cout << cur[gx][gy] << endl;
               continue;
               }
            //cout << "BEGIN SEARCH" << endl;
            //**************************************************************************************************************
            int allin=1;
            for (int k=prev.size()-1; k>=0; k--) {
                hull.clear();
                priority_queue<pair<int,int> > pqi;
                for (int i=0; i<all.size(); i++)
                    if (all[i].sco>0) pqi.push(make_pair(all[i].sco,i));
                for (int i=0; i<M; i++)
                    if (!pqi.empty()) {
                       hull.insert(pqi.top().second);
                       //cout << pqi.top().second << "  sco: " << pqi.top().first << endl;
                       pqi.pop();
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
                vector<pair<int,int> > coor,vex;
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
                if (used.find(prev[k][sucx[k]/sz][sucy[k]/sz])!=used.end() && k==prev.size()-1) {
                   all[prev[k][sucx[k]/sz][sucy[k]/sz]].sco+=10; continue;
                   }
                int pnr=coor.size();
                //cout << pnr << endl;
                if (pnr==0) {
                   all[prev[k][sucx[k]/sz][sucy[k]/sz]].sco+=10;
                   continue;
                   }
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
                /*
                for (int i=0; i<vex.size(); i++)
                    cout << vex[i].first << " " << vex[i].second << endl;
                */
                //if convex hull has too many points
                //   all points-, regenerate
                int inhull=1,cx=0,cy=0,vnr=vex.size();
                for (int i=0; i<vnr; i++) {
                    cx+=vex[i].first; cy+=vex[i].second;
                    }
                cx/=vnr; cy/=vnr;
                for (int i=0; i<vnr; i++) {
                    int cpc=cross(vex[(i+1)%vnr].first-vex[i].first,vex[(i+1)%vnr].second-vex[i].second,cx-vex[i].first,cy-vex[i].second);
                    int cps=cross(vex[(i+1)%vnr].first-vex[i].first,vex[(i+1)%vnr].second-vex[i].second,sucx[k]-vex[i].first,sucy[k]-vex[i].second);
                    if (cpc*cps<0) inhull=0;
                    }
                if (inhull==1) {
                   cout << "INHULL" << endl;
                   if (hull.size()==M) {
                      int area=0;
                      for (int i=0; i<vnr; i++)
                          area+=vex[i].first*vex[(i+1)%vnr].second-vex[i].second*vex[(i+1)%vnr].first;
                      area=abs(area)/2;
                      cout << "area: " << area << endl;
                      if (area<325) {
                         for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                             all[*it].sco+=21;
                         }
                         else if (area<1000) {
                         for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                             all[*it].sco+=15;
                         }
                         else if (area<2000) {
                         for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                             all[*it].sco+=8;
                         }
                         else if (area<5000) {
                         for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                             all[*it].sco+=4;
                         }
                         else if (area<10000) {
                         for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                             all[*it].sco+=1;
                         }
                         
                      }
                   }
                   else {
                   cout << "OUTHULL" << endl;
                   allin=0;
                   int mx=0;
                   for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                       if (all[*it].sco>mx) mx=all[*it].sco;
                   all[prev[k][sucx[k]/sz][sucy[k]/sz]].sco+=21;
                   //see if adding a image cover point
                   vector<int> canadd;
                   for (int i=0; i<10; i++)
                       for (int j=0; j<10; j++)
                           if (prev[k][i][j]!=-1) {
                              coor.push_back(make_pair(i*sz,j*sz));
                              coor.push_back(make_pair(i*sz+sz-1,j*sz));
                              coor.push_back(make_pair(i*sz,j*sz+sz-1));
                              coor.push_back(make_pair(i*sz+sz-1,j*sz+sz-1));
                              int st=0,now=0;
                              vex.clear();
                              for (int l=0; l<pnr+4; l++)
                                  if (coor[l]<coor[now]) now=l;
                              st=now;
                              do {
                                 vex.push_back(make_pair(coor[now].first,coor[now].second));
                                 int tpt=-1;
                                 for (int l=0; l<pnr+4; l++)
                                     if (l!=now) {
                                        if (tpt==-1) tpt=l;
                                        int crs=cross(coor[l].first-coor[now].first,coor[l].second-coor[now].second,coor[tpt].first-coor[now].first,coor[tpt].second-coor[now].second);
                                        if (crs<0) tpt=l;
                                        }
                                 now=tpt;
                                 } while (now!=st);
                              coor.erase(coor.end()-4,coor.end());
                              vnr=vex.size();
                              inhull=1;
                              for (int l=0; l<vnr; l++) {
                                  int cpc=cross(vex[(l+1)%vnr].first-vex[l].first,vex[(l+1)%vnr].second-vex[l].second,cx-vex[l].first,cy-vex[l].second);
                                  int cps=cross(vex[(l+1)%vnr].first-vex[l].first,vex[(l+1)%vnr].second-vex[l].second,sucx[k]-vex[l].first,sucy[k]-vex[l].second);
                                  if (cpc*cps<0) inhull=0;
                                  }
                              if (inhull==1) canadd.push_back(prev[k][i][j]);
                              }
                   for (int i=0; i<canadd.size(); i++)
                       if (hull.find(canadd[i])==hull.end())
                          all[canadd[i]].sco+=6;
                   }
                //break;
                }
            //if sucx,sucy is in corner (the triangle), corner+=999999;
            //if sucx,sucy is in side(or +1), hull point nearer to side+
            if (prev.size()>99 || (allin==1 && prev.size()>M*10-1)) break;
            //cout << endl << endl;
            }
         priority_queue<pair<int,int> > pq;
         for (int i=0; i<all.size(); i++)
             if (all[i].sco>0) pq.push(make_pair(all[i].sco,i));
         for (int i=0; i<M; i++)
             if (!pq.empty()) {
                cout << pq.top().second << "  sco: " << pq.top().first << endl;
                
                for (int j=0; j<sz; j++) {
                    for (int k=0; k<sz; k++)  
                        if (all[pq.top().second].det[j][k]=='1') cout << "#";
                           else cout << ".";
                    cout << endl;
                    }
                
                final.push_back(pq.top().second);
                pq.pop();
                }
         return 1;
         }
      vector <int> loginAttempt(vector <string> scr) {
         vector<int> ret(2);
         //ret.push_back(90); ret.push_back(90);
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
         */
         //cout << "New" << endl;
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
                                if (all[final[k]].det[i][j]!=scr[m*sz+i][n*sz+j]) dif++;
                        if (dif<=8 && k<prior) {
                           ret[0]=(n*sz+9); ret[1]=(m*sz+9);
                           prior=k;
                           //cout << m << " " << n << endl;
                           //cout << k << " dif: " << dif << endl;
                           }
                        }
                    }
                 }
             }
         if (prior==1000) {
            ret[0]=90; ret[1]=90;
            }
         return ret;
         }
};

