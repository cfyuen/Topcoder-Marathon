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
double ta=0,tb=0;

int main () {
    a=fopen("result v1.3.txt","r");
    b=fopen("result v1.4a.txt","r");
    out=fopen("compare 1.3,1.4a.txt","w");
    char ts[1000];
    while (fscanf(a,"%s",ts)==1 && fscanf(b,"%s",ts)==1) {
        double da,db;
        fscanf(a,"%s%d\n",ts,&cnt);
        fgets(ts,1000,a);
        fgets(ts,1000,a);
        fscanf(a,"%s%s%lf\n",ts,ts,&da);
        fscanf(b,"%s%d\n",ts,&cnt);
        fgets(ts,1000,b);
        fgets(ts,1000,b);
        fscanf(b,"%s%s%lf\n",ts,ts,&db);
        if (da==0) a0++;
        if (db==0) b0++;
        if (abs(da-db)<1e-5) draw++;
           else {
           if (da>db) awin++;
           if (db>da) bwin++;
           }
        fprintf(out,"%0.4lf  %0.4lf\n",da,db);
        tot++;
        ta+=da; tb+=db;
        }
    fprintf(out,"tot: %0.4lf   %0.4lf\n",ta,tb);
    fprintf(out,"average: %0.4lf   %0.4lf\n",ta/tot,tb/tot);
    fprintf(out,"zero: %d    %d\n",a0,b0);
    return 0;
}
