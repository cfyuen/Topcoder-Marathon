/*****************
v4.2
various height(hill climb, minor adjust),approx binpack
change findheight
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
int done[1<<27],hisco=0,use[1400];
vector<int> bestret,besth;

//delete when submit
FILE *debug,*result;
double tlim1=4.0,tlim2=5.5;

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
bool cmprev (int a,int b) {
     return a>b;
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
      
      /*
      vector<int> approxbinpack (vector<int> shh) {
          sort(shh.begin(),shh.end());
          memset(used,0,sizeof(used));
          vector<int> bret,wid;
          for (int i=0; i<b.size(); i++) bret.push_back(-1);
          vector<Book> tb=b;
          wid.resize(shh.size());
          for (int i=0; i<shh.size(); i++) {
              for (int j=0; j<b.size(); j++)
                  if (tb[j].h<=shh[i]) {
                     tb[j].h=shh[i];
                     }
              sort(tb.begin(),tb.end(),cmpr);
              fprintf(debug,"shelf: %d\n",i);
              for (int j=0; j<b.size(); j++)
                  if (tb[j].h<=shh[i] && wid[i]+tb[j].w<=W && used[j]==0) {
                     wid[i]+=tb[j].w;
                     bret[tb[j].ind]=i;
                     fprintf(debug,"%d: %10d  %10d  %10d\n",j,tb[j].h,tb[j].w,tb[j].v);
                     used[j]=1;
                     }
              }
          return bret;
          }
      */
      vector<int> binpack (vector<int> shh) {
          sort(shh.begin(),shh.end());
          sort(b.begin(),b.end(),cmpv);
          vector<int> bret;
          int curmx=0;
          for (int i=0; i<b.size(); i++) bret.push_back(-1);
          for (int i=0; i<shh.size(); i++) {
              fprintf(debug,"Height %d: %d\n",i,shh[i]);
              memset(bdp,-1,sizeof(bdp));
              memset(prev,-1,sizeof(prev));
              curmx=0;
              for (int j=0; j<=b.size(); j++) {
                  int ind=j-1,nowmx;
                  if (j==0) nowmx=maxpershelf;
                     else nowmx=curmx+b[ind].v;
                  //fprintf(debug,"nowmx: %d\n",nowmx); fflush(debug);
                  //for (int k=0; k<=nowmx; k++) {
                  for (int k=0; k<maxpershelf/factor; k++) {
                      if (k==0) { bdp[j][k]=0; continue; }
                      if (j==0) { bdp[j][k]=2000000; continue; }
                      if (b[ind].v>k) { bdp[j][k]=bdp[j-1][k]; prev[j][k]=prev[j-1][k]; continue; }
                      if (b[ind].w+bdp[j-1][k-b[ind].v]<bdp[j-1][k] && bret[b[ind].ind]==-1 && b[ind].h<=shh[i]) {
                         bdp[j][k]=b[ind].w+bdp[j-1][k-b[ind].v];
                         curmx=max(k,curmx);
                         //prev[j][k]=prev[j-1][k-b[
                         }
                         else { bdp[j][k]=bdp[j-1][k]; prev[j][k]=prev[j-1][k]; }
                      }
                     /*
                     fprintf(debug,"%d: ",j);
                     for (int k=0; k<1000; k++) 
                         fprintf(debug,"%8d ",bdp[j][k]);
                     fprintf(debug,"\n");
                     fflush(debug);
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
          int asum=0;
          memset(use,-1,sizeof(use));
          for (int i=0; i<b.size(); i++)
              if (asum+b[i].h*b[i].w<H*W) {
                 asum+=b[i].h*b[i].w;
                 use[b[i].ind]=H*W/asum;
                 }
          sort(b.begin(),b.end(),cmph);
          vector<int> tryh;
          int noww=100000,sumv=0,last=-1;
          for (int i=0; i<b.size(); i++) {
              if (use[b[i].ind]!=-1) {
                 if (noww>W || ((last==-1 || b[last].h-b[i].h>3) && use[b[i].ind]>0 && b[i].h>50)) {
                    tryh.push_back(b[i].h);
                    if (noww>W) noww=0;
                    }
                 noww+=b[i].w;
                 fprintf(debug,"%d: %10d  %10d  %10d (%d) %0.4lf\n",i,b[i].h,b[i].w,b[i].v,use[b[i].ind],(50+b[i].h)*b[i].w*1.0/b[i].v);
                 sumv+=b[i].v;
                 last=i;
                 }
              }
          fprintf(debug,"sumv: %d\n",sumv);
          return tryh;
          }
      void present (int a,int ba,int sz) {
           for (int i=0; i<sz; i++) {
                  if (a<0) {
                     fprintf(debug,"%d ",-((abs(a-ba/2)%ba)-ba/2));
                     a=(a-ba/2)/ba;
                     }
                     else {
                     fprintf(debug,"%d ",(a+ba/2)%ba-ba/2);
                     a=(a+ba/2)/ba;
                     }
                  }
              fprintf(debug,"\n");
           }
      void minoradjust () {
          //only apply for the middle numbers
          vector<int> nowh,orih=besth,tret;
          priority_queue<pair<int,int> > mpq;
          mpq.push(make_pair(0,0));
          int del,ndel,base,mulb[50];
          if (besth.size()>8) base=5;
             else if (besth.size()>6) base=7;
             else if (besth.size()>4) base=9;
             else base=13;
          mulb[0]=1;
          for (int i=1; mulb[i-1]*base<(1<<30); i++) mulb[i]=mulb[i-1]*base; 
          int opr=0;
          while (!mpq.empty() && runtime()<tlim2) {
              nowh=orih;
              del=mpq.top().second; ndel=del;
              mpq.pop();
              //fprintf(debug,"state %d\n",del);
              //present(del,base,nowh.size());
              for (int i=0; i<nowh.size(); i++) {
                  if (del<0) {
                     nowh[i]-=abs(del-base/2)%base-base/2;
                     del=(del-base/2)/base;
                     }
                     else {
                     nowh[i]+=(del+base/2)%base-base/2;
                     del=(del+base/2)/base;
                     }
                  }
              opr++;
              fprintf(debug,"MSel: ");
              for (int j=0; j<nowh.size(); j++)
                  fprintf(debug,"%d ",nowh[j]);
              fprintf(debug,"\n"); fflush(debug);
              int scor;
              tret=approxbinpack(nowh);
              scor=score(tret);
              fprintf(debug,"Sco = %d (%d)\n",scor,hisco); fflush(debug);
              if (scor>hisco) {
                 bestret=tret;
                 besth=nowh;
                 hisco=scor;
                 //fprintf(debug,"HiSco from %d opr = (%d)\n",fr,opr);
                 }
              del=ndel;
              present(del,base,nowh.size());
              for (int i=0; i<nowh.size(); i++)
                  for (int j=0; j<nowh.size(); j++)
                      if (i!=j && nowh[i]>=orih[i] && nowh[i]-orih[i]<=base/2-1 && nowh[j]<=orih[j] && orih[j]-nowh[j]<=base/2-1) {
                         ndel=del;
                         if (i>j) {
                            ndel+=mulb[i];
                            ndel-=mulb[j];
                            }
                            else {
                            ndel+=mulb[i];
                            ndel-=mulb[j];
                            }
                         //fprintf(debug,"%d --   ",ndel); present(ndel,base,nowh.size());
                         mpq.push(make_pair(scor,ndel));
                         }
              }
          fprintf(debug,"opreation = %d\n",opr);
          }
      void select () {
          vector<int> geth;
          int curleast=100000;
          geth=findheight(); 
          sort(geth.begin(),geth.end(),cmprev);
          int getsz=geth.size();
          for (int i=0; i<geth.size(); i++)
              fprintf(debug,"FinH: %d\n",geth[i]);
          fflush(debug);
          for (int i=1; i<getsz; i++)
              for (int j=0; j<b.size(); j++)
                  if (abs((geth[i]+geth[i-1])/2-b[j].h)<=1 && use[b[j].ind]!=-1) {
                     geth.push_back(b[j].h);
                     break;
                     }
          if (geth.size()>27) geth.resize(27);
          sort(geth.begin(),geth.end(),cmprev);
          for (int i=1; i<geth.size(); i++)
              if (geth[i-1]-geth[i]<2) { geth.erase(geth.begin()+i); i--; }
          fflush(debug);
          //factor=max(4,(int)b.size()/50);
          factor=1;
          fprintf(debug,"Factor: %d\n",factor);
          for (int i=0; i<b.size(); i++) {
              b[i].v/=factor;
              b[i].v++;
              }
          
          vector<int> selh=geth,tret;
          for (int i=0; i<geth.size(); i++)
              fprintf(debug,"FinH: %d\n",geth[i]);
          fflush(debug);
          int at=0,th=0,sumh=0,scor,least;
          /*
          for (int i=0; i<geth.size(); i++) {
              if (geth[i]+th+10<=H) {
                 th+=geth[i]+10;
                 at+=1<<i;
                 least=i;
                 }
              }
          */
          int opr=0,fr;
          at+=1<<0;
          least=0;
          memset(done,0,sizeof(done));
          priority_queue<pair<int,int> > pq;
          pq.push(make_pair(0,at));
          done[at-(1<<least)]=1;
          while (!pq.empty() && runtime()<tlim1) {
              at=pq.top().second;
              fr=pq.top().first;
              pq.pop();
              selh.clear();
              sumh=0;
              opr++;
              for (int i=0; i<geth.size(); i++)
                  if (at & (1<<i)) {
                     sumh+=geth[i]+10;
                     selh.push_back(geth[i]);
                     least=i;
                     }
              selh[selh.size()-1]+=H-sumh;
              fprintf(debug,"Sel: ");
              for (int j=0; j<selh.size(); j++)
                  fprintf(debug,"%d ",selh[j]);
              fprintf(debug,"\n"); fflush(debug);
              tret=approxbinpack(selh);
              scor=score(tret);
              fprintf(debug,"Sco(%d) = %d (%d)\n",fr,scor,hisco); fflush(debug);
              if (scor>hisco) {
                 bestret=tret;
                 besth=selh;
                 hisco=scor;
                 fprintf(debug,"HiSco from %d opr = (%d)\n",fr,opr);
                 }
              int nsum,nat,nleast;
              for (int i=0; i<geth.size(); i++)
                  if (at & (1<<i))
                     for (int j=0; j<geth.size(); j++) 
                         if (at & (1<<j)) {}
                            else {
                            nsum=sumh-geth[i]+geth[j];
                            nat=at;
                            nleast=least;
                            nat-=(1<<i);
                            nat+=(1<<j);
                            //fprintf(debug,"%d,%d,%d\n",i,j,nleast); fflush(debug);
                            if (i==nleast) {
                               for (int k=i-1; k>=0; k--) {
                                   nleast--;
                                   //fprintf(debug,"%d\n",k);
                                   if (at & (1<<k)) break;
                                   }
                               }
                            nleast=max(j,nleast);
                            //fprintf(debug,"%d,%d,%d\n",i,j,nleast); fflush(debug);
                            //fprintf(debug,"%d,%d,%d\n",nsum,nat,(1<<nleast)); fflush(debug);
                            if (nsum<=H && done[nat-(1<<nleast)]==0) {
                               done[nat-(1<<nleast)]=1;
                               pq.push(make_pair(scor,nat));
                               }
                            }
              for (int i=0; i<geth.size(); i++)
                  if (at & (1<<i)) {}
                     else {
                     nsum=sumh+geth[i]+10;
                     nat=at;
                     nat+=(1<<i);
                     //fprintf(debug,"%d,%d,%d\n",nsum,nat,(1<<max(i,least))); fflush(debug);
                     if (nsum<=H && done[nat-(1<<max(i,least))]==0) {
                        done[nat-(1<<max(i,least))]=1;
                        pq.push(make_pair(scor,nat));
                        }
                     }
              }
          fprintf(debug,"opreation = %d\n",opr);
          fprintf(stderr,"Time 1: %0.4lf\n",runtime()); fflush(stderr);
          minoradjust();
          fprintf(stderr,"Time 2: %0.4lf\n",runtime()); fflush(stderr);
          factor=1;
          //b=orib;
          sort(b.begin(),b.end(),cmpr);
          bestret=binpack(besth);
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
          fprintf(stderr,"Time: %0.4lf\n",runtime());
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
