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

class SnowCleaning {
	public:
		int init(int boardSize, int salary, int snowFine) {
			return 0;
		}
		vector <string> nextDay(vector <int> snowFalls) {
			vector<string> ret;
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

