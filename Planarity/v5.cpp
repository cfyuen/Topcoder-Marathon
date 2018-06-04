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
int layer=7;
int rang[]={700,300,120,70,40,20,8};
int intvl[]={250,120,60,40,28,15,5};
int vert,vp[705][705],miinter,tried[105];
vector<int> bestret,ret,edg[105];
vector<pair<int,int> > edgsz;

//delete when submit
FILE *debug,*result;
double timelim=30;

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
*/

bool pairrev (pair<int,int> a,pair<int,int> b) {
     if (a.first==b.first) return a.second<b.second;
     return a.first>b.first;
}

int A1,A2,B1,B2,C1,C2,det,xx,yy;

int intersect (coor a1,coor a2,coor b1,coor b2) {
    if (min(a1.x,a2.x)>max(b1.x,b2.x)) return 0;
    if (max(a1.x,a2.x)<min(b1.x,b2.x)) return 0;
    if (min(a1.y,a2.y)>max(b1.y,b2.y)) return 0;
    if (max(a1.y,a2.y)<min(b1.y,b2.y)) return 0;
    A1=a2.y-a1.y; B1=a1.x-a2.x; C1=A1*a1.x+B1*a1.y;
    A2=b2.y-b1.y; B2=b1.x-b2.x; C2=A2*b1.x+B2*b1.y;
    det=A1*B2-A2*B1;
    if (det==0) return 0;
    xx=(B2*C1-B1*C2)/det;
    yy=(A1*C2-A2*C1)/det;
    int ok=0;
    if ((min(a1.x,a2.x)<=xx && xx<max(a1.x,a2.x)) || (xx==max(a1.x,a2.x) && (B2*C1-B1*C2)%det==0)) ok++;
    if ((min(b1.x,b2.x)<=xx && xx<max(b1.x,b2.x)) || (xx==max(b1.x,b2.x) && (B2*C1-B1*C2)%det==0)) ok++;
    if ((min(a1.y,a2.y)<=yy && yy<max(a1.y,a2.y)) || (yy==max(a1.y,a2.y) && (A1*C2-A2*C1)%det==0)) ok++;
    if ((min(b1.y,b2.y)<=yy && yy<max(b1.y,b2.y)) || (yy==max(b1.y,b2.y) && (A1*C2-A2*C1)%det==0)) ok++;
    if (ok==4) return 1;
    return 0;
}

class Planarity {
   public:
      int oldallintersect () {
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
      int allintersect () {
          int sect=0;
          for (int i=0; i<vert; i++) 
              if (tried[i]!=0) {
                 for (int j=0; j<edg[i].size(); j++)
                     if (tried[edg[i][j]]!=0 && edg[i][j]>i) {
                        for (int k=i+1; k<vert; k++)
                            if (tried[k]!=0)
                               for (int l=0; l<edg[k].size(); l++)
                                   if (tried[edg[k][l]]!=0 && edg[k][l]>k && k!=i && k!=edg[i][j] && edg[k][l]!=i && edg[k][l]!=edg[i][j]) {
                                      sect+=intersect(p[i],p[edg[i][j]],p[k],p[edg[k][l]]);
                                      }
                        }
                 }
          return sect;
          }
      int oldmovintersect (int tar) {
          int sect=0;
          for (int i=0; i<edg[tar].size(); i++)
              for (int k=0; k<vert; k++)
                  for (int l=0; l<edg[k].size(); l++)
                      if (edg[k][l]>k && k!=tar && k!=edg[tar][i] && edg[k][l]!=tar && edg[k][l]!=edg[tar][i]) {
                         sect+=intersect(p[tar],p[edg[tar][i]],p[k],p[edg[k][l]]);
                         }
          return sect;
          }
      int movintersect (int tar) {
          int sect=0;
          for (int i=0; i<edg[tar].size(); i++)
              if (tried[edg[tar][i]]!=0)
                 for (int k=0; k<vert; k++)
                     if (tried[k]!=0) {
                        for (int l=0; l<edg[k].size(); l++)
                            if (tried[edg[k][l]]!=0 && edg[k][l]>k && k!=tar && k!=edg[tar][i] && edg[k][l]!=tar && edg[k][l]!=edg[tar][i]) {
                               sect+=intersect(p[tar],p[edg[tar][i]],p[k],p[edg[k][l]]);
                               }
                        }
          return sect;
          }
          
      void roughbest (int bp,int prec,int range) {
           int mi=1<<20;
           coor best,at,tc;
           int v[705][705],curi,opr=0; memset(v,0,sizeof(v));
           queue<pair<coor,int> > q;
           vp[p[bp].x][p[bp].y]=0;
           fprintf(debug,"%d: (%d,%d) = %d\n",bp,p[bp].x,p[bp].y,movintersect(bp));
           //fprintf(debug,"all: %d\n",allintersect());
           fflush(debug);
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
                   fprintf(debug,"%d ",cur);
                   }
               fprintf(debug,"\n");
               }
           fprintf(debug,"Best (%d,%d): %d\n",best.x,best.y,mi);
           fflush(debug);
           p[bp]=best;
           vp[p[bp].x][p[bp].y]=1;
           }
      void putpoint () {
          //edgsz[0].second=mid,mid
          int atp=edgsz[0].second,cntp=0;
          memset(tried,0,sizeof(tried));
          vector<int> tryp;
          priority_queue<pair<int,int> > pq;
          pq.push(make_pair(edgsz[0].first,edgsz[0].second));
          tried[atp]=1;
          p[atp].x=350; p[atp].y=350;
          vp[p[atp].x][p[atp].y]=1;
          while (!pq.empty()) {
             atp=pq.top().second;
             pq.pop();
             cntp++;
             fprintf(debug,"consider(%d): %d  - %0.4lf\n\n",cntp,atp,runtime());
             tryp.clear();
             /*
             for (int i=0; i<edg[atp].size(); i++) 
                 if (tried[edg[atp][i]]==0) {
                    tryp.push_back(edg[atp][i]);
                    //delete edg[atp][i] in edg[edg[atp][i]] ?
                    pq.push(make_pair(edg[edg[atp][i]].size(),edg[atp][i]));
                    }
             */
             for (int i=0; i<edg[atp].size(); i++) {
                 if (tried[edg[atp][i]]==0 || (tried[edg[atp][i]]!=0 && rand()%3==1)) tryp.push_back(edg[atp][i]);
                 if (tried[edg[atp][i]]==0) pq.push(make_pair(edg[edg[atp][i]].size(),edg[atp][i]));
                 }
             /*
             for (int i=0; i<tryp.size(); i++) {
                 tried[tryp[i]]=1;
                 fprintf(debug,"%d of %d: %d\n",i+1,tryp.size(),tryp[i]);
                 int rpx=rand()%700,rpy=rand()%700;
                 while (vp[rpx][rpy]!=0) {
                    rpx=rand()%700; rpy=rand()%700;
                    }
                 p[tryp[i]].x=rpx; p[tryp[i]].y=rpy;
                 for (int k=0; k<layer; k++) {
                     roughbest(tryp[i],rand()%(layer+1-k)-(layer+1-k)/2+intvl[k],rang[k]);
                     }
                 fprintf(debug,"Final %d = (%d,%d)\n",tryp[i],p[tryp[i]].x,p[tryp[i]].y);
                 }
             */
             int best=1<<20;
             vector<coor> bestp;
             for (int m=0; m<1; m++) {
                 for (int i=0; i<tryp.size(); i++) {
                     tried[tryp[i]]=1;
                     fprintf(debug,"%d of %d: %d\n",i+1,tryp.size(),tryp[i]);
                     int rpx=rand()%700,rpy=rand()%700;
                     while (vp[rpx][rpy]!=0) {
                        rpx=rand()%700; rpy=rand()%700;
                        }
                     p[tryp[i]].x=rpx; p[tryp[i]].y=rpy;
                     }
                 for (int k=0; k<layer-2; k++) {
                     for (int i=0; i<tryp.size(); i++) {
                         roughbest(tryp[i],rand()%(layer+1-k)-(layer+1-k)/2+intvl[k],rang[k]);
                         }
                     }
                 for (int i=0; i<tryp.size(); i++) 
                     fprintf(debug,"Final %d = (%d,%d)\n",tryp[i],p[tryp[i]].x,p[tryp[i]].y);
                 int res=allintersect();
                 fprintf(debug,"m=%d -- %d\n",m,res);
                 if (res<best) {
                    best=res;
                    bestp.clear();
                    for (int i=0; i<tryp.size(); i++) bestp.push_back(p[tryp[i]]);
                    }
                 }
             fprintf(debug,"bestm = %d\n",best);
             for (int i=0; i<tryp.size(); i++)
                 p[tryp[i]]=bestp[i];
             }
          //for all link to edgsz, find the min intersect
          //rescale + minoradjust
          for (int i=0; i<vert; i++) {
              bestret.push_back(p[i].x);
              bestret.push_back(p[i].y);
              }
          }
      vector <int> untangle(int _v, vector <int> Edg) {
          vert=_v;
          start();
          miinter=1<<20;
          for (int i=0; i<Edg.size(); i+=2) {
              edg[Edg[i]].push_back(Edg[i+1]);
              edg[Edg[i+1]].push_back(Edg[i]);
              //fprintf(debug,"%d <-> %d\n",Edg[i],Edg[i+1]);
              }
          for (int i=0; i<vert; i++)
              edgsz.push_back(make_pair(edg[i].size(),i));
          sort(edgsz.begin(),edgsz.end(),pairrev);
          putpoint();
          fprintf(stderr,"Time: %0.4lf\n",runtime()); fflush(stderr);
          return bestret;
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
