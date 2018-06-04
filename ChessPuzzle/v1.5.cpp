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
int reachsum=0,emptysq=0;
vector<pair<int,int> > seq;
double heuv;

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


//delete when submit
FILE *debug,*result;

int sttime;
int tcnt=0;
void startt() {
    sttime=clock();
}

double runtime() {
    return (clock()-sttime)/1000.0;
}

void disbd () {
    fprintf(debug,"heuv = %0.4lf\n",heuv);
    for (int i=0; i<bd.size(); i++) {
        for (int j=0; j<bd[0].size(); j++)
            fprintf(debug,"%d ",bd[i][j]);
        fprintf(debug,"\n");
        }
    fprintf(debug,"\n");
    for (int i=0; i<bd.size(); i++) {
        for (int j=0; j<bd[0].size(); j++)
            fprintf(debug,"%3d ",reach[i][j]);
        fprintf(debug,"           ");
        for (int j=0; j<bd[0].size(); j++) {
            if (depth[i][j]==0) fprintf(debug,". ");
            if (depth[i][j]==1) fprintf(debug,"a ");
            if (depth[i][j]==2) fprintf(debug,"b ");
            if (depth[i][j]==3) fprintf(debug,"c ");
            if (depth[i][j]==4) fprintf(debug,"d ");
            if (depth[i][j]==5) fprintf(debug,"e ");
            if (depth[i][j]==6) fprintf(debug,"F ");
            if (depth[i][j]==7) fprintf(debug,"G ");
            if (depth[i][j]==8) fprintf(debug,"H ");
            if (depth[i][j]==9) fprintf(debug,"I ");
            if (depth[i][j]==10) fprintf(debug,"J ");
            }
        fprintf(debug,"\n");
        }
    fprintf(debug,"\n");
    for (int i=0; i<bd.size(); i++) {
        for (int j=0; j<bd[0].size(); j++)
            fprintf(debug,"%4.2lf ",reach[i][j]*1.0/(depth[i][j]+1));
        fprintf(debug,"           ");
        for (int j=0; j<bd[0].size(); j++)
            fprintf(debug,"%3d ",reach[i][j]-depth[i][j]*3);
        fprintf(debug,"\n");
        }
    fprintf(debug,"\n"); fprintf(debug,"\n");
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
   //depth SD
   //average reachability
   int sum=0;
   for (int i=0; i<bd.size(); i++)
       for (int j=0; j<bd[0].size(); j++)
           sum+=reach[i][j];
   reachsum=sum;
   ret=sum*1.0/(bd.size()*bd[0].size()-emptysq);
   return ret;
}


double cal_heu (double ret,int nowreach,int net,int xx,int yy) {
   ret+=depth[xx][yy]*3.0-reach[xx][yy];
   //ret+=depth[xx][yy]*(-reach[xx][yy]);
   return ret;
}

vector<State> nextstate (State now) {
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
             tnow.heur=cal_heu(now.heur,now.nowreach,-netrea,nx,ny);
             tnow.nowreach-=netrea;
             tnow.rec.push_back(make_pair(nx,ny));
             //if (nx>=1 && nx<bd.size()-1 && ny>=1 && ny<bd[0].size()-1) tnow.center++;
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
             tnow.heur=cal_heu(now.heur,now.nowreach,-netrea,nx,ny);
             tnow.nowreach-=netrea;
             tnow.rec.push_back(make_pair(nx,ny));
             //if (nx>=1 && nx<bd.size()-1 && ny>=1 && ny<bd[0].size()-1) tnow.center++;
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
             tnow.heur=cal_heu(now.heur,now.nowreach,-netrea,nx,ny);
             tnow.nowreach-=netrea;
             tnow.rec.push_back(make_pair(nx,ny));
             //if (nx>=1 && nx<bd.size()-1 && ny>=1 && ny<bd[0].size()-1) tnow.center++;
             ret.push_back(tnow);
             }
          }
      }
   return ret;
}

pair<int,int> nextmove2a () {
   priority_queue<State,vector<State>,pqcmp> pq;
   //priority_queue<State> pq;
   int searchdepth=55,maxdepth=0,searchlimit=100,nowlimit[60];
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
      //fprintf(debug,"sz: %d\n",pq.size());
      fprintf(debug,"NOW: lvl=%d  (%d,%d)[%d] = %0.2lf    (%d,%d)\n",now.lvl,now.lx,now.ly,now.lprev,now.heur,now.ox,now.oy);
      /*
      for (int i=0; i<bd.size(); i++) {
          for (int j=0; j<bd[0].size(); j++)
              fprintf(debug,"%d",now.v[i][j]);
          fprintf(debug,"\n");
          }
      */
      //fprintf(debug,"%d\n",now.dep.size());
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
      next=nextstate(now);
      for (int i=0; i<next.size(); i++)
          pq.push(next[i]);
      }
   
   return make_pair(rx,ry);
}

pair<int,int> nextmove3 () {
   //set target
   priority_queue<State,vector<State>,pqcmp> pq;
   int searchlimit=200,nowlimit[226],lvllim=200;
   memset(nowlimit,0,sizeof(nowlimit));
   int rx=-1,ry=-1,end=1,tx,ty;
   double worst=1e10,bestheur=-1e10;
   for (int i=0; i<bd.size(); i++)
       for (int j=0; j<bd[0].size(); j++)
           if (reach[i][j]*1.0/depth[i][j]<worst && !(i==px && j==py)) {
              worst=reach[i][j]*1.0/depth[i][j];
              tx=i; ty=j;
              }
   fprintf(debug,"target: (%d,%d)\n",tx,ty);
   vector<State> next;
   State now;
   now.heur=heuv;
   now.lx=px; now.ly=py; now.lprev=prev; now.ox=now.lx; now.oy=now.ly;
   now.lvl=0; now.nowreach=reachsum;
   memset(now.v,0,sizeof(now.v));
   pq.push(now);
   while (!pq.empty() || searchlimit==0) {
      now=pq.top();
      pq.pop();
      nowlimit[now.lvl]++;
      if (nowlimit[now.lvl]>lvllim) continue;
      //if (now.ox!=rx || now.oy!=ry) end=0;
      if (now.lvl==1) {
         now.ox=now.lx; now.oy=now.ly;
         }
      //fprintf(debug,"sz: %d\n",pq.size());
      fprintf(debug,"NOW: lvl=%d  (%d,%d)[%d] = %0.2lf    (%d,%d)\n",now.lvl,now.lx,now.ly,now.lprev,now.heur,now.ox,now.oy);
      //fprintf(debug,"%d\n",now.dep.size());
      //if (now.lvl>maxdepth && now.lvl>2 && end==1) break;
      if (now.lx==tx && now.ly==ty) {
         searchlimit--;
         if (now.heur>bestheur) {
            bestheur=now.heur;
            rx=now.ox; ry=now.oy;
            }
         continue;
         }
      next=nextstate(now);
      for (int i=0; i<next.size(); i++) {
          pq.push(next[i]);
          }
      }
   seq=now.rec;
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
                  
          int leastr=10000;
          for (int i=1; i<bd.size()-1; i++)
              for (int j=1; j<bd[0].size()-1; j++)
                  if (reach[i][j]<leastr) {
                     leastr=reach[i][j];
                     px=i; py=j;
                     }
          memset(depcnt,0,sizeof(depcnt));
          depcnt[K]=bd.size()*bd[0].size();
          heuv=init_heu();
          //px=bd.size()/2; py=bd[0].size()/2;
          prev=bd[px][py];
          string ret=i2s(px)+" "+i2s(py);
          fprintf(debug,"(%s) = %d\n",ret.c_str(),prev);
          disbd();
          fflush(debug);
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
          //heuv=cal_heu(heuv,reachsum,curnet);
          reachsum+=curnet;
          pair<int,int> res;
          //res=nextmove2a();
          if (!seq.empty()) {
             res=*seq.begin();
             seq.erase(seq.begin());
             }
             else {
             if (K*bd.size()*bd[0].size()-removed<2*bd.size()*bd[0].size()) {
                res=nextmove2a();
                seq.resize(1);
                fprintf(debug,"hehe: (%d,%d) (%d,%d)\n",res.first,res.second,seq[0].first,seq[0].second);
                res=*seq.begin();
                seq.erase(seq.begin());
                }
                else {
                res=nextmove3();
                if (seq.size()>bd.size()*bd[0].size()*1/2) seq.resize(bd.size()*bd[0].size()*1/2);
                res=*seq.begin();
                seq.erase(seq.begin());
                }
             }
          
          px=res.first; py=res.second;
          prev=bd[px][py];
          string ret=i2s(px)+" "+i2s(py);
          fprintf(debug,"(%s) = %d\n",ret.c_str(),prev);
          //fprintf(stderr,"%0.4lf\n",runtime()); fflush(stderr);
          disbd();
          fflush(debug);
          return ret;
      }
};

int main(){
    ChessPuzzle CP;
    debug=fopen("D:\\Documents\\Computer\\TopCoder\\ChessPuzzle\\debug.txt","w");
    int K,R;
    scanf("%d%d",&K,&R);
    vector<string> BD;
    BD.resize(R);
    for (int i=0; i<R; i++)
        cin >> BD[i];
    cout << CP.start(K,BD) << endl;
    fflush(stdout);
    while (true) {
        string Rev;
        cin >> Rev;
        cout << CP.click(Rev) << endl;
        fflush(stdout);
        }
    
}
