#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<cmath>
using namespace std;
/*********************
v2 Approach:
	1. Rank the gold to take
	    a. Gold value
	    b. Surrounding gold value
	    c. Estimated round of bomb to get the gold
	    d. Number of steps required to take the gold
	2. Get the gold efficiently
 *********************/

struct Bomb {
	int left;
	int eff[6][6];
} b[10];

struct Grid {
	int gold,rock;
} g[105][105];

struct Coor {
	int x,y;
};

struct PutBomb {
	int x,y,typ,tl;
};

struct State {
	int x,y;
	int mv; //move left
	int gc,gd; //gold collected, gold destroyed
	string route;
	vector<PutBomb> pb;
};

int n,m,dn,move;
int dx[]={1,0,-1,0,0},dy[]={0,1,0,-1,0};
int reach[105][105];
PutBomb cur[300],fin[300];
int bplace=0,bbsco[300],bfin=0;

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

void disbd () {
	for (int i=0; i<n; i++) {
	    for (int j=0; j<m; j++)
	        fprintf(debug,"%d(%d) ",g[i][j].rock,g[i][j].gold);
		fprintf(debug,"\n");
	}
	fflush(debug);
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

string dir (int d) {
	if (d==0) return "S";
	if (d==1) return "E";
	if (d==2) return "N";
	if (d==3) return "W";
	return "-";
}

void fill (Coor at) {
	for (int i=0; i<4; i++) {
		Coor nat;
		nat.x=at.x+dx[i]; nat.y=at.y+dy[i];
		if (nat.x>=0 && nat.y>=0 && nat.x<n && nat.y<m && reach[nat.x][nat.y]==0 && g[nat.x][nat.y].rock<=0) {
			reach[nat.x][nat.y]=1;
			fill(nat);
		}
	}
}

class RockyMine {
	public:
		Coor target (Coor at) {
			Coor bc;
			double bestsco=-1e9;
			for (int i=0; i<n; i++)
			    for (int j=0; j<m; j++) {
					double cursco=0.0;
					cursco+=g[i][j].gold;
					for (int x=i-5; x<=i+5; x++)
					    for (int y=j-5; y<=j+5; y++)
					        if (x>=0 && y>=0 && x<n && y<m && !(x==i && y==j)) {
								cursco+=g[x][y].gold*0.6/(abs(x-i)+abs(y-j));
							}
					cursco-=abs(at.x-i)+abs(at.y-j);
					if (cursco>bestsco) {
						bestsco=cursco;
						bc.x=i; bc.y=j;
					}
					//fprintf(debug,"(%d,%d) %0.4lf\n",i,j,cursco);
				}
			return bc;
		}
		int scorepb (int tgc,int tgd,int numb) {
			return tgc*4-tgd-numb*max(40,max(n,m));
		}
		void placebomb (Coor at,Coor tar,int tgc,int tgd) {
			if (runtime()>6.0) return;			
			memset(reach,0,sizeof(reach));
			reach[at.x][at.y]=1;
			fill(at);
			fprintf(debug,"(%d,%d)[%d] %d,%d=%d [%d]\n",at.x,at.y,bplace,tgc,tgd,scorepb(tgc,tgd,bplace),bbsco[0]);
			//for (int i=0; i<bplace; i++)
			//	fprintf(debug,"  (%d,%d)-%d\n",cur[i].x,cur[i].y,cur[i].typ);
			//disbd();
			if (reach[tar.x][tar.y]==1) {
				if (g[tar.x][tar.y].rock==0 && scorepb(tgc,tgd,bplace)>=bbsco[0]) {
					bbsco[0]=scorepb(tgc,tgd,bplace);
					bfin=bplace;
					disbd();
					for (int i=0; i<bfin; i++)
						fin[i]=cur[i];
				}
				return;
			}
			int mindis=1000;
			for (int i=0; i<n; i++)
			    for (int j=0; j<m; j++)
					if (reach[i][j]==1) {
						mindis=min(abs(i-tar.x)+abs(j-tar.y),mindis);
					}
			fprintf(debug,"Dist = %d\n",mindis);
			if (scorepb(tgc,tgd,bplace)<bbsco[mindis]-100) return;
			bbsco[mindis]=max(bbsco[mindis],scorepb(tgc,tgd,bplace));
			Coor pt[100]; int pn=0;
			for (int i=0; i<n; i++)
				for (int j=0; j<m; j++)
				    if (reach[i][j]==1 && abs(i-tar.x)+abs(j-tar.y)<=mindis+1) {
						pt[pn].x=i; pt[pn].y=j;
						pn++;
					}
			fprintf(debug,"Possible move = %d\n",pn);
			int ngc,ngd;
			for (int i=0; i<pn; i++) {
				for (int j=0; j<dn; j++)
				    if (b[j].left>0) {
						b[j].left--;
						PutBomb nb;
						nb.x=pt[i].x; nb.y=pt[i].y; nb.typ=j;
						int sx,sy;
						sx=nb.x-2; sy=nb.y-2;
						ngc=tgc; ngd=tgd;
						for (int xx=max(0,sx); xx<=min(n-1,sx+4); xx++)
						    for (int yy=max(0,sy); yy<=min(m-1,sy+4); yy++) {
								g[xx][yy].rock-=b[j].eff[xx-sx][yy-sy];
								if (g[xx][yy].rock==0 && b[j].eff[xx-sx][yy-sy]!=0) ngc+=g[xx][yy].gold;
								if (g[xx][yy].rock<0 && g[xx][yy].rock+b[j].eff[xx-sx][yy-sy]>0) ngd+=g[xx][yy].gold;
							}
						cur[bplace]=nb;
						bplace++;
						placebomb(pt[i],tar,ngc,ngd);
						bplace--;
						b[j].left++;
						for (int xx=max(0,sx); xx<=min(n-1,sx+4); xx++)
						    for (int yy=max(0,sy); yy<=min(m-1,sy+4); yy++)
								g[xx][yy].rock+=b[j].eff[xx-sx][yy-sy];
					}
			}
		}
		pair<Coor,string> saferoute (Coor at,int bbid) {
			fprintf(debug,"(%d,%d) -> (%d,%d)\n",at.x,at.y,fin[bbid].x,fin[bbid].y);
			int q1,q2,v[105][105];
			memset(v,0,sizeof(v));
			Coor q[1000],nat;
			string rec[1000];
			q1=0; q2=0;
			q[0].x=at.x; q[0].y=at.y;
			rec[0]="";
			v[at.x][at.y]=1;
			while (q1<=q2) {
				//fprintf(debug,"at %d,%d %s\n",q[q1].x,q[q1].y,rec[q1].c_str());
				q1++;
				if (q[q1-1].x==fin[bbid].x && q[q1-1].y==fin[bbid].y) break;
				for (int i=0; i<4; i++) {
					nat.x=q[q1-1].x+dx[i]; nat.y=q[q1-1].y+dy[i];
					if (nat.x>=0 && nat.y>=0 && nat.x<n && nat.y<m && v[nat.x][nat.y]==0 && g[nat.x][nat.y].rock<=0) {
						v[nat.x][nat.y]=1;
						q2++;
						rec[q2]=rec[q1-1]+dir(i);
						q[q2].x=nat.x; q[q2].y=nat.y;
					}
				}
			}
			string ret=rec[q1-1];
			ret+=fin[bbid].typ+'0';
			q1=0; q2=0;
			q[0].x=fin[bbid].x; q[0].y=fin[bbid].y;
			memset(v,0,sizeof(v));
			v[q[0].x][q[0].y]=1;
			rec[0]="";
			while (q1<=q2) {
				q1++;
				if (abs(q[q1-1].x-fin[bbid].x)>=3 || abs(q[q1-1].y-fin[bbid].y)>=3) break;
				if (b[fin[bbid].typ].eff[q[q1-1].x-fin[bbid].x+2][q[q1-1].y-fin[bbid].y+2]==0) break;
				for (int i=0; i<4; i++) {
					nat.x=q[q1-1].x+dx[i]; nat.y=q[q1-1].y+dy[i];
					if (nat.x>=0 && nat.y>=0 && nat.x<n && nat.y<m && v[nat.x][nat.y]==0 && g[nat.x][nat.y].rock<=0) {
						v[nat.x][nat.y]=1;
						q2++;
						rec[q2]=rec[q1-1]+dir(i);
						q[q2].x=nat.x; q[q2].y=nat.y;
					}
				}
			}
			ret+=rec[q1-1];
			for (int i=rec[q1-1].length(); i<5; i++) ret+='-';
			return make_pair(q[q1-1],ret);
		}
		string moveto (Coor at,Coor tar) {
			//Design a bombing scheme
			//  1. Get the target gold
			//  2. Do not damage other gold
			//  Score heuristically
			bplace=0;
			for (int i=0; i<300; i++) bbsco[i]=-100000;
			placebomb(at,tar,0,0);
			fprintf(debug,"best sco = %d\n",bbsco[0]);
			for (int i=0; i<bfin; i++)
				fprintf(debug,"  (%d,%d)-%d\n",fin[i].x,fin[i].y,fin[i].typ);
			//Design the least number of moves to go to target
			string ret="",nmv;
			pair<Coor,string> pcs;
			for (int i=0; i<bfin; i++) {
				pcs=saferoute(at,i);
				int sx,sy;
				sx=fin[i].x-2; sy=fin[i].y-2;
				for (int xx=max(0,sx); xx<=min(n-1,sx+4); xx++)
					for (int yy=max(0,sy); yy<=min(m-1,sy+4); yy++) {
						g[xx][yy].rock-=b[fin[i].typ].eff[xx-sx][yy-sy];
					}
				at.x=pcs.first.x; at.y=pcs.first.y;
				nmv=pcs.second;
				ret+=nmv;
				disbd();
				fprintf(debug,"bomb %d(%d,%d) : %s\n",i,fin[i].x,fin[i].y,nmv.c_str());
			}
			return ret;

		}
		string collectGold(vector <int> dynamite, vector <int> effect, int W, vector <int> Gold, vector <int> rocks, int maxMoves) {
			//initialize
			start();
			dn=dynamite.size();
			for (int i=0; i<dn; i++) {
				b[i].left=dynamite[i];
				for (int j=0; j<5; j++)
					for (int k=0; k<5; k++)
						b[i].eff[j][k]=effect[i*25+j*5+k];
			}
			m=W; n=Gold.size()/m;
			for (int i=0; i<n; i++)
			    for (int j=0; j<m; j++) {
			        g[i][j].gold=Gold[i*m+j];
			        g[i][j].rock=rocks[i*m+j];
				}
			move=maxMoves;
			memset(reach,0,sizeof(reach));
			reach[0][0]=1;
			Coor tc;
			tc.x=tc.y=0;
			fill(tc);
			//evaluate

			disbd();
			Coor now;
			now.x=0; now.y=0;
			now=target(now);
			fprintf(debug,"Best: (%d,%d)\n",now.x,now.y);
			string ret;
			ret=moveto(tc,now);
			ret.resize(move);
			fflush(debug);
			return ret;
		}
};

int main () {
	debug=fopen("debug.txt","w");
	RockyMine RM;
	int D;
	vector<int> Dym,Eff,GD,RK;
	scanf("%d",&D);
	Dym.resize(D);
	Eff.resize(25*D);
	for (int i=0; i<D; i++) scanf("%d",&Dym[i]);
	for (int i=0; i<25*D; i++) scanf("%d",&Eff[i]);
	int WW,HH;
	scanf("%d",&WW);
	scanf("%d",&HH);
	HH/=WW;
	GD.resize(WW*HH);
	RK.resize(WW*HH);
	for (int i=0; i<WW*HH; i++) scanf("%d",&GD[i]);
	for (int i=0; i<WW*HH; i++) scanf("%d",&RK[i]);
	int MXM;
	scanf("%d",&MXM);
	string RET;
	RET=RM.collectGold(Dym,Eff,WW,GD,RK,MXM);
	printf("%s\n",RET.c_str());
	fflush(stdout);
}

