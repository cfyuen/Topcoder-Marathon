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
double sco[MAXN],bsco;
int e[MAXN][MAXN];
int bp[MAXN];
vector<int> edif[MAXN][MAXN];

double TL = 9.5;

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


class PointsOnTheCircle {
public:
	double scoring(const int *per) {
		double ret = 0;
		for (int i=0; i<N; i++) {
			for (int j=i+1; j<N; j++) {
				if (e[per[i]][per[j]]) ret += sco[abs(i-j)];
			}
		}
		return ret;
	}

	double scoring2(const int *per, const double osco, int idx, int idy) {
		double ret = 0;
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

	double scoring3(const int *per, const int *rper, const double osco, int idx, int idy) {
		double ret = 0;
		for (int i=0; i<edif[per[idx]][per[idy]].size(); i++) {
			int k = edif[per[idx]][per[idy]][i];
			if (k < 500) {
				//fprintf(stderr,"** x %d - %d = - %.4f + %.4f\n",k,rper[k],sco[abs(idx-rper[k])],sco[abs(idy-rper[k])]);
				ret -= sco[abs(idx-rper[k])];
				ret += sco[abs(idy-rper[k])];
			}
			else {
				//fprintf(stderr,"** y %d - %d = - %.4f + %.4f\n",k-500,rper[k-500],sco[abs(idy-rper[k-500])],sco[abs(idx-rper[k-500])]);
				ret -= sco[abs(idy-rper[k-500])];
				ret += sco[abs(idx-rper[k-500])];
			}
		}
		return osco + ret;
	}

	void SA() {
		int a[N],ra[N]; // ra[i] is reverse of a[i]
        for (int i=0; i<N; i++) {
			a[i] = i;
        }

		random_shuffle(a,a+N);
		for (int i=0; i<N; i++) {
			ra[a[i]] = i;
		}
		double nowsco = scoring(a);
		double prevsco = nowsco;
		int iter,totaliter = 500000;
		iter = totaliter;
		double maxt = 50.0,mint = 0.001;
		double temp = maxt;
		while (iter--) {
			if (iter % 100000 == 0 && runtime() > TL) {
				break;
			}
			int idx = rand()%N,idy = rand()%N;
			nowsco = scoring3(a, ra, prevsco, idx, idy);
			swap(a[idx],a[idy]);
			swap(ra[a[idx]],ra[a[idy]]);

			double delta = -(nowsco-prevsco);
			//fprintf(stderr," %d: %.4f - (%.4f vs %.4f = %.4f) - P = %.4f\n",iter,temp,nowsco,prevsco,delta,exp(delta/temp));
			temp = maxt * pow(mint/maxt, (totaliter-iter)*1.0/totaliter);
			if (delta > 0 || exp(delta/temp) > rand()*1.0/RAND_MAX) {
				prevsco = nowsco;
				//fprintf(stderr,"Update!\n");
				if (nowsco < bsco) {
					//fprintf(stderr," New best %d: %.4f [%.4f]\n",iter,nowsco,bsco); fflush(stderr);
					bsco = nowsco;
					memcpy(bp,a,sizeof(a));
				}
			}
			else {
				swap(ra[a[idx]],ra[a[idy]]);
				swap(a[idx],a[idy]);
			}
		}

	}

    vector<int> permute(vector<int> matrix) {
    	startt();
        N = (int)sqrt(matrix.size());
        for (int i=1; i<N; i++) {
			sco[i] = 2*sin(PI / N * i);
			//fprintf(stderr," %d: %.4f\n",i,sco[i]);
        }
        for (int i=0; i<N; i++) {
			for (int j=0; j<N; j++) {
				e[i][j] = matrix[i*N+j];
			}
        }
        for (int i=0; i<N; i++) {
			for (int j=0; j<N; j++) {
				for (int k=0; k<N; k++) {
					if (i == k || j == k) continue;
					if (e[i][k] && !e[j][k]) edif[i][j].push_back(k);
					if (!e[i][k] && e[j][k]) edif[i][j].push_back(k+500);
				}
			}
        }
        bsco = 1e10;

        for (int i=0; i<N; i++) {
			bp[i] = i;
        }

        int sa = 0;
        while (++sa) {
			fprintf(stderr,"SA restart %d\n",sa);
			SA();
			fprintf(stderr," New best: %.4f\n",bsco); fflush(stderr);
			if (runtime() > TL) break;
        }

        vector<int> ret(N);
        for (int i = 0; i < N; ++i) {
            ret[i] = bp[i];
        }
        return ret;
    }
};

