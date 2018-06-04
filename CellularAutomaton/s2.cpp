#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<queue>
#include<cmath>
using namespace std;

vector<string> grid,ret,grd[23],origrd[23];
int n,rd,dn,dm,best=0;
string rule;
int dx[]={1,1,1,0,0,-1,-1,-1},dy[]={1,0,-1,1,-1,1,0,-1};
int pcode[105][105];
int cach[1<<16],cachesize=4,cachecnt=0,totopr=0;


timeval sttime;
double timelim=9.8;

void startt() {
    gettimeofday(&sttime,NULL);
}

double runtime() {
    timeval tt, nowt;
    gettimeofday(&tt,NULL);
    timersub(&tt,&sttime,&nowt);
    return (nowt.tv_sec*1000+nowt.tv_usec/1000)/1000.0;
    }

class CellularAutomaton {
   public:
      int cntlive (vector<string> grid) {
          int live=0;
          for (int i=0; i<dn; i++)
              for (int j=0; j<dm; j++)
                  if (grid[i][j]=='1') live++;
          return live;
          }
      vector<string> simulate (vector<string> nowg) {
          int neigh=0;
          grd[0]=nowg;
          for (int i=0; i<rd; i++) {
              grd[i+1]=grd[i];
              for (int j=0; j<dn; j+=cachesize-2) {
                  for (int k=0; k<dm; k+=cachesize-2) {
                      int code=0;
                      if (j-(cachesize-2)<0) {
                         pcode[j][k]=0;
                         for (int sx=0; sx<cachesize; sx++)
                             for (int sy=0; sy<cachesize; sy++)
                                 if (grd[i][(j+sx)%dn][(k+sy)%dm]=='1') pcode[j][k]+=1<<(sx*cachesize+sy);
                         }
                         else {
                         pcode[j][k]=pcode[j-(cachesize-2)][k];
                         pcode[j][k]=pcode[j][k]>>((cachesize-2)*cachesize);
                         for (int sx=2; sx<cachesize; sx++)
                             for (int sy=0; sy<cachesize; sy++)
                                 if (grd[i][(j+sx)%dn][(k+sy)%dm]=='1') pcode[j][k]+=1<<(sx*cachesize+sy);
                         }
                      code=pcode[j][k];
                      totopr++;
                      if (cach[code]==-1) {
                         cach[code]=0;
                         cachecnt++;
                         for (int sx=0; sx<cachesize-2; sx++)
                             for (int sy=0; sy<cachesize-2; sy++) {
                                 neigh=0;
                                 for (int d=0; d<8; d++)
                                     if (grd[i][(dx[d]+j+sx+1+dn)%dn][(dy[d]+k+sy+1+dm)%dm]=='1') neigh++;
                                 if (rule[neigh]=='+') cach[code]+=1<<(sx*(cachesize-2)+sy);
                                 if (rule[neigh]=='X') {
                                    if (code & (1<<((sx+1)*cachesize+(sy+1)))) {}
                                       else cach[code]+=1<<(sx*(cachesize-2)+sy);
                                    }
                                 if (rule[neigh]=='=') {
                                    if (code & (1<<((sx+1)*cachesize+(sy+1)))) cach[code]+=1<<(sx*(cachesize-2)+sy);
                                    }
                                 }
                         }
                      for (int sx=0; sx<cachesize-2; sx++)
                          for (int sy=0; sy<cachesize-2; sy++)
                              if (cach[code] & (1<<(sx*(cachesize-2)+sy))) grd[i+1][(j+sx+1)%dn][(k+sy+1)%dm]='1';
                                 else grd[i+1][(j+sx+1)%dn][(k+sy+1)%dm]='0';
                      }
                  }
              }
          return grd[rd];
          }
      vector<string> fastsim (vector<string> nowg,int cx,int cy,int tx,int ty) {
          int neigh=0,gv[105][105];
          grd[0]=nowg;
          for (int i=0; i<rd; i++) {
              memset(gv,0,sizeof(gv));
              int frj,toj,frk,tok,xx,yy;
              
              if (2*i+3>=dn) frj=0,toj=dn-1;
                 else frj=-i-1,toj=i+1;
              if (2*i+3>=dm) frk=0,tok=dm-1;
                 else frk=-i-1,tok=i+1;
              
              for (int j=frj; j<=toj; j++)
                  for (int k=frk; k<=tok; k++) {
                      neigh=0;
                      xx=(cx+j+dn)%dn; yy=(cy+k+dm)%dm;
                      if (gv[xx][yy]==0) {
                         gv[xx][yy]=1;
                         totopr++;
                         for (int d=0; d<8; d++)
                             if (grd[i][(dx[d]+xx+dn)%dn][(dy[d]+yy+dm)%dm]=='1') neigh++;
                         if (rule[neigh]=='-') grd[i+1][xx][yy]='0';
                         if (rule[neigh]=='+') grd[i+1][xx][yy]='1';
                         if (rule[neigh]=='X') grd[i+1][xx][yy]=1-(grd[i][xx][yy]-'0')+'0';
                         if (rule[neigh]=='=') grd[i+1][xx][yy]=grd[i][xx][yy];
                         }
                      
                      neigh=0;
                      xx=(tx+j+dn)%dn; yy=(ty+k+dm)%dm;
                      if (gv[xx][yy]==0) {
                         gv[xx][yy]=1;
                         totopr++;
                         for (int d=0; d<8; d++)
                             if (grd[i][(dx[d]+xx+dn)%dn][(dy[d]+yy+dm)%dm]=='1') neigh++;
                         if (rule[neigh]=='-') grd[i+1][xx][yy]='0';
                         if (rule[neigh]=='+') grd[i+1][xx][yy]='1';
                         if (rule[neigh]=='X') grd[i+1][xx][yy]=1-(grd[i][xx][yy]-'0')+'0';
                         if (rule[neigh]=='=') grd[i+1][xx][yy]=grd[i][xx][yy];
                         }
                      }
              }
          return grd[rd];
          }
      double move (int ov,int nv,double frac) {
          if (nv>ov) return 20000.0;
          double prob=1.0/((ov-nv)*(250-sqrt(ov)*2)+80-sqrt(ov)/6)*exp(-1.8*frac-1.3)*20000*40;
          return prob;
          }
      void copygrd () {
          for (int i=0; i<=rd; i++)
              origrd[i]=grd[i];
          }
      void creategrd () {
          for (int i=0; i<=rd; i++)
              grd[i]=origrd[i];
          }
      void sa () {
          vector<string> stgr,stfin,orifin;
          vector<pair<int,int> > stch,orich;
          int orival,stval;
          stgr=grid;
          int xx,yy,sim=0;
          for (int i=0; i<n; i++) {
              xx=rand()%dn; yy=rand()%dm;
              orich.push_back(make_pair(xx,yy));
              stgr[xx][yy]=1-(stgr[xx][yy]-'0')+'0';
              }
          orifin=simulate(stgr);
          copygrd();
          orival=cntlive(orifin);
          ret=stgr;
          while (runtime()<timelim) {
             sim++;
             stch=orich;
             creategrd();
             int ind=rand()%(stch.size());
             stgr=grid;
             for (int j=0; j<stch.size(); j++) 
                 if (j!=ind) {
                    xx=stch[j].first; yy=stch[j].second;
                    stgr[xx][yy]=1-(stgr[xx][yy]-'0')+'0';
                    }
             int ox,oy;
             ox=stch[ind].first; oy=stch[ind].second;

             stch[ind]=make_pair(rand()%dn,rand()%dm);
             
             xx=stch[ind].first; yy=stch[ind].second;
             stgr[xx][yy]=1-(stgr[xx][yy]-'0')+'0';
             
             stfin=fastsim(stgr,ox,oy,xx,yy);
             
             stval=cntlive(stfin);
             if (stval>best) {
                best=stval;
                ret=stgr;
                }
             if (move(orival,stval,runtime()/timelim)>rand()%20000) {
                orich=stch; orival=stval; orifin=stfin; copygrd();
                }
             }
          fprintf(stderr,"New(%d): %d\n",sim,orival);
          }
  	  vector <string> configure(vector <string> _grid, string rules, int N, int K) {
          n=N; rd=K; rule=rules; grid=_grid; dn=grid.size(); dm=grid[0].length();
          startt();
          memset(cach,-1,sizeof(cach));
          ret=grid;
          sa();
          fprintf(stderr,"cache used = %d (%d)\n",cachecnt,totopr);
          return ret;
      }
};
