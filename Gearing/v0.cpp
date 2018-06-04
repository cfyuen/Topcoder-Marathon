#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<sstream>
#include<cmath>
using namespace std;

struct Gear {
   int pla,sz;
   double x,y;
};

int n;
vector<int> teeth;
vector<Gear> best;
vector<string> ret;

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

class Gearing {
   public:
  	  vector <string> place(int K, vector <int> Teeth) {
         teeth=Teeth;
         while (teeth[teeth.size()/2-1]==teeth[teeth.size()/2] && teeth.size()>2) {
            teeth.erase(teeth.begin()+teeth.size()/2-1,teeth.begin()+teeth.size()/2+1);
            }
         n=teeth.size();
         best.resize(teeth.size());
         
         for (int i=0; i<n; i++)
             fprintf(debug,"%d ",teeth[i]);
         fprintf(debug,"\n");
         fflush(debug);
         //main part
         int bigswap=teeth[n/2];
         for (int i=n/2; i<n-1; i++)
             teeth[i]=teeth[i+1];
         teeth[n-1]=bigswap;
         int plane=0;
         Gear tg;
         for (int i=0; i<n/2; i++) {
             tg.sz=teeth[i]; tg.pla=plane;
             if (i==0) tg.x=0.0;
                else tg.x=best[i*2-1].x;
             tg.y=0.0;
             best[i*2]=tg;
             
             tg.sz=teeth[n/2+i]; tg.pla=plane;
             tg.x=best[i*2].x+10*(best[i*2].sz+teeth[n/2+i])-9.999;
             tg.y=0.0;
             best[i*2+1]=tg;
             plane=(plane+1)%K;
             }
         
         //configure best into string
         for (int i=0; i<n; i++) {
             char cstr[1000];
             fprintf(debug,"%d %d %0.4lf %0.4lf\n",best[i].sz,best[i].pla,best[i].x,best[i].y);
             sprintf(cstr,"%d %d %0.8lf %0.8lf",best[i].sz,best[i].pla,best[i].x,best[i].y);
             string tstr=cstr;
             ret.push_back(tstr);
             //fprintf(debug,"%s\n",cstr);
             }
         return ret;
      }
};

int main () {
    Gearing GG;
    debug=fopen("debug.txt","w");
    int KK,NN;
    vector<int> RR;
    scanf("%d%d",&KK,&NN);
    RR.resize(NN);
    for (int i=0; i<NN; i++)
        scanf("%d",&RR[i]);
    vector<string> RET;
    RET=GG.place(KK,RR);
    printf("%d\n",RET.size());
    for (int i=0; i<RET.size(); i++)
        cout << RET[i] << endl;
    fflush(stdout);
    return 0;
}
