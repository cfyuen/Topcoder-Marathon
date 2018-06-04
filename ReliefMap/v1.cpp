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

int w,h,v[600][600],repave=6;
vector<double> ret;
vector<string> contour;
vector<pair<coor,double> > known;
int dx[]={1,-1,0,0},dy[]={0,0,1,-1},d8x[]={1,1,1,0,0,-1,-1,-1},d8y[]={-1,0,1,-1,1,-1,0,1};
double D=0.0;

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

class Relief {
   public:
      double measure (int x,int y) {
         printf("?\n%d\n%d\n",x,y);
         fflush(stdout);
         double Mret;
         scanf("%lf",&Mret);
         return Mret;
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
      int extpt (int x,int y) {
          if (contour[x][y]!='1') return 0;
          for (int i=0; i<4; i++)
              if (x+dx[i]>=0 && x+dx[i]<h && y+dy[i]>=0 && y+dy[i]<w && contour[x+dx[i]][y+dy[i]]=='0') return 0;
          v[x][y]=-2;
          for (int i=0; i<4; i++)
              if (x+dx[i]>=0 && x+dx[i]<h && y+dy[i]>=0 && y+dy[i]<w && v[x+dx[i]][y+dy[i]]!=-2) v[x+dx[i]][y+dy[i]]=-1;
          return 1;
          }
      vector <double> getMap(vector <string> Contour) {
         contour=Contour;
         w=contour[0].length(); h=contour.size();
         ret.clear(); ret.resize(w*h);
         for (int i=0; i<w*h; i++) ret[i]=-1.0;
         memset(v,0,sizeof(v));
         low.x=low.y=high.x=high.y=-1;
         
         //find D
         for (int i=0; i<h; i++)
             for (int j=0; j<w; j++)
                 if (extpt(i,j)==1) {
                    double m1,m2;
                    m1=Relief.measure(j,i); m2=Relief.measure(j,i);
                    fprintf(debug,"extpt: (%d,%d) - %0.4lf %0.4lf\n",i,j,m1,m2);
                    ret[i*w+j]=(m1+m2)/2;
                    coor tc; tc.x=i; tc.y=j;
                    known.push_back(make_pair(tc,ret[i*w+j]));
                    if (low.x==-1) {
                       low.x=high.x=i; low.y=high.y=j;
                       }
                    if (ret[i*w+j]<ret[low.x*w+low.y]) {
                       low.x=i; low.y=j;
                       }
                    if (ret[i*w+j]>ret[high.x*w+high.y]) {
                       high.x=i; high.y=j;
                       }
                    }
         fprintf(debug,"low: (%d,%d) - %0.4lf\n",low.x,low.y,ret[low.x*w+low.y]);
         fprintf(debug,"high: (%d,%d) - %0.4lf\n",high.x,high.y,ret[high.x*w+high.y]);
         /*
         if (ret[low.x*w+low.y]<2) {
            //find by low
            queue<coor> fq;
            for (int i=0; i<4; i++)
                if (low.x+dx[i]>=0 && low.x+dx[i]<h && low.y+dy[i]>=0 && low.y+dy[i]<w) {
                   coor tc; tc.x=low.x+dx[i]; tc.y=low.y+dy[i];
                   fq.push(tc);
                   }
            coor cline;
            int point=0;
            double mea=0.0;
            while (!fq.empty()) {
               cline=fq.front();
               fq.pop();
               if (contour[cline.x][cline.y]=='1' && v[cline.x][cline.y]==1) {
                  coor inn; inn.x=-1;
                  for (int i=0; i<4; i++)
                      if (cline.x+dx[i]>=0 && cline.x+dx[i]<h && cline.y+dy[i]>=0 && cline.y+dy[i]<w)
                         if (v[cline.x+dx[i]][cline.y+dy[i]]<0 || (v[cline.x+dx[i]][cline.y+dy[i]]==1 && contour[cline.x+dx[i]][cline.y+dy[i]]!='1')) {
                            inn.x=cline.x+dx[i]; inn.y=cline.y+dy[i];
                            break;
                            }
                  if (inn.x!=-1) {
                     mea+=Relief.measure(cline.y,cline.x);
                     fprintf(debug,"%0.4lf\n",mea);
                     mea+=Relief.measure(inn.y,inn.x);
                     fprintf(debug,"%0.4lf\n",mea);
                     point++;
                     }
                  fprintf(debug,"%d %d\n",cline.x,cline.y);
                  fprintf(debug,"%d %d\n",inn.x,inn.y);
                  if (repave==point) break;
                  continue;
                  }
               for (int i=0; i<4; i++)
                   if (cline.x+dx[i]>=0 && cline.x+dx[i]<h && cline.y+dy[i]>=0 && cline.y+dy[i]<w && v[cline.x+dx[i]][cline.y+dy[i]]==0) {
                      coor tc; tc.x=cline.x+dx[i]; tc.y=cline.y+dy[i];
                      v[tc.x][tc.y]=1;
                      fq.push(tc);
                      }
               }
            
            D=(mea)/(point*2);
            
            fprintf(debug,"D: %0.4lf\n",D);
            }
            else {
            //test 2 lines
            }
         */
         //random choose 2 near lines (with different height) to test
         if (ret[high.x*w+high.y]<60) { //not accurate enough
            int mxcross=0,dir=0;
            for (int i=0; i<4; i++) {
                int cross=0,nx=low.x,ny=low.y;
                while (nx+dx[i]>=0 && nx+dx[i]<h && ny+dy[i]>=0 && ny+dy[i]<w) {
                   nx+=dx[i]; ny+=dy[i];
                   if (v[nx][ny]>=0 && contour[nx][ny]=='1') {
                      if (contour[nx-dx[i]][ny-dy[i]]!='1') cross++;
                      }
                   }
                if (cross>mxcross) {
                   mxcross=cross; dir=i;
                   }
                }
            high.x=low.x+dx[dir]*5000;
            high.y=low.y+dy[dir]*5000;
            high.x=min(max(0,high.x),h);
            high.y=min(max(0,high.y),w);
            fprintf(debug,"EMEMEMEM\n");
            }
         fprintf(debug,"init: (%d,%d)\n",high.x,high.y);
         double c1c=0.0,c1b=0.0,c2c=0.0,c2b=0.0,c1=0.0,c2=0.0;
         int p1=0,p2=0,out1ok=-1;
         queue<coor> fq;
         coor tc; tc.x=high.x; tc.y=high.y;
         if (v[high.x][high.y]==0) v[high.x][high.y]=1;
         fq.push(tc);
         coor cline,now;
         while (!fq.empty()) {
            now=fq.front();
            fq.pop();
            
            if (contour[now.x][now.y]=='1' && v[now.x][now.y]==1) {
               cline.x=now.x; cline.y=now.y;
               continue;
               }
            for (int i=0; i<8; i++)
                if (now.x+d8x[i]>=0 && now.x+d8x[i]<h && now.y+d8y[i]>=0 && now.y+d8y[i]<w && v[now.x+d8x[i]][now.y+d8y[i]]<1) {
                   coor tc; tc.x=now.x+d8x[i]; tc.y=now.y+d8y[i];
                   if (v[tc.x][tc.y]==0) v[tc.x][tc.y]=1;
                      else v[tc.x][tc.y]+=10;
                   fq.push(tc);
                   }
            fflush(debug);
            }
         coor inn,out;
         fq.push(cline);
         for (int i=0; i<h; i++) {
             for (int j=0; j<w; j++)
                 if (contour[i][j]=='1') fprintf(debug,"*");
                    else fprintf(debug,"%d",v[i][j]);
             fprintf(debug,"\n");
             }
         fprintf(debug,"%d %d %c\n",cline.x,cline.y,contour[cline.x][cline.y]);
         fflush(debug);
         while (!fq.empty() && p1<repave) {
            now=fq.front();
            fq.pop();
            //fprintf(debug,"%d %d\n",now.x,now.y);
            inn.x=-1; out.x=-1;
            for (int i=0; i<4; i++)
                if (now.x+dx[i]>=0 && now.x+dx[i]<h && now.y+dy[i]>=0 && now.y+dy[i]<w) {
                   if (v[now.x+dx[i]][now.y+dy[i]]!=0 && contour[now.x+dx[i]][now.y+dy[i]]!='1') {
                      inn.x=now.x+dx[i]; inn.y=now.y+dy[i];
                      }
                   if (v[now.x+dx[i]][now.y+dy[i]]==0) {
                      out.x=now.x+dx[i]; out.y=now.y+dy[i];
                      }
                   }
            if (p1==0) {
               if (inn.x!=-1 && out.x!=-1) {
                  double inh,outh,conh;
                  conh=Relief.measure(cline.y,cline.x);
                  fprintf(debug,"con:%0.4lf (%d,%d)\n",conh,cline.x,cline.y);
                  outh=(Relief.measure(out.y,out.x)+Relief.measure(out.y,out.x)+Relief.measure(out.y,out.x))/3; //may repeat
                  fprintf(debug,"out:%0.4lf (%d,%d)\n",outh,out.x,out.y);
                  inh=(Relief.measure(inn.y,inn.x)+Relief.measure(inn.y,inn.x)+Relief.measure(inn.y,inn.x))/3;
                  fprintf(debug,"in:%0.4lf (%d,%d)\n",inh,inn.x,inn.y);
                  if (inh>outh) out1ok=1;
                     else out1ok=0;
                  if (out1ok==1) { c1c+=conh; c1b+=outh; }
                     else { c1c+=conh; c1b+=inh; }
                  p1++;
                  }
               }
               else {
               double bouh,conh;
               if (out1ok==1 && out.x!=-1) {
                  conh=Relief.measure(now.y,now.x);
                  fprintf(debug,"con:%0.4lf (%d,%d)\n",conh,now.x,now.y);
                  bouh=Relief.measure(out.y,out.x);
                  fprintf(debug,"out:%0.4lf (%d,%d)\n",bouh,out.x,out.y);
                  c1c+=conh; c1b+=bouh;
                  p1++;
                  }
               if (out1ok==0 && inn.x!=-1) {
                  conh=Relief.measure(now.y,now.x);
                  fprintf(debug,"con:%0.4lf (%d,%d)\n",conh,now.x,now.y);
                  bouh=Relief.measure(inn.y,inn.x);
                  fprintf(debug,"in:%0.4lf (%d,%d)\n",bouh,inn.x,inn.y);
                  c1c+=conh; c1b+=bouh;
                  p1++;
                  }
               }
               
            for (int i=0; i<4; i++)
                if (now.x+dx[i]>=0 && now.x+dx[i]<h && now.y+dy[i]>=0 && now.y+dy[i]<w && v[now.x+dx[i]][now.y+dy[i]]==1 && contour[now.x+dx[i]][now.y+dy[i]]=='1') {
                   coor tc; tc.x=now.x+dx[i]; tc.y=now.y+dy[i];
                   fq.push(tc);
                   }
            }
         c1=c1b/p1+abs(c1c/p1-c1b/p1);
         fprintf(debug,"c1: %0.4lf   p1:%d\n",c1,p1);
         fflush(debug);
         //second contour line
         while (!fq.empty()) fq.pop();
         fq.push(cline);
         while (!fq.empty()) {
            now=fq.front();
            fq.pop();
            if (contour[now.x][now.y]=='1' && v[now.x][now.y]==2) {
               cline.x=now.x; cline.y=now.y;
               double conh;
               conh=(Relief.measure(cline.y,cline.x)+Relief.measure(cline.y,cline.x)+Relief.measure(cline.y,cline.x))/3;
               fprintf(debug,"measure %0.4lf %d %d\n",conh,now.x,now.y);
               if (abs(c1-conh)>2) break;
               if (abs(c1-conh)<1.5) {
                  queue<coor> link; link.push(cline);
                  v[now.x][now.y]=3;
                  coor no;
                  while (!link.empty()) {
                     no=link.front();
                     link.pop();
                     for (int i=0; i<4; i++)
                         if (no.x+dx[i]>=0 && no.x+dx[i]<h && no.y+dy[i]>=0 && no.y+dy[i]<w && v[no.x+dx[i]][no.y+dy[i]]!=3 && contour[no.x+dx[i]][no.y+dy[i]]=='1') {
                            v[no.x+dx[i]][no.y+dy[i]]=3;
                            no.x+=dx[i]; no.y+=dy[i];
                            fprintf(debug,"* %d %d\n",no.x,no.y);
                            link.push(no);
                            no.x-=dx[i]; no.y-=dy[i];
                            }
                     }
                  }
               fprintf(debug,"fail: %0.4lf (%d,%d)\n",conh,cline.x,cline.y);
               continue;
               }
            for (int i=0; i<8; i++)
                if (now.x+d8x[i]>=0 && now.x+d8x[i]<h && now.y+d8y[i]>=0 && now.y+d8y[i]<w && v[now.x+d8x[i]][now.y+d8y[i]]<1) {
                   coor tc; tc.x=now.x+d8x[i]; tc.y=now.y+d8y[i];
                   if (v[tc.x][tc.y]==0) v[tc.x][tc.y]=2;
                      else v[tc.x][tc.y]+=10;
                   fq.push(tc);
                   }
            }
         while (!fq.empty()) fq.pop();
         fq.push(cline);
         for (int i=0; i<h; i++) {
             for (int j=0; j<w; j++)
                 fprintf(debug,"%d",v[i][j]);
             fprintf(debug,"\n");
             }
         while (!fq.empty() && p2<repave) {
            now=fq.front();
            fq.pop();
            //fprintf(debug,"%d %d\n",now.x,now.y);
            inn.x=-1; out.x=-1;
            for (int i=0; i<4; i++)
                if (now.x+dx[i]>=0 && now.x+dx[i]<h && now.y+dy[i]>=0 && now.y+dy[i]<w) {
                   if (v[now.x+dx[i]][now.y+dy[i]]==2 && contour[now.x+dx[i]][now.y+dy[i]]!='1') {
                      inn.x=now.x+dx[i]; inn.y=now.y+dy[i];
                      }
                   if (v[now.x+dx[i]][now.y+dy[i]]==0) {
                      out.x=now.x+dx[i]; out.y=now.y+dy[i];
                      }
                   }
            double bouh,conh;
            if (out1ok==1 && out.x!=-1) {
               conh=Relief.measure(now.y,now.x);
               fprintf(debug,"con:%0.4lf (%d,%d)\n",conh,now.x,now.y);
               bouh=Relief.measure(out.y,out.x);
               fprintf(debug,"out:%0.4lf (%d,%d)\n",bouh,out.x,out.y);
               c2c+=conh; c2b+=bouh;
               p2++;
               }
            if (out1ok==0 && inn.x!=-1) {
               conh=Relief.measure(now.y,now.x);
               fprintf(debug,"con:%0.4lf (%d,%d)\n",conh,now.x,now.y);
               bouh=Relief.measure(inn.y,inn.x);
               fprintf(debug,"in:%0.4lf (%d,%d)\n",bouh,inn.x,inn.y);
               c2c+=conh; c2b+=bouh;
               p2++;
               }
            for (int i=0; i<4; i++)
                if (now.x+dx[i]>=0 && now.x+dx[i]<h && now.y+dy[i]>=0 && now.y+dy[i]<w && v[now.x+dx[i]][now.y+dy[i]]<5 && contour[now.x+dx[i]][now.y+dy[i]]=='1') {
                   coor tc; tc.x=now.x+dx[i]; tc.y=now.y+dy[i];
                   fq.push(tc);
                   }
            fflush(stdout);
            }
         c2=c2b/p2+abs(c2c/p2-c2b/p2);
         fprintf(debug,"c2: %0.4lf   p2:%d\n",c2,p2);
         if (c1>c2) swap(c2,c1);
         
         D=abs(c2-c1);
         double err=1e10,lo=max(2.0,D-D*0.2),hi=min(10.0,D+D*0.2);
         for (double i=lo; i<hi; i+=1e-4) {
             double terr=sqr(c1/i-(int)(c1/i+0.5))+sqr(c2/i-(int)(c2/i+0.5));
             if (terr<err && abs((int)(c1/i+0.5)-(int)(c2/i+0.5))==1) {
                D=i; err=terr;
                }
             }
         fprintf(debug,"D: %0.4lf  --  %0.4lf\n",D,c2-c1);

         for (int i=0; i<w*h; i++)
             ret[i]=50.0;
         return ret;
      }
};

int main () {
    ReliefMap RM;
    debug=fopen("debug.txt","w");
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
    return 0;
}
