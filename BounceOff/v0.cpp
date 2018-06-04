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

//delete when submit
FILE *debug,*result;

int sttime;
int tcnt=0;
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

class BounceOff {
   public:
      vector <string> placeObstacles(vector <int> oX, vector <int> oY, int R) {
         return ret;
      }
};

int main () {
    BounceOff BO;
    debug=fopen("debug.txt","w");
    int N,RR;
    vector<int> OBX,OBY;
    scanf("%d",&N);
    OBX.resize(N); OBY.resize(N);
    for (int i=0; i<N; i++)
        cin >> OBX[i];
    for (int i=0; i<N; i++)
        cin >> OBY[i];
    scanf("%d",&RR);
    vector<string> RET;
    RET=BO.placeObstacles(OBX,OBY,RR);
    printf("%d\n",RET.size());
    for (int i=0; i<RET.size(); i++)
        cout << RET[i] << endl;
    fflush(stdout);
    return 0;
}
