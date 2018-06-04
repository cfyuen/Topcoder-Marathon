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
int cnt=0,awin=0,bwin=0,draw=0;
int casa[20][20][20],casb[20][20][20],cascnt[20][20][20];

int main () {
    a=fopen("result v4.3.txt","r");
    b=fopen("result v5.2.txt","r");
    out=fopen("compare 4.3,5.2.txt","w");
    char ts[100];
    memset(casa,0,sizeof(casa)); memset(casb,0,sizeof(casb)); memset(cascnt,0,sizeof(cascnt));
    while (fscanf(a,"%s",ts)==1 && fscanf(b,"%s",ts)==1) {
        int da,db;
        char sa[1000],sb[1000],ts1[100],ts2[100];
        fscanf(a,"%s %s",ts1,sa);
        fscanf(b,"%s %s",ts1,sb);
        sscanf(sa,"%d",&da);
        sscanf(sb,"%d",&db);
        fprintf(out,"seed = %d\n",da);
        int n,d,s;
        char sn[10],sd[10],ss[10];
        fscanf(a,"%s %s %s %s %s %s %s %s %s",ts,ts1,sn,ts2,sa,sd,sb,ts,ss);
        fscanf(b,"%s %s %s %s %s %s %s %s %s",ts,ts1,sn,ts2,sa,sd,sb,ts,ss);
        sscanf(sn,"%d",&n); sscanf(sd,"%d",&d); sscanf(ss,"%d",&s);
        cascnt[n][d][s]++;
        fscanf(a,"%s %s %s %s %s %s %s %s",ts,ts1,sn,ts2,sa,sd,sb,ts);
        fscanf(b,"%s %s %s %s %s %s %s %s",ts,ts1,sn,ts2,sa,sd,sb,ts);
        fscanf(a,"%s %s %s %s %s %s",ts,ts1,sn,ts2,sa,sd);
        fscanf(b,"%s %s %s %s %s %s",ts,ts1,sn,ts2,sa,sd);
        fscanf(a,"%s %s %s",ts,ts1,sa);
        fscanf(b,"%s %s %s",ts,ts1,sb);
        sscanf(sa,"%d",&da);
        sscanf(sb,"%d",&db);
        casa[n][d][s]+=da;
        casb[n][d][s]+=db;
        fprintf(out,"%d %d\n",da,db);
        if (da>db) awin++;
        if (db>da) bwin++;
        if (da==db) draw++;
        }
    fprintf(out,"\n");
    fprintf(out,"a: %d\nb: %d\ndraw: %d\n\n",awin,bwin,draw);;
    for (int i=4; i<=10; i++) {
        for (int j=2; j<=4; j++)
            for (int k=8; k<=12; k++) {
                fprintf(out,"N:%d D:%d S:%d   cas:%d\n",i,j,k,cascnt[i][j][k]);
                fprintf(out,"%0.4lf   %0.4lf\n",casa[i][j][k]*1.0/cascnt[i][j][k],casb[i][j][k]*1.0/cascnt[i][j][k]);
                }
        fprintf(out,"\n");
        }
    return 0;
}
