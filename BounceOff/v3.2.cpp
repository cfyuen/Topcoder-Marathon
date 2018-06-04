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
#include<queue>
using namespace std;

template <class INT> string i2s (INT I) { ostringstream OS; OS << I; return OS.str(); }

struct edge {
   int x1,y1,x2,y2;
};
struct coor {
   double x,y;
};
struct state {
   vector<edge> now;
   coor v,b; double time;
   vector<int> visit;
};

class pqcmp {
   //double sco1,sco2;
   public:
      bool operator() (const state &lhs, const state &rhs) const {
           double sco1=pow(0.995,lhs.time)*pow(0.9,lhs.now.size()*1.0);
           double sco2=pow(0.995,rhs.time)*pow(0.9,rhs.now.size()*1.0);
           return sco1<sco2;
      }
};


double eps=1e-9;
int re,n;
vector<string> sret;
vector<coor> tar;
vector<edge> ret,now;
double bestsco=-1.0;
coor init;
priority_queue<state,vector<state>,pqcmp> heu[80];

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

bool cmp (coor a,coor b) {
     return a.y>b.y;
}

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

double dist (coor p1,coor p2) {
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
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
            if (norm(ve) < 1e-6) {
               //stopped, give up
               time = 500;
               break;
               }
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
            //fprintf(result,"%0.2lf,%0.2lf -> %0.2lf %0.2lf\n",b.x,b.y,ve.x,ve.y);
            //fprintf(result,"highest: %0.4lf\n",norm(ve)*norm(ve)/20+b.y);
            }
            else {
            prev = -1;
            }
         if (nhits==tar.size())
            break;
         
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

state aimsim (state nows,int touch) {
     state fail; fail.visit.resize(5); fail.visit[0]=-1;
     int nextt=0;
     vector<edge> edg=nows.now;
     vector<int> visit(80);
     for (int i=0; i<n; i++)
         visit[i]=nows.visit[i];
     /*
     int touch;
     
     for (touch=0; touch<tar.size(); touch++)
         if (visit[touch]==0) break;
     */
     //fprintf(result,"touch: %d  %0.0lf %0.0lf\n",touch,tar[touch].x,tar[touch].y); fflush(result);
     if (edg.size()>100) {
        fprintf(result,"More than 100\n");
        return fail;
        }
     for (int i=0; i<edg.size(); i++) {
         if (edg[i].x1<1 || edg[i].x1>499 || edg[i].x2<1 || edg[i].x2>499) {
            fprintf(result,"x-coordinates exceeded\n");
            return fail;
            }
         if (edg[i].y1<1 || edg[i].y1>499 || edg[i].y2<1 || edg[i].y2>499) {
            fprintf(result,"y-coordinates exceeded\n");
            return fail;
            }
         //check that no obstacle has 0 length 
         if (edg[i].x1==edg[i].x2 && edg[i].y1==edg[i].y2) {
            fprintf(result,"0 length\n");
            return fail;
            }
         }
     for (int i=1; i<edg.size(); i++)
         for (int j=0; j<i; j++) {
            //check that the obstacles don't intersect
            if (intersect(edg[i],edg[j])==1) {
               //fprintf(result,"Obstacles %d and %d must not intersect.\n",i,j);
               fail.visit[0]=-2;
               return fail;
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
         //can only check touch point
         for (int i = 0; i<tar.size(); i++) {
             if (visit[i]) continue;
             //solve with cubic equation
             //fprintf(debug,"tar: %d (%0.2lf , %0.2lf)\n(%0.2lf , %0.2lf) | (%0.2lf , %0.2lf)\n",i,tar[i].x,tar[i].y,b.x,b.y,ve.x,ve.y);
             if (ve.x!=0) {
                double A = (-10) / (2*ve.x*ve.x);
                double B = abs(ve.y/ve.x);
                double C = b.y;
                //fprintf(debug,"qua: %0.4lf %0.4lf %0.4lf -> (%0.2lf,%0.2lf)\n",A,B,C,b.x,A*b.x*b.x+B*b.x+C);
                double tB = -2*b.x*A-B;
                double tC = A*b.x*b.x+B*b.x+C;
                B=tB; C=tC;
                //fprintf(debug,"qua: %0.4lf %0.4lf %0.4lf -> (%0.2lf,%0.2lf)\n",A,B,C,b.x,A*b.x*b.x+B*b.x+C);
                if (ve.x*ve.y>0) {
                   tB = -4*b.x*A-B;
                   tC = 4*A*b.x*b.x+2*B*b.x+C;
                   B=tB; C=tC;
                   }
                //fprintf(debug,"qua: %0.4lf %0.4lf %0.4lf -> (%0.2lf,%0.2lf)\n",A,B,C,b.x,A*b.x*b.x+B*b.x+C);
                double ca = 2*A*A;
                double cb = 3*A*B;
                double cc = B*B+2*A*C-2*A*tar[i].y+1;
                double cd = B*C-B*tar[i].y-tar[i].x;
                //ca=2; cb=-4; cc=-22; cd=24;
                //fprintf(debug,"cub: %0.6lf %0.6lf %0.4lf %0.2lf\n",ca,cb,cc,cd);
                
                double F = ((3*cc/ca)-(cb*cb/ca/ca)) / 3;
                double G = ((2*cb*cb*cb/ca/ca/ca)-(9*cb*cc/ca/ca)+(27*cd/ca)) / 27;
                double H = (G*G/4)+(F*F*F/27);
                //fprintf(debug,"FGH: %0.4lf %0.4lf %0.4lf\n",F,G,H);
                double t2;
                if (H>0) { // 1 real root
                   double R = -G/2+pow(H,0.5);
                   double S = pow(abs(R),1/3.0);
                   if (R<0) S=-S;
                   double T = -G/2-pow(H,0.5);
                   double U = pow(abs(T),1/3.0);
                   if (T<0) U=-U;
                   //fprintf(debug,"RSTU: %0.4lf %0.4lf %0.4lf %0.4lf\n",R,S,T,U);
                   coor r;
                   r.x = S+U-cb/3/ca;
                   r.y = A*r.x*r.x+B*r.x+C;
                   //fprintf(debug,"root: %0.2lf %0.2lf\n",r.x,r.y);
                   if (dist(r,tar[i])<re) t2 = (r.x-b.x)/ve.x;
                      else continue;
                   }
                   else if (F==0 && G==0 && H==0) { // all equal root
                   coor r;
                   r.x = -pow(cd/ca,1/3.0);
                   r.y = A*r.x*r.x+B*r.x+C;
                   //fprintf(debug,"root: %0.2lf %0.2lf\n",r.x,r.y);
                   if (dist(r,tar[i])<re) t2 = (r.x-b.x)/ve.x;
                      else continue;
                   }
                   else { // 3 real root
                   double I = pow((G*G/4-H),0.5);
                   double L = -pow(abs(I),1/3.0);
                   if (I<0) L=-L;
                   double K = acos(-G/2/I);
                   double M = cos(K/3);
                   double Q = sqrt(3.0)*sin(K/3);
                   double P = -(cb/3/ca);
                   coor r1,r2,r3;
                   r1.x = 2*-L*M+P; r1.y = A*r1.x*r1.x+B*r1.x+C;
                   r2.x = L*(M+Q)+P; r2.y = A*r2.x*r2.x+B*r2.x+C;
                   r3.x = L*(M-Q)+P; r3.y = A*r3.x*r3.x+B*r3.x+C;
                   //fprintf(debug,"root1: %0.2lf %0.2lf\n",r1.x,r1.y);
                   //fprintf(debug,"root2: %0.2lf %0.2lf\n",r2.x,r2.y);
                   //fprintf(debug,"root3: %0.2lf %0.2lf\n",r3.x,r3.y);
                   t2=-1;
                   if (dist(r1,tar[i])<re) t2 = (r1.x-b.x)/ve.x;
                   if ((dist(r2,tar[i])<re) && (t2<0 || (r2.x-b.x)/ve.x<t2)) t2 = (r2.x-b.x)/ve.x;
                   if ((dist(r3,tar[i])<re) && (t2<0 || (r3.x-b.x)/ve.x<t2)) t2 = (r3.x-b.x)/ve.x;
                   }
                //fprintf(debug,"t2: %0.4lf\n",t2);
                if (t2 < hitt && t2 > 0) {
                    point = 1;
                    hitp.x=-10; hitp.y=-10;
                    hitt = t2;
                    target = i;
                    //fprintf(debug,"target = %d\n",i);
                    }
                }
                else { // a vertical line
                double t2=-1;
                if (abs(tar[i].x-b.x)>re) continue;
                double my1=tar[i].y+sqrt(re*re-(tar[i].x-b.x)*(tar[i].x-b.x));
                double my2=tar[i].y-sqrt(re*re-(tar[i].x-b.x)*(tar[i].x-b.x));
                double det=ve.y*ve.y-4*5*(my1-b.y);
                if (det>=0) {
                   double ta=(-ve.y+sqrt(det)) / -10;
                   double tb=(-ve.y-sqrt(det)) / -10;
                   if (ta*tb>0) t2=min(ta,tb);
                      else t2=max(ta,tb);
                   }
                //fprintf(debug,"t2: %0.4lf\n",t2);
                if (t2 < hitt && t2 > 0) {
                    point = 1;
                    hitp.x=-10; hitp.y=-10;
                    hitt = t2;
                    target = i;
                    }
                det=ve.y*ve.y-4*5*(my2-b.y);
                if (det>=0) {
                   double ta=(-ve.y+sqrt(det)) / -10;
                   double tb=(-ve.y-sqrt(det)) / -10;
                   if (ta*tb>0) t2=min(ta,tb);
                      else t2=max(ta,tb);
                   }
                //fprintf(debug,"t2: %0.4lf\n",t2);
                if (t2 < hitt && t2 > 0) {
                    point = 1;
                    hitp.x=-10; hitp.y=-10;
                    hitt = t2;
                    target = i;
                    }
                }
             //treat it as edge
             /*
             for (int j=0; j<segment; j++) {
                 double A = aim[i*segment+j].y1 - aim[i*segment+j].y2;
                 double B = aim[i*segment+j].x2 - aim[i*segment+j].x1;
                 double C = A * aim[i*segment+j].x1 + B * aim[i*segment+j].y1;
                 double qa = -5 * B;
                 double qb = A * ve.x + B * ve.y;
                 double qc = A*b.x + B * b.y-C;
                 double det = qb*qb-4*qa*qc;
                 if (det < 0) { // no solution, doesn't get there
                    continue;
                    }
                 double t1,t2, apogee;
                 if (qa == 0) {//vertical line, solve qb t = -qc
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
                 if (t1 > 0 && t1 < hitt && distpe(aim[i*segment+j],h1) < eps) {
                    point = 1;
                    hitp.x=-10; hitp.y=-10;
                    hitt = t1;
                    target = i;
                    }
                 if (t2 > 0 && t2 < hitt && distpe(aim[i*segment+j],h2) < eps) {
                    point = 1;
                    hitp.x=-10; hitp.y=-10;
                    hitt = t2;
                    target = i;
                    }
                 }
             */
             /*
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
             */
             }
         //fprintf(debug,"point:%d(%d)  hitt:%0.2lf+%0.2lf\n",point,target,time,hitt);
         
         if (hitp.x>-1 && hitp.y>-1) {
            for (int i=0; i<tar.size(); i++)
                if (visit[i]==0 && dist(hitp,tar[i])<re) {
                   point = 1;
                   hitp.x=-10; hitp.y=-10;
                   hitt = hitt-0.01;
                   target = i;
                   }
            }
         
         //adjust position and velocity
         b.x += ve.x * hitt;
         b.y += ve.y * hitt - 5 * hitt*hitt;
         ve.y -= 10 * hitt;
         time += hitt;
         if (point==1) {
            visit[target] = 1;
            while (visit[nextt]==1 && nextt<touch) nextt++;
            //fprintf(result,"(%d %d)\n",(int)tar[target].x,(int)tar[target].y);
            //fprintf(debug,"%0.4lf,%0.4lf time:  %0.2lf\n",b.x,b.y,time);
            if (target==touch) {
               nows.visit=visit;
               nows.time=time;
               for (int j=0; j<nows.now.size(); j++)
                   fprintf(debug,"%d %d -> %d %d\n",nows.now[j].x1,nows.now[j].y1,nows.now[j].x2,nows.now[j].y2);
               for (int j=0; j<n; j++) fprintf(debug,"%d",visit[j]);
               fprintf(debug,"\nSuccess %d!   time:%0.4lf\n",touch,time);
               return nows;
               }
            nhits++;
            }
         if (hitp.x>-1 && hitp.y>-1) {
            if (norm(ve) < 1e-6) {
               //stopped, give up
               break;
               }
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
            if (norm(ve)*norm(ve)/20+b.y<tar[touch].y-re) {
               //fprintf(debug,"Too low: %0.4lf/%0.4lf\n",norm(ve)*norm(ve)/20+b.y,tar[touch].y-re);
               //for (int j=0; j<n; j++) fprintf(debug,"%d",visit[j]);
               //fprintf(debug,"\n");
               return fail;
               }
            }
            else {
            prev = -1;
            }
         
         }
     return fail;
}

void pushstate (state st,int minst) {
    int j;
    for (j=0; j<n; j++)
        if (st.visit[j]==0) break;
    //fprintf(debug,"j:%d\n",j);
    if (j<=minst) return;
    if (j!=n) heu[j].push(st);
       else {
       //can optimize
       double tsco=simulation(st.now);
       if (tsco>bestsco) {
          bestsco=tsco;
          ret=st.now;
          }
       }
}

class BounceOff {
   public:
      vector <string> placeObstacles(vector <int> oX, vector <int> oY, int R) {
         init.x=oX[0]; init.y=oY[0];
         tar.resize(oX.size()-1);
         for (int i=1; i<oX.size(); i++) {
             tar[i-1].x=oX[i]; tar[i-1].y=oY[i]; 
             }
         n=tar.size();
         sort(tar.begin(),tar.end(),cmp);
         bestsco=-1.0; sret.clear(); ret.clear();
         re=R;
         
         vector<state> vst;
         for (int i=0; i<2000; i++) {
             edge te; now.clear();
             te.x1=max(1,(int)(init.x-rand()%200-1)); te.y1=(int)(init.y-rand()%290);
             te.x2=min(499,(int)(init.x+rand()%200+1)); te.y2=(int)(init.y-rand()%290);
             now.push_back(te);
             state news; news.now=now; news.visit.resize(80);
             news=aimsim(news,0);
             if (news.visit[0]!=-1) {
                pushstate(news,0);
                }
             }
         fprintf(debug,"first placement done\n");
         for (int i=1; i<n; i++) {
             int maxlimit=20;
             fprintf(debug,"nextpoint: %d\n",i);
             state news,tst;
             if (!heu[i].empty() && bestsco<0) ret=heu[i].top().now;
             while (maxlimit>0 && !heu[i].empty()) {
                news=heu[i].top();
                fprintf(debug,"sz: %d\n",heu[i].size());
                fprintf(debug,"obs:%d  time:%0.4lf\n",news.now.size(),news.time);
                //for (int j=0; j<news.now.size(); j++)
                //    fprintf(debug,"%d %d -> %d %d\n",news.now[j].x1,news.now[j].y1,news.now[j].x2,news.now[j].y2);
                fflush(debug);
                heu[i].pop();
                tst=aimsim(news,i);
                if (tst.visit[0]!=-1) {
                   fprintf(debug,"next state\n");
                   pushstate(tst,i);
                   }
                   else {
                   
                   for (int k=0; k<100; k++) {
                       tst=news;
                       edge te; now.clear();
                       te.x1=max(1,(int)(rand()%500-1)); te.y1=(int)(init.y-rand()%490);
                       te.x2=min(499,(int)(rand()%500+1)); te.y2=(int)(init.y-rand()%490);
                       tst.now.push_back(te);
                       //for (int m=0; m<tst.now.size(); m++)
                       //    fprintf(debug,"%d %d -> %d %d\n",tst.now[m].x1,tst.now[m].y1,tst.now[m].x2,tst.now[m].y2);
                       for (int m=0; m<n; m++) {
                           tst.visit[m]=0;
                           }
                       tst=aimsim(tst,i);
                       
                       
                       
                       //simulation(tst.now);
                       if (tst.visit[0]!=-1) {
                          fprintf(debug,"add line!!\n");
                          pushstate(tst,i);
                          }
                       }
                   
                   }
                maxlimit--;
                }
             }
         /* 
         fprintf(debug,"success: %d\n",vst.size());
         for (int i=0; i<vst.size(); i++) {
             double tsco=simulation(vst[i].now);
             if (tsco>bestsco) {
                bestsco=tsco;
                ret=vst[i].now;
                }
             }
         */
         //change to string
         
         for (int i=0; i<min(100,(int)ret.size()); i++) {
             string str="";
             str+=i2s(ret[i].x1)+" "+i2s(ret[i].y1)+" "+i2s(ret[i].x2)+" "+i2s(ret[i].y2);
             sret.push_back(str);
             }
         for (int i=0; i<min(100,(int)ret.size()); i++)
             fprintf(debug,"%d %d -> %d %d\n",ret[i].x1,ret[i].y1,ret[i].x2,ret[i].y2);
         /*
         string str;
         str="219 221 499 203";
         sret.push_back(str);
         str="64 124 321 78";
         sret.push_back(str);
         str="466 300 420 375";
         sret.push_back(str);
         */
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
