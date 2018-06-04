/*****************
v2 fixed height, binpack optimally for each shelf from low height to high height (O(nW))
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
vector<int> ret,prev[6005];
int bdp[6005];

//delete when submit
FILE *debug,*result;

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
     return x.v>y.v;
}
bool cmph (Book x,Book y) {
     return x.h>y.h;
}

class BookSelection {
   public:
      vector<int> binpack (vector<int> shh) {
          sort(shh.begin(),shh.end());
          sort(b.begin(),b.end(),cmpv);
          vector<int> bret;
          for (int i=0; i<b.size(); i++) bret.push_back(-1);
          for (int i=0; i<shh.size(); i++) {
              fprintf(debug,"Height %d: %d\n",i,shh[i]);
              memset(bdp,-1,sizeof(bdp));
              bdp[0]=0;
              for (int j=0; j<=W; j++) prev[j].clear();
              int bestw=0,bind;
              for (int j=0; j<b.size(); j++)
                  if (b[j].h<=shh[i] && bret[j]==-1) {
                     for (int k=W; k>=b[j].w; k--)
                         if (bdp[k-b[j].w]!=-1 && bdp[k-b[j].w]+b[j].v>bdp[k]) {
                            bdp[k]=bdp[k-b[j].w]+b[j].v;
                            prev[k]=prev[k-b[j].w];
                            prev[k].push_back(j);
                            if (bdp[k]>bestw) {
                               bestw=bdp[k];
                               bind=k;
                               }
                            }
                     }
              int totw=0;
              for (int j=0; j<prev[bind].size(); j++) {
                  bret[prev[bind][j]]=i;
                  fprintf(debug,"%d: %10d  %10d  %10d\n",prev[bind][j],b[prev[bind][j]].h,b[prev[bind][j]].w,b[prev[bind][j]].v);
                  totw+=b[prev[bind][j]].w;
                  }
              fprintf(debug,"Width %d\n",totw);
              }
          return bret;
          }
                  
              
      void select () {
          sort(b.begin(),b.end(),cmpv);
          int totsum=0,avesum=0;
          for (int i=0; i<b.size(); i++) totsum+=b[i].v;
          avesum=totsum/b.size();
          fprintf(debug,"Tot: %d   Ave: %d\n",totsum,avesum);
          sort(b.begin(),b.end(),cmph);
          vector<int> tryh,finh;
          double mul=2.5;
          int toth=0;
          do {
             tryh.clear(); finh.clear();
             int curw=100000;
             toth=0;
             for (int i=0; i<b.size(); i++)
                 if (b[i].v>avesum*mul && curw+b[i].w>W) {
                    tryh.push_back(b[i].h);
                    curw=b[i].w;
                    }
                    else if (b[i].v>avesum*(mul*3/4)) {
                            curw+=b[i].w;
                            }
             fprintf(debug,"mul  %0.4lf\n",mul);
             for (int i=0; i<tryh.size(); i++) {
                 fprintf(debug,"Shelf: %d\n",tryh[i]);
                 if (toth+tryh[i]+10<=H) {
                    toth+=tryh[i]+10;
                    finh.push_back(tryh[i]);
                    }
                 }
             finh[finh.size()-1]+=H-toth;
             mul*=0.95;
             } while (finh[finh.size()-1]>=b[0].h+10 && mul>0.2);
          for (int i=0; i<finh.size(); i++) {
              fprintf(debug,"Shelf: %d\n",finh[i]);
              }
          fprintf(debug,"STot: %d\n",toth);
          vector<int> tret;
          tret=binpack(finh);
          for (int i=0; i<b.size(); i++)
              ret[b[i].ind]=tret[i];
          }
      vector <int> arrange(int H1, int W1, vector <int> bookH, vector <int> bookW, vector <int> bookV) {
          H=H1; W=W1;
          b.resize(bookH.size());
          for (int i=0; i<bookH.size(); i++) {
              b[i].h=bookH[i]; b[i].w=bookW[i]; b[i].v=bookV[i]; b[i].ind=i;
              }
          orib=b;
          sort(b.begin(),b.end(),cmpv);
          ret.resize(b.size());
          for (int i=0; i<b.size(); i++) {
              fprintf(debug,"%d: %10d  %10d  %10d\n",i,b[i].h,b[i].w,b[i].v);
              ret[i]=-1;
              }
          select();
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
