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

vector<string> ret;
int consum[10005];
double P,C,rcost[10005];
vector<double> bevent[10005],wevent[10005];

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

class SpaceMedkit {
   public:
      void parseResource (vector<string> ar) {
         printf("**parse resource\n");
         char par1[1000],par2[1000],par3[1000],par4[1000],tc;
         int rid;
         double m,v;
         for (int i=0; i<ar.size(); i++) {
             sscanf(ar[i].c_str(),"%s%s%s%s",par1,par2,par3,par4);
             sscanf(par1,"%c%d",&tc,&rid);
             sscanf(par2,"%d",&consum[rid]);
             sscanf(par3,"%lf",&m);
             sscanf(par4,"%lf",&v);
             rcost[rid]=m+C*v;
             }
         }
      void parseEvent (vector<string> rr) {
           printf("**parse event\n");
           int ptr=0,prev=-1,rid,mid;
           double tmp[10005];
           char par1[1000],par2[1000],par3[1000],par4[1000],tc;
           for (int i=0; i<rr.size(); i++) {
               sscanf(rr[i].c_str(),"%s%s%s%s",par1,par2,par3,par4);
               sscanf(par1,"%c%c%d",&tc,&tc,&mid);
               sscanf(par2,"%c%d",&tc,&rid);
               //fprintf(debug,"%s %s\n",par1,par2);
               //fprintf(debug,"%d %d\n",Mid,Rid);
               if (mid!=prev && prev!=-1) {
                  bevent[prev].resize(ptr/3*2);
                  wevent[prev].resize(ptr/3*2);
                  for (int j=0; j<ptr; j+=3) {
                      bevent[prev][j/3*2]=tmp[j];
                      wevent[prev][j/3*2]=tmp[j];
                      bevent[prev][j/3*2+1]=tmp[j+1];
                      wevent[prev][j/3*2+1]=tmp[j+2];
                      }
                  ptr=0;
                  /*
                  for (int j=0; j<Best[prev].size(); j++)
                      fprintf(debug,"%d: [%d]%0.4lf\n",prev,j,Best[prev][j]); */
                  //printf("%d\n",prev); fflush(stdout);
                  }
               prev=mid;
               tmp[ptr]=rid; ptr++; 
               sscanf(par3,"%lf",&tmp[ptr]); ptr++;
               sscanf(par4,"%lf",&tmp[ptr]); ptr++;
               }
           bevent[prev].resize(ptr/3*2);
           wevent[prev].resize(ptr/3*2);
           for (int j=0; j<ptr; j+=3) {
               bevent[prev][j/3*2]=tmp[j];
               wevent[prev][j/3*2]=tmp[j];
               bevent[prev][j/3*2+1]=tmp[j+1];
               wevent[prev][j/3*2+1]=tmp[j+2];
               }
           }
      vector <string> getMedkit(vector <string> ar, vector <string> rr, vector <string> mis, double _P, double _C) {
         start();
         P=_P; C=_C;
         memset(consum,-1,sizeof(consum));
         //fprintf(debug,"%0.4lf %0.4lf\n",P,C);
         //ret.push_back("R7274 30");
         //fprintf(debug,"%d %d %d %0.4lf %0.4lf\n",ar.size(),rr.size(),mis.size(),P,C);
         //fprintf(debug,"%s\n%s\n%s\n",ar[0].c_str(),rr[0].c_str(),mis[0].c_str());
         parseResource(ar);
         parseEvent(rr);
         printf("%0.3lf\n",runtime());
         for (int i=0; i<=10000; i++) {
             if (consum[i]==1) {
                string s;
                char str[1000];
                sprintf(str,"R%d %d\0",i,500);
                s=str;
                ret.push_back(s);
                }
             if (consum[i]==0) {
                string s;
                char str[1000];
                sprintf(str,"R%d %d\0",i,2);
                s=str;
                ret.push_back(s);
                }
             }
         /*
         for (int i=0; i<ret.size(); i++)
             fprintf(debug,"%s\n",ret[i].c_str());
         */
         return ret;
      }
};

