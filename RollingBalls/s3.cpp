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
vector<string> ret;
coor block[65][65],last[65][65];
int fix[65][65];
coor cfr,cto;

double timelimit = 9.0;
timeval sttime;

void startt() {
    gettimeofday(&sttime,NULL);
}

double runtime() {
    timeval tt, nowt;
    gettimeofday(&tt,NULL);
    timersub(&tt,&sttime,&nowt);
    return (nowt.tv_sec*1000+nowt.tv_usec/1000)/1000.0;
    }


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
	while (!q.empty()) {
		step = -q.top().first;
		c = q.top().second;
		q.pop();

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
	bool hasempty;
	do {
		curlvl++;
		for (int i=1; i<=n; i++)
			for (int j=1; j<=m; j++)
				if (hasball(bd[i][j]) >= 0) {
					coor st; st.x = i; st.y = j;
					moveBallLvl(st,curlvl);
					if (runtime() > timelimit) goto bye;
				}
		hasempty = false;
		for (int i=1; i<=n; i++)
			for (int j=1; j<=m; j++)
				if (bdlvl[i][j] > 1000) { hasempty = true; goto bye; }
		bye: ;
		if (runtime() > timelimit) break;
	} while (hasempty && curlvl < 10);
}

int getDep(coor cur) {
	fix[cur.x][cur.y] = 1;
	if (hasball(bd[cur.x][cur.y]) >= 0) {
		return 0;
	}
	int res = 0;
	while (bdlvl[cur.x][cur.y]>0) {
		if (bdlvl[block[cur.x][cur.y].x][block[cur.x][cur.y].y] >= 0) {
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
	return bdlvl[a.x][a.y]<bdlvl[b.x][b.y];
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
        if (fix[c.x][c.y] == 1) {
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

void calculate() {
	calcBoardScore();

	// generate sequence
	vector<coor> seq;
	for (int i=1; i<=n; i++)
		for (int j=1; j<=m; j++)
			if (hashole(bd[i][j]) >= 0) {
				coor hole; hole.x = i; hole.y = j;
				seq.push_back(hole);
			}
	sort(seq.begin(),seq.end(),cmphole);

	for (int i=0; i<seq.size(); i++) {

		vector<string> holesteps;
		int oldbd[65][65];
		memcpy(oldbd,bd,sizeof(bd));
		bool success = false;
		int iter = 0;

		do {
			memset(fix,0,sizeof(fix));
			int depcnt = getDep(seq[i]);
			vector<string> steps = fillDep();
			if (!steps.empty()) {
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
			if (iter%20 == 0)
				if (runtime() > timelimit) break;
		} while (iter < 1000);

		if (success) {
			ret.insert(ret.end(),holesteps.begin(),holesteps.end());
		}
		else {
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

			int maxstep = cntball*20;

			int lastsize = 0;
			do {
				lastsize = ret.size();
				calculate();
				fprintf(stderr," Time: %f\n",runtime());
			} while (maxstep >= ret.size() && lastsize != ret.size() && runtime() < timelimit);

			if (ret.size() > maxstep) {
				ret.resize(maxstep);
			}

            return ret;
        }
};
