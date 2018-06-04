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
};

int dx[]={0,1,0,-1},dy[]={-1,0,1,0};
int bd[65][65];
double hsco[65][65];
int n,m,cntball;
int lg2[1024];
vector<string> ret;

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


/*
1. find move ball to pos cheapest by wall
2. each wall cost is calculated by closest ball to replace and recover to original (advanced: use same ball)
*/
int dp[5][65][65];
coor lastc[65][65];
int lastd[65][65];
int cnt[65][65],bcho[65][65];
coor cho[65][65];

void moveBallToPos(coor st) {
	// try incremental add wall
	int bstep = 1000;
	int mxwall = 0;
	bool notdone = true;
	memset(dp,-1,sizeof(dp));
	int ball = hasball(bd[st.x][st.y]);
	if (ball >= 0) bd[st.x][st.y] -= (1<<ball);
	do {
		queue<pair<int,pair<coor,int> > > q;
		int step,cwall;
		coor c,nt;
		c.x = st.x; c.y = st.y;
		q.push(make_pair(0,make_pair(c,0)));
		dp[0][c.x][c.y] = 0;
		lastc[c.x][c.y] = c;
		lastd[c.x][c.y] = -1;
		while (!q.empty()) {
			step = q.front().first;
			c = q.front().second.first;
			cwall = q.front().second.second;
			q.pop();

			// no place wall
			for (int d=0; d<4; d++) {
				nt.x = c.x+dx[d]; nt.y = c.y+dy[d];
				while (!iswall(bd[nt.x][nt.y]) && hasball(bd[nt.x][nt.y]) < 0) {
					nt.x += dx[d]; nt.y += dy[d];
				}
				nt.x -= dx[d]; nt.y -= dy[d];
				if (dp[cwall][nt.x][nt.y] == -1) {
					q.push(make_pair(step+1,make_pair(nt,cwall)));
					dp[cwall][nt.x][nt.y] = step+1;
					lastc[nt.x][nt.y] = c;
					lastd[nt.x][nt.y] = d;
				}
			}
		}
		if (!notdone) break;
		if (mxwall == 0) break;
		mxwall++;
	} while (notdone);
	if (ball >= 0) bd[st.x][st.y] += (1<<ball);
}

vector<string> calculate() {
	memset(cnt,0,sizeof(cnt));
	memset(bcho,127,sizeof(bcho));
    for (int i=1; i<=n; i++)
		for (int j=1; j<=m; j++)
			if (hasball(bd[i][j]) >= 0 && hashole(bd[i][j]) != hasball(bd[i][j])) {
				coor bc; bc.x = i; bc.y = j;
				moveBallToPos(bc);
				for (int hi=1; hi<=n; hi++)
					for (int hj=1; hj<=m; hj++)
						if (hashole(bd[hi][hj]) == hasball(bd[i][j])) {
							if (dp[0][hi][hj] > 0) {
								if (dp[0][hi][hj] < bcho[hi][hj]) {
									bcho[hi][hj] = dp[0][hi][hj];
									cho[hi][hj] = bc;
								}
								cnt[hi][hj]++;
							}
						}
			}

	coor bhole;
	int mcnt = 100000;
	for (int hi=1; hi<=n; hi++)
		for (int hj=1; hj<=m; hj++) {
			if (hashole(bd[hi][hj]) >= 0 && cnt[hi][hj] > 0 && cnt[hi][hj] < mcnt) {
				mcnt = cnt[hi][hj];
				bhole.x = hi; bhole.y = hj;
			}
		}

	vector<string> pushstep;

	if (mcnt == 100000) return pushstep;

	// from cho[bhole] to bhole
	coor st = cho[bhole.x][bhole.y];
	moveBallToPos(st);

	int ball = hasball(bd[st.x][st.y]);
	bd[st.x][st.y] -= (1<<ball);
	bd[bhole.x][bhole.y] += (1<<ball);

	char rstr[100];
	while (lastd[bhole.x][bhole.y] >= 0) {
		sprintf(rstr,"%d %d %d",lastc[bhole.x][bhole.y].x-1,lastc[bhole.x][bhole.y].y-1,lastd[bhole.x][bhole.y]);
		pushstep.push_back(rstr);
		bhole = lastc[bhole.x][bhole.y];
	}
	reverse(pushstep.begin(),pushstep.end());

	return pushstep;
}

class RollingBalls {
    public:
		vector <string> restorePattern(vector <string> start, vector <string> target) {
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

			vector<string> steps;
			do {
				steps = calculate();
				ret.insert(ret.end(),steps.begin(),steps.end());
			} while (!steps.empty() && ret.size() < maxstep);

			if (ret.size() > maxstep) {
				ret.resize(maxstep);
			}

            return ret;
        }
};
