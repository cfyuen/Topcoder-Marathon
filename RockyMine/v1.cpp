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
v1 Approach:
	1. DFS find best moves in several steps
	2. Score each move
		a. Gold destroyed
		b. Gold collected
		c. Difficulty of getting other golds
		d. Amount of Golds nearby
 *********************/

struct Bomb {
	int left;
	int eff[6][6];
} b[10];

struct Grid {
	int gold,rock;
} g[105][105];

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

State eql (State a) {
    return a;
}

string dir (int d) {
	if (d==0) return "S";
	if (d==1) return "E";
	if (d==2) return "N";
	if (d==3) return "W";
	return "-";
}

class RockyMine {
	public:
		int bestsco;
		string bestmove;
		State np;
		void dfs (State p) {
			fprintf(debug,"%d - (%d,%d) %d %s\n",p.mv,p.x,p.y,p.gc,p.route.c_str());
			if (p.mv==0) {
				if (p.gc*2-p.gd>bestsco) {
					bestsco=p.gc*2-p.gd;
					bestmove=p.route;
				}
				return;
			}
			//update
			PutBomb bbed;
			bbed.tl=0;
			for (int i=0; i<p.pb.size(); i++) {
				p.pb[i].tl--;
				if (p.pb[i].tl<0) {
					int sj,sk,ok=1;
					sj=p.pb[i].x-2; sk=p.pb[i].y-2;
					for (int j=max(0,sj); j<=min(n-1,sj+4); j++)
						for (int k=max(0,sk); k<=min(m-1,sk+4); k++) {
							if (p.x==j && p.y==k && b[p.pb[i].typ].eff[j-sj][k-sk]>0) ok=0;
							}
					if (ok==0) return;
					bbed.tl=1;
					bbed.typ=p.pb[i].typ;
					bbed.x=p.pb[i].x; bbed.y=p.pb[i].y;
					for (int j=max(0,sj); j<=min(n-1,sj+4); j++)
						for (int k=max(0,sk); k<=min(m-1,sk+4); k++) {
							g[j][k].rock-=b[p.pb[i].typ].eff[j-sj][k-sk];
							}
					p.pb.erase(p.pb.begin()+i);
					i--;
				}
			}
			//move NESW-
			for (int i=0; i<4; i++) {
			    int nx,ny;
			    nx=p.x+dx[i]; ny=p.y+dy[i];
			    if (nx>=0 && ny>=0 && nx<n && ny<m && g[nx][ny].rock<=0) {
					np=eql(p);
					if (g[nx][ny].rock==0) np.gc+=g[nx][ny].gold; 
					np.route+=dir(i);
					np.x=nx; np.y=ny;
					np.mv--;
					g[nx][ny].rock--;
					dfs(np);
					g[nx][ny].rock++;
				}
			}
			//dynamite
			if (p.pb.size()<2) {
			for (int i=0; i<dn; i++) {
				if (b[i].left>0) {
					//fprintf(debug,"Bomb %d\n",i);
					b[i].left--;
					PutBomb nb;
					nb.x=p.x; nb.y=p.y; nb.typ=i; nb.tl=5;
					np=eql(p);
					np.route+=i+'0';
					np.mv--;
					np.pb.push_back(nb);
					dfs(np);
					b[i].left++;
				}
			}
			}
			//reverse bomb
			if (bbed.tl==1) {
				int si,sj;
				si=bbed.x-2; sj=bbed.y-2;
				for (int i=max(0,si); i<=min(n-1,si+4); i++)
					for (int j=max(0,sj); j<=min(m-1,sj+4); j++)
						g[i][j].rock+=b[bbed.typ].eff[i-si][j-sj];
			}
		}

		void eval () {
			bestsco=0;
			State nst;
			nst.x=0; nst.y=0; nst.mv=10;
			nst.gc=0; nst.gd=0;
			nst.route="";
			dfs(nst);
		}
		string collectGold(vector <int> dynamite, vector <int> effect, int W, vector <int> Gold, vector <int> rocks, int maxMoves) {
			//initialize
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
			//evaluate
			eval();
			string ret;
			ret=bestmove;
			fprintf(debug,"ret: %s\n",ret.c_str());
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

