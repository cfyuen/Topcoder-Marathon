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
#include<set>
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
   public:
      bool operator() (const state &lhs, const state &rhs) const {
           double sco1=pow(0.995,lhs.time)*pow(0.9,lhs.now.size()*1.0);
           double sco2=pow(0.995,rhs.time)*pow(0.9,rhs.now.size()*1.0);
           return sco1<sco2;
      }
};

class setcmp {
   public:
      bool operator() (const state &lhs, const state &rhs) const {
           double sco1=pow(0.995,lhs.time)*pow(0.9,lhs.now.size()*1.0);
           double sco2=pow(0.995,rhs.time)*pow(0.9,rhs.now.size()*1.0);
           return sco1>sco2;
      }
};

double eps=1e-9;
double tle[110],minpoint=0.06,tleout=0;
int re,n,height=0;
vector<string> sret;
vector<coor> tar;
vector<edge> ret,now;
vector<double> pqshade;
double bestsco=-1.0;
coor init;
priority_queue<state,vector<state>,pqcmp> heu[80];
set<state,setcmp> valid;

//delete when submit
FILE *debug,*result;
int local=0,final=0,computer=1;

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
bool backrev (double a,double b) {
     return a>b;
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
        return 0.0;
        }
     for (int i=0; i<edg.size(); i++) {
         if (edg[i].x1<1 || edg[i].x1>499 || edg[i].x2<1 || edg[i].x2>499) {
            return 0.0;
            }
         if (edg[i].y1<1 || edg[i].y1>499 || edg[i].y2<1 || edg[i].y2>499) {
            return 0.0;
            }
         //check that no obstacle has 0 length 
         if (edg[i].x1==edg[i].x2 && edg[i].y1==edg[i].y2) {
            return 0.0;
            }
         }
     for (int i=1; i<edg.size(); i++)
         for (int j=0; j<i; j++) {
            //check that the obstacles don't intersect
            if (intersect(edg[i],edg[j])==1) {
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
            nhits++;
            }
         if (hitp.x>-1 && hitp.y>-1) {
            if (norm(ve) < 1e-6) {
               //stopped, give up
               time = 500;
               break;
               }
            bounces++;
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

void pushstate (state st,int minst) {
    int j;
    for (j=0; j<n; j++)
        if (st.visit[j]==0) break;
    if (j<=minst) return;
    if (j!=n) {
       heu[j].push(st);
       pqshade.push_back(2*pow(0.995,st.time)*pow(0.9,st.now.size()*1.0));
       }
       else {
       //can optimize
       double tsco=simulation(st.now);
       if (tsco>bestsco) {
          bestsco=tsco;
          ret=st.now;
          }
       }
}

state aimsim (state nows,int touch) {
     state fail; fail.visit.resize(5); fail.visit[0]=-1;
     int nextt=0;
     vector<edge> edg=nows.now;
     vector<int> visit(80);
     for (int i=0; i<n; i++)
         visit[i]=nows.visit[i];
     if (edg.size()>100) {
        return fail;
        }
     for (int i=0; i<edg.size(); i++) {
         if (edg[i].x1<1 || edg[i].x1>499 || edg[i].x2<1 || edg[i].x2>499) {
            return fail;
            }
         if (edg[i].y1<1 || edg[i].y1>499 || edg[i].y2<1 || edg[i].y2>499) {
            return fail;
            }
         //check that no obstacle has 0 length 
         if (edg[i].x1==edg[i].x2 && edg[i].y1==edg[i].y2) {
            return fail;
            }
         }
     for (int i=1; i<edg.size(); i++)
         for (int j=0; j<i; j++) {
            //check that the obstacles don't intersect
            if (intersect(edg[i],edg[j])==1) {
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
     while (time < 500 && bounces < 200){
         hitp.x=-10; hitp.y=-10;
         hitt = 500-time;
         target = -1;
         hi = -1;
         double A,B,C,qa,qb,qc,det,t1,t2,apogee,x1,x2,y1,y2;
         for (int i = 0; i<edg.size(); i++) {
             A = edg[i].y1 - edg[i].y2;
             B = edg[i].x2 - edg[i].x1;
             C = A * edg[i].x1 + B * edg[i].y1;
             //segment is A x + B y = C
             qa = -5 * B;
             qb = A * ve.x + B * ve.y;
             qc = A*b.x + B * b.y-C;
             //solve: qa t^2 + qb t + qc == 0
             det = qb*qb-4*qa*qc;
             if (det < 0) { // no solution, doesn't get there
                if (i == prev){
                   //this can only happen when numerics break down
                   //just bounced off this surface, should be able to hit line again
                   time = 500;
                   break;
                   }
                continue;
                }
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

             x1 = b.x + t1 * ve.x;
             x2 = b.x + t2 * ve.x;
             y1 = b.y + t1 * ve.y - 5 * t1*t1;
             y2 = b.y + t2 * ve.y - 5 * t2*t2;
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
         double tB,tC,ca,cb,cc,cd,F,G,H,R,S,T,U,I,L,K,M,Q,P;
         for (int i = 0; i<tar.size(); i++) {
             if (visit[i]) continue;
             if (i>=height && touch<n) break;
             //solve with cubic equation
             if (ve.x!=0) {
                A = (-10) / (2*ve.x*ve.x);
                B = abs(ve.y/ve.x);
                C = b.y;
                tB = -2*b.x*A-B;
                tC = A*b.x*b.x+B*b.x+C;
                B=tB; C=tC;
                if (ve.x*ve.y>0) {
                   tB = -4*b.x*A-B;
                   tC = 4*A*b.x*b.x+2*B*b.x+C;
                   B=tB; C=tC;
                   }
                ca = 2*A*A;
                cb = 3*A*B;
                cc = B*B+2*A*C-2*A*tar[i].y+1;
                cd = B*C-B*tar[i].y-tar[i].x;
                
                F = ((3*cc/ca)-(cb*cb/ca/ca)) / 3;
                G = ((2*cb*cb*cb/ca/ca/ca)-(9*cb*cc/ca/ca)+(27*cd/ca)) / 27;
                H = (G*G/4)+(F*F*F/27);
                double t2;
                if (H>0) { // 1 real root
                   R = -G/2+pow(H,0.5);
                   S = pow(abs(R),1/3.0);
                   if (R<0) S=-S;
                   T = -G/2-pow(H,0.5);
                   U = pow(abs(T),1/3.0);
                   if (T<0) U=-U;
                   coor r;
                   r.x = S+U-cb/3/ca;
                   r.y = A*r.x*r.x+B*r.x+C;
                   if (dist(r,tar[i])<re) t2 = (r.x-b.x)/ve.x;
                      else continue;
                   }
                   else if (F==0 && G==0 && H==0) { // all equal root
                   coor r;
                   r.x = -pow(cd/ca,1/3.0);
                   r.y = A*r.x*r.x+B*r.x+C;
                   if (dist(r,tar[i])<re) t2 = (r.x-b.x)/ve.x;
                      else continue;
                   }
                   else { // 3 real root
                   I = pow((G*G/4-H),0.5);
                   L = -pow(abs(I),1/3.0);
                   if (I<0) L=-L;
                   K = acos(-G/2/I);
                   M = cos(K/3);
                   Q = sqrt(3.0)*sin(K/3);
                   P = -(cb/3/ca);
                   coor r1,r2,r3;
                   r1.x = 2*-L*M+P; r1.y = A*r1.x*r1.x+B*r1.x+C;
                   r2.x = L*(M+Q)+P; r2.y = A*r2.x*r2.x+B*r2.x+C;
                   r3.x = L*(M-Q)+P; r3.y = A*r3.x*r3.x+B*r3.x+C;
                   t2=-1;
                   if (dist(r1,tar[i])<re) t2 = (r1.x-b.x)/ve.x;
                   if ((dist(r2,tar[i])<re) && (t2<0 || (r2.x-b.x)/ve.x<t2)) t2 = (r2.x-b.x)/ve.x;
                   if ((dist(r3,tar[i])<re) && (t2<0 || (r3.x-b.x)/ve.x<t2)) t2 = (r3.x-b.x)/ve.x;
                   }
                if (t2 < hitt && t2 > 0) {
                    point = 1;
                    hitp.x=-10; hitp.y=-10;
                    hitt = t2;
                    target = i;
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
                if (t2 < hitt && t2 > 0) {
                    point = 1;
                    hitp.x=-10; hitp.y=-10;
                    hitt = t2;
                    target = i;
                    }
                }
             }
         
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
            while (nextt<touch && visit[nextt]==1) nextt++;
            //fprintf(debug,"nextt: %d\n",nextt);
            //fprintf(debug,"%d: (%d %d)\n",target,(int)tar[target].x,(int)tar[target].y);
            //fprintf(debug,"%0.4lf,%0.4lf time:  %0.2lf\n",b.x,b.y,time);
            nhits++;
            if (nhits==n) {
               nows.visit=visit;
               nows.time=time;
               pushstate(nows,0);
               fprintf(stderr,"End\n"); fflush(stderr);
               return fail;
               }
            if (target==touch) {
               nows.visit=visit;
               nows.time=time;
               for (int j=0; j<nows.now.size(); j++)
                   fprintf(debug,"%d %d -> %d %d\n",nows.now[j].x1,nows.now[j].y1,nows.now[j].x2,nows.now[j].y2);
               for (int j=0; j<n; j++) fprintf(debug,"%d",visit[j]);
               fprintf(debug,"\nSuccess %d!   time:%0.4lf\n",touch,time);
               //fprintf(stderr,"Success %d!   time:%0.4lf\n",touch,time); fflush(stderr);
               return nows;
               }
            }
         if (time>tleout) {
            //fprintf(stderr,"TLE %d!   time:%0.4lf\n",touch,time); fflush(stderr);
            return fail;
            }
         if (hitp.x>-1 && hitp.y>-1) {
            if (norm(ve) < 1e-6) {
               //stopped, give up
               break;
               }
            bounces++;
            prev = hi;
            //find normal and tangential components of velocity BEFORE reflection
            //reflection multiplies normal component by -0.99 (for energy losses)
            coor vect; vect.x=edg[hi].x2-edg[hi].x1; vect.y=edg[hi].y2-edg[hi].y1; 
            vt = mult(vect, dot(ve, vect)/dot(vect, vect));
            vn = substr(ve, vt);
            vn = mult(vn, -0.99);
            //new velocity
            ve = add(vn, vt);
            if (touch!=n+1 && norm(ve)*norm(ve)/20+b.y<tar[nextt].y-re) {
               //fprintf(debug,"Too low: %0.4lf/%0.4lf\n",norm(ve)*norm(ve)/20+b.y,tar[nextt].y-re);
               //for (int j=0; j<n; j++) fprintf(debug,"%d",visit[j]);
               //fprintf(debug,"\n");
               return fail;
               }
            }
            else {
            prev = -1;
            }
         
         }
     //fprintf(stderr,"End\n"); fflush(stderr);
     double tsco=pow(0.9,nows.now.size()*1.0)*nhits*1.0/n*pow(0.995,500.0);
     if (tsco>bestsco) {
        bestsco=tsco;
        ret=nows.now;
        fprintf(stderr,"Sco: %0.4lf\n",tsco); fflush(stderr);
        }
     return fail;
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
         for (int i=0; i<=100; i++) {
             tle[i]=minpoint/2;
             tle[i]/=pow(0.9,i*1.0);
             tle[i]=log(tle[i])/log(0.995);
             }
         tleout=200.0;
         for (int i=0; i<n; i++)
             if (tar[i].y<250) {
                height=i; break;
                }
         start();
         
         vector<state> vst;
         for (int i=0; i<2000; i++) {
             edge te; now.clear();
             te.x1=max(min(20,(int)tar[0].x),(int)(init.x-rand()%200-1)); te.y1=(int)(init.y-rand()%150);
             te.x2=min(max(480,(int)tar[0].x),(int)(init.x+rand()%200+1)); te.y2=(int)(init.y-rand()%150);
             now.push_back(te);
             state news; news.now=now; news.visit.resize(80);
             news=aimsim(news,0);
             if (news.visit[0]!=-1) {
                pushstate(news,0);
                }
             if (i<300) {
                tleout=1000.0;
                state tryone; tryone.now=now; tryone.visit.resize(80);
                tryone=aimsim(tryone,n+1);
                }
             tleout=200.0;
             }
         //with fixed range horizontal
         
         for (int i=1; i<10; i++) 
             for (int j=0; j<20+min(20,(int)(tar[0].y-tar[1].y)); j++) {
                 double localbest=1e10;
                 state lbest;
                 for (int k=0; k<10; k++) {
                     edge te; now.clear();
                     te.x1=(int)min(tar[0].x,tar[1].x)+k; te.x2=(int)max(tar[0].x,tar[1].x)-k;
                     te.y1=(int)tar[1].y-10-j; te.y2=(int)tar[1].y-10-i-j; 
                     if (init.x<te.x1) te.x1=(int)init.x;
                     if (init.x>te.x2) { te.x2=(int)init.x; swap(te.y1,te.y2); }
             
                     now.push_back(te);
                     state news; news.now=now; news.visit.resize(80);
                     news=aimsim(news,0);
                     if (news.visit[0]!=-1 && news.time<localbest) {
                        localbest=news.time;
                        lbest=news;
                        
                        }
                     }
                 if (!lbest.visit.empty()) {
                    pushstate(lbest,0);
                    }
                 }
         
         //with fixed range vertical
         
         for (int i=20; i<80; i++) 
             for (int j=1; j<20; j++) {
                 double localbest=1e10;
                 state lbest;
                 for (int k=1; k<4; k++) {
                     edge te; now.clear();
                     te.x1=(int)init.x; te.x2=(int)init.x-k;
                     te.y1=(int)init.y-i; te.y2=(int)init.y-i-j; 
                     now.push_back(te);
                     state news; news.now=now; news.visit.resize(80);
                     news=aimsim(news,0);
                     if (news.visit[0]!=-1 && news.time<localbest) {
                        localbest=news.time;
                        lbest=news;
                        }
                     }
                 for (int k=1; k<4; k++) {
                     edge te; now.clear();
                     te.x1=(int)init.x; te.x2=(int)init.x+k;
                     te.y1=(int)init.y-i; te.y2=(int)init.y-i-j; 
                     
                     now.push_back(te);
                     state news; news.now=now; news.visit.resize(80);
                     news=aimsim(news,0);
                     if (news.visit[0]!=-1 && news.time<localbest) {
                        localbest=news.time;
                        lbest=news;
                        }
                     }
                 if (!lbest.visit.empty()) {
                    pushstate(lbest,0);
                    }
                 }
         
         double firtime=runtime();
         if (computer==1) firtime/=2;
         for (int i=1; i<n; i++) {
             int maxlimit,validno=201;
             maxlimit=(int)(pow(12-firtime*2,1.0/i)*5.8);
             if (i>=height) maxlimit=20; 
             pqshade.clear();
             fprintf(debug,"nextpoint: %d\n",i);
             double curtime=runtime();
             
             state news,tst;
             if (!heu[i].empty() && bestsco<0) ret=heu[i].top().now;
             while (maxlimit>0 && !heu[i].empty()) {
                news=heu[i].top();
                curtime=runtime();
                if (computer==1) curtime/=2;
                fprintf(stderr,"runtime: %0.4lf\n",curtime);
                fprintf(stderr,"%d: %d\n",i,maxlimit); fflush(stderr);
                fprintf(debug,"sz: %d\n",heu[i].size());
                fprintf(debug,"obs:%d  time:%0.4lf\n",news.now.size(),news.time);
                //for (int j=0; j<news.now.size(); j++)
                //    fprintf(debug,"%d %d -> %d %d\n",news.now[j].x1,news.now[j].y1,news.now[j].x2,news.now[j].y2);
                fflush(debug);
                heu[i].pop();
                tst=aimsim(news,i);
                if (tst.visit.size()>=n) {
                   for (int j=0; j<n; j++) fprintf(stderr,"%d",tst.visit[j]);
                    fprintf(stderr,"\n"); fflush(stdout);
                    }
                if (tst.visit[0]!=-1) {
                   fprintf(debug,"next state\n");
                   fprintf(stderr,"next state\n");
                   pushstate(tst,i);
                   }
                   else {
                   valid.clear();
                   
                   if (pqshade.size()>maxlimit) {
                      sort(pqshade.begin(),pqshade.end(),backrev);
                      fprintf(stderr,"sz: %d  tle: %0.4lf\n",pqshade.size(),pqshade[29]);
                      tleout=pqshade[maxlimit-1]/2;
                      tleout/=pow(0.9,news.now.size()*1.0);
                      tleout=log(tleout)/log(0.995);
                      pqshade.resize(maxlimit);
                      }
                      else tleout=tle[news.now.size()];
                   fprintf(stderr,"TLE: %0.4lf\n",tleout);
                   if (i<height) {
                   int trylim=max(20,(int)(20+(15-curtime)/(height-i)*80));
                   fprintf(stderr,"trylim: %d\n",trylim);
                   for (int k=0; k<trylim; k++) {
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
                       if (i==1 && k<20) {
                          double td=tleout;
                          tleout=1000.0;
                          state tryone; tryone.now=tst.now; tryone.visit.resize(80);
                          tryone=aimsim(tryone,n+1);
                          tleout=td;
                          }
                       tst=aimsim(tst,i);
                       if (tst.visit[0]!=-1) {
                          fprintf(debug,"add line!!\n");
                          valid.insert(tst);
                          }
                       
                       }
                   for (int k=1; k<10; k++)
                       for (int x=0; x<trylim/10; x++) {
                           tst=news;
                           int m;
                           for (m=0; m<n; m++) {
                               if (tst.visit[m]==0) break;
                               }
                           edge te; now.clear();
                           te.x1=max(1,(int)(rand()%500-1)); te.y1=(int)(tar[m].y-rand()%30-20);
                           te.x2=min(499,(int)(rand()%500+1)); te.y2=(int)(te.y1-k*2);
                           tst.now.push_back(te);
                           //for (int m=0; m<tst.now.size(); m++)
                           //    fprintf(debug,"%d %d -> %d %d\n",tst.now[m].x1,tst.now[m].y1,tst.now[m].x2,tst.now[m].y2);
                           for (m=0; m<n; m++) {
                               tst.visit[m]=0;
                               }
                           tst=aimsim(tst,i);
                           if (tst.visit[0]!=-1) {
                              fprintf(debug,"add line!!\n");
                              valid.insert(tst);
                              }
                           }
                   
                   for (int k=0; k<validno; k++)
                       if (!valid.empty()) {
                          if (!(*(valid.begin())).now.empty()) {
                             //fprintf(debug,"STOP: %d %0.2lf\n",(*(valid.begin())).now.size(),(*(valid.begin())).time);
                             pushstate(*(valid.begin()),i);
                             }
                             else k--;
                          
                          valid.erase(valid.begin());
                          }
                   }
                   else {
                   
                   fprintf(stderr,"under 250\n"); fflush(stderr);
                   if (bestsco>0) {
                      tleout=bestsco/(2*pow(0.9,news.now.size()+1.0));
                      tleout=log(tleout)/log(0.995);
                      }
                   fprintf(stderr,"best: %0.4lf  TLE: %0.4lf\n",bestsco,tleout);
                   int tryleft=min(maxlimit,(int)heu[i].size());
                   for (int k=i+1; k<n; k++)
                       tryleft+=min(20,(int)heu[k].size());
                   int trylim=max(20,(int)(50+(19-curtime)/(tryleft+2)*480));
                   fprintf(stderr,"trylim: %d\n",trylim);
                   for (int k=0; k<trylim; k++) {
                       tst=news;
                       edge te; now.clear();
                       te.x1=max(1,(int)(rand()%500-1)); te.y1=(int)(250-rand()%249);
                       te.x2=min(499,(int)(rand()%500+1)); te.y2=(int)(250-rand()%249);
                       tst.now.push_back(te);
                       //for (int m=0; m<tst.now.size(); m++)
                       //    fprintf(debug,"%d %d -> %d %d\n",tst.now[m].x1,tst.now[m].y1,tst.now[m].x2,tst.now[m].y2);
                       for (int m=0; m<n; m++) {
                           tst.visit[m]=0;
                           }
                       tst=aimsim(tst,n);
                       }
                   } 
                   }
                curtime=runtime();
                if (computer==1) curtime/=2;
                if (curtime>19.6) break;
                maxlimit--;
                }
             }
         //change to string
         
         for (int i=0; i<min(100,(int)ret.size()); i++) {
             string str="";
             str+=i2s(ret[i].x1)+" "+i2s(ret[i].y1)+" "+i2s(ret[i].x2)+" "+i2s(ret[i].y2);
             sret.push_back(str);
             }
         for (int i=0; i<min(100,(int)ret.size()); i++)
             fprintf(debug,"%d %d -> %d %d\n",ret[i].x1,ret[i].y1,ret[i].x2,ret[i].y2);
         simulation(ret);
         fprintf(stderr,"runtime: %0.4lf\n",runtime()/2);
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
