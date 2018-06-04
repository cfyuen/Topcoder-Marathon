#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<cmath>
#include<windows.h>
using namespace std;

struct Tile {
   int c,s;
};

Tile board[15][15];
int n,s,d,nowd,choice[12][12][15][15],base[12][12][15][15];
int numcho[12][12],bascho[12][12]; //tile
int numava[15][15],basava[15][15]; //grid
int dx[]={1,-1,0,0},dy[]={0,0,1,-1};
int elim[]={0,0,0,0,1,1,1,1,2,2,2};

//delete when submit
//java -jar Visualizer.jar -exec "C:\Documents and Settings\yuch6304\Skrivebord\document\computer\TilesMatching\v0" -seed 1
/*
Log
v0: trivial placing
    all tiles place in first available place
v1: try to eliminate colours
    update grid and tile choice
    score
*/
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
      double scoring () {
         double score=0;
         int space=0;
         for (int i=0; i<s; i++)
             for (int j=0; j<s; j++)
                 if (board[i][j].c==-1) space++;
         if (space<=s*s*2/3) score=10000*pow((space)*1.0/(s*s*2/3),3.0);
            else score=10000;
         //fprintf(stderr,"%0.4lf\n",score);
         for (int i=0; i<s; i++)
             for (int j=0; j<s; j++) {
                 int empty=1;
                 for (int k=0; k<4; k++) {
                     int nx=i+dx[k],ny=j+dy[k];
                     if (nx>=0 && nx<s && ny>=0 && ny<s && board[nx][ny].c!=-1) empty=0;
                     }
                 if (empty==1) continue;
                 if (numava[i][j]==0) score-=n*n*3;
                 if (numava[i][j]==1) score-=n*n;
                 if (numava[i][j]==2) score-=n*n/2.0;
                 if (numava[i][j]==n) score-=3;
                 if (numava[i][j]==2*n-1) score-=1;
                 }
         //fprintf(stderr,"%0.4lf\n",score);
         for (int i=0; i<s; i++) {
             int tom=0;
             for (int j=0; j<s; j++)
                 if (board[i][j].c==-1) tom++;
             if (tom<n/2) score+=10000*pow((n-tom)*1.0/n,10.0);
             tom=0;
             for (int j=0; j<s; j++)
                 if (board[j][i].c==-1) tom++;
             if (tom<n/2) score+=10000*pow((n-tom)*1.0/n,10.0);
             }
         //fprintf(stderr,"%0.4lf\n",score);
         double okt=n*1.0*n;
         for (int i=0; i<n; i++)
             for (int j=0; j<n; j++) {
                 if (numcho[i][j]==0) okt-=1.0;
                 if (numcho[i][j]==1) okt-=0.5;
                 if (numcho[i][j]==2) okt-=0.2;
                 }
         //fprintf(stderr,"%0.4lf %0.4lf\n",okt,pow(okt/(n*1.0*n),(d-nowd+1)*1.0));
         score*=pow(okt/(n*1.0*n),(d-nowd+1)*1.0);
         fprintf(stderr,"%0.4lf\n",score);
         return score;
         }
      void addchoice (int x,int y) {
         for (int i=0; i<5; i++) {
             int nx,ny;
             if (i==4) { nx=x; ny=y; }
             nx=x+dx[i]; ny=y+dy[i];
             if (board[nx][ny].c!=-1) {
                if (i==4) {
                   for (int k=0; k<n; k++)
                       for (int l=0; l<n; l++)
                           if (choice[k][l][nx][ny]==1) {
                              choice[k][l][nx][ny]=0;
                              numcho[k][l]--;
                              }
                   numava[nx][ny]=0;
                   }
                continue;
                }
             int empty=1;
             for (int j=0; j<4; j++) {
                 int ox=nx+dx[j],oy=ny+dy[j];
                 if (ox>=0 && ox<s && oy>=0 && oy<s && board[ox][oy].c!=-1) empty=0;
                 }
             if (empty==1) {
                for (int k=0; k<n; k++)
                    for (int l=0; l<n; l++)
                        if (choice[k][l][nx][ny]==1) {
                           choice[k][l][nx][ny]=0;
                           numcho[k][l]--;
                           }
                numava[nx][ny]=0;
                continue;
                }
             for (int k=0; k<n; k++)
                 for (int l=0; l<n; l++)
                     if (choice[k][l][nx][ny]==0) {
                        choice[k][l][nx][ny]=1;
                        numcho[k][l]++;
                        }
             numava[nx][ny]=n*n;
             for (int j=0; j<4; j++) {
                 int ox=nx+dx[j],oy=ny+dy[j];
                 if (ox>=0 && ox<s && oy>=0 && oy<s && board[ox][oy].c!=-1 && board[ox][oy].c!=10) {
                    for (int k=0; k<n; k++)
                        for (int l=0; l<n; l++)
                            if (k!=board[ox][oy].c && l!=board[ox][oy].s && choice[k][l][nx][ny]==1) {
                               choice[k][l][nx][ny]=0;
                               numcho[k][l]--;
                               numava[nx][ny]--;
                               }
                    }
                 }
             }
         return;
         }
      string phase (Tile t,int x,int y) {
         string ret="";
         ret+=i2s(x)+" "+i2s(y);
         nowd=min(d,nowd+1);
         board[x][y].c=t.c; board[x][y].s=t.s;
         addchoice(x,y);
         for (int k=0; k<n; k++)
             for (int l=0; l<n; l++)
                 if (choice[k][l][x][y]==1) {
                    choice[k][l][x][y]=0;
                    numcho[k][l]--;
                    }
         int remvx=1,remvy=1;
         for (int i=0; i<s; i++) {
             if (board[x][i].c==-1) remvx=0;
             if (board[i][y].c==-1) remvy=0;
             }
         if (remvx==1) {
            for (int i=0; i<s; i++) {
                board[x][i].c=-1; board[x][i].s=-1;
                }
            for (int i=0; i<s; i++)
                addchoice(x,i);
            }
         if (remvy==1) {
            for (int i=0; i<s; i++) {
                board[i][y].c=-1; board[i][y].s=-1;
                }
            for (int i=0; i<s; i++)
                addchoice(i,y);
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
         memset(choice,0,sizeof(choice));
         memset(numcho,0,sizeof(numcho));
         memset(numava,0,sizeof(numava));
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
         
         if (tcur.c<elim[n] && nowd>0) {
            nowd--;
            return "DISCARD";
            }
         
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
         for (int i=0; i<n; i++) {
             for (int j=0; j<n; j++)
                 fprintf(stderr,"%d ",numcho[i][j]);
             fprintf(stderr,"\n");
             }
         for (int i=0; i<s; i++) {
             for (int j=0; j<s; j++)
                 fprintf(stderr,"%d ",numava[i][j]);
             fprintf(stderr,"\n");
             }
         fprintf(stderr,"score: %0.4lf\n",scoring());
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
    Sleep(2000);
    return 0;
}
