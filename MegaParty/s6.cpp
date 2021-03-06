#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<map>
#include<fstream>
#include<set>
#include<cmath>
using namespace std;

int n,v[1010];
double h,maxscore=0,tscore,e1,e2;
vector<int> cntf,cnte,ppl,A,B,final;
vector<double> ret,maxret;
vector<vector<int> > group;
int dx[]={1,0,-1,0},dy[]={0,1,0,-1};
set<pair<int,int> > can,used,newcan; //row,num correspond to 0
int hash[3][2000100];

bool cmp (int a,int b) {
   int f1=B[a],f2=B[b];
   if (f1!=f2) return f1>f2;
   if (cntf[a]!=cntf[b]) return cntf[a]>cntf[b];
   return cnte[a]<cnte[b];
}

double scoring (vector<double> tret) {
    double res=0;
    double MAx=-1e10,MIx=1e10,MAy=-1e10,MIy=1e10;
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            double dis=sqrt((tret[i*2]-tret[j*2])*(tret[i*2]-tret[j*2])+(tret[i*2+1]-tret[j*2+1])*(tret[i*2+1]-tret[j*2+1]));
            if (dis<10 && i<j) return 0;
            if (A[i*n+j]==1 && dis<e1) res+=B[i];
            if (A[i*n+j]==-1 && dis<e2) res-=B[i];
            }
        MAx=max(MAx,tret[i*2]);
        MIx=min(MIx,tret[i*2]);
        MAy=max(MAy,tret[i*2+1]);
        MIy=min(MIy,tret[i*2+1]);
        }
    return max(0.0,res/((MAy-MIy+20)*(MAx-MIx+20)));
}

class MegaParty {
   public:
      vector <double> arrangement(vector <int> vA, vector <int> vB, double X1, double X2) {
         A=vA; B=vB; e1=X1; e2=X2;
         n=B.size();
         ret.clear(); cntf.clear(); cnte.clear(); group.clear();
         memset(hash,-1,sizeof(hash));
         memset(v,0,sizeof(v));
         ret.resize(2*n); final.resize(2*n);
         cntf.resize(n); cnte.resize(n); ppl.resize(n);
         for (int i=0; i<n; i++)
             ppl[i]=i;
         for (int i=0; i<n; i++)
             for (int j=0; j<n; j++)
                 if (A[i*n+j]==1) cntf[i]++;
                    else if (A[i*n+j]==-1) cnte[i]++;
         sort(ppl.begin(),ppl.end(),cmp);
         
         int nextpoint=0;
         h=8.67;
         
         int add=0,totp=0,mx1,mx2,my1,my2,sump;
         int breakp=0,maxh=int(max(e1,e2)/h);
         vector<int> grp;
         while (totp<n) {
            if (add==0) {
               memset(hash,-1,sizeof(hash));
               grp.clear(); used.clear(); can.clear();
               mx1=mx2=my1=my2=sump=0;
               grp.push_back(ppl[nextpoint]);
               hash[0][1000001]=ppl[nextpoint];
               v[ppl[nextpoint]]=1;
               final[ppl[nextpoint]*2]=0; final[ppl[nextpoint]*2+1]=0;
               used.insert(make_pair(0,0));
               for (int i=0; i<4; i++)
                   can.insert(make_pair(dx[i],dy[i]));
               totp++;
               add=0;
               }
            add=0;
            for (int i=0; i<n; i++)
                if (v[ppl[i]]==0) {
                   int mp=-10000,mr=-10000;
                   double mxp=-1;
                   for (set<pair<int,int> >::iterator it=can.begin(); it!=can.end(); it++) {
                       int row=(*it).first,pos=(*it).second;
                       double ar=row*h,ap;
                       if (row%2==0) ap=10.0001*pos;
                          else ap=sqrt(max(0.0,100-h*h))+0.01+10.0001*pos;
                          
                       //check if valid and update mxp
                       int np=0;
                       double tx,ty;
                       for (int j=-maxh; j<=maxh; j++) {
                           int maxw;
                           maxw=(int)sqrt(max(e1,e2)*max(e1,e2)-j*j*h*h);
                           for (int k=-maxw; k<=maxw; k++) {
                               int tar=-1;
                               if ((row+j)*(pos+k)>=0) {
                                  if (hash[0][(row+j)*1000+pos+k+1000001]!=-1) tar=hash[0][(row+j)*1000+pos+k+1000001];
                                  }
                                  else {
                                  if (hash[1][(row+j)*1000+pos+k+1000001]!=-1) tar=hash[1][(row+j)*1000+pos+k+1000001];
                                  }
                               if (tar!=-1) {
                                  tx=final[tar*2]*h;
                                  if (final[tar*2]%2==0) ty=10.0001*final[tar*2+1];
                                     else ty=sqrt(max(0.0,100-h*h))+0.01+10.0001*final[tar*2+1];
                                  double td=sqrt((tx-ar)*(tx-ar)+(ty-ap)*(ty-ap));              
                                  if (A[ppl[i]*n+tar]==1 && td<e1) np+=B[ppl[i]]+B[tar];
                                  if (A[ppl[i]*n+tar]==-1 && td<e2) np-=B[ppl[i]]+B[tar];
                                  }
                               }
                           }
                       double areafac;
                       int p1=0,p2=0;
                       if (row>mx2 || row<mx1) p1++;
                       if (pos>my2 || pos<my1) p2++;
                       areafac=((my2-my1+p2)*10+20)*((mx2-mx1+p1)*h+20);
                       if ((sump+np)*1.0/areafac>mxp) {
                          mxp=(sump+np)*1.0/areafac;
                          mr=row; mp=pos;
                          }
                       }
                   if (mxp>0 || (mxp==0 && add==-1)) {
                      can.erase(make_pair(mr,mp));
                      used.insert(make_pair(mr,mp));
                      if (mr*mp>=0) hash[0][mr*1000+mp+1000001]=ppl[i];
                         else hash[1][mr*1000+mp+1000001]=ppl[i];
                      grp.push_back(ppl[i]);
                      final[ppl[i]*2]=mr; final[ppl[i]*2+1]=mp;
                      v[ppl[i]]=1;
                      mx1=min(mr,mx1); mx2=max(mr,mx2);
                      my1=min(mp,my1); my2=max(mp,my2);
                      add++; totp++;
                      for (int i=0; i<4; i++)
                          if (used.find(make_pair(mr+dx[i],mp+dy[i]))==used.end()) can.insert(make_pair(mr+dx[i],mp+dy[i]));
                      }
                   }
            if (add==0 || (add<=2 && n>50)) {
               //add group
               group.push_back(grp);
               grp.clear();
               for (int i=0; i<n; i++)
                   if (v[ppl[i]]==0) {
                      nextpoint=i; break;
                      }
               add=0;
               }
            }
         if (!grp.empty()) group.push_back(grp);
         
         //organise group into ret
         int isol=((int)(e2/10.0001))+1,nowrow=0,nowcol=0;
         for (int i=0; i<group.size(); i++) {
             int miny=2000000,maxy=-2000000;
             for (int j=0; j<group[i].size(); j++) {
                 miny=min(miny,final[group[i][j]*2+1]);
                 maxy=max(maxy,final[group[i][j]*2+1]);
                 }
             for (int j=0; j<group[i].size(); j++) {
                 ret[group[i][j]*2]=final[group[i][j]*2]*h;
                 if (final[group[i][j]*2]%2==0) ret[group[i][j]*2+1]=(final[group[i][j]*2+1]+nowrow-miny)*10.0001;
                    else ret[group[i][j]*2+1]=sqrt(max(0.0,100-h*h))+0.01+10.0001*(final[group[i][j]*2+1]+nowrow-miny);
                 }
             nowrow+=isol+maxy-miny;
             }
         tscore=scoring(ret);
         if (tscore>maxscore) {
            maxret=ret; maxscore=tscore;
            }
         isol=((int)(e2/h))+1;
         for (int i=0; i<group.size(); i++) {
             int minx=2000000,maxx=-2000000;
             for (int j=0; j<group[i].size(); j++) {
                 minx=min(minx,final[group[i][j]*2]);
                 maxx=max(maxx,final[group[i][j]*2]);
                 }
             for (int j=0; j<group[i].size(); j++) {
                 ret[group[i][j]*2]=(final[group[i][j]*2]+nowcol-minx)*h;
                 if (final[group[i][j]*2]%2==0) ret[group[i][j]*2+1]=final[group[i][j]*2+1]*10.0001;
                    else ret[group[i][j]*2+1]=sqrt(max(0.0,100-h*h))+0.01+10.0001*final[group[i][j]*2+1];
                 }
             nowcol+=isol+maxx-minx;
             }
         tscore=scoring(ret);
         if (tscore>maxscore) {
            maxret=ret; maxscore=tscore;
            }
         return ret;
      }
};


int main () {
    MegaParty MP;
    vector<int> VA,VB;
    double E1,E2;
    int NUM;
    scanf("%d",&NUM);
    VA.resize(NUM);
    for (int i=0; i<NUM; i++)
        scanf("%d",&VA[i]);
    scanf("%d",&NUM);
    VB.resize(NUM);
    for (int i=0; i<NUM; i++)
        scanf("%d",&VB[i]);
    scanf("%lf %lf",&E1,&E2);
    vector<double> RET;
    RET=MP.arrangement(VA,VB,E1,E2);
    for (int i=0; i<2*NUM; i++)
        printf("%0.4lf\n",RET[i]);
    fflush(stdout);
    return 0;
}
