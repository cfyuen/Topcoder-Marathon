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

class Gearing {
   public:
  	  vector <string> place(int K, vector <int> Teeth) {
         teeth=Teeth;
         while (teeth[teeth.size()/2-1]==teeth[teeth.size()/2] && teeth.size()>2) {
            teeth.erase(teeth.begin()+teeth.size()/2-1);
            teeth.erase(teeth.begin()+teeth.size()/2-1);
            }
         n=teeth.size();
         best.resize(teeth.size());
         
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
             sprintf(cstr,"%d %d %0.8lf %0.8lf",best[i].sz,best[i].pla,best[i].x,best[i].y);
             string tstr=cstr;
             ret.push_back(tstr);
             }
         return ret;
      }
};

