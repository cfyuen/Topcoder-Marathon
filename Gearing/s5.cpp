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

int n,plane,K;
Gear tg;
vector<int> teeth;
vector<Gear> best,now;
vector<string> ret;
double pi=acos(-1.0),highscore=1e18;

bool cmp (int a,int b) {
    return a>b;
}

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
         if (maxx-minx<maxy-miny) return 1e10;
         for (int i=0; i<n; i++)
             for (int j=i+2; j<min(n,i+12); j++) {
                 if (abs(vg[i].x-vg[j].x) + abs(vg[i].y-vg[j].y) <0.1) continue;
                 if (vg[i].pla!=vg[j].pla) continue;
                 double actdis=sqrt((vg[i].x-vg[j].x)*(vg[i].x-vg[j].x)+(vg[i].y-vg[j].y)*(vg[i].y-vg[j].y));
                 if (actdis<10*(vg[i].sz+vg[j].sz)) return 1e12;
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
            if (scoring(newg)<scoring(gteeth)) { exit=0; gteeth=newg; }
            } while (exit==0);
         return gteeth;
         }
      void fitin (vector<int> gteeth) {
         plane=0;
         now.clear(); now.resize(n);
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
         //here, try 3 or more
         now=lobest;
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
             prod=compress(now);
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
         
         //main part
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
         while (runtime()<28.5) {
            random_shuffle(vpara.begin(),vpara.begin()+n/2);
            fitin(vpara);
            rep++;
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
