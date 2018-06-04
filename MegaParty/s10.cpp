#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<map>
#include<fstream>
#include<set>
#include<cstdlib>
#include<cmath>
using namespace std;

int n,v[1010];
double h,maxscore=0,tscore,e1,e2,absmax=0;
vector<int> cntf,cnte,ppl,A,B,final,bestfinal;
vector<double> ret,maxret;
vector<vector<int> > group;
int dx[]={1,0,-1,0},dy[]={0,1,0,-1};
set<pair<int,int> > can,used,newcan; //row,num correspond to 0
int hash[3][2000100];

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
         
         int add=0,totp=0,mx1,mx2,my1,my2,sump,intp;
         int breakp=0,jump=0,maxh=int(max(e1,e2)/h);
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
                       if (np*1.0/areafac>mxp) {
                          mxp=np*1.0/areafac; intp=np;
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
                      add++; totp++; sump+=intp;
                      for (int j=0; j<4; j++)
                          if (used.find(make_pair(mr+dx[j],mp+dy[j]))==used.end()) can.insert(make_pair(mr+dx[j],mp+dy[j]));
                      }
                   }
            if (add==0 || (add<=2 && n>50)) {
               
               //adjust group
               handle:
               vector<int> adjp,tv; int doneadj=1,ther=max(2,(int)sqrt(grp.size()));
               while (grp.size()>9 && doneadj==1) {
                  int minx=2000000,maxx=-2000000,miny=2000000,maxy=-2000000,minp=2000,ind=0;
                  doneadj=0;
                  for (int j=0; j<grp.size(); j++) {
                      minx=min(minx,final[grp[j]*2]);
                      maxx=max(maxx,final[grp[j]*2]);
                      miny=min(miny,final[grp[j]*2+1]);
                      maxy=max(maxy,final[grp[j]*2+1]);
                      }
                  adjp.clear();
                  tv.clear();
                  for (int j=0; j<grp.size(); j++)
                      if (final[grp[j]*2]==minx) tv.push_back(grp[j]);
                  if (tv.size()<minp && tv.size()<=ther) {
                     minp=tv.size(); adjp=tv; ind=0;
                     }
                  tv.clear();
                  for (int j=0; j<grp.size(); j++)
                      if (final[grp[j]*2]==maxx) tv.push_back(grp[j]);
                  if (tv.size()<minp && tv.size()<=ther) {
                     minp=tv.size(); adjp=tv; ind=1;
                     }
                  tv.clear();
                  for (int j=0; j<grp.size(); j++)
                      if (final[grp[j]*2+1]==miny) tv.push_back(grp[j]);
                  if (tv.size()<minp && tv.size()<=ther) {
                     minp=tv.size(); adjp=tv; ind=2;
                     }
                  tv.clear();
                  for (int j=0; j<grp.size(); j++)
                      if (final[grp[j]*2+1]==maxy) tv.push_back(grp[j]);
                  if (tv.size()<minp && tv.size()<=ther) {
                     minp=tv.size(); adjp=tv; ind=3;
                     }
                  //adjust
                  int mxp=-200000,orip=0,mr,mp;
                  double ar,ap;
                  for (int i=0; i<adjp.size(); i++) 
                      if (final[adjp[i]*2]*final[adjp[i]*2+1]>=0) hash[0][final[adjp[i]*2]*1000+final[adjp[i]*2+1]+1000001]=-1;
                         else hash[1][final[adjp[i]*2]*1000+final[adjp[i]*2+1]+1000001]=-1;
                  for (int i=0; i<adjp.size(); i++) {
                      orip=0; mxp=-20000; mr=final[adjp[i]*2]; mp=final[adjp[i]*2+1];
                      for (int j=0; j<grp.size(); j++)
                          if (adjp[i]!=grp[j]) {
                             ar=h*mr;
                             if (mr%2==0) ap=10.0001*mp;
                                else ap=sqrt(max(0.0,100-h*h))+0.01+10.0001*mp;
                             double tx,ty;
                             tx=final[grp[j]*2]*h;
                             if (final[grp[j]*2]%2==0) ty=10.0001*final[grp[j]*2+1];
                                else ty=sqrt(max(0.0,100-h*h))+0.01+10.0001*final[grp[j]*2+1];
                             double td=sqrt((tx-ar)*(tx-ar)+(ty-ap)*(ty-ap));
                             if (A[adjp[i]*n+grp[j]]==1 && td<e1) orip+=B[adjp[i]]+B[grp[j]];
                             if (A[adjp[i]*n+grp[j]]==-1 && td<e2) orip-=B[adjp[i]]+B[grp[j]];
                             }
                      for (set<pair<int,int> >::iterator it=can.begin(); it!=can.end(); it++) {
                          int row=(*it).first,pos=(*it).second;
                          if (row>maxx || row<minx || pos>maxy || pos<miny) continue;
                          if (row==minx && ind==0) continue;
                          if (row==maxx && ind==1) continue;
                          if (pos==miny && ind==2) continue;
                          if (pos==maxy && ind==3) continue;
                          ar=h*row;
                          if (row%2==0) ap=10.0001*pos;
                             else ap=sqrt(max(0.0,100-h*h))+0.01+10.0001*pos;
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
                                     if (tar!=-1 && tar!=adjp[i]) {
                                        tx=final[tar*2]*h;
                                        if (final[tar*2]%2==0) ty=10.0001*final[tar*2+1];
                                           else ty=sqrt(max(0.0,100-h*h))+0.01+10.0001*final[tar*2+1];
                                        double td=sqrt((tx-ar)*(tx-ar)+(ty-ap)*(ty-ap));              
                                        if (A[adjp[i]*n+tar]==1 && td<e1) np+=B[adjp[i]]+B[tar];
                                        if (A[adjp[i]*n+tar]==-1 && td<e2) np-=B[adjp[i]]+B[tar];
                                        }
                                     }
                                  }
                          if (np>mxp) {
                             mxp=np;
                             mr=row; mp=pos;
                             }
                          }
                      if (sump*1.0/(((maxx-minx)*h+20)*((maxy-miny)*10.0001+20))>(sump+mxp-orip)*1.0/(((maxx-minx)*h+19)*((maxy-miny)*10.0001+20))) {
                         mxp=-20000; mr=final[adjp[i]*2]; mp=final[adjp[i]*2+1];
                         }
                      can.erase(make_pair(mr,mp));
                      used.insert(make_pair(mr,mp));
                      if (mr*mp>=0) hash[0][mr*1000+mp+1000001]=adjp[i];
                         else hash[1][mr*1000+mp+1000001]=adjp[i];
                      if (mxp!=-20000) {
                         doneadj=1;
                         sump+=mxp-orip;
                         }
                      final[adjp[i]*2]=mr; final[adjp[i]*2+1]=mp;
                      
                      for (int j=0; j<4; j++)
                          if (used.find(make_pair(mr+dx[j],mp+dy[j]))==used.end()) can.insert(make_pair(mr+dx[j],mp+dy[j]));
                      }
                  }
               if (jump==1) goto back;
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
         if (!grp.empty()) {
            group.push_back(grp); jump=1; goto handle; back: ;
            }
         
         
         //organise group into final
         memset(hash,-1,sizeof(hash));
         int isol=((int)(e2/h))+1,nowcol=0,width=0,height=0,stop;
         for (int i=0; i<group.size(); i++) {
             int miny=2000000,maxy=-2000000;
             for (int j=0; j<group[i].size(); j++) {
                 miny=min(miny,final[group[i][j]*2+1]);
                 maxy=max(maxy,final[group[i][j]*2+1]);
                 }
             width=max(maxy-miny,width);
             }
         for (int i=0; i<group.size(); i++) {
             int minx=2000000,maxx=-2000000,miny=2000000,maxy=-2000000;
             for (int j=0; j<group[i].size(); j++) {
                 minx=min(minx,final[group[i][j]*2]);
                 maxx=max(maxx,final[group[i][j]*2]);
                 miny=min(miny,final[group[i][j]*2+1]);
                 maxy=max(maxy,final[group[i][j]*2+1]);
                 }
             if (i==0) stop=maxx;
             if (nowcol%2!=(abs(minx))%2) nowcol++;
             for (int j=0; j<group[i].size(); j++) {
                 final[group[i][j]*2]+=nowcol-minx;
                 height=max(final[group[i][j]*2],height);
                 final[group[i][j]*2+1]+=-miny;
                 hash[0][final[group[i][j]*2]*1000+final[group[i][j]*2+1]+1000001]=group[i][j];
                 }
             nowcol+=isol+maxx-minx;
             }
         bestfinal=final;
         //compress
         int nowscore=0;
         for (int i=0; i<n; i++) {
             for (int j=0; j<n; j++) {
                 double ar,ap,tx,ty;
                 ar=h*final[i*2];
                 if (final[i*2]%2==0) ap=10.0001*final[i*2+1];
                    else ap=sqrt(max(0.0,100-h*h))+0.01+10.0001*final[i*2+1];
                 tx=final[j*2]*h;
                 if (final[j*2]%2==0) ty=10.0001*final[j*2+1];
                    else ty=sqrt(max(0.0,100-h*h))+0.01+10.0001*final[j*2+1];
                 double dis=sqrt((tx-ar)*(tx-ar)+(ty-ap)*(ty-ap));
                 if (A[i*n+j]==1 && dis<e1) nowscore+=B[i];
                 if (A[i*n+j]==-1 && dis<e2) nowscore-=B[i];
                 }
             }
         absmax=nowscore*1.0/(width*height);
         for (int i=height; i>stop; i--)
             for (int j=0; j<=width; j++)
                 if (hash[0][i*1000+j+1000001]!=-1) {
                    int hp=hash[0][i*1000+j+1000001],gain=0;
                    double areanow=(i*h+20)*(width*10+20);
                    for (int l=i-1; ; l--)
                        for (int k=0; k<=width; k++)
                            if (hash[0][(l)*1000+k+1000001]!=-1) {
                               gain=l; goto bye123;
                               }
                    bye123:
                    
                    double mxp=-200000;
                    int orip=0,mr=i,mp=j,intsco;
                    double ar,ap;
                    hash[0][i*1000+j+1000001]=-1;
                    for (int k=0; k<n; k++)
                        if (hp!=k) {
                           ar=h*mr;
                           if (mr%2==0) ap=10.0001*mp;
                              else ap=sqrt(max(0.0,100-h*h))+0.01+10.0001*mp;
                           double tx,ty;
                           tx=final[k*2]*h;
                           if (final[k*2]%2==0) ty=10.0001*final[k*2+1];
                              else ty=sqrt(max(0.0,100-h*h))+0.01+10.0001*final[k*2+1];
                           double td=sqrt((tx-ar)*(tx-ar)+(ty-ap)*(ty-ap));
                           if (A[hp*n+k]==1 && td<e1) orip+=B[hp]+B[k];
                           if (A[hp*n+k]==-1 && td<e2) orip-=B[hp]+B[k];
                           }
                    for (int row=i-1; row>=0; row--)
                        for (int pos=0; pos<=width; pos++) 
                          if (hash[0][row*1000+pos+1000001]==-1) {
                            ar=h*row;
                            if (row%2==0) ap=10.0001*pos;
                               else ap=sqrt(max(0.0,100-h*h))+0.01+10.0001*pos;
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
                                    if (tar!=-1 && tar!=hp) {
                                        tx=final[tar*2]*h;
                                        if (final[tar*2]%2==0) ty=10.0001*final[tar*2+1];
                                           else ty=sqrt(max(0.0,100-h*h))+0.01+10.0001*final[tar*2+1];
                                        double td=sqrt((tx-ar)*(tx-ar)+(ty-ap)*(ty-ap));              
                                        if (A[hp*n+tar]==1 && td<e1) np+=B[hp]+B[tar];
                                        if (A[hp*n+tar]==-1 && td<e2) np-=B[hp]+B[tar];
                                        }
                                    }
                              }
                            if ((nowscore+np-orip)*1.0/((width*10+20)*(max(row,gain)*h+20))>mxp) {
                               mxp=(nowscore+np-orip)*1.0/((width*10+20)*(max(row,gain)*h+20)); intsco=np;
                               mr=row; mp=pos;
                               }
                          }
                      if (nowscore*1.0/areanow>mxp) {
                         mxp=-200000; mr=final[hp*2]; mp=final[hp*2+1];
                         }
                      if (mxp>absmax) {
                         absmax=mxp;
                         bestfinal=final;
                         }
                      hash[0][mr*1000+mp+1000001]=hp;
                      if (mxp>-20000) {
                         nowscore+=intsco-orip;
                         }
                      final[hp*2]=mr; final[hp*2+1]=mp;
                      if (mxp<-20000 || runtime()>19.8) goto exit;
                      }
         exit:
         //turn final to return
         final=bestfinal;
         for (int i=group.size()-1; i>=0; i--) {
             for (int j=0; j<group[i].size(); j++) {
                 ret[group[i][j]*2]=final[group[i][j]*2]*h;
                 if (final[group[i][j]*2]%2==0) ret[group[i][j]*2+1]=final[group[i][j]*2+1]*10.0001;
                    else ret[group[i][j]*2+1]=sqrt(max(0.0,100-h*h))+0.01+10.0001*final[group[i][j]*2+1];
                 }
             }
         
         tscore=scoring(ret);
         if (tscore>maxscore) {
            maxret=ret; maxscore=tscore;
            }
         return ret;
      }
};
