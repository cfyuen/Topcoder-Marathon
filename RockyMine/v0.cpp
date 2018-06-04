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

struct Bomb {
	int left;
	int eff[6][6];
} b[10];

int n,m,dn;

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

class RockyMine {
	public:
		string collectGold(vector <int> dynamite, vector <int> effect, int W, vector <int> gold, vector <int> rocks, int maxMoves) {
			int dn=dynamite.size();
			for (int i=0; i<dn; i++) {
				b[i].left=dynamite[i];
				for (int j=0; j<5; j++)
					for (int k=0; k<5; k++)
						b[i].eff[j][k]=effect[i*25+j*5+k];
			}
			string ret="EEWW";
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

