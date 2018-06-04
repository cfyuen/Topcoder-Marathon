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

vector<string> ret;
int n,sal,fine,day = 0;
int bd[55][55],cntp = 0;
int dx[]={1,0,-1,0,1,1,-1,-1},dy[]={0,1,0,-1,1,-1,1,-1};
char dir[]={'D','R','U','L'};

int sumx,sumy,v[55][55];
int clus,csz[1000];
coor ccen[1000];

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

void flood(int x,int y) {
	v[x][y] = clus;
	sumx += x; sumy += y;
	csz[clus]++;
	for (int i=0; i<8; i++) {
		int nx = x+dx[i],ny = y+dy[i];
		if (valid(nx,ny) && v[nx][ny] == 0 && bd[nx][ny] == 1) flood(nx,ny);
	}
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
			memset(csz,0,sizeof(csz));
			clus = 1;
			for (int i=0; i<n; i++)
				for (int j=0; j<n; j++)
					if (v[i][j] == 0 && bd[i][j] == 1) {
						sumx = sumy = 0;
						flood(i,j);
						ccen[clus].x = sumx/csz[clus]; ccen[clus].y = sumy/csz[clus];

						// make sure center is on the snow
						int bx,by,best=10000;
						for (int tx=0; tx<n; tx++)
							for (int ty=0; ty<n; ty++)
								if (v[tx][ty] == clus) {
									if (abs(tx-ccen[clus].x)+abs(ty-ccen[clus].y) < best) {
										best = abs(tx-ccen[clus].x)+abs(ty-ccen[clus].y);
										bx = tx; by = ty;
									}
								}
						ccen[clus].x = bx; ccen[clus].y = by;

						fprintf(debug,"cluster %d: sz %d (%d,%d)\n",clus,csz[clus],ccen[clus].x,ccen[clus].y);
						clus++;
					}
		}

		void placeworker() {
			int totw[1000];
			memset(totw,0,sizeof(totw));

			// move worker to a nearest cluster
			for (int i=0; i<cntp; i++) {
				int fd = -1;

				if (bd[ppl[i].x][ppl[i].y] == 0) {
					int bx,by,best=10000;
					for (int tx=0; tx<n; tx++)
						for (int ty=0; ty<n; ty++)
							if (bd[tx][ty] == 1) {
								if (abs(tx-ppl[i].x)+abs(ty-ppl[i].y) < best) {
									best = abs(tx-ppl[i].x)+abs(ty-ppl[i].y);
									bx = tx; by = ty;
								}
							}
					//fprintf(debug,"%d: (%d,%d) -> (%d,%d)\n",i,ppl[i].x,ppl[i].y,bx,by);

					// todo: need to determine direction
					for (int d=0; d<4; d++)
						if ((bx-ppl[i].x)*dx[d]+(by-ppl[i].y)*dy[d] > 0) {
							fd = d;
							ppl[i].x += dx[d]; ppl[i].y += dy[d];
							break;
						}
				}

				for (int j=0; j<i; j++)
					if (ppl[j].x == ppl[i].x && ppl[j].y == ppl[i].y) {
						if (fd != -1) { // undo move
							ppl[i].x -= dx[fd]; ppl[i].y -= dy[fd];
						}
						while (true) {
							int rd = rand()%4;
							int nx = ppl[i].x+dx[rd],ny = ppl[i].y+dy[rd];
							if (valid(nx,ny) && !(ppl[j].x == nx && ppl[j].y == ny)) {
								fd = rd;
								ppl[i].x += dx[rd]; ppl[i].y += dy[rd];
								break;
							}
						}
						break;
					}

				if (fd != -1) ret.push_back("M "+tostr(i)+" "+dir[fd]);

				if (bd[ppl[i].x][ppl[i].y] == 1) {
					totw[v[ppl[i].x][ppl[i].y]]++;
				}
			}
			// add new people
			// if size of cluster > thereshold, place a worker
			for (int i=1; i<clus; i++) {
				// todo: better criteria
				if (cntp < 40 && csz[i] > 3 && totw[i] == 0) {
					ppl[cntp++] = ccen[i];
					ret.push_back("H "+tostr(ppl[cntp-1].x)+" "+tostr(ppl[cntp-1].y));
					totw[i]++;
				}
			}
		}

		vector <string> nextDay(vector <int> snowFalls) {
			fprintf(debug,"Day: %d\n",day);
			ret.clear();
			for (int i=0; i<snowFalls.size(); i+=2) {
				int sx = snowFalls[i],sy = snowFalls[i+1];
				bd[sx][sy] = 1;
			}
			findcluster();
			placeworker();
			for (int i=0; i<cntp; i++)
				bd[ppl[i].x][ppl[i].y] = 0;

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

