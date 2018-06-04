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
vector<int> cntf,cnte,ppl,A,B;
vector<double> ret;
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
         ret.clear(); cntf.clear(); cnte.clear();
         memset(hash,-1,sizeof(hash));
         memset(v,0,sizeof(v));
         ret.resize(2*n);
         cntf.resize(n); cnte.resize(n); ppl.resize(n);
         for (int i=0; i<n; i++)
             ppl[i]=i;
         for (int i=0; i<n; i++)
             for (int j=0; j<n; j++)
                 if (A[i*n+j]==1) cntf[i]++;
                    else if (A[i*n+j]==-1) cnte[i]++;
         sort(ppl.begin(),ppl.end(),cmp);
         
         h=8.67;
         ret[ppl[0]*2]=0; ret[ppl[0]*2+1]=0;
         hash[0][1000001]=ppl[0];
         v[ppl[0]]=1;
         used.insert(make_pair(0,0));
         for (int i=0; i<4; i++)
             can.insert(make_pair(dx[i],dy[i]));
         int add=0,totp=1;
         int breakp=0,maxh=int(max(e1,e2)/h);
         while (totp<n) {
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
                                  double td=sqrt((ret[tar*2]-ar)*(ret[tar*2]-ar)+(ret[tar*2+1]-ap)*(ret[tar*2+1]-ap));
                                  if (A[ppl[i]*n+tar]==1 && td<e1) np+=B[ppl[i]]+B[tar];
                                  if (A[ppl[i]*n+tar]==-1 && td<e2) np-=B[ppl[i]]+B[tar];
                                  }
                               }
                           }
                       
                       if (np>mxp || (np==mxp && max(row,pos)<max(mr,mp))) {
                          mxp=np;
                          mr=row; mp=pos;
                          }
                       }
                   if (mxp>0 || (mxp==0 && add==-1)) {
                      can.erase(make_pair(mr,mp));
                      used.insert(make_pair(mr,mp));
                      if (mr*mp>=0) hash[0][mr*1000+mp+1000001]=ppl[i];
                         else hash[1][mr*1000+mp+1000001]=ppl[i];
                      ret[ppl[i]*2]=mr*h;
                      if (mr%2==0) ret[ppl[i]*2+1]=10.0001*mp;
                         else ret[ppl[i]*2+1]=sqrt(max(0.0,100-h*h))+0.01+10.0001*mp;
                      v[ppl[i]]=1;
                      add=1; totp++;
                      for (int i=0; i<4; i++)
                          if (used.find(make_pair(mr+dx[i],mp+dy[i]))==used.end()) can.insert(make_pair(mr+dx[i],mp+dy[i]));
                      }
                   }
            if (add<=0) {
               //add a layer
               vector<pair<int,int> > tv;
               for (set<pair<int,int> >::iterator it=can.begin(); it!=can.end(); it++) {
                   used.insert(*it);
                   tv.push_back(*it);
                   }
               can.clear();
               for (int j=0; j<tv.size(); j++)
                   for (int k=0; k<4; k++)
                       if (used.find(make_pair(tv[j].first+dx[k],tv[j].second+dy[k]))==used.end()) 
                          can.insert(make_pair(tv[j].first+dx[k],tv[j].second+dy[k]));
               add=-1;
               }
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

