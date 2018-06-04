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
int cx[150],cy[150],vx[150],vy[150];
priority_queue<pair<int,int> > pq;
char alldet[300][20][20];
int prev[110][15][15];
vector<int> sucx,sucy,final;
set<int> hull,used;
vector<string> suc;

int cross (int x1,int y1,int x2,int y2) {
    return x1*y2-y1*x2;
}

void processgrid() {
     for (int m=0; m<10; m++) {
         for (int n=0; n<10; n++) {
             int empty=1,i,j;
             for (i=0; i<sz; i++)
                 for (j=0; j<sz; j++)
                     if (suc[m*sz+i+2][n*sz+j]!='0') empty=0;
             if (empty==0) {
                int k;
                for (k=0; k<allsco.size(); k++) {
                    int dif=0;
                    for (i=0; i<sz; i++)
                        for (j=0; j<sz; j++)
                            if (alldet[k][i][j]!=suc[m*sz+i+2][n*sz+j]) dif++;
                    if (dif<=8) {
                       prev[callnr][m][n]=k;
                       break;
                       }
                    }
                if (k==allsco.size()) {
                   allsco.push_back(0);
                   for (i=0; i<sz; i++)
                       for (j=0; j<sz; j++)
                           alldet[k][i][j]=suc[m*sz+i+2][n*sz+j];
                   prev[callnr][m][n]=k;
                   }
                }
                else prev[callnr][m][n]=-1;
             }
         }
}

class GraphicalAuthentication {
   public:
      int shoulderSurfing(int K, int M, int SZ) {
         cout << K << " " << M << endl;
         sz=SZ;
         while (true) {
            suc=Spy::successfulLogin();
            sucx.push_back(s2i(suc[1]));
            sucy.push_back(s2i(suc[0]));
            int gx=sucx[sucx.size()-1]/sz,gy=sucy[sucy.size()-1]/sz;
            
            //process grid
            processgrid();
            
            callnr++;
            hull.clear();
            while (!pq.empty()) pq.pop();
            for (int i=0; i<allsco.size(); i++)
                pq.push(make_pair(allsco[i],i));
            for (int i=0; i<M; i++)
                if (!pq.empty() && pq.top().first>0) {
                   hull.insert(pq.top().second);
                   pq.pop();
                   }
            
            if (hull.empty()) {
               if (prev[callnr-1][gx][gy]==-1) continue;
               allsco[prev[callnr-1][gx][gy]]=10;
               continue;
               }

            int allin=1;
            for (int k=callnr-1; k>=0; k--) {
                hull.clear();
                while (!pq.empty()) pq.pop();
                for (int i=0; i<allsco.size(); i++)
                    pq.push(make_pair(allsco[i],i));
                for (int i=0; i<M; i++)
                    if (!pq.empty() && pq.top().first>0) {
                       hull.insert(pq.top().second);
                       pq.pop();
                       }
                
                //make convex hull
                used.clear();
                int pnr=0,inhull=1,cax=0,cay=0,vnr=0;
                for (int i=0; i<10; i++)
                    for (int j=0; j<10; j++) {
                        if (hull.find(prev[k][i][j])!=hull.end()) {
                           cx[pnr]=i*sz; cy[pnr]=j*sz; pnr++;
                           cx[pnr]=i*sz+sz-1; cy[pnr]=j*sz; pnr++;
                           cx[pnr]=i*sz; cy[pnr]=j*sz+sz-1; pnr++;
                           cx[pnr]=i*sz+sz-1; cy[pnr]=j*sz+sz-1; pnr++;
                           used.insert(prev[k][i][j]);
                           }
                        }
                if (used.find(prev[k][sucx[k]/sz][sucy[k]/sz])!=used.end() && k==callnr-1) {
                   allsco[prev[k][sucx[k]/sz][sucy[k]/sz]]+=10; continue;
                   }
                if (pnr==0) {
                   if (prev[k][sucx[k]/sz][sucy[k]/sz]!=-1)
                      allsco[prev[k][sucx[k]/sz][sucy[k]/sz]]+=10;
                   continue;
                   }
                //core part of convex hull
                int st=0,now=0;
                for (int i=0; i<pnr; i++)
                    if (cx[i]<cx[now] || (cx[i]==cx[now] && cy[i]<cy[now])) now=i;
                st=now;
                do {
                    vx[vnr]=cx[now]; vy[vnr]=cy[now]; vnr++;
                    int tpt=-1;
                    for (int i=0; i<pnr; i++)
                        if (i!=now) {
                           if (tpt==-1) tpt=i;
                           int ti1=cx[i]-cx[now],ti2=cy[i]-cy[now];
                           int ti3=cx[tpt]-cx[now],ti4=cy[tpt]-cy[now];
                           int crs=cross(ti1,ti2,ti3,ti4);
                           if (crs<0) tpt=i;
                           }
                    now=tpt;
                    } while (now!=st);
                for (int i=0; i<vnr; i++) {
                    cax+=vx[i]; cay+=vy[i];
                    }
                
                cax/=vnr; cay/=vnr;
                int p1,p2;
                for (int i=0; i<vnr; i++) {
                    p1=vx[(i+1)%vnr]-vx[i];
                    p2=vy[(i+1)%vnr]-vy[i];
                    int cpc=cross(p1,p2,cax-vx[i],cay-vy[i]);
                    int cps=cross(p1,p2,sucx[k]-vx[i],sucy[k]-vy[i]);
                    if (cpc*cps<0) inhull=0;
                    }
                if (inhull==1) {
                   //INHULL
                   if (hull.size()==M) {
                      int area=0;
                      for (int i=0; i<vnr; i++)
                          area+=vx[i]*vy[(i+1)%vnr]-vy[i]*vx[(i+1)%vnr];
                      area=abs(area)/2;
                      if (area<325) {
                         for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                             allsco[*it]+=201;
                         }
                         else if (area<800) {
                         for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                             allsco[*it]+=31;
                         }
                         else if (area<2000) {
                         for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                             allsco[*it]+=12;
                         }
                         else if (area<5000) {
                         for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                             allsco[*it]+=4;
                         }
                         else if (area<10000) {
                         for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                             allsco[*it]+=1;
                         }
                      }
                   }
                   else {
                   //OUTHULL
                   allin=0;
                   if (prev[k][sucx[k]/sz][sucy[k]/sz]!=-1)
                      allsco[prev[k][sucx[k]/sz][sucy[k]/sz]]+=21;
                   
                   //see if adding a image cover point
                   vector<int> canadd;
                   for (int i=0; i<10; i++)
                       for (int j=0; j<10; j++)
                           if (prev[k][i][j]!=-1) {
                              cx[pnr]=i*sz; cy[pnr]=j*sz;
                              cx[pnr]=i*sz+sz-1; cy[pnr]=j*sz;
                              cx[pnr]=i*sz; cy[pnr]=j*sz+sz-1;
                              cx[pnr]=i*sz+sz-1; cy[pnr]=j*sz+sz-1;
                              int st=0,now=0;
                              vnr=0;
                              for (int l=0; l<pnr+4; l++)
                                  if (cx[l]<cx[now] || (cx[l]==cx[now] && cy[l]<cy[now])) now=l;
                              st=now;
                              do {
                                 vx[vnr]=cx[now]; vy[vnr]=cy[now]; vnr++;
                                 int tpt=-1;
                                 for (int l=0; l<pnr+4; l++)
                                     if (l!=now) {
                                        if (tpt==-1) tpt=l;
                                        int ti1=cx[l]-cx[now],ti2=cy[l]-cy[now];
                                        int ti3=cx[tpt]-cx[now],ti4=cy[tpt]-cy[now];
                                        int crs=cross(ti1,ti2,ti3,ti4);
                                        if (crs<0) tpt=l;
                                        }
                                 now=tpt;
                                 } while (now!=st);
                              inhull=1;
                              int ti1,ti2;
                              for (int l=0; l<vnr; l++) {
                                  ti1=vx[(l+1)%vnr]-vx[l];
                                  ti2=vy[(l+1)%vnr]-vy[l];
                                  int cpc=cross(ti1,ti2,cax-vx[l],cay-vy[l]);
                                  int cps=cross(ti1,ti2,sucx[k]-vx[l],sucy[k]-vy[l]);
                                  if (cpc*cps<0) inhull=0;
                                  }
                              if (inhull==1) canadd.push_back(prev[k][i][j]);
                              }
                   for (int i=0; i<canadd.size(); i++)
                       if (hull.find(canadd[i])==hull.end()) {
                          if (canadd.size()<=3) allsco[canadd[i]]+=8;
                             else if (canadd.size()<=5) allsco[canadd[i]]+=6;
                             else if (canadd.size()<=10) allsco[canadd[i]]+=4;
                             else if (canadd.size()<=20) allsco[canadd[i]]+=2;
                          }
                   }
                }
            if (callnr>99 || (allin==1 && callnr>M*4-1)) break;
            }
         while (!pq.empty()) pq.pop();
         for (int i=0; i<allsco.size(); i++)
             if (allsco[i]>0) pq.push(make_pair(allsco[i],i));
         for (int i=0; i<M+5; i++)
             if (!pq.empty()) {
                if (i<M) final.push_back(pq.top().second);
                pq.pop();
                }
         return 1;
         }
      vector <int> loginAttempt(vector <string> scr) {
         vector<int> ret;
         ret.push_back(90); ret.push_back(90);
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
                           ret[0]=(n*sz+9); ret[1]=(m*sz+9);
                           prior=k;
                           }
                        }
                    }
                 }
             }
         return ret;
         }
};

