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

int v;
vector<int> ret,edg;

//delete when submit
FILE *debug,*result;

int sttime;
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

class Planarity {
   public:
      vector <int> untangle(int _v, vector <int> _edg) {
          edg=_edg; v=_v;
          for (int i=0; i<v; i++) {
              int a,b;
              a=(i+i%2)*(700/(v+2)); b=(i+1-i%2)*(700/(v+1));
              ret.push_back(a);
              ret.push_back(b);
              fprintf(debug,"(%d,%d)\n",a,b);
              }
          return ret;
      }
};

vector<int> RET;

int main(){
    Planarity PY;
    debug=fopen("D:\\Documents\\Computer\\TopCoder\\Planarity\\debug.txt","w");
    int N,V;
    scanf("%d%d",&V,&N);
    vector<int> EDG;
    EDG.resize(N);
    for (int i=0; i<N; i++) scanf("%d",&EDG[i]);
    RET=PY.untangle(V,EDG);
    for (int i=0; i<RET.size(); i++) {
        printf("%d\n",RET[i]);
        fflush(stdout);
        }
}
