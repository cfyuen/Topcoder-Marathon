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

int w,h;
vector<double> ret;

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

class Relief {
   public:
      double measure (int x,int y) {
         printf("?\n%d\n%d\n",x,y);
         fflush(stdout);
         double Mret;
         scanf("%lf",&Mret);
         return Mret;
      }
};
Relief Lib;

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

class ReliefMap {
   public:
      vector <double> getMap(vector <string> contour) {
         w=contour[0].length(); h=contour.size();
         ret.clear(); ret.resize(w*h);
         for (int i=0; i<h; i++)
             fprintf(debug,"%s\n",contour[i].c_str());
         
         double mea=Lib.measure(0,0);
         fprintf(debug,"%0.4lf\n",mea);
         for (int i=0; i<w*h; i++)
             ret[i]=50.0;
         ret[0]=mea;
         return ret;
      }
};

int main () {
    ReliefMap RM;
    debug=fopen("debug.txt","w");
    vector<string> CON;
    int HH;
    scanf("%d",&HH);
    CON.resize(HH);
    for (int i=0; i<HH; i++)
        cin >> CON[i];
    vector<double> RET;
    RET=RM.getMap(CON);
    printf("!\n");
    for (int i=0; i<CON.size()*CON[0].length(); i++)
        cout << RET[i] << endl;
    fflush(stdout);
    return 0;
}
