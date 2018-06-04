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
	int x,y;
};

int dx[]={0,1,0,-1},dy[]={-1,0,1,0};
int bd[65][65];
double hsco[65][65];
int n,m,cntball;
int lg2[1024];
vector<string> ret;

//delete when submit
FILE *debug,*result;

int sttime;
void startt() {
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

/******** Main ***********/
void calcHoleScore() {
// v1: use adj num of walls
// v2: use num of pos to reach that hole in 2 steps in empty board
	memset(hsco,0,sizeof(hsco));
	for (int i=1; i<=n; i++)
		for (int j=1; j<=n; j++) {
			if (hashole(bd[i][j]) >= 0) {
				int wall = 0;
				for (int d=0; d<4; d++) {
					int x=i+dx[d],y=j+dy[d];
					if (iswall(bd[x][y])) wall++;
				}
				switch (wall) {
					case 0: hsco[i][j] = 100.0; break;
					case 1: hsco[i][j] = 40.0; break;
					case 2: hsco[i][j] = 18.0; break;
					case 3: hsco[i][j] = 12.0; break;
					case 4: hsco[i][j] = 100.0; break;
				}
			}
		}
}

// v1: move 1 ball, find best average score
double move1ball(int ix,int iy,int dir) {
	double initsco = hsco[ix][iy];
	if (initsco > 0 && hashole(bd[ix][iy]) != hasball(bd[ix][iy])) initsco /= 2;
	int ball = hasball(bd[ix][iy]);
	double bsco = -1e10;
	int v[65][65],step;
	memset(v,-1,sizeof(v));
	bd[ix][iy] -= (1<<ball);
	queue<pair<coor,int> > q;
	coor c,nt;
	c.x = ix; c.y = iy;
	q.push(make_pair(c,0));
	v[c.x][c.y] = 0;
	//fprintf(debug," init %d,%d - %d\n",c.x,c.y,dir);
	while (!q.empty()) {
		c = q.front().first;
		step = q.front().second;
		q.pop();
		if (step > 5) continue;
		if (step > 0) {
			double holesco = hsco[c.x][c.y];
			if (holesco > 0 && hashole(bd[c.x][c.y]) != hasball(bd[ix][iy])) holesco /= 2;
			double cursco = (holesco - initsco) - step*0.1;
			//fprintf(debug,"   step %d: %d,%d = %f\n",step,c.x,c.y,cursco);
			if (cursco > bsco) {
				bsco = cursco;
			}
			for (int d=0; d<4; d++) {
				nt.x = c.x+dx[d]; nt.y = c.y+dy[d];
				while (!iswall(bd[nt.x][nt.y]) && hasball(bd[nt.x][nt.y]) < 0) {
					nt.x += dx[d]; nt.y += dy[d];
				}
				nt.x -= dx[d]; nt.y -= dy[d];
				if (v[nt.x][nt.y] == -1) {
					q.push(make_pair(nt,step+1));
					v[nt.x][nt.y] = step+1;
				}
			}
		}
		else {
			nt.x = c.x+dx[dir]; nt.y = c.y+dy[dir];
			while (!iswall(bd[nt.x][nt.y]) && hasball(bd[nt.x][nt.y]) < 0) {
				nt.x += dx[dir]; nt.y += dy[dir];
			}
			nt.x -= dx[dir]; nt.y -= dy[dir];
			if (v[nt.x][nt.y] == -1) {
				q.push(make_pair(nt,step+1));
				v[nt.x][nt.y] = step+1;
			}
		}
	}
	bd[ix][iy] += (1<<ball);
	//fprintf(debug," ret = %f\n",bsco);
	return bsco;
}

void getNextMove() {
	double sco,bsco = -1.0;
	int bx,by,bdir;
    for (int i=1; i<=n; i++)
		for (int j=1; j<=m; j++)
			if (hasball(bd[i][j]) >= 0) {
				for (int d=0; d<4; d++) {
					sco = move1ball(i,j,d);
					if (sco > bsco) {
						bsco = sco; bx = i; by = j; bdir = d;
						fprintf(debug,"bestsco = %f (%d,%d,%d)\n",bsco,bx,by,bdir);
					}
				}
			}
	if (bsco > 0) {
		coor c,nt;
		c.x = bx; c.y = by;
		nt.x = c.x+dx[bdir]; nt.y = c.y+dy[bdir];
		while (!iswall(bd[nt.x][nt.y]) && hasball(bd[nt.x][nt.y]) < 0) {
			nt.x += dx[bdir]; nt.y += dy[bdir];
		}
		nt.x -= dx[bdir]; nt.y -= dy[bdir];
		int ball = hasball(bd[c.x][c.y]);
		bd[c.x][c.y] -= (1<<ball);
		bd[nt.x][nt.y] += (1<<ball);
		fprintf(debug,"move = %d,%d,%d -- %f\n",bx,by,bdir,bsco);

		char rstr[100];
		sprintf(rstr,"%d %d %d",bx-1,by-1,bdir);
		ret.push_back(rstr);
	}
	display();
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
			display();

			calcHoleScore();

			int maxstep = cntball*20;
			while (maxstep--) {
				fprintf(debug,"At step %d\n",cntball*20-maxstep);
				getNextMove();
				if (ret.size() < cntball*20-maxstep) break;
			}

//			for (int i=0; i<ret.size(); i++)
//				fprintf(debug,"%s\n",ret[i].c_str());

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
