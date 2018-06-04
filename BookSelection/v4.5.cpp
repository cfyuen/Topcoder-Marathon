/*****************
v4.5
various height(hill climb, minor adjust),approx binpack
change findheight, estimate average - Bug fixed
Bugs fixed in valley...
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
int bdp[1400][6005],prev[1400][6005],hcho[250];
int maxpershelf=1800,factor=1;
int done[1<<27],hisco=0,use[1400],tots[30],totcnt[30];
double aver[30];
vector<int> bestret,besth,indsco[30];


//delete when submit
FILE *debug,*result;
double tlim1_1=2.0,tlim1_2=4.0,tlim2=5.4,tout=5.9;

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
double tlim1_1=0.6666,tlim1_2=1.3333,tlim2=1.8,tout=1.98;

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
          for (int i=0; i<b.size(); i++)
              if (res[i]!=-1) {
                 sco+=orib[i].v;
                 }
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
                  if (runtime()>tout) return approxbinpack(shh);
                  for (int k=0; k<maxpershelf/factor; k++) {
                      if (k==0) { bdp[j][k]=0; continue; }
                      if (j==0) { bdp[j][k]=2000000; continue; }
                      if (b[ind].v>k) { bdp[j][k]=bdp[j-1][k]; prev[j][k]=prev[j-1][k]; continue; }
                      if (b[ind].w+bdp[j-1][k-b[ind].v]<bdp[j-1][k] && bret[b[ind].ind]==-1 && b[ind].h<=shh[i]) {
                         bdp[j][k]=b[ind].w+bdp[j-1][k-b[ind].v];
                         curmx=max(k,curmx);
                         }
                         else { bdp[j][k]=bdp[j-1][k]; prev[j][k]=prev[j-1][k]; }
                      }
                  }
              int bval=0,vnow;
              for (int j=0; j<maxpershelf/factor; j++)
                  if (bdp[b.size()][j]<=W) {
                     bval=j;
                     }
              vnow=bval;
              fprintf(debug,"best: %d\n",bval);
              for (int j=b.size()-1; j>=0; j--)
                  if (bdp[j][vnow]!=bdp[j+1][vnow]) {
                     bret[b[j].ind]=i;
                     vnow-=b[j].v;
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
          sort(tryh.begin(),tryh.end(),cmprev);
          int getsz=tryh.size();
          for (int i=0; i<tryh.size(); i++)
              fprintf(debug,"FinH: %d\n",tryh[i]);
          fflush(debug);
          for (int i=1; i<getsz; i++)
              for (int j=0; j<b.size(); j++)
                  if (abs((tryh[i]+tryh[i-1])/2-b[j].h)<=1 && use[b[j].ind]!=-1) {
                     tryh.push_back(b[j].h);
                     break;
                     }
          if (tryh.size()>27) tryh.resize(27);
          sort(tryh.begin(),tryh.end(),cmprev);
          for (int i=1; i<tryh.size(); i++)
              if (tryh[i-1]-tryh[i]<2) { tryh.erase(tryh.begin()+i); i--; }
          fflush(debug);
          return tryh;
          }
      void minoradjust () {
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
                 }
              del=ndel;
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
                         mpq.push(make_pair(scor,ndel));
                         }
              }
          fprintf(debug,"opreation = %d\n",opr);
          }
       vector<int> majorheight (vector<int> geth,double timelim) {
          int curleast=100000;
          sort(geth.begin(),geth.end(),cmprev);
          vector<int> selh=geth,tret;
          for (int i=0; i<geth.size(); i++) {
              fprintf(debug,"FinH: %d\n",geth[i]);
              indsco[i].clear();
              }
          fflush(debug);
          int at=0,th=0,sumh=0,scor,least,cntopr=0;
          int opr=0,fr;
          at+=1<<0;
          least=0;
          memset(totcnt,0,sizeof(totcnt));
          memset(tots,0,sizeof(tots));
          memset(done,-1,sizeof(done));
          priority_queue<pair<int,int> > pq;
          pq.push(make_pair(0,at));
          done[at-(1<<least)]=1;
          while (!pq.empty() && runtime()<timelim) {
              at=pq.top().second;
              fr=pq.top().first;
              pq.pop();
              selh.clear();
              sumh=0;
              opr++; cntopr++;
              for (int i=0; i<geth.size(); i++)
                  if (at & (1<<i)) fprintf(debug,"#");
                     else fprintf(debug,".");
              fprintf(debug,"\n");
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
                            if (i==nleast) {
                               for (int k=i-1; k>=0; k--) {
                                   nleast--;
                                   if (at & (1<<k)) break;
                                   }
                               }
                            nleast=max(j,nleast);
                            //fprintf(debug,"%d,%d,%d\n",i,j,nleast); fflush(debug);
                            //fprintf(debug,"%d,%d,%d\n",nsum,nat,(1<<nleast)); fflush(debug);
                            if (nsum<=H && (done[nat-(1<<nleast)]==-1 || scor-done[nat-(1<<nleast)]>300)) {
                               done[nat-(1<<nleast)]=scor;
                               pq.push(make_pair(scor,nat));
                               }
                            }
              for (int i=0; i<geth.size(); i++)
                  if (at & (1<<i)) {
                     indsco[i].push_back(scor);
                     }
                     else {
                     nsum=sumh+geth[i]+10;
                     nat=at;
                     nat+=(1<<i);
                     //fprintf(debug,"%d,%d,%d\n",nsum,nat,(1<<max(i,least))); fflush(debug);
                     if (nsum<=H && (done[nat-(1<<max(i,least))]==-1 || scor-done[nat-(1<<max(i,least))]>100)) {
                        done[nat-(1<<max(i,least))]=scor;
                        pq.push(make_pair(scor,nat));
                        }
                     }
              }
          fprintf(debug,"opreation = %d (%d)\n",opr,cntopr);
          double aversco=0; int not0=0;
          for (int i=0; i<geth.size(); i++) {
              sort(indsco[i].begin(),indsco[i].end(),cmprev);
              for (int j=0; j<30 && j<indsco[i].size()/2; j++) {
                  tots[i]+=indsco[i][j];
                  totcnt[i]++;
                  }
              if (totcnt[i]>0) {
                 aver[i]=tots[i]*1.0/totcnt[i];
                 aversco+=aver[i];
                 not0++;
                 }
                 else aver[i]=1e9;
              }
          sort(aver,aver+geth.size());
          aversco/=not0;
          if (cntopr>200) {
             if (geth.size()/2<10)
                aversco=min(aversco,aver[max(0,(int)geth.size()-10)]-1e-5);
             }
             else aversco=aver[0]-1;
          fprintf(debug,"Average (%0.4lf) : \n",aversco);
          for (int i=0; i<geth.size(); i++)
              fprintf(debug,"%d = %0.2lf (%d,%d)\n",geth[i],tots[i]*1.0/totcnt[i],tots[i],totcnt[i]);
          vector<int> newh;
          for (int i=0; i<geth.size(); i++)
              if (totcnt[i]==0 || tots[i]*1.0/totcnt[i]>aversco-10) {
                 fprintf(debug,"in: %d\n",geth[i]);
                 newh.push_back(geth[i]);
                 }
          return newh;
          }
      void select () {
          memset(hcho,0,sizeof(hcho));
          vector<int> candh=findheight();
          for (int i=0; i<candh.size(); i++) {
              hcho[candh[i]]=1; hcho[candh[i]+1]=1; hcho[candh[i]-1]=1;
              }
          int times=1;
          while (runtime()<tlim1_2 && candh.size()>0) {
             fprintf(debug,"RUNRUN %d\n",times);
             candh=majorheight(candh,min(runtime()+tlim1_1,tlim1_2));
             times++;
             fprintf(stderr,"Time 1_1: %0.4lf\n",runtime()); fflush(stderr);
             int getsz=candh.size();
             for (int i=1; i<=getsz; i++) {
                 int ncand;
                 if (i==getsz) ncand=candh[getsz-1]-4;
                    else {
                    if (candh[i-1]-candh[i]>4 || getsz<=10) ncand=(candh[i]+candh[i-1])/2;
                       else continue;
                    }
                 for (int j=0; j<b.size(); j++)
                     if (abs(ncand-b[j].h)<=1 && use[b[j].ind]!=-1 && hcho[b[j].h]==0) {
                        candh.push_back(b[j].h);
                        hcho[b[j].h]=1; hcho[b[j].h+1]=1; hcho[b[j].h-1]=1;
                        break;
                        }
                 }
             if (candh.size()>27) candh.resize(27);
             }
          fprintf(stderr,"Time 1_2: %0.4lf\n",runtime()); fflush(stderr);
          minoradjust();
          fprintf(stderr,"Time 2: %0.4lf\n",runtime()); fflush(stderr);
          factor=1;
          sort(b.begin(),b.end(),cmpr);
          bestret=binpack(besth);
          for (int i=0; i<besth.size(); i++)
              fprintf(stderr,"%d ",besth[i]);
          fprintf(stderr,"\n");
          ret=bestret;
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
              ret[i]=-1;
              }
          select();
          fprintf(stderr,"Time: %0.4lf\n",runtime());
          int rev=0;
          for (int i=0; i<b.size(); i++) {
              rev=max(rev,ret[i]);
              }
          b=orib;
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
