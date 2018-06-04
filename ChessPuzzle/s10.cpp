#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<cmath>
#include<queue>
using namespace std;

vector<vector<int> > bd;
int K,removed=0;
int depth[17][17],reach[17][17],depcnt[11];
int prev=0,px=0,py=0;
int dx[]={1,0,0,-1,1,1,-1,-1},dy[]={0,1,-1,0,1,-1,1,-1};
int dkx[]={2,2,1,1,-1,-1,-2,-2},dky[]={1,-1,2,-2,2,-2,1,-1};
int reachsum=0,emptysq=0,k12heu;
vector<pair<int,int> > seq;
double heuv,indtime=-1.0;
int indleft,toteva=0;

struct State {
   double heur; //heuristic value
   int lvl; //search depth
   int lx,ly,lprev; //last x,y
   int ox,oy; //original x,y
   int nowreach; //tot reach now;
   int v[17][17]; //visited?
   vector<pair<int,int> > rec; //record move
};

class pqcmp {
   public:
      bool operator () (const State &a,const State &b) {
           if (a.lvl!=b.lvl) return a.lvl>b.lvl;
           return a.heur<b.heur;
           }
};

class pqcmp2 {
   public:
      bool operator () (const pair<State,vector<pair<int,int> > > &a,const pair<State,vector<pair<int,int> > > &b) {
           if (a.first.lvl!=b.first.lvl) return a.first.lvl>b.first.lvl;
           return a.first.heur<b.first.heur;
           }
};

timeval sttime;
double timelim=19.8;

void startt() {
    gettimeofday(&sttime,NULL);
}

double runtime() {
    timeval tt, nowt;
    gettimeofday(&tt,NULL);
    timersub(&tt,&sttime,&nowt);
    return (nowt.tv_sec*1000+nowt.tv_usec/1000)/1000.0;
    }


string i2s (int a) {
   char tc[15];
   sprintf(tc,"%d",a);
   string ts=tc;
   return ts;
}

int boardcode (char tile) {
    if (tile>='1' && tile<='4') return tile-'0';
    if (tile=='K') return 5;
    if (tile=='B') return 6;
    if (tile=='R') return 7;
    if (tile=='Q') return 8;
    return 0;
}

int updatereach (int typ,int lx,int ly,int plus) {
   int oper=0;
   if (typ>=1 && typ<=4) {
      for (int k=0; k<8; k++) {
          int nx=lx+dx[k]*typ,ny=ly+dy[k]*typ;
          if (nx>=0 && nx<bd.size() && ny>=0 && ny<bd[0].size() && bd[nx][ny]!=0) {
             reach[nx][ny]+=plus; oper++;
             }
          }
      return oper;
      }
   if (typ==5) {
      for (int k=0; k<8; k++) {
          int nx=lx+dkx[k],ny=ly+dky[k];
          if (nx>=0 && nx<bd.size() && ny>=0 && ny<bd[0].size() && bd[nx][ny]!=0) {
             reach[nx][ny]+=plus; oper++;
             }
          }
      return oper;
      }
   if (typ>=6 && typ<=8) {
      int fr,to;
      if (typ==6) { fr=4; to=8; }
      if (typ==7) { fr=0; to=4; }
      if (typ==8) { fr=0; to=8; }
      for (int k=fr; k<to; k++) {
          int mul=10000;
          if (dx[k]==-1) mul=min(mul,lx);
          if (dx[k]==1) mul=min(mul,(int)bd.size()-lx-1);
          if (dy[k]==-1) mul=min(mul,ly);
          if (dy[k]==1) mul=min(mul,(int)bd[0].size()-ly-1);
          int nx=lx+dx[k]*mul,ny=ly+dy[k]*mul;
          if (!(lx==nx && ly==ny) && bd[nx][ny]!=0) {
             reach[nx][ny]+=plus; oper++;
             }
          }
      return oper;
      }
   return oper;
}

double init_heu () {
   double ret=0.0;
   int sum=0;
   for (int i=0; i<bd.size(); i++)
       for (int j=0; j<bd[0].size(); j++)
           sum+=reach[i][j];
   reachsum=sum;
   ret=sum*1.0/(bd.size()*bd[0].size()-emptysq);
   return ret;
}

double cal_heu2 (double ret,int nowreach,int net,int xx,int yy) {
   ret=(nowreach+net)*1.0/(bd.size()*bd[0].size()-emptysq);
   return ret;
}

double cal_heu1 (double ret,int nowreach,int net,int xx,int yy) {
   ret+=depth[xx][yy]*3.0-reach[xx][yy];
   if (bd[xx][yy]>=6 && K>=2) ret+=0.5;
   return ret;
}

double cal_heu (double ret,int nowreach,int net,int xx,int yy) {
   ret+=depth[xx][yy]*3.0-reach[xx][yy];
   if (bd[xx][yy]>=6) ret+=0.8*(depth[xx][yy]+5);
   if (bd[xx][yy]==8) ret-=0.2;
   return ret;
}

vector<State> nextstate (State now,int heuno) {
   vector<State> ret;
   State tnow;
   int netrea=updatereach(now.lprev,now.lx,now.ly,0);
   if (now.lprev>=1 && now.lprev<=4) {
      for (int k=0; k<8; k++) {
          int nx=now.lx+dx[k]*now.lprev,ny=now.ly+dy[k]*now.lprev;
          if (nx>=0 && nx<bd.size() && ny>=0 && ny<bd[0].size() && bd[nx][ny]!=0 && now.v[nx][ny]==0) {
             tnow=now;
             tnow.lx=nx; tnow.ly=ny; tnow.v[nx][ny]=1; tnow.lprev=bd[nx][ny];
             tnow.lvl=now.lvl+1;
             if (depth[now.lx][now.ly]==1) {
                netrea+=reach[now.lx][now.ly];
                }
             if (heuno==0) tnow.heur=cal_heu(now.heur,now.nowreach,-netrea,nx,ny);
                else if (heuno==1) tnow.heur=cal_heu1(now.heur,now.nowreach,-netrea,nx,ny);
                else tnow.heur=cal_heu2(now.heur,now.nowreach,-netrea,nx,ny);
             tnow.nowreach-=netrea;
             tnow.rec.push_back(make_pair(nx,ny));
             ret.push_back(tnow);
             }
          }
      }
   if (now.lprev==5) {
      for (int k=0; k<8; k++) {
          int nx=now.lx+dkx[k],ny=now.ly+dky[k];
          if (nx>=0 && nx<bd.size() && ny>=0 && ny<bd[0].size() && bd[nx][ny]!=0 && now.v[nx][ny]==0) {
             tnow=now;
             tnow.lx=nx; tnow.ly=ny; tnow.v[nx][ny]=1; tnow.lprev=bd[nx][ny];
             tnow.lvl=now.lvl+1;
             if (depth[now.lx][now.ly]==1) {
                netrea+=reach[now.lx][now.ly];
                }
             if (heuno==0) tnow.heur=cal_heu(now.heur,now.nowreach,-netrea,nx,ny);
                else if (heuno==1) tnow.heur=cal_heu1(now.heur,now.nowreach,-netrea,nx,ny);
                else tnow.heur=cal_heu2(now.heur,now.nowreach,-netrea,nx,ny);
             tnow.nowreach-=netrea;
             tnow.rec.push_back(make_pair(nx,ny));
             ret.push_back(tnow);
             }
          }
      }
   if (now.lprev>=6 && now.lprev<=8) {
      int fr,to;
      if (now.lprev==6) { fr=4; to=8; }
      if (now.lprev==7) { fr=0; to=4; }
      if (now.lprev==8) { fr=0; to=8; }
      for (int k=fr; k<to; k++) {
          int mul=10000;
          if (dx[k]==-1) mul=min(mul,now.lx);
          if (dx[k]==1) mul=min(mul,(int)bd.size()-now.lx-1);
          if (dy[k]==-1) mul=min(mul,now.ly);
          if (dy[k]==1) mul=min(mul,(int)bd[0].size()-now.ly-1);
          int nx=now.lx+dx[k]*mul,ny=now.ly+dy[k]*mul;
          if (!(px==nx && py==ny) && bd[nx][ny]!=0 && now.v[nx][ny]==0) {
             tnow=now;
             tnow.lx=nx; tnow.ly=ny; tnow.v[nx][ny]=1; tnow.lprev=bd[nx][ny];
             tnow.lvl=now.lvl+1;
             if (depth[now.lx][now.ly]==1) {
                netrea+=reach[now.lx][now.ly];
                }
             if (heuno==0) tnow.heur=cal_heu(now.heur,now.nowreach,-netrea,nx,ny);
                else if (heuno==1) tnow.heur=cal_heu1(now.heur,now.nowreach,-netrea,nx,ny);
                else tnow.heur=cal_heu2(now.heur,now.nowreach,-netrea,nx,ny);
             tnow.nowreach-=netrea;
             tnow.rec.push_back(make_pair(nx,ny));
             ret.push_back(tnow);
             }
          }
      }
   return ret;
}

pair<int,int> nextmovefinal (int funcheu,int searchdepth,int searchlimit) {
   priority_queue<State,vector<State>,pqcmp> pq;
   int maxdepth=0,nowlimit[60];
   memset(nowlimit,0,sizeof(nowlimit));
   int rx=0,ry=0,end=1;
   vector<State> next;
   State now;
   now.heur=heuv;
   now.lx=px; now.ly=py; now.lprev=prev; now.ox=now.lx; now.oy=now.ly;
   now.lvl=0; now.nowreach=reachsum;
   memset(now.v,0,sizeof(now.v));
   pq.push(now);
   while (!pq.empty()) {
      now=pq.top();
      nowlimit[now.lvl]++;
      pq.pop();
      if (nowlimit[now.lvl]>searchlimit) continue;
      if (now.ox!=rx || now.oy!=ry) end=0;
      if (now.lvl==1) {
         now.ox=now.lx; now.oy=now.ly;
         }
      if (now.lvl>maxdepth && now.lvl>2 && end==1) break;
      if (now.lvl>maxdepth) {
         maxdepth=now.lvl;
         rx=now.ox; ry=now.oy;
         seq=now.rec;
         end=1;
         }
      if (now.lvl==searchdepth) {
         break;
         }
      next=nextstate(now,funcheu);
      
      if (nowlimit[2]==1 && next.size()==0) {
         if (bd[now.ox][now.oy]==4 && bd[now.lx][now.ly]==4 && depth[now.ox][now.oy]>2 && depth[now.ly][now.ly]>1) {
            seq=now.rec;
            break;
            }
         }
      
      for (int i=0; i<next.size(); i++)
          pq.push(next[i]);
      }
   
   return make_pair(rx,ry);
}

pair<int,int> nextmoveinit (int funcheu,int lvllim) {
   priority_queue<State,vector<State>,pqcmp> pq;
   int nowlimit[226];
   memset(nowlimit,0,sizeof(nowlimit));
   int rx=-1,ry=-1,end=1,tx,ty;
   double worst=1e10,bestheur=-1e10,curbest=-1e10;
   vector<State> next;
   State now;
   now.heur=heuv;
   now.lx=px; now.ly=py; now.lprev=prev; now.ox=now.lx; now.oy=now.ly;
   now.lvl=0; now.nowreach=reachsum;
   memset(now.v,0,sizeof(now.v));
   pq.push(now);
   while (!pq.empty()) {
      now=pq.top();
      pq.pop();
      nowlimit[now.lvl]++;
      if (nowlimit[now.lvl]>lvllim) continue;
      if (now.lvl==1) {
         now.ox=now.lx; now.oy=now.ly;
         }
      if (nowlimit[now.lvl]==1) {
         seq=now.rec;
         rx=now.ox; ry=now.oy;
         }
      next=nextstate(now,funcheu);
      for (int i=0; i<next.size(); i++) {
          pq.push(next[i]);
          }
      }
   return make_pair(rx,ry);
}

class ChessPuzzle {
   public:
      string start(int K1, vector <string> board) {
          startt();
          K=K1;
          bd.resize(board.size());
          for (int i=0; i<bd.size(); i++) {
              bd[i].resize(board[0].length());
              for (int j=0; j<bd[0].size(); j++) {
                  depth[i][j]=K;
                  bd[i][j]=boardcode(board[i][j]);
                  }
              }
          memset(reach,0,sizeof(reach));
          for (int i=0; i<bd.size(); i++)
              for (int j=0; j<bd[0].size(); j++) {
                  updatereach(bd[i][j],i,j,1);
                  }
                  
          vector<pair<int,pair<int,int> > > cand;
          vector<pair<int,int> > bestseq;
          int leastr=10000;
          for (int i=1; i<bd.size()-1; i++)
              for (int j=1; j<bd[0].size()-1; j++)
                  cand.push_back(make_pair(reach[i][j],make_pair(i,j)));
          sort(cand.begin(),cand.end());
          cand.resize(bd.size()*bd[0].size()/8);
          if (K==1) {
             double rough=runtime();
             for (int i=0; runtime()+rough<timelim*7.0/10 && i<cand.size(); i++) {
                 px=cand[i].second.first; py=cand[i].second.second; prev=bd[px][py];
                 nextmoveinit(1,300);
                 if (seq.size()>bestseq.size()) {
                    bestseq=seq; k12heu=1;
                    }
                 if (i==0) rough=runtime()-rough;
                 if (runtime()+rough>timelim*7.0/10) break;
                 px=cand[i].second.first; py=cand[i].second.second; prev=bd[px][py];
                 nextmoveinit(2,300);
                 if (seq.size()>bestseq.size()) {
                    bestseq=seq; k12heu=2;
                    }
                 }
             seq=bestseq;
             px=seq[0].first; py=seq[0].second;
             seq.erase(seq.begin());
             seq.resize(bd.size()*bd[0].size()/10);
             }
             else { px=cand[0].second.first; py=cand[0].second.second; }
          memset(depcnt,0,sizeof(depcnt));
          depcnt[K]=bd.size()*bd[0].size();
          heuv=init_heu();
          prev=bd[px][py];
          string ret=i2s(px)+" "+i2s(py);
          return ret;
      }
      string click(string revealed) {
          bd[px][py]=boardcode(revealed[0]);
          removed++;
          depcnt[depth[px][py]]--;
          depth[px][py]--;
          depcnt[depth[px][py]]++;
          int curnet=0;
          curnet-=updatereach(prev,px,py,-1);
          curnet+=updatereach(bd[px][py],px,py,1);
          if (depth[px][py]==0) {
             curnet-=reach[px][py];
             emptysq++;
             reach[px][py]=-1;
             }
          reachsum+=curnet;
          pair<int,int> res;
          if (!seq.empty()) {
             res=*seq.begin();
             seq.erase(seq.begin());
             }
             else {


                if (K*bd.size()*bd[0].size()-removed<=max(250,2*(int)bd.size()*(int)bd[0].size())) {
                   int evalim; double tn=runtime();
                   if (indtime<0) {
                      indtime=tn;
                      indleft=K*bd.size()*bd[0].size()-removed;
                      }
                   if (tn>timelim*9.3/10) {
                      evalim=75;
                      }
                      else {
                      if (K*bd.size()*bd[0].size()-removed<indleft-10) {
                         if (K*bd.size()*bd[0].size()-removed>bd.size()*bd[0].size()/3) {
                            double pert=(tn-indtime)/(timelim*9.3/10-indtime);
                            
                            evalim=(int)((toteva/pert*(1-pert))/(K*bd.size()*bd[0].size()-removed-bd.size()*bd[0].size()/3));
                            fprintf(stderr,"%d -> %0.4lf %d\n",toteva,(toteva/pert*(1-pert)),(K*bd.size()*bd[0].size()-removed-bd.size()*bd[0].size()/3));
                            evalim=min(evalim,250);
                            evalim=max(evalim,100);
                            }
                            else evalim=250;
                         }
                         else {
                         evalim=220-(int)((K+8)*(bd.size()*bd[0].size())/25.0);
                         }
                      }
                   fprintf(stderr,"evalim: %d (%0.4lf) %d\n",evalim,tn,K*bd.size()*bd[0].size()-removed); fflush(stderr);
                   res=nextmovefinal(1,55,evalim);
                   if (tn<timelim) seq.resize(1);
                   res=*seq.begin();
                   seq.erase(seq.begin());
                   }
                   else {
                   int evalim; double tn=runtime();
                   
                   if (tn<timelim/3.0) evalim=250;
                      else if (tn<timelim*2.0/3) evalim=(int)((timelim*2.0/3-tn)/(timelim*2.0/3) * 300 + 100);
                      else evalim=80;
                   
                   
                   fprintf(stderr,"evalim: %d (%0.4lf)\n",evalim,tn); fflush(stderr);
                   res=nextmoveinit(0,evalim);
                   if (seq.size()>bd.size()*bd[0].size()*1/2+10) seq.resize(bd.size()*bd[0].size()*1/2);
                      else seq.resize(max(1,(int)seq.size()-20));
                   res=*seq.begin();
                   seq.erase(seq.begin());
                   }
             }
          
          px=res.first; py=res.second;
          prev=bd[px][py];
          string ret=i2s(px)+" "+i2s(py);
          return ret;
      }
};
