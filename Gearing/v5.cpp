#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<sstream>
#include<set>
#include<cmath>
using namespace std;

struct Gear {
   int pla,sz;
   double x,y;
};

int n,plane,K,err1,err2;
Gear tg;
vector<int> teeth;
vector<Gear> best,now;
vector<string> ret;
double pi=acos(-1.0),highscore=1e18;

bool cmp (int a,int b) {
    return a>b;
}

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

class Gearing {
   public:
      double distance (double ax,double ay,double bx,double by) {
         return sqrt((bx-ax)*(bx-ax)+(by-ay)*(by-ay));
         }
      double scoring (vector<Gear> vg) {
         double minx,miny,maxx,maxy; int cond=1;
         minx=miny=1e10; maxx=maxy=-1e10;
         for (int i=0; i<n; i++) {
             if (vg[i].x<vg[0].x) cond=0;
             minx=min(minx,vg[i].x-vg[i].sz*10);
             miny=min(miny,vg[i].y-vg[i].sz*10);
             maxx=max(maxx,vg[i].x+vg[i].sz*10);
             maxy=max(maxy,vg[i].y+vg[i].sz*10);
             if (vg[i].x-vg[i].x!=vg[i].x-vg[i].x) return 1e10;
             if (vg[i].y-vg[i].y!=vg[i].y-vg[i].y) return 1e10;
             }
         if (n<=20 && cond==0) return 1e10;
         if (n>20 && maxx-minx<maxy-miny) return 1e10;
         for (int i=0; i<n; i++)
             for (int j=i+2; j<min(n,i+12); j++) {
                 if (abs(vg[i].x-vg[j].x) + abs(vg[i].y-vg[j].y) <0.1) continue;
                 if (vg[i].pla!=vg[j].pla) continue;
                 double actdis=distance(vg[i].x,vg[i].y,vg[j].x,vg[j].y);
                 if (actdis<10*(vg[i].sz+vg[j].sz)) {
                    err1=i; err2=j;
                    return 1e12;
                    }
                 //later can be solved by compression
                 }
         return (maxx-minx)*(maxy-miny);
         }
      vector<Gear> compress (vector<Gear> gteeth) {
         //max y press
         vector<pair<double,int> > upp;
         vector<Gear> newg;
         double lvl,todoh,minus; int stind=0;
         upp.push_back(make_pair(gteeth[0].y+gteeth[0].sz*10,0));
         int init=0;
         if (gteeth[1].y>gteeth[3].y) init=1; else init=3;
         for (int i=init; i<n; i+=4) upp.push_back(make_pair(gteeth[i].y+gteeth[i].sz*10,i));
         sort(upp.begin(),upp.end());
         int exit=1;
         do {
            newg=gteeth; exit=1;
            todoh=(upp[upp.size()-1]).first;
            for (int i=upp.size()-1; i>=0; i--)
                if (abs(upp[i].first-todoh)>1e-5) {
                   lvl=upp[i].first; stind=i+1; break;
                   }
            minus=todoh-lvl;
            for (int i=upp.size()-1; i>=stind; i--) {
                double odx,ody,ndx,ndy,okey,nkey,tmovx=0.0,movx=0.0;
                int ind=upp[i].second;
                upp[i].first=lvl;
                if (ind!=0) {
                   odx=gteeth[ind].x-gteeth[ind-1].x;
                   ody=gteeth[ind].y-gteeth[ind-1].y;
                   okey=sqrt(odx*odx+ody*ody);
                   tmovx=sqrt(okey*okey-(ody-minus)*(ody-minus))-odx;
                   }
                if (ind!=n-1) {
                   ndx=gteeth[ind].x-gteeth[ind+2].x;
                   ndy=gteeth[ind].y-gteeth[ind+2].y;
                   nkey=sqrt(ndx*ndx+ndy*ndy);
                   movx=sqrt(nkey*nkey-(ndy-minus)*(ndy-minus))+ndx;
                   movx+=tmovx;
                   }
                for (int j=ind; j<n; j++)
                    if (j==ind || j==ind+1) {
                       newg[j].x+=tmovx;
                       newg[j].y-=minus;
                       }
                       else newg[j].x+=movx;
                }
            //fprintf(debug,"score: %0.4lf -> %0.4lf\n",scoring(gteeth),scoring(newg));
            fflush(debug);
            if (scoring(newg)<scoring(gteeth)) { exit=0; gteeth=newg; }
            } while (exit==0);
         //min y press
         vector<pair<double,int> > low;
         stind=0;
         low.push_back(make_pair(gteeth[0].y-gteeth[0].sz*10,0));
         for (int i=4-init; i<n; i+=4) low.push_back(make_pair(gteeth[i].y-gteeth[i].sz*10,i));
         sort(low.begin(),low.end());
         exit=1;
         do {
            newg=gteeth; exit=1;
            todoh=(low[0]).first;
            for (int i=0; i<low.size(); i++)
                if (abs(low[i].first-todoh)>1e-5) {
                   lvl=low[i].first; stind=i; break;
                   }
            minus=todoh-lvl;
            for (int i=0; i<stind; i++) {
                double odx,ody,ndx,ndy,okey,nkey,tmovx=0.0,movx=0.0;
                int ind=low[i].second;
                low[i].first=lvl;
                if (ind!=0) {
                   odx=gteeth[ind].x-gteeth[ind-1].x;
                   ody=gteeth[ind].y-gteeth[ind-1].y;
                   okey=sqrt(odx*odx+ody*ody);
                   tmovx=sqrt(okey*okey-(ody-minus)*(ody-minus))-odx;
                   }
                if (ind!=n-1) {
                   ndx=gteeth[ind].x-gteeth[ind+2].x;
                   ndy=gteeth[ind].y-gteeth[ind+2].y;
                   nkey=sqrt(ndx*ndx+ndy*ndy);
                   movx=sqrt(nkey*nkey-(ndy-minus)*(ndy-minus))+ndx;
                   movx+=tmovx;
                   }
                for (int j=ind; j<n; j++)
                    if (j==ind || j==ind+1) {
                       newg[j].x+=tmovx;
                       newg[j].y-=minus;
                       }
                       else newg[j].x+=movx;
                }
            //fprintf(debug,"score: %0.4lf -> %0.4lf\n",scoring(gteeth),scoring(newg));
            fflush(debug);
            if (scoring(newg)<scoring(gteeth)) { exit=0; gteeth=newg; }
            } while (exit==0);
         fprintf(debug,"score3: %0.4lf\n",scoring(gteeth));
         return gteeth;
         }
      void fitin (vector<int> gteeth) {
         plane=0;
         now.clear(); now.resize(n);
         for (int i=0; i<n/2; i++)
             fprintf(debug,"%d ",gteeth[i]);
         fprintf(debug,"\n");
         for (int i=0; i<n/2; i++)
             fprintf(debug,"%d ",gteeth[n/2+i]);
         fprintf(debug,"\n");
         for (int i=0; i<n/2; i++) {
             tg.sz=gteeth[i]; tg.pla=plane;
             if (i==0) { tg.x=0.0; tg.y=0.0; }
                else { tg.x=now[i*2-1].x; tg.y=now[i*2-1].y; }
             now[i*2]=tg;
             
             if (i==0) {
                tg.sz=gteeth[n/2+i]; tg.pla=plane;
                tg.x=now[i*2].x+10*(now[i*2].sz+gteeth[n/2+i])-9.999;
                tg.y=0.0;
                now[i*2+1]=tg;
                }
                else {
                double key,ovl,sid;
                key=10*(gteeth[n/2+i]+gteeth[i])-9.999;
                ovl=10*(gteeth[n/2+i])+10.1;
                if (K==3 && i!=n/2-1 && i!=1) {
                   ovl=max(ovl,10.1*(gteeth[n/2+i-2]+gteeth[i+1]));
                   //fprintf(debug,"%d %d\n",teeth[n/2+i-2],teeth[i+1]);
                   }
                sid=10*(gteeth[n/2+i-1]+gteeth[i-1])-9.999;
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
                //fprintf(debug,"key:%0.4lf  ovl:%0.4lf  sid:%0.4lf\n",key,ovl,sid);
                //fprintf(debug,"m1:%0.4lf  m2a:%0.4lf m2b:%0.4lf angle:%0.4lf\n",m1,m2a,m2b,angle/pi*180);
                //fprintf(debug,"dx1:%0.4lf dy1:%0.4lf  dx2:%0.4lf dy2:%0.4lf\n",dx1,dy1,dx2,dy2);
                
                tg.sz=gteeth[n/2+i]; tg.pla=plane;
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
         double cos1=cos(pi/360),sin1=sin(pi/360),losco=1e13;
         int chosen=0;
         vector<Gear> lobest;
         for (int i=0; i<360; i++) {
             if (scoring(now)>1e11) {
                double ovl,key,cx1,cy1,cx2,cy2,dist;
                ovl=10*(now[err1].sz+now[err2].sz)+0.001;
                cx1=now[err1].x; cy1=now[err1].y;
                key=10*(now[err2].sz+now[err2-1].sz)-9.999;
                cx2=now[err2-1].x; cy2=now[err2-1].y;
                dist=distance(cx1,cy1,cx2,cy2);
                double r1x,r2x,r1y,r2y,tx,ty;
                tx=now[err2].x; ty=now[err2].y;
                r1x=(cx1+cx2)/2 + (cx2-cx1)*(ovl*ovl-key*key)/(2*dist*dist) + (cy2-cy1)/(2*dist*dist)*sqrt( ((ovl+key)*(ovl+key)-dist*dist)*(dist*dist-(key-ovl)*(key-ovl)) );
                r1y=(cy1+cy2)/2 + (cy2-cy1)*(ovl*ovl-key*key)/(2*dist*dist) - (cx2-cx1)/(2*dist*dist)*sqrt( ((ovl+key)*(ovl+key)-dist*dist)*(dist*dist-(key-ovl)*(key-ovl)) );
                r2x=(cx1+cx2)/2 + (cx2-cx1)*(ovl*ovl-key*key)/(2*dist*dist) - (cy2-cy1)/(2*dist*dist)*sqrt( ((ovl+key)*(ovl+key)-dist*dist)*(dist*dist-(key-ovl)*(key-ovl)) );
                r2y=(cy1+cy2)/2 + (cy2-cy1)*(ovl*ovl-key*key)/(2*dist*dist) + (cx2-cx1)/(2*dist*dist)*sqrt( ((ovl+key)*(ovl+key)-dist*dist)*(dist*dist-(key-ovl)*(key-ovl)) );
                //fprintf(debug,"err: %d %d\n",err1,err2);
                //fprintf(debug,"key:%0.4lf ovl:%0.4lf\n",key,ovl);
                //fprintf(debug,"c1:%0.4lf %0.4lf c2:%0.4lf %0.4lf\n",cx1,cy1,cx2,cy2);
                //fprintf(debug,"i:%d r1: %0.4lf %0.4lf     r2: %0.4lf %0.4lf\n",i,r1x,r1y,r2x,r2y);
                double rx,ry,d1,d2;
                d1=(r1x-now[err2-3].x)*(r1x-now[err2-3].x)+(r1y-now[err2-3].y)*(r1y-now[err2-3].y);
                d2=(r2x-now[err2-3].x)*(r2x-now[err2-3].x)+(r2y-now[err2-3].y)*(r2y-now[err2-3].y);
                if (d1<d2) { rx=r1x; ry=r1y; }
                   else { rx=r2x; ry=r2y; }
                for (int j=err2; j<n; j++) {
                    now[j].x+=rx-tx; now[j].y+=ry-ty;
                    }
                }
             if (scoring(now)<losco) {
                losco=scoring(now);
                lobest=now;
                chosen=i;
                }
             for (int j=0; j<n; j++) {
                 double ox=now[j].x,oy=now[j].y;
                 now[j].x=ox*cos1+oy*sin1;
                 now[j].y=-ox*sin1+oy*cos1;
                 }
             }
         fprintf(debug,"score2: %0.4lf\n",scoring(lobest));
         now=lobest;
         //here, try 3 or more
         vector<Gear> fixed; fixed.resize(n);
         double may=-1e11,miy=1e11;
         for (int i=0; i<n; i++) {
             miy=min(miy,now[i].y-now[i].sz*10);
             may=max(may,now[i].y+now[i].sz*10);
             }
         double maxh=may-miy,limx; int updo=1;
         fprintf(debug,"maxh: %0.4lf\n",maxh);
         plane=0;
         for (int i=0; i<n/2; i++) {
             tg.pla=plane; tg.sz=gteeth[i];
             if (i==0) {
                tg.x=gteeth[i]*10;
                tg.y=gteeth[i]*10;
                }
                else {
                tg.x=fixed[i*2-1].x;
                tg.y=fixed[i*2-1].y;
                }
             fixed[i*2]=tg;
             redo:
             tg.pla=plane; tg.sz=gteeth[n/2+i];
             if (i==0) {
                tg.x=gteeth[n/2+i]*10;
                double key,hori;
                hori=(gteeth[n/2+i]-gteeth[i])*10;
                key=(gteeth[n/2+i]+gteeth[i])*10-9.999;
                tg.y=fixed[i*2].y+sqrt(key*key-hori*hori);
                if (tg.y+gteeth[n/2+i]*10>maxh) {
                   tg.y=maxh-gteeth[n/2+i]*10;
                   tg.x=fixed[i*2].x+sqrt(key*key-(tg.y-fixed[i*2].y)*(tg.y-fixed[i*2].y));
                   updo=-1;
                   }
                }
                else {
                double key,ovl[200];
                vector<pair<double,double> > cand;
                key=(gteeth[n/2+i]+gteeth[i])*10-9.999;
                //ovl=gteeth[n/2+i]*10+10.1;
                fprintf(debug,"%d: key:%0.4lf ovl:%0.4lf\n",i*2+1,key,ovl);
                for (int j=i-1; j>=0; j--) {
                    double dist;
                    double cx1,cx2,cy1,cy2;
                    cx1=fixed[i*2].x; cy1=fixed[i*2].y;
                    cx2=fixed[j*2].x; cy2=fixed[j*2].y;
                    dist=distance(cx1,cy1,cx2,cy2);
                    ovl[j]=gteeth[n/2+i]*10+10.1;
                    if (j!=0 && fixed[j*2-1].pla==plane) ovl[j]=(gteeth[n/2+i]+fixed[j*2-1].sz)*10+10.1;
                    if (fixed[j*2].pla==plane) ovl[j]=max(ovl[j],(gteeth[n/2+i]+fixed[j*2].sz)*10+10.1);
                    if (j!=0 && i!=n/2-1 && fixed[j*2-1].pla==(plane+1)%K) ovl[j]=max(ovl[j],(gteeth[i+1]+fixed[j*2-1].sz)*10+10.1);
                    fprintf(debug,"%d,%d: key:%0.4lf ovl:%0.4lf\n",i*2+1,j,key,ovl);
                    //fprintf(debug,"*c1:%0.4lf %0.4lf c2:%0.4lf %0.4lf   dist:%0.4lf\n",cx1,cy1,cx2,cy2,dist);
                    if (ovl[j]+key>=dist) {
                       double r1x,r1y,r2x,r2y;
                       r1x=(cx1+cx2)/2 + (cx2-cx1)*(key*key-ovl[j]*ovl[j])/(2*dist*dist) + (cy2-cy1)/(2*dist*dist)*sqrt( ((ovl[j]+key)*(ovl[j]+key)-dist*dist)*(dist*dist-(key-ovl[j])*(key-ovl[j])) );
                       r1y=(cy1+cy2)/2 + (cy2-cy1)*(key*key-ovl[j]*ovl[j])/(2*dist*dist) - (cx2-cx1)/(2*dist*dist)*sqrt( ((ovl[j]+key)*(ovl[j]+key)-dist*dist)*(dist*dist-(key-ovl[j])*(key-ovl[j])) );
                       r2x=(cx1+cx2)/2 + (cx2-cx1)*(key*key-ovl[j]*ovl[j])/(2*dist*dist) - (cy2-cy1)/(2*dist*dist)*sqrt( ((ovl[j]+key)*(ovl[j]+key)-dist*dist)*(dist*dist-(key-ovl[j])*(key-ovl[j])) );
                       r2y=(cy1+cy2)/2 + (cy2-cy1)*(key*key-ovl[j]*ovl[j])/(2*dist*dist) + (cx2-cx1)/(2*dist*dist)*sqrt( ((ovl[j]+key)*(ovl[j]+key)-dist*dist)*(dist*dist-(key-ovl[j])*(key-ovl[j])) );
                       fprintf(debug,"c1:%0.4lf %0.4lf c2:%0.4lf %0.4lf\n",cx1,cy1,cx2,cy2);
                       fprintf(debug,"%d: r1: %0.4lf %0.4lf  r2: %0.4lf %0.4lf\n",j,r1x,r1y,r2x,r2y);
                       if (r1x>cx2 && r1y>=gteeth[n/2+i]*10 && r1y<=maxh-gteeth[n/2+i]*10) cand.push_back(make_pair(r1x,r1y));
                       if (r2x>cx2 && r2y>=gteeth[n/2+i]*10 && r2y<=maxh-gteeth[n/2+i]*10) cand.push_back(make_pair(r2x,r2y));
                       }
                    }
                double rx,ry;
                ry=gteeth[n/2+i]*10; 
                if (key*key-(ry-fixed[i*2].y)*(ry-fixed[i*2].y)>=0) {
                   rx=fixed[i*2].x+sqrt(key*key-(ry-fixed[i*2].y)*(ry-fixed[i*2].y));
                   cand.push_back(make_pair(rx,ry));
                   rx=fixed[i*2].x-sqrt(key*key-(ry-fixed[i*2].y)*(ry-fixed[i*2].y));
                   if (rx>gteeth[n/2+i]*10) cand.push_back(make_pair(rx,ry));
                   }
                ry=maxh-gteeth[n/2+i]*10; 
                if (key*key-(ry-fixed[i*2].y)*(ry-fixed[i*2].y)>=0) {
                   rx=fixed[i*2].x+sqrt(key*key-(ry-fixed[i*2].y)*(ry-fixed[i*2].y));
                   cand.push_back(make_pair(rx,ry));
                   rx=fixed[i*2].x-sqrt(key*key-(ry-fixed[i*2].y)*(ry-fixed[i*2].y));
                   if (rx>gteeth[n/2+i]*10) cand.push_back(make_pair(rx,ry));
                   }
                rx=gteeth[n/2+i]*10;
                ry=fixed[i*2].y+sqrt(key*key-(rx-fixed[i*2].x)*(rx-fixed[i*2].x));
                if (ry-gteeth[n/2+1]*10>=0 && ry+gteeth[n/2+1]*10<=maxh)
                   cand.push_back(make_pair(rx,ry));
                for (int j=0; j<cand.size(); j++) {
                    int ok=1;
                    fprintf(debug,"cand: %0.4lf %0.4lf\n",cand[j].first,cand[j].second);
                    for (int k=i-1; k>=0; k--)
                        if (distance(fixed[k*2].x,fixed[k*2].y,cand[j].first,cand[j].second)<ovl[k]-1e-5) {
                           fprintf(debug,"out: %d   dist:%0.4lf\n",k,distance(fixed[k*2].x,fixed[k*2].y,cand[j].first,cand[j].second));
                           ok=0; break;
                           }
                    if (ok==0) {
                       cand.erase(cand.begin()+j);
                       j--;
                       }
                    }
                tg.x=1e6; tg.y=1e6;
                if (cand.size()==0) {
                   limx=fixed[i*2].x+0.01;
                   fprintf(debug,"limx: %0.4lf\n",limx);
                   plane=(plane+K-1)%K;
                   if (limx>1e6) break; 
                   i--; goto redo;
                   }
                fprintf(debug,"updo: %d\n",updo);
                for (int j=0; j<cand.size(); j++) {
                    fprintf(debug,"okcand: %0.4lf %0.4lf\n",cand[j].first,cand[j].second);    

                    if (updo==1) {
                       if (cand[j].second>fixed[i*2].y || abs(maxh-cand[j].second-gteeth[n/2+i]*10)<1e-4)
                          if (cand[j].first<tg.x && cand[j].first>limx) {
                             tg.x=cand[j].first; tg.y=cand[j].second;
                             }
                       }
                       else {
                       if (cand[j].second<fixed[i*2].y || abs(cand[j].second-gteeth[n/2+i]*10)<1e-4)
                          if (cand[j].first<tg.x && cand[j].first>limx) {
                             tg.x=cand[j].first; tg.y=cand[j].second;
                             }
                       }
                    /*
                    if (cand[j].first<tg.x && cand[j].first>limx) {
                       tg.x=cand[j].first; tg.y=cand[j].second;
                       }
                    */
                    }
                if (abs(maxh-tg.y-gteeth[n/2+i]*10)<200) updo=-1;
                if (abs(tg.y-gteeth[n/2+i]*10)<200) updo=1;
                fprintf(debug,"\n");
                }
             limx=-1;
             fixed[i*2+1]=tg;
             plane=(plane+1)%K;
             }
         //try 5 degree
         cos1=cos(pi/36); sin1=sin(pi/36);
         for (int i=0; i<n; i++) {
             double ox=now[i].x,oy=now[i].y;
             now[i].x=ox*cos1-oy*sin1;
             now[i].y=ox*sin1+oy*cos1;
             }
         cos1=cos(pi/1800); sin1=sin(pi/1800); //0.1 degree
         vector<Gear> prod;
         for (int i=0; i<100; i++) {
             //prod=compress(now);
             if (scoring(prod)<losco) {
                losco=scoring(prod);
                lobest=prod;
                }
             for (int j=0; j<n; j++) {
                 double ox=now[j].x,oy=now[j].y;
                 now[j].x=ox*cos1+oy*sin1;
                 now[j].y=-ox*sin1+oy*cos1;
                 }
             }
         fprintf(debug,"score2*: %0.4lf\n",scoring(lobest));
         lobest=fixed;
         if (scoring(lobest)<highscore) {
            highscore=scoring(lobest);
            best=lobest;
            }
         }
  	  vector <string> place(int Kin, vector <int> Teeth) {
         teeth=Teeth; K=Kin;
         while (teeth[teeth.size()/2-1]==teeth[teeth.size()/2] && teeth.size()>2) {
            teeth.erase(teeth.begin()+teeth.size()/2-1,teeth.begin()+teeth.size()/2+1);
            }
         n=teeth.size();
         best.resize(teeth.size());
         highscore=1e18; best.clear(); now.clear(); ret.clear();
         start();
         
         for (int i=0; i<n; i++)
             fprintf(debug,"%d ",teeth[i]);
         fprintf(debug,"\n");
         fflush(debug);
         //main part
         //approach 1, straight line
         /*
         int bigswap=teeth[n/2];
         for (int i=n/2; i<n-1; i++)
             teeth[i]=teeth[i+1];
         teeth[n-1]=bigswap;
         */
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
         fprintf(debug,"score1: %0.4lf\n",scoring(now));
         if (scoring(now)<highscore) {
            //highscore=scoring(now);
            //best=now;
            }
         //approach 2, overlap
         vector<int> vpara;
         vpara=teeth;
         fitin(vpara);
         vpara=teeth;
         sort(vpara.begin(),vpara.begin()+n/2,cmp);
         fitin(vpara);
         vpara=teeth;
         sort(vpara.begin()+1,vpara.begin()+n/2,cmp);
         fitin(vpara);
         if (best[0].x-best[0].sz*10>best[1].x-best[1].sz*10) {
            vpara=teeth; int limit=vpara[0];
            int maxsz=int((best[0].x-best[0].sz*10-(best[1].x-best[1].sz*10))/20)+vpara[0]+5;
            for (int j=maxsz; j>limit && runtime()<28.5; j--) {
                vpara=teeth;
                for (int i=n/2; i>=0; i--)
                    if (vpara[i]==j) {
                       swap(vpara[i],vpara[0]); break;
                       }
                if (vpara[0]==limit) continue;
                sort(vpara.begin()+1,vpara.begin()+n/2,cmp);
                fitin(vpara);
                }
            }
         int rep=0;
         
         while (runtime()<8.5) {
            random_shuffle(vpara.begin(),vpara.begin()+n/2);
            fitin(vpara);
            rep++;
            }
         
         fprintf(debug,"TRIED: %d\n",rep);
         
         //configure best into string
         for (int i=0; i<n; i++) {
             char cstr[1000];
             fprintf(debug,"#%d: %d %d %0.4lf %0.4lf\n",i,best[i].sz,best[i].pla,best[i].x,best[i].y);
             sprintf(cstr,"%d %d %0.8lf %0.8lf",best[i].sz,best[i].pla,best[i].x,best[i].y);
             string tstr=cstr;
             ret.push_back(tstr);
             //fprintf(debug,"%s\n",cstr);
             }
         return ret;
      }
};

int main () {
    Gearing GG;
    debug=fopen("debug.txt","w");
    result=fopen("result.txt","w");
    
    int KK,NN;
    vector<int> RR;
    scanf("%d%d",&KK,&NN);
    RR.resize(NN);
    for (int i=0; i<NN; i++)
        scanf("%d",&RR[i]);
    vector<string> RET;
    RET=GG.place(KK,RR);
    printf("%d\n",RET.size());
    for (int i=0; i<RET.size(); i++)
        cout << RET[i] << endl;
    fflush(stdout);
    
    /*
    int seed=0,st=1001,til=2000;
    while (seed<til) {
       int KK=rand()%4+3,NN=(rand()%46)*2+10,M=rand()%36+5;
       vector<int> RR; RR.resize(NN);
       for (int i=0; i<NN; i++)
           RR[i]=rand()%(50-M+1)+M;
       sort(RR.begin(),RR.end());
       if (seed>=st && KK<=4) {
          vector<string> RET;
          RET=GG.place(KK,RR);
          vector<Gear> CH; CH.resize(RET.size());
          for (int i=0; i<RET.size(); i++) {
              Gear TG;
              sscanf(RET[i].c_str(),"%d %d %lf %lf",&TG.sz,&TG.pla,&TG.x,&TG.y);
              CH[i]=TG;
              }
          printf("Seed: %d\n",seed);
          fprintf(result,"Seed: %d\n",seed);
          fprintf(result,"K: %d    N: %d     M: %d\n",KK,NN,M);
          fprintf(result,"Score: %0.2lf\n",GG.scoring(CH));
          }
       seed++;
       }
    */
    return 0;
}
