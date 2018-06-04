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

vector<vector<int> > bd;
int depth[17][17],reach[17][17];
int prev=0,px=0,py=0;
int dx[]={1,0,0,-1,1,1,-1,-1},dy[]={0,1,-1,0,1,-1,1,-1};
int dkx[]={2,2,1,1,-1,-1,-2,-2},dky[]={1,-1,2,-2,2,-2,1,-1};

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

void disbd () {
    for (int i=0; i<bd.size(); i++) {
        for (int j=0; j<bd[0].size(); j++)
            fprintf(debug,"%d ",bd[i][j]);
        fprintf(debug,"\n");
        }
    fprintf(debug,"\n");
    for (int i=0; i<bd.size(); i++) {
        for (int j=0; j<bd[0].size(); j++)
            fprintf(debug,"%d ",reach[i][j]);
        fprintf(debug,"\n");
        }
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

string i2s (int a) {
   char tc[15];
   sprintf(tc,"%d",a);
   string ts=tc;
   return ts;
}

int boardcode (char tile) {
    if (tile>='1' && tile<='4') return tile-'0';
    if (tile=='K') return 5;
    if (tile=='B') return 6;
    if (tile=='R') return 7;
    if (tile=='Q') return 8;
    return 0;
}

void updatereach (int typ,int lx,int ly,int plus) {
   if (typ>=1 && typ<=4) {
      for (int k=0; k<8; k++) {
          int nx=lx+dx[k]*typ,ny=ly+dy[k]*typ;
          if (nx>=0 && nx<bd.size() && ny>=0 && ny<bd[0].size() && bd[nx][ny]!=0) {
             reach[nx][ny]+=plus;
             }
          }
      return;
      }
   if (typ==5) {
      for (int k=0; k<8; k++) {
          int nx=lx+dkx[k],ny=ly+dky[k];
          if (nx>=0 && nx<bd.size() && ny>=0 && ny<bd[0].size() && bd[nx][ny]!=0) {
             reach[nx][ny]+=plus;
             }
          }
      return;
      }
   if (typ>=6 && typ<=8) {
      int fr,to;
      if (typ==6) { fr=4; to=8; }
      if (typ==7) { fr=0; to=4; }
      if (typ==8) { fr=0; to=8; }
      for (int k=fr; k<to; k++) {
          int mul=10000;
          if (dx[k]==-1) mul=min(mul,lx);
          if (dx[k]==1) mul=min(mul,(int)bd.size()-lx-1);
          if (dy[k]==-1) mul=min(mul,ly);
          if (dy[k]==1) mul=min(mul,(int)bd[0].size()-ly-1);
          int nx=lx+dx[k]*mul,ny=ly+dy[k]*mul;
          if (!(lx==nx && ly==ny) && bd[nx][ny]!=0) {
             reach[nx][ny]+=plus;
             }
          }
      return;
      }
   return;
}

pair<int,int> nextmove () {
   int bestx,besty,deepest=-1,leastreach=10000;
   if (prev>=1 && prev<=4) {
      for (int k=0; k<8; k++) {
          int nx=px+dx[k]*prev,ny=py+dy[k]*prev;
          if (nx>=0 && nx<bd.size() && ny>=0 && ny<bd[0].size() && bd[nx][ny]!=0) {
             /*
             if (depth[nx][ny]>deepest) {
                bestx=nx; besty=ny;
                deepest=depth[nx][ny];
                }
             */
             if (reach[nx][ny]-depth[nx][ny]*3<leastreach) {
                leastreach=reach[nx][ny]-depth[nx][ny]*3;
                bestx=nx; besty=ny;
                }
             }
          }
      return make_pair(bestx,besty);
      }
   if (prev==5) {
      for (int k=0; k<8; k++) {
          int nx=px+dkx[k],ny=py+dky[k];
          if (nx>=0 && nx<bd.size() && ny>=0 && ny<bd[0].size() && bd[nx][ny]!=0) {
             /*
             if (depth[nx][ny]>deepest) {
                bestx=nx; besty=ny;
                deepest=depth[nx][ny];
                }
             */
             if (reach[nx][ny]-depth[nx][ny]*3<leastreach) {
                leastreach=reach[nx][ny]-depth[nx][ny]*3;
                bestx=nx; besty=ny;
                }
             }
          }
      return make_pair(bestx,besty);
      }
   if (prev>=6 && prev<=8) {
      int fr,to;
      if (prev==6) { fr=4; to=8; }
      if (prev==7) { fr=0; to=4; }
      if (prev==8) { fr=0; to=8; }
      for (int k=fr; k<to; k++) {
          int mul=10000;
          if (dx[k]==-1) mul=min(mul,px);
          if (dx[k]==1) mul=min(mul,(int)bd.size()-px-1);
          if (dy[k]==-1) mul=min(mul,py);
          if (dy[k]==1) mul=min(mul,(int)bd[0].size()-py-1);
          int nx=px+dx[k]*mul,ny=py+dy[k]*mul;
          if (!(px==nx && py==ny) && bd[nx][ny]!=0) {
             /*
             if (depth[nx][ny]>deepest) {
                bestx=nx; besty=ny;
                deepest=depth[nx][ny];
                }
             */
             if (reach[nx][ny]-depth[nx][ny]*3<leastreach) {
                leastreach=reach[nx][ny]-depth[nx][ny]*3;
                bestx=nx; besty=ny;
                }
             }
          }
      return make_pair(bestx,besty);
      }
   return make_pair(0,0);
}


class ChessPuzzle {
   public:
      string start(int K, vector <string> board) {
          bd.resize(board.size());
          for (int i=0; i<bd.size(); i++) {
              bd[i].resize(board[0].length());
              for (int j=0; j<bd[0].size(); j++) {
                  depth[i][j]=K;
                  bd[i][j]=boardcode(board[i][j]);
                  }
              }
          memset(reach,0,sizeof(reach));
          for (int i=0; i<bd.size(); i++)
              for (int j=0; j<bd[0].size(); j++) {
                  updatereach(bd[i][j],i,j,1);
                  }
                  
          int leastr=10000;
          for (int i=1; i<bd.size()-1; i++)
              for (int j=1; j<bd[0].size()-1; j++)
                  if (reach[i][j]<leastr) {
                     leastr=reach[i][j];
                     px=i; py=j;
                     }
          //px=bd.size()/2; py=bd[0].size()/2;
          prev=bd[px][py];
          string ret=i2s(px)+" "+i2s(py);
          fprintf(debug,"(%s) = %d\n",ret.c_str(),prev);
          disbd();
          fflush(debug);
          return ret;
      }
      string click(string revealed) {
          bd[px][py]=boardcode(revealed[0]);
          depth[px][py]--;
          updatereach(prev,px,py,-1);
          updatereach(bd[px][py],px,py,1);
          pair<int,int> res=nextmove();
          px=res.first; py=res.second;
          prev=bd[px][py];
          string ret=i2s(px)+" "+i2s(py);
          fprintf(debug,"(%s) = %d\n",ret.c_str(),prev);
          disbd();
          fflush(debug);
          return ret;
      }
};

int main(){
    ChessPuzzle CP;
    debug=fopen("D:\\Documents\\Computer\\TopCoder\\ChessPuzzle\\debug.txt","w");
    int K,R;
    scanf("%d%d",&K,&R);
    vector<string> BD;
    BD.resize(R);
    for (int i=0; i<R; i++)
        cin >> BD[i];
    cout << CP.start(K,BD) << endl;
    fflush(stdout);
    while (true) {
        string Rev;
        cin >> Rev;
        cout << CP.click(Rev) << endl;
        fflush(stdout);
        }
    
}
