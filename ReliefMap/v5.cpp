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

//delete when submit
FILE *debug,*result;
int localtest=0,stseed=0,endseed=10;
int NoM=0;
double C[600][600];

int sttime;
int tcnt=0;
void start() {
    sttime=clock();
}

double runtime() {
    return (clock()-sttime)/1000.0;
}

double dourand() {
   return (rand()*32768+rand())/1073741824.0;
}
double Gaussian() {
   double a,b,r;
   do {
      a=dourand()*2-1; b=dourand()*2-1;
      r=a*a+b*b;
      } while (r*r>1);
   return sqrt(-2*log(r*r)/(r*r))*a;
}

class Relief {
   public:
      double measure (int x,int y) {
         if (localtest==0) {
         printf("?\n%d\n%d\n",x,y);
         fflush(stdout);
         double Mret;
         scanf("%lf",&Mret);
         return Mret;
         }
         else {
         NoM++;
         return max(min(C[y][x] + Gaussian(),100.0),0.0);
         }
      }
};
Relief Relief;

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
      double accmea (int x,int y) {
         double td[5];
         if (ask[y][x]>-1e-9) return ask[y][x];
         for (int i=0; i<3; i++)
             td[i]=Relief.measure(x,y);
         sort(td,td+3);
         ask[y][x]=td[1];
         return td[1];
         }
      int extpt (int x,int y) {
          if (x<0 || x>=h || y<0 || y>=w) return 0;
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
         for (int i=0; i<h; i++)      
             for (int j=0; j<w; j++)
                 ask[i][j]=-1;
         start();
         
         for (int k=0; k<h; k++) {
             for (int l=0; l<w; l++)
                 if (contour[k][l]=='1') {
                    if (v[k][l]==0) fprintf(debug,"*");
                       else fprintf(debug,"!");
                       }
                    else fprintf(debug,"%d",v[k][l]);
             fprintf(debug,"\n");
             } 
         //find D
         
         memset(v,0,sizeof(v));
         for (int i=0; i<h; i++)      
             for (int j=0; j<w; j++)
                 if (extpt(i,j)==1 && v[i][j]==0) {
                    /*
                    if (extpt(i-1,j)==0 && extpt(i+1,j)==0 && extpt(i,j-1)==0 && extpt(i,j+1)==0) {
                       coor tc; tc.x=i; tc.y=j;
                       double td=accmea(j,i);
                       known.push_back(make_pair(tc,td));
                       fprintf(debug,"measure: (%d,%d) %0.4lf\n",tc.x,tc.y,td);
                       }
                    */
                    coor tc; tc.x=i; tc.y=j;
                    double td=accmea(j,i);
                    known.push_back(make_pair(tc,td));
                    fprintf(debug,"measure: (%d,%d) %0.4lf\n",tc.x,tc.y,td);
                    queue<coor> eq;
                    v[tc.x][tc.y]=1;
                    eq.push(tc);
                    while (!eq.empty()) {
                       tc=eq.front();
                       eq.pop();
                       for (int l=0; l<4; l++) {
                           coor ttc; ttc.x=tc.x+dx[l]; ttc.y=tc.y+dy[l];
                           if (ttc.x>=0 && ttc.x<h && ttc.y>=0 && ttc.y<w)
                              if (v[ttc.x][ttc.y]==0 && contour[ttc.x][ttc.y]=='1') {
                                 v[ttc.x][ttc.y]=1;
                                 eq.push(ttc);
                                 }
                           }
                       }
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
                                    //fprintf(debug,"%d %d\n",tc.x,tc.y);
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
                    /*
                    for (int k=0; k<h; k++) {
                           for (int l=0; l<w; l++)
                               if (contour[k][l]=='1') {
                                  if (v[k][l]==0) fprintf(debug,"*");
                                     else fprintf(debug,"!");
                                  }
                                  else fprintf(debug,"%d",v[k][l]);
                           fprintf(debug,"\n");
                           } 
                    */
                    //fprintf(debug,"l=%d\n",line);
                    if (line<2) {
                       fprintf(debug,"LINE==1\n");
                       int addedge=1;
                       for (int k=0; k<h; k++)
                           for (int l=0; l<w; l++)
                               if (v[k][l]==1 && extpt(k,l)==1) {
                                  
                                  if (ask[k][l]<-1e-9) {
                                     double ph=accmea(l,k);
                                     coor tc; tc.x=k; tc.y=l;
                                     //known.push_back(make_pair(tc,ph));
                                     fprintf(debug,"measure: (%d,%d) %0.4lf\n",tc.x,tc.y,ph);
                                     }
                                  addedge=0;
                                  goto stop;
                                  }
                       stop: ;
                       if (addedge==1 && edge.size()<4) {
                          /*
                          int point=0;
                          double toth=0.0;
                          coor tc;
                          for (int k=0; k<h; k++)
                              for (int l=0; l<w; l++)
                                  if (v[k][l]==1 && contour[k][l]=='1' && point<repave) {
                                     double tm[4]; int tdx[4],tdy[4];
                                     memset(tdx,-1,sizeof(tdx));
                                     tdx[0]=k; tdy[0]=l;
                                     for (int m=0; m<4; m++) 
                                         if (k+dx[m]>=0 && k+dx[m]<h && l+dy[m]>=0 && l+dy[m]<w && contour[k+dx[m]][l+dy[m]]=='0')
                                            if (v[k+dx[m]][l+dy[m]]==1) { tdx[1]=k+dx[m]; tdy[1]=l+dy[m]; break; }
                                     for (int m=0; m<4; m++) 
                                         if (k+dx[m]>=0 && k+dx[m]<h && l+dy[m]>=0 && l+dy[m]<w && contour[k+dx[m]][l+dy[m]]=='0')
                                            if (v[k+dx[m]][l+dy[m]]==0) { tdx[2]=k+dx[m]; tdy[2]=l+dy[m]; break; }
                                     if (tdx[1]!=-1 && tdx[2]!=-1) {
                                        for (int m=0; m<3; m++)
                                            tm[m]=Relief.measure(tdy[m],tdx[m]);
                                        for (int m=0; m<3; m++) toth+=tm[m];
                                        toth-=max(tm[0],max(tm[1],tm[2]));
                                        fprintf(debug,"(%d,%d)-%0.4lf (%d,%d)-%0.4lf (%d,%d)-%0.4lf %0.4lf\n",tdx[0],tdy[0],tm[0],tdx[1],tdy[1],tm[1],tdx[2],tdy[2],tm[2],toth);
                                        tc.x=k; tc.y=l;
                                        point++;
                                        }
                                     }
                          edge.push_back(toth/(point*2));
                          fprintf(debug,"edge: (%d,%d) %0.4lf\n",tc.x,tc.y,toth/(point*2));
                          */
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
                                            //original pos
                                            }
                                        tc.x=tdx[0]; tc.y=tdy[0];
                                        known.push_back(make_pair(tc,tm[0]));
                                        toth=(tm[0]+min(tm[1],tm[2]))/2;
                                        fprintf(debug,"(%d,%d)-%0.4lf (%d,%d)-%0.4lf (%d,%d)-%0.4lf %0.4lf\n",tdx[0],tdy[0],tm[0],tdx[1],tdy[1],tm[1],tdx[2],tdy[2],tm[2],toth);
                                        tc.x=k; tc.y=l;
                                        edge.push_back(make_pair(min(tm[1],tm[2]),tm[0]));
                                        fprintf(debug,"edge: (%d,%d) %0.4lf\n",tc.x,tc.y,toth);
                                        goto finish;
                                        }
                                     }
                          finish: ;
                          }
                       }
                    //fprintf(debug,"\n\n");
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
               fprintf(debug,"(%d %d)\n",tc.x,tc.y);
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
                      //original pos
                      }
                  coor ttc;
                  ttc.x=tdx[0]; ttc.y=tdy[0];
                  //known.push_back(make_pair(ttc,tm[0]));
                  toth=(tm[0]+min(tm[1],tm[2]))/2;
                  fprintf(debug,"(%d,%d)-%0.4lf (%d,%d)-%0.4lf (%d,%d)-%0.4lf %0.4lf\n",tdx[0],tdy[0],tm[0],tdx[1],tdy[1],tm[1],tdx[2],tdy[2],tm[2],toth);
                  edge.push_back(make_pair(min(tm[1],tm[2]),tm[0]));
                  fprintf(debug,"edge: (%d,%d) %0.4lf\n",tc.x,tc.y,toth);
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
         fprintf(debug,"AllEdge: %d\n",edge.size());
         for (int i=0; i<edge.size(); i++)
             fprintf(debug,"%d: %0.4lf-%0.4lf, %0.4lf\n",i,edge[i].first,edge[i].second,(edge[i].first+edge[i].second)/2);
         //use edge data to calculate D
         double lasterr=1e10,stpt=1e10;
         int ok=0;
         for (int i=0; i<edge.size(); i++) {
             stpt=min(stpt,(edge[i].first+edge[i].second)/2);
             for (int j=i+1; j<edge.size(); j++)
                 if (abs( (edge[i].first+edge[i].second)/2-(edge[j].first+edge[j].second)/2 )>1.45)
                    stpt=min(stpt,abs( (edge[i].first+edge[i].second)/2-(edge[j].first+edge[j].second)/2 ));
             }
         fprintf(debug,"stpt = %0.4lf\n",stpt);
         for (double td=max(2.0,stpt*0.8); td<10; td+=1e-3) {
             double err=0.0;
             for (int i=0; i<edge.size(); i++)
                 err+=sqrt(abs(((edge[i].first+edge[i].second)/2)/td-(int)(((edge[i].first+edge[i].second)/2)/td+0.5)));
             //if (td<stpt+2) fprintf(debug,"%0.4lf: %0.4lf\n",td,err);
             if (err>lasterr) {
                if (ok==1) cand.push_back(make_pair(err,td-1e-3));
                ok=0;
                }
                else ok=1;
             lasterr=err;
             }
         sort(cand.begin(),cand.end());
         int tsz=cand.size();
         for (int i=0; i<tsz; i++)
             if (cand[i].second/2>2) cand.push_back(make_pair(cand[i].first,cand[i].second/2));
         D=cand[0].second;
         
         for (int i=0; i<cand.size(); i++)
             fprintf(debug,"canD: %0.4lf\n",cand[i].second);
         fprintf(debug,"time find D: %0.4lfs\n",runtime());
         
         //find out all contour line
         int fillall=1,newlin=0,rep=0;
         adjust:
         rep=0;
         fprintf(debug,"D = %0.4lf\n",D);
         memset(draw,0,sizeof(draw));
         for (int i=0; i<h; i++)
             for (int j=0; j<w; j++)
                 est[i][j].clear();
         fillall=1;
         
      do {
         vector<coor> willdraw;
         fillall=1;
         newlin=0;
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
                       
                       int edb=0,prev=0;
                       for (int k=0; k<4; k++) {                                   
                           if (c1.x+dx[k]>=0 && c1.x+dx[k]<h && c1.y+dy[k]>=0 && c1.y+dy[k]<w && contour[c1.x+dx[k]][c1.y+dy[k]]=='1') edb++;
                           if (!(c1.x+dx[k]>=0 && c1.x+dx[k]<h && c1.y+dy[k]>=0 && c1.y+dy[k]<w)) edb++;
                           }
                       if (extpt(c1.x,c1.y)==0 && edb>=2 && contour[c1.x][c1.y]=='1') { prev=1; willdraw.push_back(c1); betw++; }
                          else prev=0;
                       for (int tx=c1.x+1; tx<=c2.x; tx++) {
                           int ty=c1.y+(int)((c2.y-c1.y)*((tx-c1.x)*1.0/(c2.x-c1.x)));
                           edb=0;
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
                       int edb=0,prev=0;
                       for (int k=0; k<4; k++) {                                   
                           if (c1.x+dx[k]>=0 && c1.x+dx[k]<h && c1.y+dy[k]>=0 && c1.y+dy[k]<w && contour[c1.x+dx[k]][c1.y+dy[k]]=='1') edb++;
                           if (!(c1.x+dx[k]>=0 && c1.x+dx[k]<h && c1.y+dy[k]>=0 && c1.y+dy[k]<w)) edb++;
                           }
                       if (extpt(c1.x,c1.y)==0 && edb>=2 && contour[c1.x][c1.y]=='1') { prev=1; willdraw.push_back(c1); betw++; }
                          else prev=0;
                       for (int ty=c1.y+1; ty<=c2.y; ty++) {
                           int tx=c1.x+(int)((c2.x-c1.x)*((ty-c1.y)*1.0/(c2.y-c1.y)));
                           edb=0;
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
                    fprintf(debug,"(%d,%d) %0.4lf -- (%d,%d) %0.4lf\n",c1.x,c1.y,h1,c2.x,c2.y,h2);
                    fprintf(debug,"%d %d betw:%d\n",lvl1,lvl2,betw);
                    if (abs(lvl2-lvl1)>=betw+3) {
                       canind++;
                       if (canind==cand.size()) break;
                       D=cand[canind].second;
                       goto adjust;
                       }
                    if (abs(lvl2-lvl1)>=betw-1 && betw>0) {
                    //if (abs(lvl2-lvl1)==betw && betw>0) {
                       int mul,initlvl;
                       if (lvl2>lvl1) {
                          mul=1; initlvl=(int)(min(h1-0.8,100.0)/D)+1;
                          //fprintf(debug,"initlvl: %0.4lf\n",initlvl*D);
                          int fx=willdraw[0].x,fy=willdraw[0].y;
                          if ((abs(fx-c2.x)<=1 || abs(fy-c2.y)<=1) && abs(fx-c2.x)+abs(fy-c2.y)<=5) initlvl++;
                          //fprintf(debug,"initlvl: %0.4lf\n",initlvl*D);
                          }
                          else {
                          mul=-1; initlvl=(int)(min(h2+0.8,100.0)/D);
                          initlvl+=willdraw.size();
                          int fx=willdraw[willdraw.size()-1].x,fy=willdraw[willdraw.size()-1].y;
                          //fprintf(debug,"fin(%d,%d) %d initlvl: %0.4lf\n",fx,fy,abs(fx-c2.x)+abs(fy-c2.y),initlvl*D);
                          fflush(debug);
                          if ((abs(fx-c2.x)<=1 || abs(fy-c2.y)<=1) && abs(fx-c2.x)+abs(fy-c2.y)<=5) initlvl--;
                          //fprintf(debug,"initlvl: %0.4lf\n",initlvl*D);
                          }
                       //flood fill contour line
                       int check=0;
                       for (int k=0; k<willdraw.size(); k++) {
                           coor tc=willdraw[k];
                           double here=(initlvl+mul*k)*D;
                           
                           if ((check==0 || k==willdraw.size()-1)) {
                              double cmea; check=1;
                              if (draw[tc.x][tc.y]==0) cmea=accmea(tc.y,tc.x);
                                 else cmea=ret[tc.x*w+tc.y];
                              
                              if (abs(here-cmea)>2.2) {
                                 fprintf(debug,"corr (%d,%d):%0.4lf %0.4lf\n",tc.x,tc.y,here,cmea);
                                 if (here>cmea) initlvl-=(int)((here-cmea)/D+0.5);
                                    else initlvl+=(int)((cmea-here)/D+0.5);
                                 }
                              }
                           
                           here=(initlvl+mul*k)*D;
                           if (draw[tc.x][tc.y]==0) {
                              newlin=1;
                              fprintf(debug,"(%d,%d) nhere:%0.4lf\n",tc.x,tc.y,here);
                              //if (mul==1) here-=D;
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
         for (int i=0; i<known.size(); i++) {
             coor tc=known[i].first;
             //if (contour[tc.x][tc.y]=='1') fprintf(debug,"(%d,%d) draw:%d\n",tc.x,tc.y,draw[tc.x][tc.y]);
             if (contour[tc.x][tc.y]=='1' && draw[tc.x][tc.y]==0) {
                draw[tc.x][tc.y]=1;
                newlin=1;
                double here=known[i].second;
                ret[tc.x*w+tc.y]=here;
                if (extpt(tc.x,tc.y)==0) {
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
         if (newlin==0) {
            canind++;
            if (canind==cand.size()) break;
            D=cand[canind].second;
            goto adjust;
            }
         /*
         for (int k=0; k<h; k++) {
             for (int l=0; l<w; l++)
                 if (contour[k][l]=='1') {
                    if (draw[k][l]==0) fprintf(debug,"*");
                       else fprintf(debug,"!");
                       }
                    else 
                    if (draw[k][l]==0) fprintf(debug,"0");
                       else fprintf(debug,"1");
             fprintf(debug,"\n");
             } 
         */
         fprintf(debug,"time find contour: %0.4lfs\n",runtime());
         //fill in the other squares
         
         memset(v,0,sizeof(v));
         int tcnt=0;
         for (int i=0; i<h; i++)
             for (int j=0; j<w; j++) {
                 coor tc; tc.x=i; tc.y=j;
                 if (v[tc.x][tc.y]==0 && draw[tc.x][tc.y]==1) {
                    double here=ret[tc.x*w+tc.y];
                    memset(appr,0,sizeof(appr));
                    queue<pair<int,coor> > fq;
                    queue<coor> eq;
                    eq.push(tc);
                    fq.push(make_pair(0,tc));
                    draw[tc.x][tc.y]=2;
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
                                 draw[ttc.x][ttc.y]=2;
                                 eq.push(ttc);
                                 fq.push(make_pair(0,ttc));
                                 }
                           }
                       }
                    int lvl=0;
                    fprintf(debug,"(%d,%d) here:%0.4lf\n",tc.x,tc.y,here);
                    while (!fq.empty()) {
                       tc=fq.front().second;
                       lvl=fq.front().first;
                       fq.pop();
                       est[tc.x][tc.y].push_back(make_pair(lvl,here));
                       for (int l=0; l<4; l++) {
                           coor ttc; ttc.x=tc.x+dx[l]; ttc.y=tc.y+dy[l];
                           if (ttc.x>=0 && ttc.x<h && ttc.y>=0 && ttc.y<w) {
                              int pass=0;
                              if (extpt(ttc.x-1,ttc.y)==1 || extpt(ttc.x+1,ttc.y)==1 || extpt(ttc.x,ttc.y-1)==1 || extpt(ttc.x,ttc.y+1)==1) pass=1;
                              if (appr[ttc.x][ttc.y]==0 && (contour[ttc.x][ttc.y]=='0' || pass==1)) {
                                 appr[ttc.x][ttc.y]=1;
                                 fq.push(make_pair(lvl+1,ttc));
                                 }
                              }
                           }
                       }
                    }
                 }
         fprintf(debug,"time fill other:%0.4lfs\n",runtime());
         
         //confirm 4 corners
         if (est[0][0].size()==0) {
            if (ask[0][0]<-1e-7) {
               double td=accmea(0,0);
               coor tc; tc.x=0; tc.y=0;
               known.push_back(make_pair(tc,td));
               fprintf(debug,"measure: (%d,%d) %0.4lf\n",tc.x,tc.y,td);
            }
            fillall=0;
            }
         if (est[h-1][0].size()==0) {
            if (ask[h-1][0]<-1e-7) {
               double td=accmea(0,h-1);
               coor tc; tc.x=h-1; tc.y=0;
               known.push_back(make_pair(tc,td));
               fprintf(debug,"measure: (%d,%d) %0.4lf\n",tc.x,tc.y,td);
            }
            fillall=0;
            }
         if (est[0][w-1].size()==0) {
            if (ask[0][w-1]<-1e-7) {
               double td=accmea(w-1,0);
               coor tc; tc.x=0; tc.y=w-1;
               known.push_back(make_pair(tc,td));
               fprintf(debug,"measure: (%d,%d) %0.4lf\n",tc.x,tc.y,td);
            }
            fillall=0;
            }
         if (est[h-1][w-1].size()==0) {
            if (ask[h-1][w-1]<-1e-7) {
               double td=accmea(w-1,h-1);
               coor tc; tc.x=h-1; tc.y=w-1;
               known.push_back(make_pair(tc,td));
               fprintf(debug,"measure: (%d,%d) %0.4lf\n",tc.x,tc.y,td);
            }
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
                      fprintf(debug,"measure: (%d,%d) %0.4lf\n",tc.x,tc.y,td);
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
                      fprintf(debug,"measure: (%d,%d) %0.4lf\n",tc.x,tc.y,td);
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
                      fprintf(debug,"measure: (%d,%d) %0.4lf\n",tc.x,tc.y,td);
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
                      fprintf(debug,"measure: (%d,%d) %0.4lf\n",tc.x,tc.y,td);
                      fillall=0;
                      end=-1; st=-1;
                      }
                }
            }
         for (int i=0; i<h; i++)
             for (int j=0; j<w; j++)
                 if (est[i][j].size()==0) fillall=0;
         rep++;
      } while (fillall==0 && rep<5);
         for (int k=0; k<h; k++) {
             for (int l=0; l<w; l++)
                 fprintf(debug,"%d",est[k][l].size());
             fprintf(debug,"\n");
             } 
         //fine tuning
         int mxlvl=0; coor ob;
      do {
         mxlvl=0;
         for (int i=0; i<h; i++)
             for (int j=0; j<w; j++)
                 if (est[i][j].size()==1) {
                    if (est[i][j][0].first>mxlvl) {
                       mxlvl=est[i][j][0].first;
                       ob.x=i; ob.y=j;
                       }
                    }
         if (mxlvl>3) {
            memset(v,0,sizeof(v));
            double here=accmea(ob.y,ob.x);
            memset(appr,0,sizeof(appr));
            queue<pair<int,coor> > fq;
            fq.push(make_pair(0,ob));
            v[ob.x][ob.y]=1;
            draw[ob.x][ob.y]=1;
            ret[ob.x*w+ob.y]=here;
            fprintf(debug,"new:(%d,%d) here:%0.4lf\n",ob.x,ob.y,here);
            int lvl;
            while (!fq.empty()) {
               ob=fq.front().second;
               lvl=fq.front().first;
               fq.pop();
               est[ob.x][ob.y].push_back(make_pair(lvl,here));
               for (int l=0; l<4; l++) {
                   coor ttc; ttc.x=ob.x+dx[l]; ttc.y=ob.y+dy[l];
                   if (ttc.x>=0 && ttc.x<h && ttc.y>=0 && ttc.y<w)
                      if (appr[ttc.x][ttc.y]==0 && contour[ttc.x][ttc.y]=='0') {
                         appr[ttc.x][ttc.y]=1;
                         fq.push(make_pair(lvl+1,ttc));
                         }
                   }
               }
            }
         } while (mxlvl>3);
         for (int k=0; k<h; k++) {
             for (int l=0; l<w; l++)
                 fprintf(debug,"%d",est[k][l].size());
             fprintf(debug,"\n");
             } 
         //use data to estimate height
         for (int i=0; i<h; i++)
             for (int j=0; j<w; j++)
                 if (ret[i*w+j]<-1e-7) {
                    draw[i][j]=1;
                    int weight=0;
                    double sum=0.0;
                    sort(est[i][j].begin(),est[i][j].end());
                    if (est[i][j].size()==0) {
                       fprintf(debug,"OBS: (%d,%d)\n",i,j);
                       continue;
                       }
                    if (est[i][j].size()==1) {
                       fprintf(debug,"HI: (%d,%d)\n",i,j);
                       ret[i*w+j]=est[i][j][0].second; 
                       continue;
                       }
                    for (int k=1; k<est[i][j].size(); k++)
                        if (abs(est[i][j][k].second-est[i][j][0].second)>1e-7) {
                           swap(est[i][j][k],est[i][j][1]); break;
                           }
                    weight=est[i][j][0].first+est[i][j][1].first;
                    //fprintf(debug,"(%d,%d) sz:%d w:%d: %0.4lf %0.4lf\n",i,j,est[i][j].size(),weight,est[i][j][0].second,est[i][j][1].second);
                    sum=est[i][j][1].first*est[i][j][0].second+est[i][j][0].first*est[i][j][1].second;
                    fflush(debug);
                    
                    if (weight!=0) {
                       sum/=weight;
                       //fprintf(debug,"(%d,%d) s:%0.4lf\n",i,j,sum);
                       }
                    fflush(debug);
                    ret[i*w+j]=sum;
                    }
         for (int i=0; i<h; i++)
             for (int j=0; j<w; j++)
                 if (ret[i*w+j]<-1e-7) ret[i*w+j]=50;
         fprintf(debug,"time estimate:%0.4lfs\n",runtime());
         
         return ret;
      }
};

int main () {
    ReliefMap RM;
    debug=fopen("debug.txt","w");
    result=fopen("result.txt","w");
    if (localtest==0) {
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
    }
    else {
    int nowseed=0;
    while (nowseed<endseed) {
       int H,W,MaxM;
       NoM=0;
       H=rand()%451+50; W=rand()%451+50;
       MaxM=(int)sqrt(W*H);
       int Bas,Bx[30],By[30],Bz[30],Bf[30];
       double Bw[30];
       Bas=rand()%11+15;
       for (int i=0; i<Bas; i++) {
           Bx[i]=rand()%W; By[i]=rand()%H;
           Bz[i]=rand()%100; Bf[i]=rand()%100+100;
           Bw[i]=dourand()*0.5+0.3;
           }
       for (int i=0; i<H; i++)
           for (int j=0; j<W; j++) {
               double NN,DD,Dis;
               NN=0.0; DD=0.0;
               for (int k=0; k<Bas; k++) {
                   Dis=sqrt(pow(i-By[k],2.0)+pow(j-Bx[k],2.0));
                   Dis=pow(Dis+Bf[k],-Bw[k]);
                   NN+=Dis*Bz[k];
                   DD+=Dis;
                   }
               C[i][j]=NN/DD;
               }
       double Hmin=C[0][0],Hmax=C[0][0];
       for (int i=0; i<H; i++)
           for (int j=0; j<W; j++) {
               Hmin=min(Hmin,C[i][j]);
               Hmax=max(Hmax,C[i][j]);
               }
       for (int i=0; i<H; i++)
           for (int j=0; j<W; j++)
               C[i][j] = (C[i][j]-Hmin)/(Hmax-Hmin)*100;
       vector<string> CON;
       for (int i=0; i<H; i++) {
           string all0="";
           for (int j=0; j<W; j++) all0+="0";
           CON.push_back(all0);
           }
       double CD;
       CD=dourand()*8.0+2.0;
       for (double m=0.0; m<100; m+=CD) {
           for (int i=0; i<H; i++)
               for (int j=0; j<W; j++)
                   if (C[i][j]<m) {
                      for (int k=0; k<8; k++) {
                          int tx=i+d8x[k],ty=j+d8y[k];
                          if (tx>=0 && tx<H && ty>=0 && ty<W && C[tx][ty]>m)
                             CON[i][j]='1';
                          }
                      }
           }
       int Ismin,Ismax;
       for (int i=1; i<H-1; i++)
           for (int j=1; j<W-1; j++) {   
               Ismin=1; Ismax=1;
               for (int k=0; k<8; k++) {
                   if (C[i][j]<=C[i+d8x[k]][j+d8y[k]]) Ismax=0;
                   if (C[i][j]>=C[i+d8x[k]][j+d8y[k]]) Ismin=0;
                   }
               if (Ismin || Ismax) {
                  CON[i][j]='1';
                  CON[i+1][j]='1';
                  CON[i-1][j]='1';
                  CON[i][j+1]='1';
                  CON[i][j-1]='1';
                  }
               }
        vector<double> RET;
        printf("Seed: %d\n",nowseed);
        if (nowseed>=stseed) {
           fprintf(result,"H = %d   W = %d\n",H,W);
           fprintf(result,"D = %0.8lf\n",CD);
           /*
           for (int k=0; k<H; k++) {
               for (int l=0; l<W; l++)
                   if (CON[k][l]=='1') fprintf(result,"*");
                      else fprintf(result,"0");
               fprintf(result,"\n");
               } 
           */
           fflush(result);
           }
        RET=RM.getMap(CON);
        double AvgErr=0.0;
        for (int i=0; i<H; i++)
            for (int j=0; j<W; j++)
                AvgErr += pow(C[i][j]-RET[i*W+j],2);
        AvgErr/=(W*H);
        double Sco;
        Sco=1.0/(AvgErr*pow(100,NoM*1.0/MaxM));
        if (nowseed>=stseed) {
           fprintf(result,"Error = %0.6lf\n",AvgErr);
           fprintf(result,"%d of %d measures used\n",NoM,MaxM);
           fprintf(result,"Score = %0.8lf\n",Sco);
           fprintf(result,"\n\n\n");
           }
        
        nowseed++;
        }
    }
    return 0;
}
