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
vector<int> ret;

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

class BookSelection {
   public:
      void select () {
          int toth=0;
          vector<Shelf> coll;
          for (int i=0; i<b.size(); i++) {
              int put=0;
              fprintf(debug,"Book: %d,%d -- %d\n",b[i].h,b[i].w,b[i].v);
              for (int j=0; j<coll.size(); j++) {
                  if (coll[j].w+b[i].w<=W && toth+max(0,b[i].h-coll[j].h)<=H) {
                     if (b[i].h>coll[j].h) {
                        toth+=b[i].h-coll[j].h;
                        coll[j].h=b[i].h;
                        }
                     coll[j].w+=b[i].w;
                     coll[j].bok.push_back(b[i].ind);
                     ret[b[i].ind]=j;
                     put=1;
                     fprintf(debug,"Shelf %d : %d,%d\n",j,coll[j].h,coll[j].w);
                     break;
                     }
                  }
              if (put==0) {
                 if (toth+10+b[i].h<=H) {
                    Shelf nsh;
                    fprintf(debug,"New Shelf: %d\n",coll.size());
                    nsh.bok.push_back(b[i].ind);
                    nsh.h=b[i].h;
                    toth+=10+b[i].h;
                    nsh.w=b[i].w;
                    ret[b[i].ind]=coll.size();
                    coll.push_back(nsh);
                    }
                 }
              }
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
