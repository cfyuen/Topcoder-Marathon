#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cstring>
using namespace std;
#define MAXN 205

double PI = acos(-1.0);

int N;
int sco[MAXN],bsco,factor = 10000;
int e[MAXN][MAXN];
int bp[MAXN];

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

class PointsOnTheCircle {
public:
	int scoring(const int *per) {
		int ret = 0;
		for (int i=0; i<N; i++) {
			for (int j=i+1; j<N; j++) {
				if (e[per[i]][per[j]]) ret += sco[abs(i-j)];
			}
		}
		return ret;
	}

	int scoring2(const int *per, const int osco, int idx, int idy) {
		int ret = 0;
		for (int i=0; i<N; i++) {
			if (e[per[idx]][per[i]] && i != idy) {
				//fprintf(stderr," x (%d -> %d : %d) - %.4f + %.4f\n",idx,idy,i,sco[abs(idx-i)],sco[abs(idy-i)]);
				ret -= sco[abs(idx-i)];
				ret += sco[abs(idy-i)];
			}
			if (e[per[idy]][per[i]] && i != idx) {
				//fprintf(stderr," y (%d -> %d : %d) - %.4f + %.4f\n",idy,idx,i,sco[abs(idy-i)],sco[abs(idx-i)]);
				ret -= sco[abs(idy-i)];
				ret += sco[abs(idx-i)];
			}
		}
		return osco + ret;
	}

	void SA() {
		int a[N];
        for (int i=0; i<N; i++) {
			a[i] = i;
        }

		random_shuffle(a,a+N);
		int nowsco = scoring(a);
		int prevsco = nowsco;
		int iter,totaliter = 50000;
		iter = totaliter;
		double maxt = factor*10.0,mint = factor*0.001;
		double temp = maxt;
		while (iter--) {
			int idx = rand()%N,idy = rand()%N;
			nowsco = scoring2(a, prevsco, idx, idy);
			swap(a[idx],a[idy]);

			int delta = -(nowsco-prevsco);
			//fprintf(stderr," %d: %.4f - (%d vs %d = %d) - P = %.4f\n",iter,temp,nowsco,prevsco,delta,exp(delta/temp));
			temp = maxt * pow(mint/maxt, (totaliter-iter)*1.0/totaliter);
			if (delta > 0 || exp(delta/temp) > rand()*1.0/RAND_MAX) {
				prevsco = nowsco;
				//fprintf(stderr,"Update!\n");
				if (nowsco < bsco) {
					//fprintf(stderr," New best %d: %d [%d]\n",iter,nowsco,bsco); fflush(stderr);
					bsco = nowsco;
					memcpy(bp,a,sizeof(a));
				}
			}
			else {
				swap(a[idx],a[idy]);
			}
		}

	}

    vector<int> permute(vector<int> matrix) {
        N = (int)sqrt(matrix.size());
        for (int i=1; i<N; i++) {
			sco[i] = factor*(int)2*sin(PI / N * i);
			fprintf(stderr," %d: %d\n",i,sco[i]);
        }
        for (int i=0; i<N; i++) {
			for (int j=0; j<N; j++) {
				e[i][j] = matrix[i*N+j];
			}
        }
        bsco = (1<<30)-10+(1<<30);

        for (int i=0; i<N; i++) {
			bp[i] = i;
        }

        int sa = 10;
        while (sa--) {
			fprintf(stderr,"SA restart %d\n",sa);
			SA();
			fprintf(stderr," New best: %d\n",bsco); fflush(stderr);
        }

        vector<int> ret(N);
        for (int i = 0; i < N; ++i) {
            ret[i] = bp[i];
        }
        return ret;
    }
};
