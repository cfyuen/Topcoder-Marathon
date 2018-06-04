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

vector<string> grid,ret;
int n,rd,dn,dm,best=0;
string rule;
int dx[]={1,1,1,0,0,-1,-1,-1},dy[]={1,0,-1,1,-1,1,0,-1};


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

class CellularAutomaton {
   public:
      void vis (vector<string> visg) {
          for (int i=0; i<dn; i++) {
              for (int j=0; j<dm; j++)
                  if (visg[i][j]=='1') fprintf(debug,"#");
                     else fprintf(debug,".");
              fprintf(debug,"\n");
              }
          fprintf(debug,"\n");
          }
      int stimulate (vector<string> nowg) {
          int neigh=0;
          vector<string> newg=nowg;
          //fprintf(debug,"Now:\n"); vis(nowg);
          for (int i=0; i<rd; i++) {
              for (int j=0; j<dn; j++) {
                  for (int k=0; k<dm; k++) {
                      neigh=0;
                      for (int d=0; d<8; d++)
                          if (nowg[(dx[d]+j+dn)%dn][(dy[d]+k+dm)%dm]=='1') neigh++;
                      if (rule[neigh]=='-') newg[j][k]='0';
                      if (rule[neigh]=='+') newg[j][k]='1';
                      if (rule[neigh]=='X') newg[j][k]=1-(nowg[j][k]-'0')+'0';
                      }
                  }
              nowg=newg;
              //vis(nowg);
              }
          int live=0;
          for (int i=0; i<dn; i++)
              for (int j=0; j<dm; j++)
                  if (nowg[i][j]=='1') live++;
          //fprintf(debug,"Final:\n"); vis(nowg);
          fprintf(debug,"Live: %d\n",live);
          return live;
          }
  	  vector <string> configure(vector <string> _grid, string rules, int N, int K) {
          n=N; rd=K; rule=rules; grid=_grid; dn=grid.size(); dm=grid[0].length();
          ret=grid;
          vector<string> testg=grid;
          int score=0;
          for (int i=0; i<100; i++) {
              testg=grid;
              for (int j=0; j<n; j++) {
                  int xx,yy;
                  xx=rand()%dn; yy=rand()%dm;
                  testg[xx][yy]=1-(testg[xx][yy]-'0')+'0';
                  }
              score=stimulate(testg);
              if (score>best) {
                 best=score;
                 ret=testg;
                 }
              }    
          return ret;
      }
};

vector<string> RET;

int main(){
    CellularAutomaton CA;
    debug=fopen("D:\\Documents\\Computer\\TopCoder\\CellularAutomaton\\debug.txt","w");
    int RR,NN,KK;
    scanf("%d",&RR);
    vector<string> Grid;
    string Rule;
    Grid.resize(RR);
    for (int i=0; i<RR; i++) cin >> Grid[i];
    cin >> Rule;
    scanf("%d%d",&NN,&KK); 
    RET=CA.configure(Grid,Rule,NN,KK);
    for (int i=0; i<RET.size(); i++) {
        printf("%s\n",RET[i].c_str());
        fflush(stdout);
        }
}
