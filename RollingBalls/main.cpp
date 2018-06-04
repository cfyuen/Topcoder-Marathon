#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<cmath>
#include<map>
#include<cstring>
#include<queue>
using namespace std;

struct coor {
	int x;
	int y;
	coor(int x=0, int y=0) : x(x), y(y) {}
	coor& operator =(const coor& a) {
		x = a.x;
		y = a.y;
		return *this;
	}
	bool operator==(const coor& a) const {
        if (x == a.x && y == a.y) return true;
        return false;
    }
	bool operator<(const coor& a) const {
        if (x == a.x) return y < a.y;
        return x < a.x;
    }
};

int dx[]={0,1,0,-1},dy[]={-1,0,1,0};
int bd[65][65],bdlvl[65][65];
double hsco[65][65];
int n,m,cntball;
int lg2[1024];
vector<string> ret,curret,bestret;
int bestscore = 0;
coor block[65][65],last[65][65];
int fix[65][65];
coor cfr,cto;

//delete when submit
FILE *debug,*result;
double timelimit = 19.0;

int sttime;
void startt() {
    sttime=clock();
}

double runtime() {
    return (clock()-sttime)/1000.0;
}

//til here

/* have it when submit
double timelimit = 9.0;
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

/******** Utilities ***********/
bool iswall(int x) {
	if (x & (1<<20)) return true;
	return false;
}

int hasball(int x) {
	if ((x & -x) > 0 && (x & -x) < 1024) return lg2[(x & -x)];
	return -1;
}

int hashole(unsigned x) {
	union { double a; int b[2]; };
	a = x;
	if ((b[1] >> 20) - 1023 == 20) return -1;
	if ((b[1] >> 20) - 1023 >= 10) return (b[1] >> 20) - 1033;
	return -1;
}

void display() {
	for (int i=1; i<=n; i++) {
		for (int j=1; j<=m; j++) {
			if (bd[i][j] & (1<<20)) fprintf(debug,"#");
			else if (hasball(bd[i][j]) >= 0 && hashole(bd[i][j]) >= 0) fprintf(debug,"@");
			else if (hashole(bd[i][j]) >= 0) fprintf(debug,"O");
			else if (hasball(bd[i][j]) >= 0) fprintf(debug,"a");
			else fprintf(debug,".");
		}
		fprintf(debug,"\n");
	}
	fprintf(debug,"\n");
	fflush(debug);
}

void displayLvl() {
	for (int i=1; i<=n; i++) {
		for (int j=1; j<=m; j++) {
			if (bdlvl[i][j] == -1) fprintf(debug,"#");
			else if (bdlvl[i][j] > 1000) fprintf(debug,".");
			else fprintf(debug,"%d",bdlvl[i][j]);
		}
		fprintf(debug,"\n");
	}
	fprintf(debug,"\n");
	fflush(debug);
}

/******** Main ***********/
void moveBallLvl(coor st,int lvl) {
	// try incremental add wall
	int bstep = 1000;

	priority_queue<pair<int,coor> > q;
	int v[65][65];
	int step;
	memset(v,127,sizeof(v));
	coor c,nt;
	c.x = st.x; c.y = st.y;
	v[c.x][c.y] = 0;
	q.push(make_pair(0,c));
	//fprintf(debug," init %d,%d - %d\n",c.x,c.y,dir);
	while (!q.empty()) {
		step = -q.top().first;
		c = q.top().second;
		q.pop();
		//fprintf(debug,"   step %d: %d,%d\n",step,c.x,c.y);

		// no place wall
		for (int d=0; d<4; d++) {
			nt.x = c.x; nt.y = c.y;
			while (bdlvl[nt.x+dx[d]][nt.y+dy[d]] >= 0) {
				if (step+1 < v[nt.x][nt.y] && bdlvl[nt.x+dx[d]][nt.y+dy[d]] < lvl) {
					v[nt.x][nt.y] = step+1;
					q.push(make_pair(-(step+1),nt));
					if (bdlvl[nt.x][nt.y] > lvl) {
						bdlvl[nt.x][nt.y] = lvl;
						block[nt.x][nt.y].x = nt.x+dx[d]; block[nt.x][nt.y].y = nt.y+dy[d];
						last[nt.x][nt.y] = c;
					}
				}
				nt.x += dx[d]; nt.y += dy[d];
			}
			if (step+1 < v[nt.x][nt.y]) {
				v[nt.x][nt.y] = step+1;
				q.push(make_pair(-(step+1),nt));
				if (bdlvl[nt.x][nt.y] > lvl) {
					bdlvl[nt.x][nt.y] = lvl;
					block[nt.x][nt.y].x = nt.x+dx[d]; block[nt.x][nt.y].y = nt.y+dy[d];
					last[nt.x][nt.y] = c;
				}
			}
		}
	}
}

void calcBoardScore() {
	memset(bdlvl,127,sizeof(bdlvl));
	for (int i=0; i<n+2; i++)
		for (int j=0; j<m+2; j++) {
			if (i == 0 || j == 0 || i == n+1 || j == m+1) {
				bdlvl[i][j] = -1; continue;
			}
			if (iswall(bd[i][j]))
				bdlvl[i][j] = -1;
			if (hasball(bd[i][j]) >= 0)
				bdlvl[i][j] = 0;
		}
	int curlvl = 0;
	bool filled;
	do {
		curlvl++;
		//fprintf(debug,"Level %d\n",curlvl);
		for (int i=1; i<=n; i++)
			for (int j=1; j<=m; j++)
				if (hasball(bd[i][j]) >= 0) {
					coor st; st.x = i; st.y = j;
					moveBallLvl(st,curlvl);
					if (runtime() > timelimit) goto bye;
				}
		filled = false;
		for (int i=1; i<=n; i++)
			for (int j=1; j<=m; j++)
				if (bdlvl[i][j] == curlvl) { filled = true; goto bye; }
		bye: ;
		if (runtime() > timelimit) break;
		//displayLvl();
	} while (filled && curlvl < 10);
	displayLvl();
}

int getDep(coor cur) {
	fix[cur.x][cur.y] = 1;
	if (hasball(bd[cur.x][cur.y]) >= 0) {
		return 0;
	}
	int res = 0;
	while (bdlvl[cur.x][cur.y]>0) {
		//fprintf(debug,"  %d,%d block %d,%d [%d]\n",cur.x,cur.y,block[cur.x][cur.y].x,block[cur.x][cur.y].y,bdlvl[block[cur.x][cur.y].x][block[cur.x][cur.y].y]); fflush(debug);
		if (bdlvl[block[cur.x][cur.y].x][block[cur.x][cur.y].y] >= 0) {
			//dep[cur.x][cur.y].push_back(block[cur.x][cur.y]);
			res++;
			if (bdlvl[block[cur.x][cur.y].x][block[cur.x][cur.y].y] > 0) {
				res += getDep(block[cur.x][cur.y]);
			}
		}
		cur = last[cur.x][cur.y];
	}
	return res;
}

bool cmpdep(coor a,coor b) {
	return bdlvl[a.x][a.y]>bdlvl[b.x][b.y];
}

coor lastc[1000][65][65];
int lastd[1000][65][65],vb[1000][65][65];
int stmap[65][65];
int stcnt = 0;
queue< pair<pair<int,coor>,coor> > qb;

vector<string> fillDep() {
	while (!qb.empty()) qb.pop();
	stcnt = 0;
	for (int i=1; i<=n; i++)
		for (int j=1; j<=m; j++)
			if (hasball(bd[i][j]) >= 0 && hasball(bd[i][j]) != hashole(bd[i][j]) && fix[i][j] == 0) {
				coor st; st.x = i; st.y = j;
				//fprintf(debug,"  FillDep start %d,%d\n",st.x,st.y);
				lastd[stcnt][st.x][st.y] = -1;
				memset(vb[stcnt],0,sizeof(vb[stcnt]));
				vb[stcnt][st.x][st.y] = 1;
				stmap[st.x][st.y] = stcnt++;
				qb.push(make_pair(make_pair(0,st),st));
			}
	int step,sid;
	coor c,nt,st;
	while (!qb.empty()) {
		step = qb.front().first.first;
		c = qb.front().first.second;
		st = qb.front().second;
		sid = stmap[st.x][st.y];
		//fprintf(debug,"  step %d: [%d] %d,%d\n",step,sid,c.x,c.y); fflush(debug);
        if (fix[c.x][c.y] == 1) {
			//fprintf(debug," (%d,%d)[%d] vs (%d,%d)[%d]\n",st.x,st.y,hasball(bd[st.x][st.y]),c.x,c.y,hashole(bd[c.x][c.y])); fflush(debug);
			if (hashole(bd[c.x][c.y]) < 0 || (hasball(bd[st.x][st.y]) == hashole(bd[c.x][c.y])))
				break;
        }
        qb.pop();
		for (int d=0; d<4; d++) {
			nt.x = c.x; nt.y = c.y;
			// not wall and not ball (exclude self)
			while (!iswall(bd[nt.x+dx[d]][nt.y+dy[d]]) && (hasball(bd[nt.x+dx[d]][nt.y+dy[d]]) < 0 || (nt.x+dx[d] == st.x && nt.y+dy[d] == st.y))) {
				nt.x += dx[d]; nt.y += dy[d];
			}
			if (vb[sid][nt.x][nt.y] == 0) {
				vb[sid][nt.x][nt.y] = 1;
				qb.push(make_pair(make_pair(step+1,nt),st));
				lastc[sid][nt.x][nt.y] = c;
				lastd[sid][nt.x][nt.y] = d;
			}
		}
	}

	vector<string> steps;
	if (qb.empty()) return steps;

	cfr = st; cto = c;
	char rstr[100];
	while (lastd[sid][c.x][c.y] >= 0) {
		//fprintf(debug,"  backtrack (%d,%d - %d)\n",lastc[sid][c.x][c.y].x,lastc[sid][c.x][c.y].y,lastd[sid][c.x][c.y]); fflush(debug);
		sprintf(rstr,"%d %d %d",lastc[sid][c.x][c.y].x-1,lastc[sid][c.x][c.y].y-1,lastd[sid][c.x][c.y]);
		steps.push_back(rstr);
		c = lastc[sid][c.x][c.y];
	}
	reverse(steps.begin(),steps.end());
	return steps;
}

bool cmphole(coor a,coor b) {
	return bdlvl[a.x][a.y]<bdlvl[b.x][b.y];
}

bool revcmphole(coor a,coor b) {
	return bdlvl[a.x][a.y]>bdlvl[b.x][b.y];
}

void calculate(int curiter) {
	fprintf(stderr," Score Time: %f\n",runtime()); fflush(stderr);
	// bottleneck
	calcBoardScore();

	// generate sequence
	vector<coor> seq;
	for (int i=1; i<=n; i++)
		for (int j=1; j<=m; j++)
			if (hashole(bd[i][j]) >= 0) {
				coor hole; hole.x = i; hole.y = j;
				seq.push_back(hole);
			}
	if (curiter == 0) sort(seq.begin(),seq.end(),cmphole);
	else if (curiter == 1) sort(seq.begin(),seq.end(),revcmphole);
	else random_shuffle(seq.begin(),seq.end());
	fprintf(stderr," Seq Time: %f\n",runtime()); fflush(stderr);


	for (int i=0; i<seq.size(); i++) {
		fprintf(debug,"Hole %d: %d,%d = %d\n",i,seq[i].x,seq[i].y,bdlvl[seq[i].x][seq[i].y]);

		vector<string> holesteps;
		int oldbd[65][65];
		memcpy(oldbd,bd,sizeof(bd));
		bool success = false;
		int iter = 0;

		do {
			//fprintf(debug," Iteration %d [%d,%d]\n",iter,seq[i].x,seq[i].y); fflush(debug);
			memset(fix,0,sizeof(fix));
			int depcnt = getDep(seq[i]);
			//fprintf(debug,"  depcnt = %d\n",depcnt); fflush(debug);
			vector<string> steps = fillDep();
			if (!steps.empty()) {
				fprintf(debug," steps: %d -- %d,%d -> %d,%d\n",steps.size(),cfr.x,cfr.y,cto.x,cto.y); fflush(debug);
				int ball = hasball(bd[cfr.x][cfr.y]);
				if (ball >= 0) bd[cfr.x][cfr.y] -= (1<<ball);
				bd[cto.x][cto.y] += (1<<ball);
				holesteps.insert(holesteps.end(),steps.begin(),steps.end());
				if (cto == seq[i]) {
					success = true;
					break;
				}
			}
			else {
				success = false;
				break;
			}
			iter++;
		} while (iter < 1000);

		if (success) {
			fprintf(debug,"SUCCESS!\n");
			curret.insert(curret.end(),holesteps.begin(),holesteps.end());
		}
		else {
			fprintf(debug,"FAILED!\n");
			memcpy(bd,oldbd,sizeof(bd));
		}
		if (runtime() > timelimit) break;
	}

}


class RollingBalls {
    public:
		vector <string> restorePattern(vector <string> start, vector <string> target) {
			startt();

			for (int i=0; i<10; i++) {
				lg2[(1<<i)] = i;
			}
			ret.clear();
			cntball = 0;
			n = start.size(); m = start[0].length();
			memset(bd,0,sizeof(bd));
			for (int i=0; i<n+2; i++)
				for (int j=0; j<m+2; j++) {
					if (i == 0 || j == 0 || i == n+1 || j == m+1) {
						bd[i][j] = (1<<20);
						continue;
					}
					if (start[i-1][j-1] == '#') {
						bd[i][j] = (1<<20);
					}
					else {
						if (start[i-1][j-1] >= '0' && start[i-1][j-1] <= '9') {
							bd[i][j] += (1<<(start[i-1][j-1]-'0'));
							cntball++;
						}
						if (target[i-1][j-1] >= '0' && target[i-1][j-1] <= '9') {
							bd[i][j] += (1<<(target[i-1][j-1]-'0'+10));
						}
					}
				}
			display();

			int maxstep = cntball*20;
			bestscore = 0;

			int oribd[65][65];
			int totaliter = 0;

			memcpy(oribd,bd,sizeof(bd));

			do {
				int lastsize = 0;
				curret.clear();
				do {
					fprintf(debug,"Run again (Time: %f)\n",runtime());
					fprintf(stderr,"Run again (Time: %f)\n",runtime());
					lastsize = curret.size();
					calculate(totaliter);
				} while (maxstep >= curret.size() && lastsize != curret.size() && runtime() < timelimit);

				int curscore = 0;
				for (int i=1; i<=n; i++)
					for (int j=1; j<=m; j++)
						if (hashole(bd[i][j]) >= 0 && hasball(bd[i][j]) >= 0) {
							if (hashole(bd[i][j]) == hasball(bd[i][j])) curscore += 2;
							else curscore += 1;
						}
				fprintf(debug,"Iter %d: Bestscore = %d  cur score = %d\n",totaliter,bestscore,curscore);
				fprintf(stderr,"Iter %d: Bestscore = %d  cur score = %d\n",totaliter,bestscore,curscore);
				if (curscore > bestscore) {
					bestret = curret;
					bestscore = curscore;
				}
				memcpy(bd,oribd,sizeof(bd));
				totaliter++;
			} while (runtime() < timelimit);

			ret = bestret;

			if (ret.size() > maxstep) {
				ret.resize(maxstep);
			}

			//for (int i=0; i<ret.size(); i++)
			//	fprintf(debug,"%s\n",ret[i].c_str());

            return ret;
        }
};

int main() {
    RollingBalls RB;
    debug=fopen("D:\\Document\\Computer\\TopCoder\\RollingBalls\\debug.txt","w");
    int N;
    scanf("%d",&N);
    vector<string> Start,Target;
    Start.resize(N);
    char BoardStr[1005];
    for (int i=0; i<N; i++) {
		scanf("%s",BoardStr);
		Start[i] = BoardStr;
    }
    scanf("%d",&N);
    Target.resize(N);
    for (int i=0; i<N; i++) {
		scanf("%s",BoardStr);
		Target[i] = BoardStr;
    }
	vector<string> RET;
	RET = RB.restorePattern(Start, Target);
	printf("%d\n",RET.size());
	for (int i=0; i<RET.size(); i++){
		printf("%s\n",RET[i].c_str());
	}

	fflush(stdout);
	fflush(debug);
    fflush(stderr);

}
