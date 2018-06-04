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

int N,M;

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

class ImageToScan {
	public:
		string scan (int row) {
			printf("?\n");
			printf("%d\n",row);
			fflush(stdout);
			string rs;
			cin >> rs;
			return rs;
		}
};

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

class ImageScanner {
	public:
		vector <string> restore(int H, int W, int nb, int nLetter) {
			N=H; M=W;
			ImageToScan Lib;
			
			
			vector<string> ret;
			string ts="";
			for (int i=0; i<M; i++) ts+='0';
			for (int i=0; i<N; i++) {
				ret.push_back(ts);
			}
			for (int i=0; i<N; i+=2) {
				ret[i]=Lib.scan(i);
			}
			for (int i=1; i<N; i+=2) {
				for (int j=0; j<M; j++)
					if (ret[i-1][j]=='1' && i+1<N && ret[i+1][j]=='1') ret[i][j]='1';
			}
			return ret;
		}
};

int main () {
	debug=fopen("debug.txt","w");
	ImageScanner IS;
	int H,W,NB,NL;
	scanf("%d%d%d%d",&H,&W,&NB,&NL);
	vector<string> RET;
	RET=IS.restore(H,W,NB,NL);
	printf("!\n");
	for (int i=0; i<RET.size(); i++)
		printf("%s\n",RET[i].c_str());
	fflush(stdout);
}

