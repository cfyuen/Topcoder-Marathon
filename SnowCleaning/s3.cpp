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

struct coor {
	int x,y;
} ppl[105];

struct Cluster {
	int sz;
	coor cen;
	coor mi,mx;
	coor corn[4];
} c[1000];

vector<string> ret;
int n,sal,fine,day = 0;
int bd[55][55],cntp = 0;
int dx[]={1,0,-1,0,1,1,-1,-1},dy[]={0,1,0,-1,1,-1,1,-1};
char dir[]={'D','R','U','L'};

int sumx,sumy,v[55][55];
int cntc;

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


string tostr(int cur) {
	char str[100];
	sprintf(str,"%d",cur);
	return str;
}

bool valid(int x,int y) {
	if (x >= 0 && x < n && y >= 0 && y < n) return true;
	return false;
}
bool valid(coor c) {
	if (c.x >= 0 && c.x < n && c.y >= 0 && c.y < n) return true;
	return false;
}

void flood(int x,int y) {
	v[x][y] = cntc;
	sumx += x; sumy += y;
	c[cntc].sz++;
	c[cntc].mi.x = min(c[cntc].mi.x,x); c[cntc].mx.x = max(c[cntc].mx.x,x);
	c[cntc].mi.y = min(c[cntc].mi.y,y); c[cntc].mx.y = max(c[cntc].mx.y,y);
	for (int i=0; i<8; i++) {
		int nx = x+dx[i],ny = y+dy[i];
		if (valid(nx,ny) && v[nx][ny] == 0 && bd[nx][ny] == 1) flood(nx,ny);
	}
}


// Hungarian algorithm
int mx[105],my[105],lx[105],ly[105],adj[105][105];
int p[105],dpy[105],pdpy[105],vx[105],vy[105];
int q[105],q1,q2;

void fresh(int x) {
	for (int y=0; y<cntp; y++)
		if (adj[x][y] < (1<<29) && lx[x]+ly[y]-adj[x][y] < dpy[y]) {
			dpy[y] = lx[x]+ly[y]-adj[x][y];
			pdpy[y] = x;
		}
}

void reweight() {
	int d = (1<<30);
	for (int y=0; y<cntp; y++)
		if (!vy[y]) d = min(d,dpy[y]);
	for (int x=0; x<cntp; x++)
		if (vx[x]) lx[x] -= d;
	for (int y=0; y<cntp; y++)
		if (vy[y]) ly[y] += d;
	for (int y=0; y<cntp; y++)
		if (!vy[y]) dpy[y] -= d;
}

void augment(int x,int y) {
	for (int ty; x != -1; x=p[x], y=ty) {
		ty = mx[x]; my[y] = x; mx[x] = y;
	}
}

bool branch1() {
	while (q1 < q2) {
		for (int x=q[q1++], y=0; y<cntp; y++) {
			if (!vy[y] && lx[x]+ly[y] == adj[x][y]) {
				if (my[y] == -1) {
					augment(x,y);
					return true;
				}
				int z = my[y];
				vx[z] = vy[y] = 1;
				p[z] = x;
				q[q2++] = z;
				fresh(z);
			}
		}
	}
	return false;
}

bool branch2() {
	for (int y=0; y<cntp; y++) {
        if (!vy[y] && dpy[y] == 0) {
            if (my[y] == -1) {
                augment(pdpy[y],y);
                return true;
            }
            int z = my[y];
            vx[z] = vy[y] = 1;
            p[z] = pdpy[y];
            q[q2++] = z;
            fresh(z);
        }
    }
    return false;
}


bool match(int r) {
	memset(vx,0,sizeof(vx));
	memset(vy,0,sizeof(vy));
	memset(dpy,127,sizeof(dpy));
	memset(p,-1,sizeof(p));
	q1 = q2 = 0;
	q[q2++] = r;

	vx[r] = 1;
	fresh(r);

	while (q1 < q2) {
		if (branch1()) return true;
		reweight();
		if (branch2()) return true;
	}
	return false;
}

int Hungarian() {
	memset(mx,-1,sizeof(mx));
	memset(my,-1,sizeof(my));
	memset(lx,0,sizeof(lx));
	memset(ly,0,sizeof(ly));
	for (int x=0; x<cntp; x++)
		for (int y=0; y<cntp; y++) {
			lx[x] = max(lx[x],adj[x][y]);
		}
	for (int x=0; x<cntp; x++)
		if (!match(x)) mx[x] = -1;
	int cost = 0;
	for (int x=0; x<cntp; x++)
		if (mx[x] != -1)
			cost += adj[x][mx[x]];
	return cost;
}

class SnowCleaning {
	public:
		int init(int boardSize, int salary, int snowFine) {
			n = boardSize;
			sal = salary;
			fine = snowFine;
			memset(bd,0,sizeof(bd));
			return 0;
		}

		void findcluster() {
			// find size and center of cluster
			memset(v,0,sizeof(v));
			cntc = 1;
			for (int i=0; i<n; i++)
				for (int j=0; j<n; j++)
					if (v[i][j] == 0 && bd[i][j] == 1) {
						c[cntc].sz = 0;
						c[cntc].mi.x = 1000; c[cntc].mi.y = 1000;
						c[cntc].mx.x = 0; c[cntc].mx.y = 0;
						sumx = sumy = 0;
						flood(i,j);
						c[cntc].cen.x = sumx/c[cntc].sz; c[cntc].cen.y = sumy/c[cntc].sz;

						// make sure center is on the snow
						int bx,by,best=10000,bxc[4],byc[4],bes[4];
						memset(bes,120,sizeof(bes));
						coor tcc[4];
						tcc[0] = c[cntc].mi; tcc[1] = c[cntc].mx;
						tcc[2].x = c[cntc].mi.x; tcc[2].y = c[cntc].mx.y;
						tcc[3].x = c[cntc].mx.x; tcc[3].y = c[cntc].mi.y;
						for (int tx=0; tx<n; tx++)
							for (int ty=0; ty<n; ty++)
								if (v[tx][ty] == cntc) {
									if (abs(tx-c[cntc].cen.x)+abs(ty-c[cntc].cen.y) < best) {
										best = abs(tx-c[cntc].cen.x)+abs(ty-c[cntc].cen.y);
										bx = tx; by = ty;
									}
									for (int k=0; k<4; k++) {
										if (abs(tx-tcc[k].x)+abs(ty-tcc[k].y) < bes[k]) {
											bes[k] = abs(tx-tcc[k].x)+abs(ty-tcc[k].y);
											bxc[k] = tx; byc[k] = ty;
										}
									}
								}
						c[cntc].cen.x = bx; c[cntc].cen.y = by;
						for (int k=0; k<4; k++) {
							c[cntc].corn[k].x = bxc[k]; c[cntc].corn[k].y = byc[k];
						}

						cntc++;
					}
		}

		void placeworker2() {
			// decide which spot is most worthy for destination
			int totsnow = 0;
			for (int i=1; i<cntc; i++) {
				totsnow += c[i].sz;
			}
			coor tar[505];
			int cntt = 0;
			for (int i=1; i<cntc; i++) {
				if (c[i].sz <= 5) { // 1 worker
					tar[cntt++] = c[i].cen;
				}
				else if (c[i].sz <= 20) { // 2 worker
					for (int j=0; j<2; j++)
						tar[cntt++] = c[i].corn[j];
				}
				else if (c[i].sz <= 40) { // 4 workers
					for (int j=0; j<4; j++)
						tar[cntt++] = c[i].corn[j];
				}
				else { // 5 workers
					tar[cntt++] = c[i].cen;
					for (int j=0; j<4; j++)
						tar[cntt++] = c[i].corn[j];
				}
			}

			// assign workers to those spots
			int dis[105][505];
			for (int i=0; i<cntp; i++) {
				for (int j=0; j<cntt; j++)
					dis[i][j] = abs(tar[j].x-ppl[i].x)+abs(tar[j].y-ppl[i].y);
			}

			int used[505];
			coor to[105];
			memset(used,0,sizeof(used));

			if (cntt <= cntp) {
				// matching
				for (int i=0; i<cntp; i++)
					for (int j=0; j<cntp; j++)
						if (j >= cntt) adj[i][j] = 0;
						else adj[i][j] = -dis[i][j];

				Hungarian();

				for (int i=0; i<cntt; i++) {
					used[my[i]] = 1;
					to[my[i]] = tar[i];
				}
				// find other pos to standby
				int fa[55][55];
				for (int cx=0; cx<n; cx++)
					for (int cy=0; cy<n; cy++) {
						int cur = 10000;
						for (int j=0; j<cntt; j++)
							cur = min(cur,abs(tar[j].x-cx)+abs(tar[j].y-cy));
						for (int j=-1; j<=n; j++) {
							cur = min(cur,abs(j-cx)+abs(-1-cy));
							cur = min(cur,abs(-1-cx)+abs(j-cy));
							cur = min(cur,abs(j-cx)+abs(n-cy));
							cur = min(cur,abs(n-cx)+abs(j-cy));
						}
						fa[cx][cy] = cur;
					}
				for (int i=cntt; i<cntp; i++) {
					int far = 0;
					coor best;
					// todo: speed improvement needed
					for (int cx=0; cx<n; cx++)
						for (int cy=0; cy<n; cy++)
							if (fa[cx][cy] > far) {
								far = fa[cx][cy]; best.x = cx; best.y = cy;
							}
					tar[cntt++] = best;
					for (int cx=0; cx<n; cx++)
						for (int cy=0; cy<n; cy++)
							fa[cx][cy] = min(fa[cx][cy],abs(cx-best.x)+abs(cy-best.y));
				}
				// put workers into new location
				for (int i=0; i<cntp; i++) {
					for (int j=0; j<cntt; j++)
						dis[i][j] = abs(tar[j].x-ppl[i].x)+abs(tar[j].y-ppl[i].y);
				}
				for (int i=0; i<cntp; i++) {
					int best = (1<<20),cur = 0;
					for (int j=0; j<cntp; j++)
						if (used[j] == 0 && dis[j][i] < best) {
							best = dis[j][i]; cur = j;
						}
					used[cur] = 1;
					to[cur] = tar[i];
				}
			}
			else {
				// matching
				for (int i=0; i<cntp; i++)
					for (int j=0; j<cntp; j++)
						if (j >= cntt) adj[i][j] = 0;
						else adj[i][j] = -dis[i][j];

				Hungarian();

				for (int i=0; i<cntp; i++) {
					used[mx[i]] = 1;
					to[i] = tar[mx[i]];
				}
				// decide whether new workers are needed
				for (int i=0; i<cntt; i++)
					if (used[i] == 0) {
						if (cntp < 40) {
							ppl[cntp++] = tar[i];
							ret.push_back("H "+tostr(ppl[cntp-1].x)+" "+tostr(ppl[cntp-1].y));
							to[cntp-1] = ppl[cntp-1];
						}
					}
			}

			// move to to[]
			for (int i=0; i<cntp; i++) {
				for (int d=0; d<4; d++)
					if ((to[i].x-ppl[i].x)*dx[d]+(to[i].y-ppl[i].y)*dy[d] > 0) {
						ppl[i].x += dx[d]; ppl[i].y += dy[d];
						ret.push_back("M "+tostr(i)+" "+dir[d]);
						break;
					}
			}
		}

		vector <string> nextDay(vector <int> snowFalls) {
			ret.clear();
			for (int i=0; i<snowFalls.size(); i+=2) {
				int sx = snowFalls[i],sy = snowFalls[i+1];
				bd[sx][sy] = 1;
			}
			findcluster();
			placeworker2();
			for (int i=0; i<cntp; i++)
				bd[ppl[i].x][ppl[i].y] = 0;

			day++;
			return ret;
		}
};
