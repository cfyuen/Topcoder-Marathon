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
   int h,w,v;
};

vector<Book> b;
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

class BookSelection {
   public:
      vector <int> arrange(int H1, int W1, vector <int> bookH, vector <int> bookW, vector <int> bookV) {
          H=H1; W=W1;
          b.resize(bookH.size());
          for (int i=0; i<bookH.size(); i++) {
              b[i].h=bookH[i]; b[i].w=bookW[i]; b[i].v=bookV[i];
              }
          for (int i=0; i<b.size(); i++)
              ret.push_back(-1);
          ret[0]=0;
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
