#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<cmath>
#include<sstream>
using namespace std;

template <class INT> string i2s (INT I) { ostringstream OS; OS << I; return OS.str(); }

struct edge {
   int x1,y1,x2,y2;
};
struct coor {
   double x,y;
};

double eps=1e-9;
int re;
vector<string> sret;
vector<coor> tar;
vector<edge> ret,now;
double bestsco=-1.0;
coor init;

//delete when submit
FILE *debug,*result;
int local=1,final=0;

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


coor mult (coor c,double mul) {
    c.x*=mul; c.y*=mul;
    return c;
}

coor add (coor p1,coor p2) {
    coor cr;
    cr.x=p1.x+p2.x; cr.y=p1.y+p2.y;
    return cr;
}

coor substr (coor p1,coor p2) {
    coor cr;
    cr.x=p1.x-p2.x; cr.y=p1.y-p2.y;
    return cr;
}

double norm (coor c) {
    return sqrt(c.x*c.x+c.y*c.y);
}

double dot (coor p1,coor p2) {
    return p1.x*p2.x+p1.y*p2.y;
}

int intersect (edge a,edge b) {
    int A1 = a.y1 - a.y2;
    int B1 = a.x2 - a.x1;
    int C1 = A1 * a.x1 + B1 * a.y1;
    int A2 = b.y1 - b.y2;
    int B2 = b.x2 - b.x1;
    int C2 = A2 * b.x1 + B2 * b.y1;
    int det=A1*B2-A2*B1;
    if (det==0) return 0;
    double x,y;
    x=(B2*C1-B1*C2)*1.0/det;
    y=(A1*C2-A2*C1)*1.0/det;
    //fprintf(result,"%0.4lf %0.4lf\n",x,y);
    if ((A1==0 && B2==0) || (A1==0 && B2==0)) {
       if (min(a.x1,a.x2)>max(b.x1,b.x2)) return 0;
       if (max(a.x1,a.x2)<min(b.x1,b.x2)) return 0;
       if (min(a.y1,a.y2)>max(b.y1,b.y2)) return 0;
       if (max(a.y1,a.y2)<min(b.y1,b.y2)) return 0;
       return 1;
       }
    if (A1==0 || B1==0) {
       if (min(a.y1,a.y2)>y || max(a.y1,a.y2)<y) return 0;
       if (min(b.y1,b.y2)>y || max(b.y1,b.y2)<y) return 0;
       return 1;
       }
    if (A1!=0 && B1!=0) {
       if (min(a.x1,a.x2)>x || max(a.x1,a.x2)<x) return 0;
       if (min(b.x1,b.x2)>x || max(b.x1,b.x2)<x) return 0;
       return 1;
       }
    return 0;
}

double distpe (edge e,coor p) {
    coor vect;
    vect.x=e.x2-e.x1; vect.y=e.y2-e.y1;
    coor p1; p1.x=e.x1; p1.y=e.y1;
    coor p2; p2.x=e.x2; p2.y=e.y2;
    //distance from p to the obstacle
    if (dot(vect, substr(p, p1))<=0)
       return norm(substr(p, p1));			//from p to p1
    if (dot(vect, substr(p, p2))>=0)
       return norm(substr(p, p2));			//from p to p2
    //distance to the line itself
    return abs(-vect.y*p.x+vect.x*p.y+p1.x*p2.y-p1.y*p2.x)/norm(vect);

}


double simulation (vector<edge> edg) {
     if (edg.size()>100) {
        fprintf(result,"More than 100\n");
        return 0.0;
        }
     for (int i=0; i<edg.size(); i++) {
         if (edg[i].x1<1 || edg[i].x1>499 || edg[i].x2<1 || edg[i].x2>499) {
            fprintf(result,"x-coordinates exceeded\n");
            return 0.0;
            }
         if (edg[i].y1<1 || edg[i].y1>499 || edg[i].y2<1 || edg[i].y2>499) {
            fprintf(result,"y-coordinates exceeded\n");
            return 0.0;
            }
         //check that no obstacle has 0 length 
         if (edg[i].x1==edg[i].x2 && edg[i].y1==edg[i].y2) {
            fprintf(result,"0 length\n");
            return 0.0;
            }
         }
     for (int i=1; i<edg.size(); i++)
         for (int j=0; j<i; j++) {
            //check that the obstacles don't intersect
            if (intersect(edg[i],edg[j])==1) {
               fprintf(result,"Obstacles %d and %d must not intersect.\n",i,j);
               return 0.0;
            }
         }

     edge te; 
     te.x1=0; te.y1=0; te.x2=0; te.y2=500;
     edg.push_back(te);
     te.x1=0; te.y1=0; te.x2=500; te.y2=0;
     edg.push_back(te);
     te.x1=500; te.y1=0; te.x2=500; te.y2=500;
     edg.push_back(te);
     te.x1=0; te.y1=500; te.x2=500; te.y2=500;
     edg.push_back(te);
     
     coor b; b=init;
     coor ve;  ve.x=ve.y=0.0; //ball's velocity
     coor vn,vt;				//normal and tangential components of velocity
     coor hitp;
     double hitt;
     double time = 0.0;
     int target, hi;
     int visit[80]; memset(visit,0,sizeof(visit));
     int nhits = 0;
     int prev = -1;
     int bounces = 0;
     while (time < 500 && bounces < 100000){
         hitp.x=-10; hitp.y=-10;
         hitt = 500-time;
         target = -1;
         hi = -1;
         for (int i = 0; i<edg.size(); i++) {
             double A = edg[i].y1 - edg[i].y2;
             double B = edg[i].x2 - edg[i].x1;
             double C = A * edg[i].x1 + B * edg[i].y1;
             //segment is A x + B y = C
             double qa = -5 * B;
             double qb = A * ve.x + B * ve.y;
             double qc = A*b.x + B * b.y-C;
             //solve: qa t^2 + qb t + qc == 0
             double det = qb*qb-4*qa*qc;
             if (det < 0) { // no solution, doesn't get there
                if (i == prev){
                   //this can only happen when numerics break down
                   //just bounced off this surface, should be able to hit line again
                   time = 500;
                   break;
                   }
                continue;
                }
             double t1,t2, apogee;
             if (qa == 0) {//vertical line, solve qb t = -qc
                if (i == prev) continue;//can't bounce off a vertical twice in a row
                t1 = t2 = -qc/qb;
                apogee = 0;
                }
                else {//hits line twice, we want first one greater than 0
                t1 = (-qb - sqrt(qb*qb-4*qa*qc)) / 2 / qa;
                t2 = (-qb + sqrt(qb*qb-4*qa*qc)) / 2 / qa;
                if (t2 < t1) {
                   swap(t1,t2);
                   }
                apogee = -qb/2/qa;
                }

             double x1 = b.x + t1 * ve.x;
             double x2 = b.x + t2 * ve.x;
             double y1 = b.y + t1 * ve.y - 5 * t1*t1;
             double y2 = b.y + t2 * ve.y - 5 * t2*t2;
             coor h1; h1.x=x1; h1.y=y1;
             coor h2; h2.x=x2; h2.y=y2;

             if (t1 > 0 && i != prev && t1 < hitt && distpe(edg[i],h1) < eps) {
                //Have to be careful not to bounce on the wall we just bounced off of due to floating point issues
                //Note that we count hitting the edge as hitting
                hitp = h1;
                hitt = t1;
                hi = i;
                }
             if (t2 > 0 && (i != prev || apogee > 0) && t2 < hitt && distpe(edg[i],h2) < eps) {
                //here we might bounce off the same segment again, but this is only allowed if apogee > 0
                hitp = h2;
                hitt = t2;
                hi = i;
                }
             }
         int point = 0;
         for (int i = 0; i<tar.size(); i++) {
             if (visit[i]) continue;
             //solve with newton's method
             //A t^4 + B t^3 + C t^2 + D t + E = 0
             double A = 25;
             double B = -10 * ve.y;
             double C = ve.x*ve.x+ve.y*ve.y-10*(b.y-tar[i].y);
             double D = 2*ve.x*(b.x-tar[i].x) + 2*ve.y*(b.y-tar[i].y);
             double E = (b.x-tar[i].x)*(b.x-tar[i].x) + (b.y-tar[i].y)*(b.y-tar[i].y) - re*re;
             double t, t2;
             //seems a bit fragile, but should converge to roots when started on one end or the other
             //start in the middle too, just to be safe
             //hitt <= 10
             for (t = 0; t<hitt+1; t++) {
                 t2 = t;
                 double d = 0, dd = 0;
                 for (int j = 0; j<20; j++) {
                     d = A * t2*t2*t2*t2 + B * t2*t2*t2 + C * t2*t2 + D*t2 + E;
                     dd = 4 * A * t2*t2*t2 + 3 * B * t2 * t2 + 2 * C * t2 + D;
                     t2 -= d / dd;
                     }
                 if (abs(d)<eps && t2 < hitt && t2 > 0) {
                    point = 1;
                    hitp.x=-10; hitp.y=-10;
                    hitt = t2;
                    target = i;
                    }
                 }
             }
         
         //adjust position and velocity
         b.x += ve.x * hitt;
         b.y += ve.y * hitt - 5 * hitt*hitt;
         ve.y -= 10 * hitt;
         time += hitt;
         if (point==1) {
            visit[target] = true;
            //fprintf(result,"(%d %d)\n",(int)tar[target].x,(int)tar[target].y);
            //fprintf(result,"%0.4lf,%0.4lf :  %0.2lf\n",b.x,b.y,time);
            nhits++;
            }
         if (hitp.x>-1 && hitp.y>-1) {
            bounces++;
            //fprintf(result,"%0.4lf,%0.4lf\n",hitp.x,hitp.y);
            prev = hi;
            //find normal and tangential components of velocity BEFORE reflection
            //reflection multiplies normal component by -0.99 (for energy losses)
            coor vect; vect.x=edg[hi].x2-edg[hi].x1; vect.y=edg[hi].y2-edg[hi].y1; 
            vt = mult(vect, dot(ve, vect)/dot(vect, vect));
            vn = substr(ve, vt);
            vn = mult(vn, -0.99);
            //new velocity
            ve = add(vn, vt);
            }
            else {
            prev = -1;
            }
         if (nhits==tar.size())
            break;
         if (norm(ve) < 1e-6) {
            //stopped, give up
            time = 500;
            break;
            }
         }
     if (bounces == 100000) time = 500;
     //score calculation
     double sco;
     if (nhits==tar.size()) sco=2;
        else sco=nhits*1.0/tar.size();
     sco*=pow(0.995,time);
     sco*=pow(0.9,(edg.size()-4)*1.0);
     if (final==1) {
        fprintf(result,"Targets hit: %d of %d\n",nhits,tar.size());
        fprintf(result,"Time taken: %0.2lf seconds\n",time);
        fprintf(result,"Total obstacles: %d\n",(edg.size()-4));
        fprintf(result,"Final Position: ( %0.3lf, %0.3lf )\n",b.x,b.y);
        fprintf(result,"Score = %0.6lf\n",sco);
        printf("Score = %0.6lf\n",sco);
        fflush(result);
        }
        else {
        fprintf(debug,"Targets hit: %d of %d\n",nhits,tar.size());
        fprintf(debug,"Time taken: %0.2lf seconds\n",time);
        fprintf(debug,"Total obstacles: %d\n",(edg.size()-4));
        fprintf(debug,"Final Position: ( %0.3lf, %0.3lf )\n",b.x,b.y);
        fprintf(debug,"Score = %0.6lf\n",sco);
        fflush(debug);
        }
     return sco;
}

class BounceOff {
   public:
      vector <string> placeObstacles(vector <int> oX, vector <int> oY, int R) {
         init.x=oX[0]; init.y=oY[0];
         tar.resize(oX.size()-1);
         for (int i=1; i<oX.size(); i++) {
             tar[i-1].x=oX[i]; tar[i-1].y=oY[i]; 
             }
         bestsco=-1.0; ret.clear(); sret.clear();
         re=R;
         
         for (int i=0; i<200; i++) {
             edge te; now.clear();
             te.x1=max(1,(int)(init.x-rand()%100-1)); te.y1=(int)(init.y-rand()%400);
             te.x2=min(499,(int)(init.x+rand()%100+1)); te.y2=(int)(init.y-rand()%400);
             now.push_back(te);
             double tsco=simulation(now);
             if (tsco>bestsco) {
                bestsco=tsco;
                ret=now;
                }
             }
         for (int i=0; i<0; i++) {
             edge te; now.clear();
             te.x1=max(1,(int)(init.x-rand()%100-1)); te.y1=(int)(init.y-rand()%400);
             te.x2=min(499,(int)(init.x+rand()%100+1)); te.y2=(int)(init.y-rand()%400);
             now.push_back(te);
             te.x1=max(1,(int)(rand()%500)); te.y1=(int)(init.y-rand()%400);
             te.x2=min(499,(int)(rand()%500+1)); te.y2=(int)(init.y-rand()%400);
             now.push_back(te);
             if (intersect(now[0],now[1])==1) {
                //i--;
                continue;
                }
             //fprintf(result,"%d %d -> %d %d\n",now[0].x1,now[0].y1,now[0].x2,now[0].y2);
             //fprintf(result,"%d %d -> %d %d\n",now[1].x1,now[1].y1,now[1].x2,now[1].y2);
             //fprintf(result,"%d\n",intersect(now[0],now[1]));
             double tsco=simulation(now);
             if (tsco>bestsco) {
                bestsco=tsco;
                ret=now;
                }
             }
         for (int i=0; i<ret.size(); i++) {
             string str="";
             str+=i2s(ret[i].x1)+" "+i2s(ret[i].y1)+" "+i2s(ret[i].x2)+" "+i2s(ret[i].y2);
             sret.push_back(str);
             }
         simulation(ret);
         return sret;
      }
};

int main () {
    BounceOff BO;
    debug=fopen("debug.txt","w");
    result=fopen("result.txt","w");
    if (local==0) {
    int N,RR;
    vector<int> OBX,OBY;
    scanf("%d",&N);
    OBX.resize(N); OBY.resize(N);
    for (int i=0; i<N; i++)
        cin >> OBX[i];
    for (int i=0; i<N; i++)
        cin >> OBY[i];
    scanf("%d",&RR);
    vector<string> RET;
    RET=BO.placeObstacles(OBX,OBY,RR);
    printf("%d\n",RET.size());
    for (int i=0; i<RET.size(); i++)
        cout << RET[i] << endl;
    fflush(stdout);
    }
    else {
    int seed;
    int N,RR;
    freopen("seed.txt","r",stdin);
    vector<int> OBX,OBY;
    while (scanf("%d",&seed)==1) {
        int N,RR;
        vector<int> OBX,OBY;
        scanf("%d",&N);
        OBX.resize(N); OBY.resize(N);
        for (int i=0; i<N; i++)
            cin >> OBX[i];
        for (int i=0; i<N; i++)
            cin >> OBY[i];
        scanf("%d",&RR);
        vector<string> RET;
        final=0;
        printf("Seed = %d\n",seed);
        RET=BO.placeObstacles(OBX,OBY,RR);
        final=1;
        fprintf(result,"Seed = %d\n",seed);
        simulation(ret);
        fprintf(result,"\n");
        }
    }
    return 0;
}
