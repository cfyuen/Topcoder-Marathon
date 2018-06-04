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

FILE *a,*b,*out,*best;
int cnt=0,tot=0,awin=0,bwin=0,draw=0,a0=0,b0=0;
double ta=0,tb=0,th=0,hsco[1200];

int main () {
    a=fopen("score v7 1-3.txt","r");
    b=fopen("score v7.2 1-3.txt","r");
    out=fopen("compare 7,7.2.txt","w");
    best=fopen("highscore.txt","r");
    int ti,cnt=1;
    char ts[1000];
    for (int i=1; i<=300; i++) {
        double da,db;
        fscanf(a,"%s%s%s%lf\n",ts,ts,ts,&da);
        fscanf(b,"%s%s%s%lf\n",ts,ts,ts,&db);
        fscanf(best,"%d %lf",&ti,&hsco[i]);
        if (da<=0) a0++;
        if (db<=0) b0++;
        if (abs(da-db)<1e-5) draw++;
           else {
           if (da>db) {
              if (da-db>100) fprintf(out,"Note\n");
              awin++;
              }
           if (db>da) bwin++;
           }
        hsco[i]=max(hsco[i],max(da,db));
        
        fprintf(out,"Seed %d: %0.4lf  %0.4lf\n",i,da,db);
        tot++;
        ta+=da; tb+=db;
        }
    fprintf(out,"Tot: %0.4lf   %0.4lf\n",ta,tb);
    fprintf(out,"Win: %d    %d     Draw %d\n",awin,bwin,draw);
    fprintf(out,"Average: %0.4lf   %0.4lf\n",ta/tot,tb/tot);
    fprintf(out,"Zero: %d    %d\n",a0,b0);
    best=fopen("highscore.txt","w");
    for (int i=1; i<=300; i++) {
        fprintf(best,"%d %12.4lf\n",i,hsco[i]);
        th+=hsco[i];
        }
    fprintf(best,"%0.4lf\n",th/tot);
    return 0;
}
