#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<cmath>
#include<queue>
using namespace std;

struct coor {
   int x,y;
} p[105];

int dx[]={1,0,-1,0},dy[]={0,1,0,-1};
int rang[]={700,200,100,50,20};
int intvl[]={140,80,40,20,8};
int vert,vp[705][705],miinter;
vector<int> bestret,ret,edg[105];

timeval sttime;
double timelim=9.8;

void start() {
    gettimeofday(&sttime,NULL);
}

double runtime() {
    timeval tt, nowt;
    gettimeofday(&tt,NULL);
    timersub(&tt,&sttime,&nowt);
    return (nowt.tv_sec*1000+nowt.tv_usec/1000)/1000.0;
    }

int intersect (coor a1,coor a2,coor b1,coor b2) {
    if (min(a1.x,a2.x)>max(b1.x,b2.x)) return 0;
    if (max(a1.x,a2.x)<min(b1.x,b2.x)) return 0;
    if (min(a1.y,a2.y)>max(b1.y,b2.y)) return 0;
    if (max(a1.y,a2.y)<min(b1.y,b2.y)) return 0;
    int A1,A2,B1,B2,C1,C2;
    A1=a2.y-a1.y; B1=a1.x-a2.x; C1=A1*a1.x+B1*a1.y;
    A2=b2.y-b1.y; B2=b1.x-b2.x; C2=A2*b1.x+B2*b1.y;
    int det=A1*B2-A2*B1;
    if (det==0) return 0;
    double x=(B2*C1-B1*C2)*1.0/det;
    double y=(A1*C2-A2*C1)*1.0/det;
    int ok=0;
    if (min(a1.x,a2.x)<=x && x<=max(a1.x,a2.x)) ok++;
    if (min(b1.x,b2.x)<=x && x<=max(b1.x,b2.x)) ok++;
    if (min(a1.y,a2.y)<=y && y<=max(a1.y,a2.y)) ok++;
    if (min(b1.y,b2.y)<=y && y<=max(b1.y,b2.y)) ok++;
    if (ok==4) return 1;
    return 0;
}

class Planarity {
   public:
      int allintersect () {
          int sect=0;
          for (int i=0; i<vert; i++) {
              for (int j=0; j<edg[i].size(); j++)
                  if (edg[i][j]>i) {
                     for (int k=i+1; k<vert; k++)
                         for (int l=0; l<edg[k].size(); l++)
                             if (edg[k][l]>k && k!=i && k!=edg[i][j] && edg[k][l]!=i && edg[k][l]!=edg[i][j]) {
                                sect+=intersect(p[i],p[edg[i][j]],p[k],p[edg[k][l]]);
                                }
                     }
              }
          return sect;
          }
      int movintersect (int tar) {
          int sect=0;
          for (int i=0; i<edg[tar].size(); i++)
              for (int k=0; k<vert; k++)
                  for (int l=0; l<edg[k].size(); l++)
                      if (edg[k][l]>k && k!=tar && k!=edg[tar][i] && edg[k][l]!=tar && edg[k][l]!=edg[tar][i]) {
                         sect+=intersect(p[tar],p[edg[tar][i]],p[k],p[edg[k][l]]);
                         }
          return sect;
          }
      void roughbest (int bp,int prec,int range) {
           int mi=1<<20;
           coor best,at,tc;
           int v[705][705],curi,opr=0; memset(v,0,sizeof(v));
           queue<pair<coor,int> > q;
           vp[p[bp].x][p[bp].y]=0;
           mi=movintersect(bp); best=p[bp];
           int mix,miy,mxx,mxy;
           mix=max(0,p[bp].x-range); miy=max(0,p[bp].y-range);
           mxx=min(699,p[bp].x+range); mxy=min(699,p[bp].y+range);
           for (int i=mix+prec/2; i<=mxx; i+=prec) {
               for (int j=miy+prec/2; j<=mxy; j+=prec) {
                   p[bp].x=i; p[bp].y=j;
                   while (vp[p[bp].x][p[bp].y]!=0 && p[bp].x<700) {
                      p[bp].x++;
                      }
                   int cur=movintersect(bp);
                   if (cur<mi) {
                      mi=cur; best=p[bp];
                      }
                   }
               }
           p[bp]=best;
           vp[p[bp].x][p[bp].y]=1;
           }
      void finebest (int bp) {
           int mi,ori;
           coor best,at,tc;
           int v[705][705],curi,opr=0; memset(v,0,sizeof(v));
           queue<pair<coor,int> > q;
           vp[p[bp].x][p[bp].y]=0;
           ori=movintersect(bp); mi=ori; best=p[bp];
           at=p[bp];
           q.push(make_pair(at,ori));
           while (!q.empty() && opr<400) {
              at=q.front().first;
              curi=q.front().second;
              q.pop();
              opr++;
              if (curi>mi) continue;
              for (int k=0; k<4; k++) {
                  tc=at;
                  tc.x+=dx[k]; tc.y+=dy[k];
                  if (tc.x>=0 && tc.x<700 && tc.y>=0 && tc.y<700 && v[tc.x][tc.y]==0 && vp[tc.x][tc.y]!=1) {
                     p[bp]=tc;
                     v[tc.x][tc.y]=1;
                     int cur=movintersect(bp);
                     if (cur<mi) {
                        mi=cur; best=tc;
                        }
                     if (cur<=ori) {
                        q.push(make_pair(tc,cur));
                        }
                     }
                  }
              }
           p[bp]=best;
           
           vp[p[bp].x][p[bp].y]=1;
           }
      int retry () {
          memset(vp,0,sizeof(vp));
          vector<int> randx;
          for (int i=0; i<vert; i++) randx.push_back(i);
          random_shuffle(randx.begin(),randx.end());
          for (int i=0; i<vert; i++) {
              p[i].x=699*randx[i]/(vert-1); p[i].y=rand()%700;
              vp[p[i].x][p[i].y]=1;
              }
          int cro=allintersect();
          for (int k=0; k<5; k++) {
              for (int i=0; i<vert && runtime()<timelim; i++) {
                  roughbest(i,rand()%(7-k)-(7-k)/2+intvl[k],rang[k]);
                  }
              cro=allintersect();
              }
          ret.clear();
          for (int i=0; i<vert; i++) {
              ret.push_back(p[i].x);
              ret.push_back(p[i].y);
              }
          return allintersect();
          
          }
      vector <int> untangle(int _v, vector <int> Edg) {
          vert=_v;
          start();
          miinter=1<<20;
          for (int i=0; i<Edg.size(); i+=2) {
              edg[Edg[i]].push_back(Edg[i+1]);
              edg[Edg[i+1]].push_back(Edg[i]);
              }
          int tri=0;
          while (runtime()<timelim) {
             int crs=retry();
             if (crs<miinter) {
                miinter=crs;
                bestret=ret;
                }
             }
          fprintf(stderr,"Time: %0.4lf\n",runtime()); fflush(stderr);
          return bestret;
      }
};

