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
double hsco[1200];
int cnt=0;

int main () {
    a=fopen("result 2 1000.txt","r");
    b=fopen("result 9.1 1000a.txt","r");
    out=fopen("compare.txt","w");
    best=fopen("highscore.txt","r");
    int ti;
    while (fscanf(best,"%d %lf",&ti,&hsco[cnt])==2) {
       cnt++;
       }
    best=fopen("highscore.txt","w");
    double ta=0.0,tb=0.0;
    int cas=0;
    char ts[100];
    while (fscanf(a,"%s",ts)==1 && fscanf(b,"%s",ts)==1) {
        double da,db;
        char sa[1000],sb[1000],ts[100];
        fgets(sa,1000,a); fgets(sa,1000,a); fgets(sa,1000,a); fgets(sa,1000,a); fgets(sa,1000,a); fgets(sa,1000,a);
        fgets(sb,1000,b); fgets(sb,1000,b); fgets(sb,1000,b); fgets(sb,1000,b); fgets(sb,1000,b); fgets(sb,1000,b);
        //fprintf(out,"%s %s\n",sa,sb);
        sscanf(sa,"%s = %lf",ts,&da);
        sscanf(sb,"%s = %lf",ts,&db);
        fprintf(out,"seed = %d     %0.5lf %0.5lf     %0.5lf\n",cas+1,da,db,hsco[cas]);
        hsco[cas]=max(hsco[cas],max(da,db));
        ta+=da/hsco[cas];
        tb+=db/hsco[cas];
        cas++;
        }
    for (int i=0; i<cas; i++)
        fprintf(best,"%d %0.6lf\n",i+1,hsco[i]);
    fprintf(out,"case: %d  %0.5lf %0.5lf\n",cas,ta,tb);
    return 0;
}
