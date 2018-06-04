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

Tile board[15][15],tryboard[15][15];
int n,s,d,nowd,choice[12][12][15][15],base[12][12][15][15];
int numcho[12][12],bascho[12][12]; //tile
int numava[15][15],basava[15][15]; //grid
int dx[]={1,-1,0,0},dy[]={0,0,1,-1};
int elimc[]={0,0,0,0,0,0,1,2,2,2,3};
int elims[]={0,0,0,0,0,0,0,0,1,1,2};
double nowscore;

//delete when submit
//java -jar Visualizer.jar -exec "C:\Documents and Settings\yuch6304\Skrivebord\document\computer\TilesMatching\v0" -seed 1
/*
Log
v0: trivial placing
    all tiles place in first available place
v1: try to eliminate colours
    update grid and tile choice
    score
v2: choose best score to place
    changed score function
    seed 4,15 ko
v2.1: discard remove
      bug fixed
v2.2: scoring changed
v2.3: place remove
v3: scoring changed: potential finish
    change elimination
    new rules for new normal - dead on seed 34 (solved when changes wildcard placement)
    solved 5,6 colours
    bug fixed in addchoice
    solved wildcard placement, solved 7 colours!
v3.1: scoring changed in potential finish
      discard normal change (seed 59,68,81)
v3.2: todo: row/col count added to scoring
      numcho ratio changed (9 colours big board, seed 51+58, solved)
v4: local tester
v4.1: todo: probability of dying
      only count points in top 2/3(2.5) rowa/cols (seed 105 solved)
*/
/*
unsolved: 45 - 7,med,empty
          101- 6,sma,full
          51 - 9,big,empty

*/
FILE *debug,*result;
int debugmsg=0,localtest=0,st=2001,end=3000;

int sttime;
int tcnt=0;
void start() {
    sttime=clock();
}

double runtime() {
    return (clock()-sttime)/1000.0;
}

// ********************* MERSENNE TWISTER (from ploh) **********************

typedef unsigned uint32;

#define MT_N 624
#define MT_M 397
#define MT_MSB 0x80000000U
#define MT_LS31B 0x7FFFFFFFU
#define MT_A 2567483615U

class MersenneTwister { // pseudo-random number generator
  uint32 twistory[MT_N]; // history (i.e., previous states) of the generator
  int pos;
public:
  double pairGaussian;
  MersenneTwister(uint32 seed = 0) {
    twistory[0] = seed;
    for (int i = 1; i < MT_N; i++)
      twistory[i] = 1812433253U * (twistory[i-1] ^ (twistory[i-1]>>30)) + i;
    pos = 0;
  }
  void generate(void) {
    uint32 tmp; int i;
    for (i = 0; i < MT_N-MT_M; i++) {
      tmp = (twistory[i]&MT_MSB) + (twistory[i+1]&MT_LS31B);
      twistory[i] = twistory[i+MT_M] ^ (tmp>>1) ^ (MT_A & -(tmp&1));
    }
    for (; i < MT_N-1; i++) {
      tmp = (twistory[i]&MT_MSB) + (twistory[i+1]&MT_LS31B);
      twistory[i] = twistory[i+MT_M-MT_N] ^ (tmp>>1) ^ (MT_A & -(tmp&1));
    }
    tmp = (twistory[i]&MT_MSB) + (twistory[0]&MT_LS31B);
    twistory[i] = twistory[MT_M-1] ^ (tmp>>1) ^ (MT_A & -(tmp&1));
  }
  uint32 rand_unsigned(void) {
    if (pos == 0) generate();
    uint32 ans = twistory[pos++];
    pos &= -(pos!=624); // if (pos == 624) pos = 0;
    ans ^= ans>>11;
    ans ^= (ans<<7) & 2636928640U;
    ans ^= (ans<<15) & 4022730752U;
    ans ^= ans>>18;
    return ans;
  }
  int rand_signed(void) {
    return rand_unsigned()>>1;
  }

  int nextInt(int n) {
    return rand_unsigned() % n; // close enough
  }
  double nextDouble(void) {
    return (rand_unsigned()+0.5) / 4294967296.0; // close enough... div slow?
  }
  double nextGaussian(void) { // not optimized; throw away a value
    double x, y, r2;
    do {
      x = -1.0 + 2.0 * nextDouble();
      y = -1.0 + 2.0 * nextDouble();
      r2 = x*x + y*y;
    } while (r2 > 1.0);
    double factor = sqrt(-2.0 * log(r2) / r2);
    pairGaussian = y * factor;
    return x * factor;
  }
};

// ****************** END MERSENNE TWISTER ***************************

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
         //fprintf(stderr,"%0.4lf\n",score);
         /*
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
         */
         //fprintf(stderr,"%0.4lf\n",score);
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
                //score+=bassco*pow((s-tom)*1.0/s,4.0);
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
                //score+=bassco*pow((s-tom)*1.0/s,4.0);
                s2=bassco*pow((s-tom)*1.0/s,4.0);
                col.push_back(s2);
                }
             if (debugmsg==1)
                fprintf(stderr,"%0.4lf %0.4lf\n",s1,s2);
             }
         sort(row.begin(),row.end(),revcmp);
         sort(col.begin(),col.end(),revcmp);
         for (int i=0; i<3; i++) {
             score+=row[i];
             score+=col[i];
             }
         score+=row[3]*1.5/3; //2.5
         score+=col[3]*1.5/3;
         //fprintf(stderr,"%0.4lf\n",score);
         
         double okt=n*1.0*n;
         for (int i=0; i<n; i++)
             for (int j=0; j<n; j++) {
                 if (numcho[i][j]==0) okt-=1.0;
                 if (numcho[i][j]==1 && space<=s*s*4/5) okt-=0.1;
                 if (numcho[i][j]==2 && space<=s*s*2/3) okt-=0.01;
                 }
         if (debugmsg==1)
         for (int i=0; i<n; i++) {
             for (int j=0; j<n; j++) 
                 fprintf(stderr,"%d ",numcho[i][j]);
             fprintf(stderr,"\n");
             }
         if (debugmsg==1)
            fprintf(stderr,"%0.4lf %0.4lf\n",okt,pow(okt/(n*1.0*n),((d-nowd)*1.0/d+1)*3.0));
         score*=pow(okt/(n*1.0*n),((d-nowd)*1.0/d+1)*3.0);
         
         if (debugmsg==1)
            fprintf(stderr,"%0.4lf\n",score);
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
         if (debugmsg==1)
         for (int i=0; i<s; i++) {
             for (int j=0; j<s; j++)
                 if (i==x && j==y) fprintf(stderr,"#");
                    else if (board[i][j].c!=-1) fprintf(stderr,"*");
                    else fprintf(stderr,".");
             fprintf(stderr,"\n");
             }
         
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
         /*
         int bestx,besty;
         int bestsco=10000;
         for (int i=0; i<s; i++)
             for (int j=0; j<s; j++)
                 if (valid(tcur,i,j)==1) {
                    if (basava[i][j]<bestsco) {
                       bestsco=basava[i][j];
                       bestx=i; besty=j;
                       }
                    }
         */
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
         //if (debugmsg==1)
         for (int i=0; i<s; i++) {
             for (int j=0; j<s; j++)
                 if (board[i][j].c==-1) fprintf(stderr,".. ");
                    else if (board[i][j].c==10) fprintf(stderr,"** ");
                    else fprintf(stderr,"%d%d ",board[i][j].c,board[i][j].s);
             fprintf(stderr,"\n");
             }
         //if (debugmsg==1)
            cerr << tile << endl;
         if (tile=="W") {
            ret=placeWild();
            }
            else if (tile=="R") {
            ret=placeRemove();
            }
            else ret=placeNormal(tile);
         
         if (debugmsg==1)
         for (int i=0; i<n; i++) {
             for (int j=0; j<n; j++)
                 fprintf(stderr,"%d ",bascho[i][j]);
             fprintf(stderr,"\n");
             }
         /*
         for (int i=0; i<s; i++) {
             for (int j=0; j<s; j++)
                 fprintf(stderr,"%d ",basava[i][j]);
             fprintf(stderr,"\n");
             }
         */
         nowscore=scoring();
         cerr << ret << endl;
         fprintf(stderr,"score: %0.4lf\n\n",nowscore);
         
         fflush(stderr);
         return ret;
      }
};

int main () {
    TilesMatching TM;
    debug=fopen("debug.txt","w");
    if (localtest==0) {
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
    }
    else {
    MersenneTwister r;
    result=fopen("result v4.1-32.txt","w");
    for (int i=st; i<=end; i++) {
        r = MersenneTwister(i);
        int N,S,D;
        N=r.nextInt(7)+4;
        S=r.nextInt(5)+8;
        D=r.nextInt(3)+2;
        TM.init(N,S,D);
        int nt,nd,nm,sc,cc,tc;
        nt=nd=nm=sc=cc=tc=0;
        int maxm=10000;
        int t;
        int bv[15][15];
        memset(bv,0,sizeof(bv));
        string newt;
        printf("Seed = %d\n",i);
        printf("N = %d   D = %d   S = %d\n",N,D,S);
        fflush(stdout);
        while (true) {
           t=r.nextInt(30-N+2);
           if (nt==0 || t==0) newt="W";
              else if (t==1) newt="R";
              else newt=i2s(r.nextInt(N))+i2s(r.nextInt(N));
           string RET=TM.placeTile(newt);
           if (RET=="GIVE UP") break;
           nm++;
           if (nm%1000==0) { printf("move %d\n",nm); fflush(stdout); } 
           if (RET=="DISCARD") {
              nd++;
              if (nm==maxm) break;
              continue;
              }
           int rr,rc;
           sscanf(RET.c_str(),"%d %d",&rr,&rc);
           if (newt=="R") {
              bv[rr][rc]=0;
              nt--;
              }
              else {
              bv[rr][rc]=1;
              int remr,remc;
              remr=remc=1;
              for (int j=0; j<S; j++) {
                  if (bv[rr][j]==0) remr=0;
                  if (bv[j][rc]==0) remc=0;
                  }
              if (remr==1) {
                 for (int j=0; j<S; j++) bv[rr][j]=0;
                 }
              if (remc==1) {
                 for (int j=0; j<S; j++) bv[j][rc]=0;
                 }
              if (remr==1 && remc==1) {
                 nt-=2*S-1;
                 sc+=3*S;
                 tc++;
                 }
                 else if (remr==1 || remc==1) {
                 nt-=S;
                 sc+=S;
                 cc++;
                 }
              }
           sc++;
           if (nm==maxm) break;
           }
        fprintf(result,"Seed = %d\n",i);
        fprintf(result,"N = %d   D = %d   S = %d\n",N,D,S);
        fprintf(result,"Total discard = %d   Total Move = %d\n",nd,nm);
        fprintf(result,"Clear = %d   Together = %d\n",cc,tc);
        fprintf(result,"Score = %d\n\n",sc);
        fflush(result);
        }
    }
    return 0;
}
