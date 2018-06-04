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

Tile board[15][15],tryboard[15][15];
int n,s,d,nowd,choice[12][12][15][15],base[12][12][15][15];
int numcho[12][12],bascho[12][12]; //tile
int numava[15][15],basava[15][15]; //grid
int dx[]={1,-1,0,0},dy[]={0,0,1,-1};
int elimc[]={0,0,0,0,0,0,1,2,2,2,3};
int elims[]={0,0,0,0,0,0,0,0,1,1,2};
double nowscore;

string i2s (int a) {
   char str[10];
   sprintf(str,"%d",a);
   return str;
}

bool revcmp (double a,double b) {
   return a>b;
}

class TilesMatching {
   public:
      double scoring () {
         double score=0;
         int space=0;
         for (int i=0; i<s; i++)
             for (int j=0; j<s; j++)
                 if (board[i][j].c==-1) space++;
         if (space<=s*s*5/6) score=10000*pow((space)*1.0/(s*s*5/6),3.0);
            else score=10000;
         vector<double> row,col;
         for (int i=0; i<s; i++) {
             int tom=0,bassco=1000;
             double s1,s2;
             for (int j=0; j<s; j++)
                 if (board[i][j].c==-1) {
                    tom++;
                    int cont=0;
                    for (int k=0; k<4; k++) {
                        int nx=i+dx[k],ny=j+dy[k];
                        if (nx>=0 && nx<s && ny>=0 && ny<s && board[nx][ny].c!=-1) cont=1;
                        }
                    if (cont==0) continue;
                    if (numava[i][j]==0) bassco-=300;
                    if (numava[i][j]==1) bassco-=200;
                    if (numava[i][j]==2) bassco-=100;
                    if (numava[i][j]==n) bassco-=50;
                    if (numava[i][j]==2*n-1) bassco-=20;
                    }
             if (tom<s+1) {
                s1=bassco*pow((s-tom)*1.0/s,4.0);
                row.push_back(s1);
                }
             tom=0; bassco=1000;
             for (int j=0; j<s; j++)
                 if (board[j][i].c==-1) {
                    tom++;
                    int cont=0;
                    for (int k=0; k<4; k++) {
                        int nx=j+dx[k],ny=i+dy[k];
                        if (nx>=0 && nx<s && ny>=0 && ny<s && board[nx][ny].c!=-1) cont=1;
                        }
                    if (cont==0) continue;
                    if (numava[j][i]==0) bassco-=300;
                    if (numava[j][i]==1) bassco-=200;
                    if (numava[j][i]==2) bassco-=100;
                    if (numava[j][i]==n) bassco-=50;
                    if (numava[j][i]==2*n-1) bassco-=20;
                    }
             if (tom<s+1) {
                s2=bassco*pow((s-tom)*1.0/s,4.0);
                col.push_back(s2);
                }
             }
         sort(row.begin(),row.end(),revcmp);
         sort(col.begin(),col.end(),revcmp);
         for (int i=0; i<2; i++) {
             score+=row[i];
             score+=col[i];
             }
         score+=row[2]/2; //2.5
         score+=col[2]/2;
         
         double okt=n*1.0*n;
         for (int i=0; i<n; i++)
             for (int j=0; j<n; j++) {
                 if (numcho[i][j]==0) okt-=1.0;
                 if (numcho[i][j]==1 && space<=s*s*4/5) okt-=0.1;
                 if (numcho[i][j]==2 && space<=s*s*2/3) okt-=0.01;
                 }

         score*=pow(okt/(n*1.0*n),((d-nowd)*1.0/d+1)*3.0);
         return score;
         }
      void addchoice (int x,int y) {
         for (int i=0; i<5; i++) {
             int nx,ny;
             if (i==4) { nx=x; ny=y; }
                else { nx=x+dx[i]; ny=y+dy[i]; }
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
      double tryscore (Tile t,int x,int y) {
         memcpy(tryboard,board,sizeof(board));
         memcpy(choice,base,sizeof(choice));
         memcpy(numcho,bascho,sizeof(numcho));
         memcpy(numava,basava,sizeof(numava));
         board[x][y].c=t.c; board[x][y].s=t.s;
         
         addchoice(x,y);
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
         double ret=scoring();
         memcpy(board,tryboard,sizeof(board));
         return ret;
         }
      string phase (Tile t,int x,int y) {
         string ret="";
         ret+=i2s(x)+" "+i2s(y);
         nowd=min(d,nowd+1);
         memcpy(choice,base,sizeof(choice));
         memcpy(numcho,bascho,sizeof(numcho));
         memcpy(numava,basava,sizeof(numava));
         board[x][y].c=t.c; board[x][y].s=t.s;
         addchoice(x,y);
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
         memcpy(base,choice,sizeof(choice));
         memcpy(bascho,numcho,sizeof(numcho));
         memcpy(basava,numava,sizeof(numava));
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
         memset(base,0,sizeof(base));
         memset(numcho,0,sizeof(numcho));
         memset(bascho,0,sizeof(bascho));
         memset(numava,0,sizeof(numava));
         memset(basava,0,sizeof(basava));
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
         
         int bestx,besty;
         double bestsco=-1e10;
         for (int i=0; i<s; i++)
             for (int j=0; j<s; j++)
                 if (valid(tcur,i,j)==1) {
                    double sco=tryscore(tcur,i,j);
                    if (sco>bestsco) {
                       bestsco=sco;
                       bestx=i; besty=j;
                       }
                    }
         return phase(tcur,bestx,besty);
         }
      string placeRemove () {
         Tile tcur;
         tcur.c=-1; tcur.s=-1;
         
         int bestx=-1,besty=-1;
         double bestsco=scoring();
         for (int i=0; i<s; i++)
             for (int j=0; j<s; j++)
                 if (board[i][j].c!=-1 && board[i][j].c!=10) {
                    double sco=tryscore(tcur,i,j);
                    if (sco>bestsco) {
                       bestsco=sco;
                       bestx=i; besty=j;
                       }
                    }
                    
         if (bestx!=-1) return phase(tcur,bestx,besty);
         if (nowd>0) {
            nowd--;
            return "DISCARD";
            }
         for (int i=0; i<s; i++)
             for (int j=0; j<s; j++)
                 if (board[i][j].c!=-1)
                    return phase(tcur,i,j);
         }
      string placeNormal (string tile) {
         Tile tcur;
         tcur.c=tile[0]-'0'; tcur.s=tile[1]-'0';
         
         
         int bestx,besty;
         double bestsco=-1e10;
         for (int i=0; i<s; i++)
             for (int j=0; j<s; j++)
                 if (valid(tcur,i,j)==1) {
                    double sco=tryscore(tcur,i,j);
                    if (sco>bestsco) {
                       bestsco=sco;
                       bestx=i; besty=j;
                       }
                    }
         if (bestsco>nowscore)
            return phase(tcur,bestx,besty);
         if (tcur.c<elimc[n] && nowd>0) {
            nowd--;
            return "DISCARD";
            }
         if (tcur.s<elims[n] && nowd>0) {
            nowd--;
            return "DISCARD";
            }
         if (bestsco>1000 || (bestsco>1e-9 && nowd==0) || bestsco>nowscore) {
            return phase(tcur,bestx,besty);
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
         nowscore=scoring();
         return ret;
      }
};
