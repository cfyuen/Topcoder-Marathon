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
#define LL long long

struct coor {
	int x,y;
} ppl[105];

struct Cluster {
	int sz;
	coor cen;
	coor mi,mx;
	coor corn[4];
	coor all[500];
} c[1000];

struct order {
	coor at;
	int val;
} tar[1005];

vector<string> ret;
int n,sal,fine,day = 0;
int bd[55][55],cntp = 0;
int dx[]={1,0,-1,0,1,1,-1,-1},dy[]={0,1,0,-1,1,-1,1,-1};
char dir[]={'D','R','U','L'};
int inf = (1<<30);
LL projfine,projsal,totsal,totfine;
int opt;

int sumx,sumy,v[55][55];
int cntc;

//delete when submit
FILE *debug,*result;

int sttime;
int tcnt=0;
void startt() {
	sttime = clock();
}

double runtime() {
	return (clock()-sttime)/1000.0;
}

//til here

/* have it when submit
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
*/

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

int mahdis(coor a,coor b) {
	return abs(a.x-b.x)+abs(a.y-b.y);
}

void flood(int x,int y) {
	v[x][y] = cntc;
	sumx += x; sumy += y;
	c[cntc].all[c[cntc].sz].x = x; c[cntc].all[c[cntc].sz].y = y;
	c[cntc].sz++;
	c[cntc].mi.x = min(c[cntc].mi.x,x); c[cntc].mx.x = max(c[cntc].mx.x,x);
	c[cntc].mi.y = min(c[cntc].mi.y,y); c[cntc].mx.y = max(c[cntc].mx.y,y);
	for (int i=0; i<8; i++) {
		int nx = x+dx[i],ny = y+dy[i];
		if (valid(nx,ny) && v[nx][ny] == 0 && bd[nx][ny] == 1) flood(nx,ny);
	}
}


// Hungarian algorithm
int mx[1005],my[1005],lx[1005],ly[1005],adj[1005][1005];
int p[1005],dpy[1005],pdpy[1005],vx[1005],vy[1005];
int q[1005],q1,q2;
int node;

void fresh(int x) {
	for (int y=0; y<node; y++)
		if (adj[x][y] < (1<<29) && lx[x]+ly[y]-adj[x][y] < dpy[y]) {
			dpy[y] = lx[x]+ly[y]-adj[x][y];
			pdpy[y] = x;
		}
}

void reweight() {
	int d = (1<<30);
	for (int y=0; y<node; y++)
		if (!vy[y]) d = min(d,dpy[y]);
	for (int x=0; x<node; x++)
		if (vx[x]) lx[x] -= d;
	for (int y=0; y<node; y++)
		if (vy[y]) ly[y] += d;
	for (int y=0; y<node; y++)
		if (!vy[y]) dpy[y] -= d;
}

void augment(int x,int y) {
	for (int ty; x != -1; x=p[x], y=ty) {
		ty = mx[x]; my[y] = x; mx[x] = y;
	}
}

bool branch1() {
	while (q1 < q2) {
		for (int x=q[q1++], y=0; y<node; y++) {
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
	for (int y=0; y<node; y++) {
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
	for (int x=0; x<node; x++)
		for (int y=0; y<node; y++) {
			lx[x] = max(lx[x],adj[x][y]);
		}
	for (int x=0; x<node; x++)
		if (!match(x)) mx[x] = -1;
	int cost = 0;
	for (int x=0; x<node; x++)
		if (mx[x] != -1)
			cost += adj[x][mx[x]];
	return cost;
}
/* Hungarian end */

bool cmp(order a,order b) {
	return a.val < b.val;
}

class SnowCleaning {
	public:
		int init(int boardSize, int salary, int snowFine) {
			n = boardSize;
			sal = salary;
			fine = snowFine;
			memset(bd,0,sizeof(bd));
			projfine = 0;
			projsal = 0;
			totsal = 0;
			totfine = 0;
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

						//fprintf(debug,"cluster %d: sz %d (%d,%d)\n",cntc,c[cntc].sz,c[cntc].cen.x,c[cntc].cen.y);
						cntc++;
					}
		}

		int cntt;

		void gettarget2() {
			int addtar[1005],mc[1005],mp[105];
			int cat = 0;

			for (int i=1; i<cntc; i++) {
				fprintf(debug,"Cluster %d: (%d) cntt = %d \n",i,c[i].sz,cntt);
				cat = 0;
				// internal priority
				int mindis;
				if (cntp == 0) {
					addtar[cat++] = 0;
				}
				else {
					memset(mc,0,sizeof(mc));
					// no. 1
					mindis = inf;
					for (int j=0; j<c[i].sz; j++)
						for (int k=0; k<cntp; k++)
							if (mahdis(c[i].all[j],ppl[k]) < mindis) {
								mindis = mahdis(c[i].all[j],ppl[k]);
								addtar[cat] = j;
							}
					mc[addtar[cat++]] = 1;
				}
				// subsequent, furthest from any other, if tie, nearest to ppl
				for (int tt=1; tt<c[i].sz; tt++) {
					int maxdis = -1;
					mindis = inf;
					for (int j=0; j<c[i].sz; j++) {
						if (mc[j]) continue;
						int cur = inf;
						for (int k=0; k<cat; k++)
							cur = min(cur,mahdis(c[i].all[j],c[i].all[addtar[k]]));
						if (cur >= maxdis) {
							int curp = inf;
							for (int k=0; k<cntp; k++)
								curp = min(curp,mahdis(c[i].all[j],ppl[k]));
							if (cur > maxdis || curp < mindis) {
								maxdis = cur;
								mindis = curp;
								addtar[cat] = j;
							}
						}
					}
					mc[addtar[cat++]] = 1;
				}

				if (cntp == 0) {
					for (int j=0; j<cat; j++) {
						tar[cntt].at = c[i].all[addtar[j]];
						tar[cntt++].val = j;
					}
					continue;
				}
				// determine how many should be used
				memset(mp,0,sizeof(mp));
				memset(mc,0,sizeof(mc));
				int last = inf,left = c[i].sz;
				for (int j=0; j<cat && left>0; j++) {
					int bp = 0,cur = inf;
					for (int k=0; k<cntp; k++) {
						if (mp[k]) continue;
						if (mahdis(c[i].all[addtar[j]],ppl[k]) < cur) {
							cur = mahdis(c[i].all[addtar[j]],ppl[k]);
							bp = k;
						}
					}
					if (cur > last) {
						left -= (cur-last)*j;
					}
					mp[bp] = 1;
					tar[cntt].at = c[i].all[addtar[j]];
					tar[cntt++].val = j;
					last = cur;
				}

				fprintf(debug,"cat %d\n",cat);

			}
		}

		void placeworker2() {
			// decide which spot is most worthy for destination
			int totsnow = 0;
			for (int i=1; i<cntc; i++) {
				totsnow += c[i].sz;
			}
			cntt = 0;
			fprintf(debug,"opt %d\n",opt);

			gettarget2();
			//sort(tar,tar+cntt,cmp);

			// assign workers to those spots
			int dis[105][1005];
			for (int i=0; i<cntp; i++) {
				for (int j=0; j<cntt; j++)
					dis[i][j] = abs(tar[j].at.x-ppl[i].x)+abs(tar[j].at.y-ppl[i].y);
			}

			int used[1005];
			coor to[105];
			memset(used,0,sizeof(used));
			fprintf(debug,"People %d   Target %d\n",cntp,cntt);
			//for (int i=0; i<cntt; i++)
			//	fprintf(debug,"T (%d,%d)\n",tar[i].x,tar[i].y);

			if (cntt <= cntp) {
				// matching
				for (int i=0; i<cntp; i++)
					for (int j=0; j<cntp; j++)
						if (j >= cntt) adj[i][j] = 0;
						else adj[i][j] = -dis[i][j];

				node = cntp;
				Hungarian();

				for (int i=0; i<cntt; i++) {
					used[my[i]] = 1;
					to[my[i]] = tar[i].at;
				}
				// find other pos to standby
				int fa[55][55];
				for (int cx=0; cx<n; cx++)
					for (int cy=0; cy<n; cy++) {
						int cur = 10000;
						for (int j=0; j<cntt; j++)
							cur = min(cur,abs(tar[j].at.x-cx)+abs(tar[j].at.y-cy));

						cur = min(cur,abs(-1-cx)+abs(-1-cy));
						cur = min(cur,abs(-1-cx)+abs(n-cy));
						cur = min(cur,abs(n-cx)+abs(-1-cy));
						cur = min(cur,abs(n-cx)+abs(n-cy));
						cur = min(cur,abs(-1-cx)+abs(n/2-cy));
						cur = min(cur,abs(n/2-cx)+abs(-1-cy));
						cur = min(cur,abs(n/2-cx)+abs(n-cy));
						cur = min(cur,abs(n-cx)+abs(n/2-cy));

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
					tar[cntt++].at = best;
					for (int cx=0; cx<n; cx++)
						for (int cy=0; cy<n; cy++)
							fa[cx][cy] = min(fa[cx][cy],abs(cx-best.x)+abs(cy-best.y));
					//fprintf(debug,"new pos (%d,%d) %d\n",best.x,best.y,far);
				}
				// put workers into new location
				for (int i=0; i<cntp; i++) {
					for (int j=0; j<cntt; j++)
						dis[i][j] = abs(tar[j].at.x-ppl[i].x)+abs(tar[j].at.y-ppl[i].y);
				}
				for (int i=0; i<cntp; i++) {
					int best = (1<<20),cur = 0;
					for (int j=0; j<cntp; j++)
						if (used[j] == 0 && dis[j][i] < best) {
							best = dis[j][i]; cur = j;
						}
					used[cur] = 1;
					to[cur] = tar[i].at;
				}
			}
			else {
				// matching
				for (int i=0; i<cntt; i++)
					for (int j=0; j<cntt; j++)
						if (i >= cntp) adj[i][j] = 0;
						else adj[i][j] = -dis[i][j];

				node = cntt;
				Hungarian();

				for (int i=0; i<cntp; i++) {
					used[mx[i]] = 1;
					to[i] = tar[mx[i]].at;
				}
				// add worker
				for (int i=cntp; i<opt; i++) {
					int maxdis = -1;
					coor bt; bt.x = -1;
					for (int cx=0; cx<n; cx++)
						for (int cy=0; cy<n; cy++)
							if (bd[cx][cy] == 1) {
								if (bt.x == -1) { bt.x = cx; bt.y = cy; }
								int cur = inf;
								for (int j=0; j<cntp; j++) {
									cur = min(cur,abs(cx-to[j].x)+abs(cy-to[j].y));
								}
								if (cur > maxdis) {
									maxdis = cur;
									bt.x = cx; bt.y = cy;
								}
							}
					if (maxdis == 0) break;
					ppl[cntp++] = bt;
					ret.push_back("H "+tostr(ppl[cntp-1].x)+" "+tostr(ppl[cntp-1].y));
					fprintf(debug,"worker added at (%d,%d) %d\n",bt.x,bt.y,maxdis);
					to[cntp-1] = ppl[cntp-1];
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
			fprintf(debug,"Day: %d %d\n",day,snowFalls.size()/2);
			projfine = (totfine+snowFalls.size()/2*fine)/(day+1)*2000;
			projsal = totsal+cntp*sal*(2000-day);
			if (projfine > 0) {
				double bestp = 0.1;
				for (opt=cntp+1; opt<=80; opt++) {
					LL psal = totsal+opt*sal*(2000-day);
					double val = projfine*(1-0.2*(2000-day)/2000) / psal;
					fprintf(debug," - %d: %lld,%lld %f\n",opt,projfine,psal,val);
					if (val < 1+(0.5*(2000-day)/2000)) break;
				}
				opt--;
				//if (bestp > 0.09) opt = cntp;

				//if (day < 300) opt = min(opt,(n*2+fine)*(day+10)/15/sal);
				//fprintf(debug," --- %d\n",(n*2+fine)*(day+10)/15/sal);
				opt = max(opt,cntp);
			}
			//fprintf(debug,"proj %lld %lld\n",projfine,projsal);
			ret.clear();
			for (int i=0; i<snowFalls.size(); i+=2) {
				int sx = snowFalls[i],sy = snowFalls[i+1];
				bd[sx][sy] = 1;
			}
			findcluster();
			placeworker2();
			for (int i=0; i<cntp; i++)
				bd[ppl[i].x][ppl[i].y] = 0;
			for (int i=0; i<n; i++)
				for (int j=0; j<n; j++)
					if (bd[i][j] == 1) totfine += fine;
			totsal += cntp*sal;

			day++;
			fflush(debug);
			return ret;
		}
};

int main () {
	debug = fopen("debug.txt","w");
	SnowCleaning SC;
	int SZ,SA,SF;
	scanf("%d%d%d",&SZ,&SA,&SF);
	SC.init(SZ,SA,SF);

	vector<int> Fall;
	vector<string> RET;
	int CNT;
	for (int i=0; i<2000; i++) {
		scanf("%d",&CNT);
		Fall.resize(2*CNT);
		for (int j=0; j<2*CNT; j++) {
			scanf("%d",&Fall[j]);
		}
		RET = SC.nextDay(Fall);
		printf("%d\n",RET.size());
		for (int j=0; j<RET.size(); j++)
			printf("%s\n",RET[j].c_str());
		fflush(stdout);
	}

	fflush(stdout);
}

