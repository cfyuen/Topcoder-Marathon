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

template <class NUM> NUM sqr (NUM X) { return X*X; }
struct coor {
   int x,y;
} low,high;

int w,h,v[600][600],per[600][600],em[600][600],draw[600][600],appr[600][600],repave=6,canind=0;
vector<double> ret;
vector<pair<double,double> > edge,cand;
vector<pair<int,double> > est[600][600];
vector<string> contour;
vector<pair<coor,double> > known,edgpt;
int dx[]={1,-1,0,0},dy[]={0,0,1,-1},d8x[]={1,1,1,0,0,-1,-1,-1},d8y[]={-1,0,1,-1,1,-1,0,1};
double D=0.0,ask[600][600];

class ReliefMap {
   public:
      double accmea (int x,int y) {
         double td[5];
         if (ask[y][x]>-1e-9) return ask[y][x];
         for (int i=0; i<3; i++)
             td[i]=Relief::measure(x,y);
         sort(td,td+3);
         ask[y][x]=td[1];
         return td[1];
         }
      int extpt (int x,int y) {
          if (contour[x][y]!='1') return 0;
          if (x==0 || y==0 || x==h-1 || y==w-1) return 0;
          for (int i=0; i<4; i++)
              if (x+dx[i]>=0 && x+dx[i]<h && y+dy[i]>=0 && y+dy[i]<w && contour[x+dx[i]][y+dy[i]]=='0') return 0;
          return 1;
          }
      vector <double> getMap(vector <string> Contour) {
         contour=Contour;
         w=contour[0].length(); h=contour.size();
         ret.clear(); ret.resize(w*h);
         for (int i=0; i<w*h; i++) ret[i]=-1.0;
         memset(v,0,sizeof(v));
         memset(per,0,sizeof(per));
         memset(em,0,sizeof(em));
         low.x=low.y=high.x=high.y=-1;
         D=0.0; edge.clear(); known.clear();
         for (int i=0; i<w*h; i++)
             ret[i]=50.0;
         for (int i=0; i<h; i++)      
             for (int j=0; j<w; j++)
                 ask[i][j]=-1;
         
         //find D
         
         for (int i=0; i<h; i++)      
             for (int j=0; j<w; j++)
                 if (extpt(i,j)==1)
                    if (extpt(i-1,j)==0 && extpt(i+1,j)==0 && extpt(i,j-1)==0 && extpt(i,j+1)==0) {
                       coor tc; tc.x=i; tc.y=j;
                       double td=accmea(j,i);
                       known.push_back(make_pair(tc,td));
                       }
         
         for (int i=0; i<h; i++)      
             for (int j=0; j<w; j++)
                 if (per[i][j]==0 && contour[i][j]=='0') {
                    coor now; now.x=i; now.y=j;
                    int line=0;
                    memset(v,0,sizeof(v));
                    queue<coor> fq;
                    fq.push(now);
                    per[now.x][now.y]=1; v[now.x][now.y]=1;
                    while (!fq.empty()) {
                       now=fq.front();
                       fq.pop();
                       for (int k=0; k<8; k++)
                           if (now.x+d8x[k]>=0 && now.x+d8x[k]<h && now.y+d8y[k]>=0 && now.y+d8y[k]<w && v[now.x+d8x[k]][now.y+d8y[k]]==0) {
                              coor tc; tc.x=now.x+d8x[k]; tc.y=now.y+d8y[k];
                              if (contour[tc.x][tc.y]=='1') {
                                 line++;
                                 int isline=0;
                                 queue<coor> lq;
                                 lq.push(tc);
                                 v[tc.x][tc.y]=1; per[tc.x][tc.y]=2;
                                 while (!lq.empty()) {
                                    tc=lq.front();
                                    lq.pop();
                                    int testex=0;
                                    if (extpt(tc.x,tc.y)==1) testex=1;
                                    for (int l=0; l<4; l++)
                                        if (tc.x+dx[l]>=0 && tc.x+dx[l]<h && tc.y+dy[l]>=0 && tc.y+dy[l]<w)
                                            if (contour[tc.x+dx[l]][tc.y+dy[l]]=='1' && extpt(tc.x+dx[l],tc.y+dy[l])==1)               
                                               testex=1;
                                    if (testex==0) isline=1;
                                    for (int l=0; l<4; l++)
                                        if (tc.x+dx[l]>=0 && tc.x+dx[l]<h && tc.y+dy[l]>=0 && tc.y+dy[l]<w)
                                            if (v[tc.x+dx[l]][tc.y+dy[l]]==0 && contour[tc.x+dx[l]][tc.y+dy[l]]=='1') {
                                               coor ttc=tc; ttc.x+=dx[l]; ttc.y+=dy[l];
                                               v[ttc.x][ttc.y]=1; per[ttc.x][ttc.y]=2;
                                               lq.push(ttc);
                                               }
                                    }
                                 if (isline==0) line--;
                                 }
                                 else {
                                 v[tc.x][tc.y]=1; per[tc.x][tc.y]=1;
                                 fq.push(tc);
                                 }
                              }
                       }
                    if (line<2) {
                       int addedge=1;
                       for (int k=0; k<h; k++)
                           for (int l=0; l<w; l++)
                               if (v[k][l]==1 && extpt(k,l)==1) {
                                  
                                  if (ask[k][l]<-1e-9) {
                                     double ph=accmea(l,k);
                                     coor tc; tc.x=k; tc.y=l;
                                     }
                                  addedge=0;
                                  goto stop;
                                  }
                       stop: ;
                       if (addedge==1 && edge.size()<4) {
                          coor tc;
                          for (int k=0; k<h; k++)
                              for (int l=0; l<w; l++)
                                  if (v[k][l]==1 && contour[k][l]=='1') {
                                     double tm[4],toth=0.0; int tdx[4],tdy[4];
                                     memset(tdx,-1,sizeof(tdx));
                                     tdx[0]=k; tdy[0]=l;
                                     for (int m=0; m<4; m++) 
                                         if (k+dx[m]>=0 && k+dx[m]<h && l+dy[m]>=0 && l+dy[m]<w && contour[k+dx[m]][l+dy[m]]=='0')
                                            if (v[k+dx[m]][l+dy[m]]==1) { tdx[1]=k+dx[m]; tdy[1]=l+dy[m]; break; }
                                     for (int m=0; m<4; m++) 
                                         if (k+dx[m]>=0 && k+dx[m]<h && l+dy[m]>=0 && l+dy[m]<w && contour[k+dx[m]][l+dy[m]]=='0')
                                            if (v[k+dx[m]][l+dy[m]]==0) { tdx[2]=k+dx[m]; tdy[2]=l+dy[m]; break; }
                                     if (tdx[1]!=-1 && tdx[2]!=-1) {
                                        for (int m=0; m<3; m++) {
                                            tm[m]=accmea(tdy[m],tdx[m]);
                                            }
                                        tc.x=tdx[0]; tc.y=tdy[0];
                                        known.push_back(make_pair(tc,tm[0]));
                                        toth=(tm[0]+min(tm[1],tm[2]))/2;
                                        tc.x=k; tc.y=l;
                                        edge.push_back(make_pair(min(tm[1],tm[2]),tm[0]));
                                        goto finish;
                                        }
                                     }
                          finish: ;
                          }
                       }
                    }
         queue<coor> eq;
         memset(v,0,sizeof(v));
         for (int i=0; i<h; i++)
             for (int j=0; j<w; j++)
                 if (extpt(i,j)==1) {
                    v[i][j]=1;
                    v[i+1][j]=1;
                    v[i-1][j]=1;
                    v[i][j+1]=1;
                    v[i][j-1]=1;
                    }
         coor tc; tc.x=h/2; tc.y=w/2;
         eq.push(tc);
         v[tc.x][tc.y]=1;
         while (edge.size()<4 && !eq.empty()) {
            tc=eq.front();
            eq.pop();
            if (contour[tc.x][tc.y]=='1' && extpt(tc.x,tc.y)==0 && em[tc.x][tc.y]==0) {
               double tm[4],toth=0.0; int tdx[4],tdy[4];
               memset(tdx,-1,sizeof(tdx));
               tdx[0]=tc.x; tdy[0]=tc.y;
               for (int m=0; m<4; m++) 
                   if (tc.x+dx[m]>=0 && tc.x+dx[m]<h && tc.y+dy[m]>=0 && tc.y+dy[m]<w && contour[tc.x+dx[m]][tc.y+dy[m]]=='0')
                      if (v[tc.x+dx[m]][tc.y+dy[m]]==1) { tdx[1]=tc.x+dx[m]; tdy[1]=tc.y+dy[m]; break; }
               for (int m=0; m<4; m++) 
                   if (tc.x+dx[m]>=0 && tc.x+dx[m]<h && tc.y+dy[m]>=0 && tc.y+dy[m]<w && contour[tc.x+dx[m]][tc.y+dy[m]]=='0')
                      if (v[tc.x+dx[m]][tc.y+dy[m]]==0) { tdx[2]=tc.x+dx[m]; tdy[2]=tc.y+dy[m]; break; }
               if (tdx[1]!=-1 && tdx[2]!=-1) {
                  for (int m=0; m<3; m++) {
                      tm[m]=accmea(tdy[m],tdx[m]);
                      }
                  coor ttc;
                  ttc.x=tdx[0]; ttc.y=tdy[0];
                  toth=(tm[0]+min(tm[1],tm[2]))/2;
                  edge.push_back(make_pair(min(tm[1],tm[2]),tm[0]));
                  queue<coor> lq;
                  lq.push(tc);
                  em[tc.x][tc.y]=1;
                  coor now;
                  while (!lq.empty()) {
                     now=lq.front();
                     lq.pop();
                     for (int i=0; i<4; i++)
                         if (now.x+dx[i]>=0 && now.x+dx[i]<h && now.y+dy[i]>=0 && now.y+dy[i]<w && contour[now.x+dx[i]][now.y+dy[i]]=='1' && em[now.x+dx[i]][now.y+dy[i]]==0) {
                            em[now.x+dx[i]][now.y+dy[i]]=1;
                            coor ttc; ttc.x=now.x+dx[i]; ttc.y=now.y+dy[i];
                            lq.push(ttc);
                            }
                     }
                  }
               }
            for (int i=0; i<4; i++)
                if (tc.x+dx[i]>=0 && tc.x+dx[i]<h && tc.y+dy[i]>=0 && tc.y+dy[i]<w && v[tc.x+dx[i]][tc.y+dy[i]]==0) {
                   v[tc.x+dx[i]][tc.y+dy[i]]=1;
                   coor ttc; ttc.x=tc.x+dx[i]; ttc.y=tc.y+dy[i];
                   eq.push(ttc);
                   }
            }
         //use edge data to calculate D
         double lasterr=1e10,stpt=1e10;
         int ok=0;
         for (int i=0; i<edge.size(); i++) {
             stpt=min(stpt,(edge[i].first+edge[i].second)/2);
             for (int j=i+1; j<edge.size(); j++)
                 if (abs( (edge[i].first+edge[i].second)/2-(edge[j].first+edge[j].second)/2 )>1.45)
                    stpt=min(stpt,abs( (edge[i].first+edge[i].second)/2-(edge[j].first+edge[j].second)/2 ));
             }
         for (double td=max(2.0,stpt*0.8); td<10; td+=1e-3) {
             double err=0.0;
             for (int i=0; i<edge.size(); i++)
                 err+=sqrt(abs(((edge[i].first+edge[i].second)/2)/td-(int)(((edge[i].first+edge[i].second)/2)/td+0.5)));
             if (err>lasterr) {
                if (ok==1) cand.push_back(make_pair(err,td-1e-3));
                ok=0;
                }
                else ok=1;
             lasterr=err;
             }
         sort(cand.begin(),cand.end());
         D=cand[0].second;
         //find out all contour line
         int fillall=1,newlin=0,rep=0;
         adjust:
         memset(draw,0,sizeof(draw));
         for (int i=0; i<h; i++)
             for (int j=0; j<w; j++)
                 est[i][j].clear();
         fillall=1;
         newlin=0;
      do {
         vector<coor> willdraw;
         fillall=1;
         
         for (int i=0; i<known.size(); i++)
             for (int j=i+1; j<known.size(); j++) {
                 coor c1=known[i].first,c2=known[j].first;
                 willdraw.clear();
                 double h1=known[i].second,h2=known[j].second;
                 int lvl1=(int)(h1/D),lvl2=(int)(h2/D);
                 if (abs(c1.x-c2.x)+abs(c1.y-c2.y)>5) {
                    int betw=0;
                    if (abs(c1.x-c2.x)>abs(c1.y-c2.y)) {
                       if (c1.x>c2.x) { swap(c1,c2); swap(h1,h2); swap(lvl1,lvl2); }
                       int prev=0;
                       for (int tx=c1.x+1; tx<c2.x; tx++) {
                           int ty=c1.y+(int)((c2.y-c1.y)*((tx-c1.x)*1.0/(c2.x-c1.x)));
                           int edb=0;
                           for (int k=0; k<4; k++) {                                   
                               if (tx+dx[k]>=0 && tx+dx[k]<h && ty+dy[k]>=0 && ty+dy[k]<w && contour[tx+dx[k]][ty+dy[k]]=='1') edb++;
                               if (!(tx+dx[k]>=0 && tx+dx[k]<h && ty+dy[k]>=0 && ty+dy[k]<w)) edb++;
                               }
                           if (extpt(tx,ty)==0 && edb>=2 && contour[tx][ty]=='1') {
                              if (prev==0) { 
                                 coor tc; tc.x=tx; tc.y=ty;
                                 willdraw.push_back(tc);
                                 prev=1; betw++;
                                 }
                              }
                              else prev=0;
                           }
                       }
                       else {
                       if (c1.y>c2.y) { swap(c1,c2); swap(h1,h2); swap(lvl1,lvl2); }
                       int prev=0;
                       for (int ty=c1.y+1; ty<c2.y; ty++) {
                           int tx=c1.x+(int)((c2.x-c1.x)*((ty-c1.y)*1.0/(c2.y-c1.y)));
                           int edb=0;
                           for (int k=0; k<4; k++) {                                    
                               if (tx+dx[k]>=0 && tx+dx[k]<h && ty+dy[k]>=0 && ty+dy[k]<w && contour[tx+dx[k]][ty+dy[k]]=='1') edb++;
                               if (!(tx+dx[k]>=0 && tx+dx[k]<h && ty+dy[k]>=0 && ty+dy[k]<w)) edb++;
                               }
                           if (extpt(tx,ty)==0 && edb>=2 && contour[tx][ty]=='1') {
                              if (prev==0) {
                                 coor tc; tc.x=tx; tc.y=ty;
                                 willdraw.push_back(tc);
                                 prev=1; betw++;
                                 }
                              }
                              else prev=0;
                           }
                       }
                    if (abs(lvl2-lvl1)>=betw+3) {
                       canind++;
                       D=cand[canind].second;
                       goto adjust;
                       }
                    if (abs(lvl2-lvl1)>=betw-1) {
                       int mul;
                       if (lvl2>lvl1) mul=1; else mul=-1;
                       newlin=1;
                       //flood fill contour line
                       for (int k=0; k<willdraw.size(); k++) {
                           coor tc=willdraw[k];
                           if (draw[tc.x][tc.y]==0) {
                              double here=(lvl1+mul*(k+1)+1)*D;
                              if (mul==1) here-=D;
                              ret[tc.x*w+tc.y]=here;
                              draw[tc.x][tc.y]=1;
                              queue<coor> eq;
                              eq.push(tc);
                              while (!eq.empty()) {
                                 tc=eq.front();
                                 eq.pop();
                                 for (int l=0; l<4; l++) {
                                     coor ttc; ttc.x=tc.x+dx[l]; ttc.y=tc.y+dy[l];
                                     if (ttc.x>=0 && ttc.x<h && ttc.y>=0 && ttc.y<w)
                                        if (draw[ttc.x][ttc.y]==0 && contour[ttc.x][ttc.y]=='1') {
                                           draw[ttc.x][ttc.y]=1;
                                           ret[ttc.x*w+ttc.y]=here;
                                           eq.push(ttc);
                                           }
                                     }
                                 }
                              }
                           }
                       }
                    }
                 }
         if (newlin==0) {
            canind++;
            D=cand[canind].second;
            goto adjust;
            }
         //fill in the other squares
         
         memset(v,0,sizeof(v));
         int tcnt=0;
         for (int i=0; i<h; i++)
             for (int j=0; j<w; j++) {
                 coor tc; tc.x=i; tc.y=j;
                 int edb=0;
                 for (int k=0; k<4; k++)                                    
                     if (tc.x+dx[k]>=0 && tc.x+dx[k]<h && tc.y+dy[k]>=0 && tc.y+dy[k]<w && contour[tc.x+dx[k]][tc.y+dy[k]]=='1') edb++;
                 if (v[tc.x][tc.y]==0 && draw[tc.x][tc.y]==1) {
                    double here=ret[tc.x*w+tc.y];
                    memset(appr,0,sizeof(appr));
                    queue<pair<int,coor> > fq;
                    queue<coor> eq;
                    eq.push(tc);
                    fq.push(make_pair(0,tc));
                    v[tc.x][tc.y]=1;
                    appr[tc.x][tc.y]=1;
                    while (!eq.empty()) {
                       tc=eq.front();
                       eq.pop();
                       for (int l=0; l<4; l++) {
                           coor ttc; ttc.x=tc.x+dx[l]; ttc.y=tc.y+dy[l];
                           if (ttc.x>=0 && ttc.x<h && ttc.y>=0 && ttc.y<w)
                              if (v[ttc.x][ttc.y]==0 && contour[ttc.x][ttc.y]=='1') {
                                 v[ttc.x][ttc.y]=1;
                                 appr[ttc.x][ttc.y]=1;
                                 eq.push(ttc);
                                 fq.push(make_pair(0,ttc));
                                 }
                           }
                       }
                    int lvl=0;
                    while (!fq.empty()) {
                       tc=fq.front().second;
                       lvl=fq.front().first;
                       fq.pop();
                       est[tc.x][tc.y].push_back(make_pair(lvl,here));
                       for (int l=0; l<4; l++) {
                           coor ttc; ttc.x=tc.x+dx[l]; ttc.y=tc.y+dy[l];
                           if (ttc.x>=0 && ttc.x<h && ttc.y>=0 && ttc.y<w)
                              if (appr[ttc.x][ttc.y]==0 && contour[ttc.x][ttc.y]=='0') {
                                 appr[ttc.x][ttc.y]=1;
                                 fq.push(make_pair(lvl+1,ttc));
                                 }
                           }
                       }
                    }
                 }
         
         //confirm 4 corners
         if (est[0][0].size()==0) {
            double td=accmea(0,0);
            coor tc; tc.x=0; tc.y=0;
            known.push_back(make_pair(tc,td));
            fillall=0;
            }
         if (est[h-1][0].size()==0) {
            double td=accmea(0,h-1);
            coor tc; tc.x=h-1; tc.y=0;
            known.push_back(make_pair(tc,td));
            fillall=0;
            }
         if (est[0][w-1].size()==0) {
            double td=accmea(w-1,0);
            coor tc; tc.x=0; tc.y=w-1;
            known.push_back(make_pair(tc,td));
            fillall=0;
            }
         if (est[h-1][w-1].size()==0) {
            double td=accmea(w-1,h-1);
            coor tc; tc.x=h-1; tc.y=w-1;
            known.push_back(make_pair(tc,td));
            fillall=0;
            }
         //confirm side
         
         if (fillall==1) {
            int st=-1,end=-1;
            for (int i=0; i<h; i++) {
                if (est[i][0].size()==0) {
                   if (st==-1) st=i;
                   }
                   else
                   if (end==-1 && st!=-1) {
                      end=i;
                      double td=accmea(0,(end+st)/2);
                      coor tc; tc.x=(end+st)/2; tc.y=0;
                      known.push_back(make_pair(tc,td));
                      fillall=0;
                      end=-1; st=-1;
                      }
                }
            st=-1; end=-1;
            for (int i=0; i<h; i++) {
                if (est[i][w-1].size()==0) {
                   if (st==-1) st=i;
                   }
                   else
                   if (end==-1 && st!=-1) {
                      end=i;
                      double td=accmea(w-1,(end+st)/2);
                      coor tc; tc.x=(end+st)/2; tc.y=w-1;
                      known.push_back(make_pair(tc,td));
                      fillall=0;
                      end=-1; st=-1;
                      }
                }
            st=-1; end=-1;
            for (int i=0; i<w; i++) {
                if (est[0][i].size()==0) {
                   if (st==-1) st=i;
                   }
                   else
                   if (end==-1 && st!=-1) {
                      end=i;
                      double td=accmea((end+st)/2,0);
                      coor tc; tc.x=0; tc.y=(end+st)/2;
                      known.push_back(make_pair(tc,td));
                      fillall=0;
                      end=-1; st=-1;
                      }
                }
            st=-1; end=-1;
            for (int i=0; i<w; i++) {
                if (est[h-1][i].size()==0) {
                   if (st==-1) st=i;
                   }
                   else
                   if (end==-1 && st!=-1) {
                      end=i;
                      double td=accmea((end+st)/2,h-1);
                      coor tc; tc.x=h-1; tc.y=(end+st)/2;
                      known.push_back(make_pair(tc,td));
                      fillall=0;
                      end=-1; st=-1;
                      }
                }
            }
         rep++;
      } while (fillall==0 && rep<20);
         //use data to estimate height
         for (int i=0; i<h; i++)
             for (int j=0; j<w; j++)
                 if (draw[i][j]==0) {
                    draw[i][j]=1;
                    int weight=0;
                    double sum=0.0;
                    sort(est[i][j].begin(),est[i][j].end());
                    if (est[i][j].size()==0) { continue; }
                    if (est[i][j].size()==1) { ret[i*w+j]=est[i][j][0].second; continue; }
                    for (int k=1; k<est[i][j].size(); k++)
                        if (abs(est[i][j][k].second-est[i][j][0].second)>1e-7) {
                           swap(est[i][j][k],est[i][j][1]); break;
                           }
                    weight=est[i][j][0].first+est[i][j][1].first;
                    sum=est[i][j][1].first*est[i][j][0].second+est[i][j][0].first*est[i][j][1].second;
                    
                    if (weight!=0) {
                       sum/=weight;
                       }
                    ret[i*w+j]=sum;
                    }
         
         return ret;
      }
};

