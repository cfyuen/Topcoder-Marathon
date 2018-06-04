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

class MegaParty {
   public:
      vector <double> arrangement(vector <int> A, vector <int> B, double e1, double e2) {
         n=B.size();
         vector<double> ret;
         for (int i=0; i<n; i++) {
             double td=(i/10)*10.01;
             ret.push_back(td);
             td=(double)(i%10)*10.01;
             ret.push_back(td);
             }
         fprintf(debug,"%d\n",n);
         //for (int i=0; i<2*n; i++)
         //    fprintf(debug,"%0.4lf %0.4lf\n",ret[i*2],ret[i*2+1]);
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
