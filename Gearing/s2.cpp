#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<sstream>
#include<cmath>
using namespace std;

struct Gear {
   int pla,sz;
   double x,y;
};

int n;
vector<int> teeth;
vector<Gear> best,now;
vector<string> ret;
double pi=acos(-1.0),highscore=1e18;

class Gearing {
   public:
      double scoring (vector<Gear> vg) {
         double minx,miny,maxx,maxy;
         minx=miny=1e10; maxx=maxy=-1e10;
         for (int i=0; i<n; i++) {
             minx=min(minx,vg[i].x-vg[i].sz*10);
             miny=min(miny,vg[i].y-vg[i].sz*10);
             maxx=max(maxx,vg[i].x+vg[i].sz*10);
             maxy=max(maxy,vg[i].y+vg[i].sz*10);
             }
         return (maxx-minx)*(maxy-miny);
         }
  	  vector <string> place(int K, vector <int> Teeth) {
         teeth=Teeth;
         while (teeth[teeth.size()/2-1]==teeth[teeth.size()/2] && teeth.size()>2) {
            teeth.erase(teeth.begin()+teeth.size()/2-1,teeth.begin()+teeth.size()/2+1);
            }
         n=teeth.size();
         best.resize(teeth.size());
         
         //main part
         int plane=0;
         Gear tg;
         //approach 1, straight line
         plane=0;
         now.clear(); now.resize(n);
         for (int i=0; i<n/2; i++) {
             tg.sz=teeth[i]; tg.pla=plane;
             if (i==0) tg.x=0.0;
                else tg.x=now[i*2-1].x;
             tg.y=0.0;
             now[i*2]=tg;
             
             tg.sz=teeth[n/2+i]; tg.pla=plane;
             tg.x=now[i*2].x+10*(now[i*2].sz+teeth[n/2+i])-9.999;
             tg.y=0.0;
             now[i*2+1]=tg;
             plane=(plane+1)%K;
             }
         if (scoring(now)<highscore) {
            highscore=scoring(now);
            best=now;
            }
         
         
         //approach 2, overlap
         plane=0;
         now.clear(); now.resize(n);
         for (int i=0; i<n/2; i++) {
             tg.sz=teeth[i]; tg.pla=plane;
             if (i==0) { tg.x=0.0; tg.y=0.0; }
                else { tg.x=now[i*2-1].x; tg.y=now[i*2-1].y; }
             now[i*2]=tg;
             
             if (i==0) {
                tg.sz=teeth[n/2+i]; tg.pla=plane;
                tg.x=now[i*2].x+10*(now[i*2].sz+teeth[n/2+i])-9.999;
                tg.y=0.0;
                now[i*2+1]=tg;
                }
                else {
                double key,ovl,sid;
                key=10*(teeth[n/2+i]+teeth[i])-9.999;
                ovl=10*(teeth[n/2+i])+10.1;
                if (K==3 && i!=n/2-1 && i!=1) {
                   ovl=max(ovl,10.1*(teeth[n/2+i-1]+teeth[i+1]));
                   }
                sid=10*(teeth[n/2+i-1]+teeth[i-1])-9.999;
                //cosine law, in radian, this is the minimum angle
                double angle,orian,leng;
                angle=acos((key*key+sid*sid-ovl*ovl)/(2*key*sid));
                double tx=now[i*2].x-now[(i-1)*2].x,ty=now[i*2].y-now[(i-1)*2].y;
                double m1,m2a,m2b;
                m1=ty/tx;
                m2a=(tan(angle)+m1)/(1-m1*tan(angle));
                m2b=(m1-tan(angle))/(1+m1*tan(angle));
                double dx1,dx2,dy1,dy2;
                dx1=sqrt(key*key/(1+m2a*m2a)); dy1=dx1*m2a;
                dx2=sqrt(key*key/(1+m2b*m2b)); dy2=dx2*m2b;
                
                tg.sz=teeth[n/2+i]; tg.pla=plane;
                if (i%2==1) {
                   tg.x=now[i*2].x-abs(dx2);
                   tg.y=now[i*2].y+abs(dy2);
                   }
                   else {
                   tg.x=now[i*2].x+dx1;
                   tg.y=now[i*2].y+dy1;
                   }
                now[i*2+1]=tg;
                }
             plane=(plane+1)%K;
             }
         double cos1=cos(pi/180),sin1=sin(pi/180);
         for (int i=0; i<180; i++) {
             if (scoring(now)<highscore) {
                highscore=scoring(now);
                best=now;
                }
             for (int j=0; j<n; j++) {
                 double ox=now[j].x,oy=now[j].y;
                 now[j].x=ox*cos1+oy*sin1;
                 now[j].y=-ox*sin1+oy*cos1;
                 }
             }
         
         //configure best into string
         for (int i=0; i<n; i++) {
             char cstr[1000];
             sprintf(cstr,"%d %d %0.8lf %0.8lf",best[i].sz,best[i].pla,best[i].x,best[i].y);
             string tstr=cstr;
             ret.push_back(tstr);
             }
         return ret;
      }
};
