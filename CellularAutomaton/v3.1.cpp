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

vector<string> grid,ret;
int n,rd,dn,dm,best=0;
string rule;
int dx[]={1,1,1,0,0,-1,-1,-1},dy[]={1,0,-1,1,-1,1,0,-1};
int pcode[105][105];
int cach[1<<25],cachesize=5,cachecnt=0,totopr=0;
//be careful of memset


//delete when submit
FILE *debug,*result;
double timelim=5.0;

int sttime;
void startt() {
    sttime=clock();
}

double runtime() {
    return (clock()-sttime)/1000.0;
}

//til here

/* have it when submit
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
*/

class CellularAutomaton {
   public:
      void vis (vector<string> visg) {
          for (int i=0; i<dn; i++) {
              for (int j=0; j<dm; j++)
                  if (visg[i][j]=='1') fprintf(debug,"#");
                     else fprintf(debug,".");
              fprintf(debug,"\n");
              }
          fprintf(debug,"\n");
          }
      int stimulate (vector<string> nowg) {
          int neigh=0;
          vector<string> newg=nowg;
          for (int i=0; i<rd; i++) {
              for (int j=0; j<dn; j+=cachesize-2) {
                  for (int k=0; k<dm; k+=cachesize-2) {
                      int code=0;
                      if (j-(cachesize-2)<0) {
                         pcode[j][k]=0;
                         for (int sx=0; sx<cachesize; sx++)
                             for (int sy=0; sy<cachesize; sy++)
                                 if (nowg[(j+sx)%dn][(k+sy)%dm]=='1') pcode[j][k]+=1<<(sx*cachesize+sy);
                         }
                         else {
                         pcode[j][k]=pcode[j-(cachesize-2)][k];
                         pcode[j][k]=pcode[j][k]>>((cachesize-2)*cachesize);
                         for (int sx=2; sx<cachesize; sx++)
                             for (int sy=0; sy<cachesize; sy++)
                                 if (nowg[(j+sx)%dn][(k+sy)%dm]=='1') pcode[j][k]+=1<<(sx*cachesize+sy);
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
                                     if (nowg[(dx[d]+j+sx+1+dn)%dn][(dy[d]+k+sy+1+dm)%dm]=='1') neigh++;
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
                      //fprintf(debug,"(%d,%d) = %d -> %d\n",j,k,code,cach[code]);
                      for (int sx=0; sx<cachesize-2; sx++)
                          for (int sy=0; sy<cachesize-2; sy++)
                              if (cach[code] & (1<<(sx*(cachesize-2)+sy))) newg[(j+sx+1)%dn][(k+sy+1)%dm]='1';
                                 else newg[(j+sx+1)%dn][(k+sy+1)%dm]='0';
                      }
                  }
              nowg=newg;
              //vis(nowg);
              }
          int live=0;
          for (int i=0; i<dn; i++)
              for (int j=0; j<dm; j++)
                  if (nowg[i][j]=='1') live++;
          //fprintf(debug,"Final:\n"); vis(nowg);
          fprintf(debug,"Live: %d\n",live);
          return live;
          }
      double move (int ov,int nv,double frac) {
          //fprintf(debug,"(%d,%d,%0.3lf) = %0.4lf\n",ov,nv,frac); fflush(debug);
          if (nv>ov) return 20000.0;
          double temp=ov*sqrt(1.1-frac)*0.008;
          double prob=exp(-(ov-nv+sqrt(ov*.02))/temp);
          prob*=20000;
          //double prob=1.0/((ov-nv)*(250-sqrt(ov)*2)+80-sqrt(ov)/6)*exp(-1.8*frac-1.3)*20000;
          //double prob=exp(-(ov-nv+25)/5.0)*exp(-2.7*frac-0.9)*20000;
          fprintf(debug,"(%d,%d,%0.3lf) -> %0.4lf/%0.4lf = %0.4lf\n",ov,nv,frac,ov-nv+sqrt(ov*.2),temp,prob); fflush(debug);
          return prob;
          }
      void sa () {
          vector<string> st;
          vector<pair<int,int> > stch,orich;
          //double temp=
          //int mxchange=max(1,n/8);
          int mxchange=min(max(1,n/8),1);
          int orival,stval;
          st=grid;
          int xx,yy,stim=0;
          for (int i=0; i<n; i++) {
              xx=rand()%dn; yy=rand()%dm;
              orich.push_back(make_pair(xx,yy));
              st[xx][yy]=1-(st[xx][yy]-'0')+'0';
              }
          orival=stimulate(st);
          while (runtime()<timelim) {
             stim++;
             fprintf(debug,"New(%d): %d\n",stim,orival); fflush(debug);
             stch=orich;
             for (int j=0; j<mxchange; j++) {
                 int ind=rand()%(stch.size());
                 stch[ind]=make_pair(rand()%dn,rand()%dm);
                 }
             st=grid;
             for (int j=0; j<stch.size(); j++) {
                 xx=stch[j].first; yy=stch[j].second;
                 st[xx][yy]=1-(st[xx][yy]-'0')+'0';
                 }
             stval=stimulate(st);
             fprintf(debug,"    - %d\n",stval); fflush(debug);
             if (stval>best) {
                best=stval;
                ret=st;
                }
             if (move(orival,stval,runtime()/timelim)>rand()%20000) {
                orich=stch; orival=stval;
                }
             }
          }
  	  vector <string> configure(vector <string> _grid, string rules, int N, int K) {
          n=N; rd=K; rule=rules; grid=_grid; dn=grid.size(); dm=grid[0].length();
          startt();
          memset(cach,-1,sizeof(cach));
          ret=grid;
          sa();
          fprintf(debug,"cache used = %d (%d)\n",cachecnt,totopr);
          return ret;
      }
};

vector<string> RET;

int main(){
    CellularAutomaton CA;
    debug=fopen("D:\\Documents\\Computer\\TopCoder\\CellularAutomaton\\debug.txt","w");
    int RR,NN,KK;
    scanf("%d",&RR);
    vector<string> Grid;
    string Rule;
    Grid.resize(RR);
    for (int i=0; i<RR; i++) cin >> Grid[i];
    cin >> Rule;
    scanf("%d%d",&NN,&KK); 
    RET=CA.configure(Grid,Rule,NN,KK);
    for (int i=0; i<RET.size(); i++) {
        printf("%s\n",RET[i].c_str());
        fflush(stdout);
        }
}
