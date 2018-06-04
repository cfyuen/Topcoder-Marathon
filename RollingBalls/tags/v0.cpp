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
using namespace std;


int dx[]={0,1,0,-1},dy[]={-1,0,1,0};


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

class RollingBalls {
    public:
		vector <string> restorePattern(vector <string> start, vector <string> target) {
			vector<string> ret;
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
