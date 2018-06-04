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

int n,sal,fine,cnt,day = 0;
FILE *debug;

class SnowCleaning {
	public:
		int init(int boardSize, int salary, int snowFine) {
			n = boardSize;
			sal = salary;
			fine = snowFine;
			day = 0;
			cnt = 0;
			return 0;
		}

		vector <string> nextDay(vector <int> snowFalls) {
			vector <string> ret;
			cnt += snowFalls.size()/2;
			day++;
			if (day == 2000) {
				fprintf(debug,"%d,%d,%d,%d,\n",n,sal,fine,cnt);
				fflush(debug);
			}
			return ret;
		}
};


int main () {
	debug = fopen("param.csv","a");
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
