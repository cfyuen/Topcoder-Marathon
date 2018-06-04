/*****************
v4
various height(exhausive),approx binpack
fixed height, binpack optimally for each shelf from low height to high height (O(n^2*v))
seed 6,58
*****************/
#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<cmath>
#include<queue>
using namespace std;

struct Book {
   int h,w,v,ind;
};
struct Shelf {
   vector<int> bok;
   int w,h;
};

vector<Book> b,orib;
int H,W;
vector<int> ret;
int bdp[1400][6005],prev[1400][6005];
int maxpershelf=1800,factor=1;
int done[1<<27];
int hisco=0,canuse[500],mxsz;
vector<int> tret,besth,bestret,geth;
vector<pair<int,vector<int> > > cand;

//delete when submit
FILE *debug,*result;
double tlim=6.0;

int sttime;
int tcnt=0;
void start() {
    sttime=clock();
}

double runtime() {
    return (clock()-sttime)/1000.0;
}

//til here

/* have it when submit
timeval sttime;

void start() {
    gettimeofday(&sttime,NULL);
}

double runtime() {
    timeval tt, nowt;
    gettimeofday(&tt,NULL);
    timersub(&tt,&sttime,&nowt);
    return (nowt.tv_sec*1000+nowt.tv_usec/1000)/1000.0;
    }
*/

bool cmpv (Book x,Book y) {
     if (x.v==y.v) {
        if (orib[x.ind].v==orib[y.ind].v) return x.ind<y.ind;
        return orib[x.ind].v>orib[y.ind].v;
        }
     return x.v>y.v;
}
bool cmph (Book x,Book y) {
     return x.h>y.h;
}
bool cmpr (Book x,Book y) {
     double xs,ys;
     xs=x.w*(50+x.h)*1.0/x.v;
     ys=y.w*(50+y.h)*1.0/y.v;
     if (abs(xs-ys)<1e-9) return x.ind<y.ind;
     return xs<ys;
}

class BookSelection {
   public:
      int score (vector<int> res) {
          int sco=0;
          int shw[150],nos=0; memset(shw,0,sizeof(shw));
          for (int i=0; i<b.size(); i++)
              if (res[i]!=-1) {
                 shw[res[i]]+=orib[i].w;
                 sco+=orib[i].v;
                 nos=max(nos,res[i]);
                 }
          //for (int i=0; i<=nos; i++) fprintf(debug,"W%d = %d\n",i,shw[i]);
          return sco;
          }
      vector<int> approxbinpack (vector<int> shh) {
          sort(shh.begin(),shh.end());
          sort(b.begin(),b.end(),cmpr);
          vector<int> bret,wid;
          vector<Book> tb=b;
          for (int i=0; i<b.size(); i++)
              for (int j=0; j<shh.size(); j++)
                  if (tb[i].h<=shh[j]) {
                     tb[i].h=shh[j];
                     break;
                     }
          sort(tb.begin(),tb.end(),cmpr);
          wid.resize(shh.size());
          for (int i=0; i<b.size(); i++) bret.push_back(-1);
          for (int i=0; i<b.size(); i++) {
              for (int j=0; j<shh.size(); j++)
                  if (tb[i].h<=shh[j] && wid[j]+tb[i].w<=W) {
                     wid[j]+=tb[i].w;
                     bret[tb[i].ind]=j;
                     break;
                     }
              }
          return bret;
          }
      vector<int> binpack (vector<int> shh) {
          sort(shh.begin(),shh.end());
          sort(b.begin(),b.end(),cmpv);
          vector<int> bret;
          for (int i=0; i<b.size(); i++) bret.push_back(-1);
          for (int i=0; i<shh.size(); i++) {
              fprintf(debug,"Height %d: %d\n",i,shh[i]);
              memset(bdp,-1,sizeof(bdp));
              memset(prev,-1,sizeof(prev));
              for (int j=0; j<=b.size(); j++) {
                  int ind=j-1;
                  for (int k=0; k<maxpershelf/factor; k++) {
                      if (k==0) { bdp[j][k]=0; continue; }
                      if (j==0) { bdp[j][k]=2000000; continue; }
                      if (b[ind].v>k) { bdp[j][k]=bdp[j-1][k]; prev[j][k]=prev[j-1][k]; continue; }
                      if (b[ind].w+bdp[j-1][k-b[ind].v]<bdp[j-1][k] && bret[b[ind].ind]==-1 && b[ind].h<=shh[i]) {
                         bdp[j][k]=b[ind].w+bdp[j-1][k-b[ind].v];
                         //prev[j][k]=prev[j-1][k-b[
                         }
                         else { bdp[j][k]=bdp[j-1][k]; prev[j][k]=prev[j-1][k]; }
                      }
                     /*
                     fprintf(debug,"%d: ",j);
                     for (int k=0; k<1000; k++) 
                         fprintf(debug,"%8d ",bdp[j][k]);
                     fprintf(debug,"\n");
                     */
                  }
              int bval=0,vnow;
              for (int j=0; j<maxpershelf/factor; j++)
                  if (bdp[b.size()][j]<=W) {
                     //fprintf(debug,"%d(%d) ",bdp[b.size()][j],j);
                     bval=j;
                     }
              vnow=bval;
              fprintf(debug,"best: %d\n",bval);
              for (int j=b.size()-1; j>=0; j--)
                  if (bdp[j][vnow]!=bdp[j+1][vnow]) {
                     bret[b[j].ind]=i;
                     //fprintf(debug,"%d - %d - %d\n",bdp[j+1][vnow],bdp[j][vnow],bdp[j][vnow-b[j].v]+b[j].w);
                     vnow-=b[j].v;
                     //fprintf(debug,"%d: %10d  %10d  %10d\n",j,b[j].h,b[j].w,b[j].v);
                     }
              }
          return bret;
          }
      vector<int> findheight () {
          sort(b.begin(),b.end(),cmpr);
          int use[1400],asum=0;
          memset(use,0,sizeof(use));
          for (int i=0; i<b.size(); i++)
              if (asum+b[i].h*b[i].w<H*W) {
                 asum+=b[i].h*b[i].w;
                 use[b[i].ind]=1;
                 }
          sort(b.begin(),b.end(),cmph);
          vector<int> tryh;
          int noww=100000,sumv=0;
          for (int i=0; i<b.size(); i++) {
              if (use[b[i].ind]==1) {
                 if (noww>W) {
                    tryh.push_back(b[i].h);
                    noww=0;
                    }
                 noww+=b[i].w;
                 fprintf(debug,"%d: %10d  %10d  %10d (%d) %0.4lf\n",i,b[i].h,b[i].w,b[i].v,use[b[i].ind],(50+b[i].h)*b[i].w*1.0/b[i].v);
                 sumv+=b[i].v;
                 }
              }
          fprintf(debug,"sumv: %d\n",sumv);
          return tryh;
          }
      void recur (vector<int> selh,int sum,int mis) {
          if (sum==H) {
             tret=approxbinpack(selh);
             int scor=score(tret);
             fprintf(debug,"Sel: ");
             for (int j=0; j<selh.size(); j++)
                 fprintf(debug,"%d ",selh[j]);
             fprintf(debug,"\n"); fflush(debug);
             fprintf(debug,"Sco = %d (%d)\n",scor,hisco);
             if (hisco-scor<10) {
                cand.push_back(make_pair(scor,selh));
                hisco=max(hisco,scor);
                }
             return;
             }
          if (sum>H || selh.size()>=mxsz) return;
          if (mis<=50 && selh.size()>1) return;
          if (mis<=60 && selh.size()>2) return;
          if (mis<=70 && selh.size()>3) return;
          if (mis<=60 && selh.size()==2 && selh[1]-selh[0]<=8) return;
          if (mis<=70 && selh.size()==3 && selh[1]-selh[0]<=8 && selh[2]-selh[1]<=6) return;
          if (mis<=90 && selh.size()>6) return;
          //fprintf(debug,"%d %d\n",sum,mis); fflush(debug);
          for (int i=mis; i<=geth[0]; i++) 
              if (canuse[i]==1) {
                 if (i+sum+10>H) break;
                 if (H-(i+sum+10)<i+10) {
                    if (H-sum-10>200) break;
                    selh.push_back(H-sum-10);
                    if (!(selh.size()>=2 && selh[selh.size()-1]>177 && selh[selh.size()-1]!=200 && selh[selh.size()-2]!=133))
                       recur(selh,H,i);
                    selh.erase(selh.begin()+selh.size()-1);
                    break;
                    }
                 selh.push_back(i);
                 recur(selh,sum+10+i,i+1);
                 selh.erase(selh.begin()+selh.size()-1);
                 }
          }
      void select () {
          int curleast=100000;
          mxsz=11;
          geth=findheight(); 
          if (geth.size()>27) geth.resize(27);
          memset(canuse,0,sizeof(canuse));
          for (int i=0; i<geth.size(); i++) 
              fprintf(debug,"FinH: %d\n",geth[i]);
          canuse[200]=1;
          canuse[177]=1;
          for (int i=1; i<geth.size(); i++)
              for (int j=geth[i]-5; j<=geth[i]+5; j++) canuse[j]=1;
          for (int i=1; i<200; i++)
              if (canuse[i]+canuse[i-1]==2) canuse[i]=0;
          canuse[133]=1; canuse[132]=0;
          canuse[134]=canuse[136]=canuse[138]=0;
          canuse[158]=1;
          fflush(debug);
          recur(besth,0,1);
          //b=orib;
          sort(b.begin(),b.end(),cmpr);
          sort(cand.begin(),cand.end());
          for (int j=cand.size()-1; j>=cand.size()-10 && j>=0; j--) {
              tret=binpack(cand[j].second);
              int scor=score(tret);
              if (scor>hisco) {
                 hisco=scor;
                 bestret=tret;
                 besth=cand[j].second;
                 }
              for (int i=0; i<cand[j].second.size(); i++) 
                  fprintf(debug,"%d ",cand[j].second[i]);
              fprintf(debug,"\nSco = %d(%d)\n",scor,hisco);
              }
          for (int i=0; i<besth.size(); i++) 
              fprintf(debug,"BestH: %d\n",besth[i]);
          fprintf(debug,"Final sco: %d\n",score(bestret));
          ret=bestret;
          //for (int i=0; i<b.size(); i++)
          //    ret[b[i].ind]=bestret[i];
          }
      vector <int> arrange(int H1, int W1, vector <int> bookH, vector <int> bookW, vector <int> bookV) {
          H=H1; W=W1;
          start();
          b.resize(bookH.size());
          for (int i=0; i<bookH.size(); i++) {
              b[i].h=bookH[i]; b[i].w=bookW[i]; b[i].v=bookV[i]; b[i].ind=i;
              }
          orib=b;
          sort(b.begin(),b.end(),cmpv);
          ret.resize(b.size());
          for (int i=0; i<b.size(); i++) {
              //fprintf(debug,"%d: %10d  %10d  %10d  %0.4lf\n",i,b[i].h,b[i].w,b[i].v,b[i].h*b[i].w*1.0/b[i].v);
              ret[i]=-1;
              }
          select();
          //fprintf(stderr,"Time: %0.4lf\n",runtime());
          int rev=0;
          for (int i=0; i<b.size(); i++) {
              rev=max(rev,ret[i]);
              //fprintf(debug,"%d: %d\n",i,ret[i]);
              }
          b=orib;
          //fprintf(debug,"FS %d\n",score(ret));
          for (int i=0; i<b.size(); i++) {
              if (ret[i]!=-1) ret[i]=rev-ret[i];
              }
          return ret;
      }
};

vector<int> RET;

int main(){
    BookSelection BB;
    debug=fopen("D:\\Documents\\Computer\\TopCoder\\BookSelection\\debug.txt","w");
    int HH,WW,XX;
    scanf("%d%d%d",&HH,&WW,&XX);
    vector<int> BH,BW,BV;
    BH.resize(XX); BW.resize(XX); BV.resize(XX);
    for (int i=0; i<XX; i++) scanf("%d",&BH[i]);
    for (int i=0; i<XX; i++) scanf("%d",&BW[i]);
    for (int i=0; i<XX; i++) scanf("%d",&BV[i]);
    RET=BB.arrange(HH,WW,BH,BW,BV);
    for (int i=0; i<RET.size(); i++) {
        printf("%d\n",RET[i]);
        fflush(stdout);
        }
}
