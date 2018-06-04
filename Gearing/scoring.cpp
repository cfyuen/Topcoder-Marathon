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

FILE *a,*b,*out;

int main () {
    a=fopen("result3.2.txt","r");
    b=fopen("result4.2.txt","r");
    out=fopen("compare.txt","w");
    double ta=0.0,tb=0.0;
    int cas=0;
    char ts[100];
    while (fscanf(a,"%s",ts)==1) {
        double da,db;
        char sa[1000],sb[1000],ts[100];
        fgets(sa,1000,a); fgets(sa,1000,a); fgets(sa,1000,a);
        fgets(sb,1000,b); fgets(sb,1000,b); fgets(sb,1000,b); 
        sscanf(sa,"%s %lf",ts,&da);
        sscanf(sb,"%s %lf",ts,&db);
        fprintf(out,"%0.2lf %0.2lf\n",da,db);
        if (da<db) {
           ta+=(da/db)*(da/db); tb+=1.0;
           if (abs(da-db)>1e-4) fprintf(out,"a better\n");
           }
           else {
           ta+=1.0; tb+=(db/da)*(db/da);
           if (abs(da-db)>1e-4) fprintf(out,"b better\n");
           }
        cas++;
        }
    fprintf(out,"case: %d  %0.5lf %0.5lf\n",cas,ta,tb);
    return 0;
}
