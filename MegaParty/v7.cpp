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

bool cmp (int a,int b) {
   //int f1=(cntf[a]-cnte[a])*B[a],f2=(cntf[b]-cnte[b])*B[b];
   //int f1=(cntf[a])*B[a],f2=(cntf[b])*B[b];
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
         /*
         for (int i=0; i<n; i++) {
             for (int j=0; j<n; j++)
                 if (A[i*n+j]==-1) fprintf(debug,"%d ",A[i*n+j]);
                    else fprintf(debug,"%d  ",A[i*n+j]);
             fprintf(debug,"\n");
             }
         for (int i=0; i<n; i++)
             fprintf(debug,"%d ",B[i]);
         fprintf(debug,"\n");
         */
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
         /*
         for (int i=0; i<n; i++)
             fprintf(debug,"%d ",ppl[i]);
         fprintf(debug,"\n");
         */
         
         int nextpoint=0;
         h=8.67;
         //h=9.99;
         
         int add=0,totp=0,mx1,mx2,my1,my2,sump,intp;
         int breakp=0,jump=0,maxh=int(max(e1,e2)/h);
         vector<int> grp;
         while (totp<n) {
            fprintf(debug,"add:%d nextpoint:%d\n",add,nextpoint);
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
            fprintf(debug,"set:%d\n",can.size()); 
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
                                  //fprintf(debug,"%d: %d\n",tar,np);
                                  }
                               }
                           }
                       double areafac;
                       int p1=0,p2=0;
                       if (row>mx2 || row<mx1) p1++;
                       if (pos>my2 || pos<my1) p2++;
                       areafac=((my2-my1+p2)*10+20)*((mx2-mx1+p1)*h+20);
                       //fprintf(debug,"%d(%d): %d+%d/%0.1lf (%d %d)\n",i,ppl[i],sump,np,areafac,row,pos);
                       if (np*1.0/areafac>mxp) {
                          mxp=np*1.0/areafac; intp=np;
                          mr=row; mp=pos;
                          }
                       }
                   //fprintf(debug,"final: %d(%d): %d (%d %d)\n",i,ppl[i],mxp,mr,mp);
                   if (mxp>0 || (mxp==0 && add==-1)) {
                      can.erase(make_pair(mr,mp));
                      used.insert(make_pair(mr,mp));
                      if (mr*mp>=0) hash[0][mr*1000+mp+1000001]=ppl[i];
                         else hash[1][mr*1000+mp+1000001]=ppl[i];
                      fprintf(debug,"%d(%d) (%d %d)\n",i,ppl[i],mr,mp);
                      //fprintf(result,"(%d %d): %d\n",mr,mp,mr*1000+mp+1000001);
                      /*
                      ret[ppl[i]*2]=mr*h;
                      if (mr%2==0) ret[ppl[i]*2+1]=10.0001*mp;
                         else ret[ppl[i]*2+1]=sqrt(max(0.0,100-h*h))+0.01+10.0001*mp;
                      */
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
            /*
            for (set<pair<int,int> >::iterator it=can.begin(); it!=can.end(); it++)
                newcan.insert(*it);
            */
            if (add==0 || (add<=2 && n>50)) {
               
               //adjust group
               handle:
               fprintf(debug,"\nADJ (%d)\n",sump);
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
                  fprintf(debug,"adjp:%d ind:%d\n",adjp.size(),ind);
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
                      //fprintf(debug,"set:%d\n",can.size());
                      for (set<pair<int,int> >::iterator it=can.begin(); it!=can.end(); it++) {
                          int row=(*it).first,pos=(*it).second;
                          if (row>maxx || row<minx || pos>maxy || pos<miny) continue;
                          if (row==minx && ind==0) continue;
                          if (row==maxx && ind==1) continue;
                          if (pos==miny && ind==2) continue;
                          if (pos==maxy && ind==3) continue;
                          //fprintf(debug,"(%d %d) %d %d %d %d\n",row,pos,minx,maxx,miny,maxy);
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
                                        //fprintf(debug,"%d: %d(%d) ->%d   %0.4lf\n",tar,np,B[tar],A[adjp[i]*n+tar],td);
                                        }
                                     }
                                  }
                          //fprintf(debug,"%d: (%d %d) %d\n",adjp[i],row,pos,np);
                          if (np>mxp) {
                             mxp=np;
                             mr=row; mp=pos;
                             }
                          }
                      fprintf(debug,"%d %d->%d= %0.4lf -> %0.4lf\n",sump,orip,mxp,sump*1.0/(((maxx-minx)*h+20)*((maxy-miny)*10.0001+20)),(sump+mxp-orip)*1.0/(((maxx-minx)*h+19)*((maxy-miny)*10.0001+20)));
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
                      fprintf(debug,"adj:%d(%d) (%d %d) %d\n",i,adjp[i],mr,mp,mxp-orip);
                      final[adjp[i]*2]=mr; final[adjp[i]*2+1]=mp;
                      
                      for (int j=0; j<4; j++)
                          if (used.find(make_pair(mr+dx[j],mp+dy[j]))==used.end()) can.insert(make_pair(mr+dx[j],mp+dy[j]));
                      }
                  }
               if (jump==1) goto back;
               fprintf(debug,"\n");
               //add group
               group.push_back(grp);
               grp.clear();
               for (int i=0; i<n; i++)
                   if (v[ppl[i]]==0) {
                      nextpoint=i; break;
                      }
               add=0;
               //break;
               }
            fprintf(debug,"point:%d group:%d\n",totp,group.size());
            fflush(debug);
            //can=newcan; newcan.clear();
            }
         if (!grp.empty()) {
            group.push_back(grp); jump=1; goto handle; back: ;
            }
         
         
         //organise group into ret
         int isol=((int)(e2/10.0001))+1,nowrow=0,nowcol=0;
         for (int i=group.size()-1; i>=0; i--) {
             int miny=2000000,maxy=-2000000;
             for (int j=0; j<group[i].size(); j++) {
                 miny=min(miny,final[group[i][j]*2+1]);
                 maxy=max(maxy,final[group[i][j]*2+1]);
                 }
             fprintf(debug,"now:%d  min:%d max:%d\n",nowrow,miny,maxy);
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
         fprintf(debug,"score:%0.4lf\n",tscore);
         isol=((int)(e2/h))+1;
         for (int i=group.size()-1; i>=0; i--) {
             int minx=2000000,maxx=-2000000;
             for (int j=0; j<group[i].size(); j++) {
                 minx=min(minx,final[group[i][j]*2]);
                 maxx=max(maxx,final[group[i][j]*2]);
                 }
             fprintf(debug,"now:%d  min:%d max:%d\n",nowcol,minx,maxx);
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
         fprintf(debug,"score:%0.4lf\n",tscore);
         
         fprintf(debug,"\n%d\n",n);
         for (int i=0; i<n; i++)
             fprintf(debug,"%d:(%0.4lf %0.4lf)   pts:%d\n",i,ret[i*2],ret[i*2+1],(cntf[i]-cnte[i])*B[i]);
         for (int i=0; i<n; i++)
             fprintf(debug,"%d\n",(cntf[ppl[i]]-cnte[ppl[i]])*B[ppl[i]]);
         fprintf(debug,"totcnt: %d\n",tcnt);
         return ret;
      }
};

int main () {
    MegaParty MP;
    debug=fopen("debug.txt","w");
    result=fopen("result.txt","w");
    
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
    start();
    RET=MP.arrangement(VA,VB,E1,E2);
    fprintf(result,"Runtime: %0.3lf\n",runtime());
    for (int i=0; i<2*NUM; i++)
        printf("%0.4lf\n",RET[i]);
    fflush(stdout);
    int Sco[1010];
    memset(Sco,0,sizeof(Sco));
    for (int i=0; i<NUM; i++)
        for (int j=0; j<NUM; j++) {
            if (sqrt((RET[i*2]-RET[j*2])*(RET[i*2]-RET[j*2])+(RET[i*2+1]-RET[j*2+1])*(RET[i*2+1]-RET[j*2+1]))<10 && i<j)
               fprintf(result,"%d(%0.4lf,%0.4lf) and %d(%0.4lf,%0.4lf) are too near\n",i,RET[i*2],RET[i*2+1],j,RET[j*2],RET[j*2+1]);
            if (VA[i*NUM+j]==1 && sqrt((RET[i*2]-RET[j*2])*(RET[i*2]-RET[j*2])+(RET[i*2+1]-RET[j*2+1])*(RET[i*2+1]-RET[j*2+1]))<E1) Sco[i]+=VB[i];
            if (VA[i*NUM+j]==-1 && sqrt((RET[i*2]-RET[j*2])*(RET[i*2]-RET[j*2])+(RET[i*2+1]-RET[j*2+1])*(RET[i*2+1]-RET[j*2+1]))<E2) Sco[i]-=VB[i];
            }
    int Sum=0;
    double MAx=-1e10,MIx=1e10,MAy=-1e10,MIy=1e10;
    for (int i=0; i<NUM; i++) {
        fprintf(result,"%d ",Sco[i]);
        Sum+=Sco[i];
        MAx=max(MAx,RET[i*2]);
        MIx=min(MIx,RET[i*2]);
        MAy=max(MAy,RET[i*2+1]);
        MIy=min(MIy,RET[i*2+1]);
        if (i%10==9 && i!=NUM-1) fprintf(result,"\n");
        }
    fprintf(result,"\nSum:%d    H:%0.2lf W:%0.2lf    Area:%0.2lf\nScore:%0.4lf\n",Sum,MAy-MIy,MAx-MIx,(MAy-MIy+20)*(MAx-MIx+20),Sum/((MAy-MIy+20)*(MAx-MIx+20)));
    fflush(result);
    return 0;
}
