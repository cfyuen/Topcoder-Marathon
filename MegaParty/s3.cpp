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
double h;
vector<int> cntf,cnte,ppl,A,B,final;
vector<double> ret;
vector<vector<int> > group;
int dx[]={1,0,-1,0},dy[]={0,1,0,-1};
set<pair<int,int> > can,used; //row,num correspond to 0
int hash[3][2000100];

bool cmp (int a,int b) {
   int f1=(cntf[a]-cnte[a])*B[a],f2=(cntf[b]-cnte[b])*B[b];
   if (f1!=f2) return f1>f2;
   return cntf[a]>cntf[b];
}

class MegaParty {
   public:
      vector <double> arrangement(vector <int> vA, vector <int> vB, double e1, double e2) {
         A=vA; B=vB;
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
         
         int add=0,totp=0;
         int breakp=0,maxh=int(max(e1,e2)/h);
         vector<int> grp;
         while (totp<n) {
            if (add!=1) {
               memset(hash,-1,sizeof(hash));
               grp.clear(); used.clear(); can.clear();
               
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
            if (add!=-1) add=0;
            
            for (int i=0; i<n; i++)
                if (v[ppl[i]]==0) {
                   int mxp=-1,mp=-10000,mr=-10000;
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
                       
                       if (np>mxp || (np==mxp && max(abs(row),abs(pos))<max(abs(mr),abs(mp)) )) {
                          mxp=np;
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
                      add=1; totp++;
                      for (int i=0; i<4; i++)
                          if (used.find(make_pair(mr+dx[i],mp+dy[i]))==used.end()) can.insert(make_pair(mr+dx[i],mp+dy[i]));
                      }
                   }
                   
            if (add<=0) {
               //add group
               group.push_back(grp);
               grp.clear();
               for (int i=0; i<n; i++)
                   if (v[ppl[i]]==0) {
                      nextpoint=i; break;
                      }
               //add=-1 can be add=0?
               add=-1;
               }
            }
         if (!grp.empty()) group.push_back(grp);
         if (group.size()>=15 && n>600) return ret;
         //organise group into ret
         int isol=((int)(e2/10.0001))+1,nowrow=0;
         for (int i=0; i<group.size(); i++) {
             int miny=2000000,maxy=-2000000;
             for (int j=0; j<group[i].size(); j++) {
                 if (final[group[i][j]*2+1]<miny) miny=final[group[i][j]*2+1];
                 if (final[group[i][j]*2+1]>maxy) maxy=final[group[i][j]*2+1];
                 }
             for (int j=0; j<group[i].size(); j++) {
                 final[group[i][j]*2+1]-=miny+nowrow;
                 ret[group[i][j]*2]=final[group[i][j]*2]*h;
                 if (final[group[i][j]*2]%2==0) ret[group[i][j]*2+1]=final[group[i][j]*2+1]*10.0001;
                    else ret[group[i][j]*2+1]=sqrt(max(0.0,100-h*h))+0.01+10.0001*final[group[i][j]*2+1];
                 }
             nowrow+=isol+maxy-miny;
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
