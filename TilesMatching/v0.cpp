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

struct Tile {
   int c,s;
};

Tile board[15][15];
int n,s,d,nowd;
int dx[]={1,-1,0,0},dy[]={0,0,1,-1};

//delete when submit
//java -jar Visualizer.jar -exec "C:\Documents and Settings\yuch6304\Skrivebord\document\computer\TilesMatching\v0" -seed 1
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

class TilesMatching {
   public:
      string i2s (int a) {
         char str[10];
         sprintf(str,"%d",a);
         return str;
         }
      string phase (Tile t,int x,int y) {
         string ret="";
         ret+=i2s(x)+" "+i2s(y);
         nowd=min(d,nowd+1);
         board[x][y].c=t.c; board[x][y].s=t.s;
         int remvx=1,remvy=1;
         for (int i=0; i<s; i++) {
             if (board[x][i].c==-1) remvx=0;
             if (board[i][y].c==-1) remvy=0;
             }
         if (remvx==1)
            for (int i=0; i<s; i++) {
                board[x][i].c=-1; board[x][i].s=-1;
                }
         if (remvy==1)
            for (int i=0; i<s; i++) {
                board[i][y].c=-1; board[i][y].s=-1;
                }
         return ret;
         }
      int valid (Tile t,int x,int y) {
         int ok=0;
         if (board[x][y].c!=-1) return 0;
         for (int i=0; i<4; i++) {
             int nx=x+dx[i],ny=y+dy[i];
             if (nx>=0 && nx<s && ny>=0 && ny<s && board[nx][ny].c!=-1) {
                ok=1;
                if (board[nx][ny].c!=10 && t.c!=10)
                   if (t.c!=board[nx][ny].c && t.s!=board[nx][ny].s) return 0;
                }
             }
         return ok;
         }
  	  int init (int _n, int _s, int _d) {
         n=_n; s=_s; d=_d;
         nowd=d;
         for (int i=0; i<s; i++)
             for (int j=0; j<s; j++) {
                 board[i][j].c=-1; board[i][j].s=-1;
                 }
         return 0;
         }
      string placeWild () {
         Tile tcur;
         tcur.c=10; tcur.s=10;
         int empty=1;
         for (int i=0; i<s; i++)
             for (int j=0; j<s; j++)
                 if (board[i][j].c!=-1) {
                    empty=0;
                    }
         if (empty==1) return phase(tcur,s/2,s/2);
         for (int i=0; i<s; i++)
             for (int j=0; j<s; j++)
                 if (valid(tcur,i,j)==1) {
                    return phase(tcur,i,j);
                    }
         }
      string placeRemove () {
         Tile tcur;
         tcur.c=-1; tcur.s=-1;
         for (int i=0; i<s; i++)
             for (int j=0; j<s; j++)
                 if (board[i][j].c!=-1 && board[i][j].c!=10)
                    return phase(tcur,i,j);
         for (int i=0; i<s; i++)
             for (int j=0; j<s; j++)
                 if (board[i][j].c!=-1)
                    return phase(tcur,i,j);
         }
      string placeNormal (string tile) {
         Tile tcur;
         tcur.c=tile[0]-'0'; tcur.s=tile[1]-'0';
         for (int i=0; i<s; i++)
             for (int j=0; j<s; j++)
                 if (valid(tcur,i,j)==1) {
                    return phase(tcur,i,j);
                    }
         if (nowd>0) {
            nowd--;
            return "DISCARD";
            }
         return "GIVE UP";
         }
      string placeTile(string tile) {
         string ret;
         if (tile=="W") {
            ret=placeWild();
            }
            else if (tile=="R") {
            ret=placeRemove();
            }
            else ret=placeNormal(tile);
         cerr << ret << endl;
         fflush(stderr);
         return ret;
      }
};

int main () {
    TilesMatching TM;
    debug=fopen("debug.txt","w");
    int N,S,D;
    scanf("%d %d %d",&N,&S,&D);
    TM.init(N,S,D);
    while (true) {
       string STR;
       cin >> STR;
       STR=TM.placeTile(STR);
       cout << STR << endl;
       fflush(stdout);
       }
    return 0;
}
