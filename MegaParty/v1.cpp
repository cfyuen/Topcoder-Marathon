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

FILE *debug;

int n;
vector<int> cntf,cnte,ppl,A,B;
vector<double> ret;
int dx[]={1,0,-1,0},dy[]={0,1,0,-1};

bool cmp (int a,int b) {
   int f1=(cntf[a]-cnte[a])*B[a],f2=(cntf[b]-cnte[b])*B[b];
   if (f1!=f2) return f1>f2;
   return cntf[a]>cntf[b];
}

class MegaParty {
   public:
      vector <double> arrangement(vector <int> vA, vector <int> vB, double e1, double e2) {
         A=vA; B=vB;
         n=B.size();
         ret.clear(); cntf.clear(); cnte.clear();
         ret.resize(2*n);
         cntf.resize(n); cnte.resize(n); ppl.resize(n);
         for (int i=0; i<n; i++)
             ppl[i]=i;
         for (int i=0; i<n; i++)
             for (int j=0; j<n; j++)
                 if (A[i*n+j]==1) cntf[i]++;
                    else if (A[i*n+j]==-1) cnte[i]++;
         sort(ppl.begin(),ppl.end(),cmp);
         
         int lvl=1,dir=0,now=0;
         ret[ppl[0]*2]=0; ret[ppl[0]*2+1]=0;
         for (int i=1; i<n; i++) {
             ret[ppl[i]*2]=ret[ppl[i-1]*2]+dx[dir]*10.0001;
             ret[ppl[i]*2+1]=ret[ppl[i-1]*2+1]+dy[dir]*10.0001;
             //fprintf(debug,"%d %d %d\n",lvl,dir,now);
             now++;
             if (now==lvl) {
                dir=(dir+1)%4;
                if (dir==0 || dir==2) {
                   lvl++; 
                   }
                now=0;
                }
             }
         fprintf(debug,"%d\n",n);
         for (int i=0; i<n; i++)
             fprintf(debug,"%0.4lf %0.4lf   pts:%d\n",ret[i*2],ret[i*2+1],(cntf[i]-cnte[i])*B[i]);
         for (int i=0; i<n; i++)
             fprintf(debug,"%d\n",(cntf[ppl[i]]-cnte[ppl[i]])*B[ppl[i]]);
         return ret;
      }
};

int main () {
    MegaParty MP;
    debug=fopen("debug.txt","w");
    vector<int> VA,VB;
    double E1,E2;
    int NUM;
    scanf("%d",&NUM);
    VA.resize(NUM);
    for (int i=0; i<NUM; i++)
        scanf("%d",&VA[i]);
    scanf("%d",&NUM);
    VB.resize(NUM);
    for (int i=0; i<NUM; i++)
        scanf("%d",&VB[i]);
    scanf("%lf %lf",&E1,&E2);
    vector<double> RET;
    RET=MP.arrangement(VA,VB,E1,E2);
    for (int i=0; i<2*NUM; i++)
        printf("%0.4lf\n",RET[i]);
    fflush(stdout);
    return 0;
}
