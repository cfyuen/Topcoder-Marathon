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

int N,E;
double sco[MAXN],bsco;
int e[MAXN][MAXN];
int bp[MAXN];
vector<int> edif[MAXN][MAXN];

int totaliter = 2000000;
double maxt = 50.0,mint = 0.001;
double tpre[10000005];

long long ops = 0,ops2 = 0,ops3 = 0;
double TL = 19.8;

/*
int g_seed = 0;

inline int rand() {
  g_seed = (214013*g_seed+2531011);
  return (g_seed>>16)&0x7FFF;
}

inline int randN(int n) {
  return rand()%n;
}
*/

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
				//fprintf(debug," x (%d -> %d : %d) - %.4f + %.4f\n",idx,idy,i,sco[abs(idx-i)],sco[abs(idy-i)]);
				ret -= sco[abs(idx-i)];
				ret += sco[abs(idy-i)];
			}
			if (e[per[idy]][per[i]] && i != idx) {
				//fprintf(debug," y (%d -> %d : %d) - %.4f + %.4f\n",idy,idx,i,sco[abs(idy-i)],sco[abs(idx-i)]);
				ret -= sco[abs(idy-i)];
				ret += sco[abs(idx-i)];
			}
		}
		return osco + ret;
	}

	inline double scoring3(const int *per, const int *rper, const double osco, int idx, int idy) {
		double ret = 0;
		ops2++;
		for (int i=0; i<edif[per[idx]][per[idy]].size(); i++) {
			int k = edif[per[idx]][per[idy]][i];
			ops++;
			if (k < 500) {
				//fprintf(debug,"** x %d - %d = - %.4f + %.4f\n",k,rper[k],sco[abs(idx-rper[k])],sco[abs(idy-rper[k])]);
				ret -= sco[abs(idx-rper[k])];
				ret += sco[abs(idy-rper[k])];
			}
			else {
				//fprintf(debug,"** y %d - %d = - %.4f + %.4f\n",k-500,rper[k-500],sco[abs(idy-rper[k-500])],sco[abs(idx-rper[k-500])]);
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
		double prevsco = nowsco,delta;
		int iter = totaliter;
		double temp = maxt;
		int check_time = ((1<<15)-1);
		int idx,idy,idd;
		long long nor = 0;
		double fac = N*10.0/RAND_MAX;
		fprintf(debug,"%d %d\n",RAND_MAX,RAND_MAX);
		while (iter--) {
			if ((iter & check_time) == 0 && runtime() > TL) {
				break;
			}

			temp = tpre[iter];

			//int idx = rand()%N,idy = rand()%N;
			//idx = rand()%N; idy = rand()%N;

			if (iter < totaliter) {
				nor = 0;
				nor += rand()+rand()+rand()+rand()+rand()+rand()+rand()+rand()+rand()+rand()+rand()+rand();
				nor -= RAND_MAX*6;
				idd = (int)ceil(abs(nor*temp*fac));
				//if (iter%500 == 0) fprintf(debug,"%d\n",idd);
				idx = rand()%N; idy = (idx + idd)%N;
			}
			else {
				idx = rand()%N; idy = (idx + 1 + rand()%(N-1))%N;
			}

			nowsco = scoring3(a, ra, prevsco, idx, idy);
			swap(a[idx],a[idy]);
			swap(ra[a[idx]],ra[a[idy]]);

			delta = -(nowsco-prevsco);
			//fprintf(debug," %d: %.4f - (%.4f vs %.4f = %.4f) - P = %.4f\n",iter,temp,nowsco,prevsco,delta,exp(delta/temp));
			//temp = maxt * pow(mint/maxt, (totaliter-iter)*1.0/totaliter);
			//if (iter%500 == 0) fprintf(debug,"%.4f\t%.4f\n",prevsco,temp);
			if (delta >= 0 || exp(delta/temp)*RAND_MAX > rand()) {
				prevsco = nowsco;
				//fprintf(debug,"Update!\n");
				if (nowsco < bsco) {
					//if (iter < 30000) { fprintf(debug," New best %d: %.4f [%.4f]\n",iter,nowsco,bsco); fflush(debug); }
					bsco = nowsco;
					memcpy(bp,a,sizeof(a));
				}
			}
			else {
				ops3++;
				swap(ra[a[idx]],ra[a[idy]]);
				swap(a[idx],a[idy]);
			}
		}
		fprintf(debug,"\n");
		fprintf(debug,"Score = %.4f\n",nowsco);

		/*
		prevsco = bsco;
		for (int i=0; i<N; i++) {
			ra[bp[i]] = i;
		}
		for (int i=0; i<N; i++) {
			for (int j=i+1; j<N; j++) {
				nowsco = scoring3(bp, ra, prevsco, i, j);
				if (nowsco < prevsco) {
					fprintf(debug,"Improvement(from %.4f): %d,%d (%.4f)\n",prevsco,i,j,prevsco-nowsco);
				}
			}
		}
		*/
	}

    vector<int> permute(vector<int> matrix) {
    	startt();
        N = (int)sqrt(matrix.size());
        for (int i=1; i<N; i++) {
			sco[i] = 2*sin(PI / N * i);
			//fprintf(debug," %d: %.4f\n",i,sco[i]);
        }
        E = 0;
        for (int i=0; i<N; i++) {
			//fprintf(debug,"%d:",i);
			for (int j=0; j<N; j++) {
				e[i][j] = matrix[i*N+j];
				//if (e[i][j]) fprintf(debug," %d",j);
				if (e[i][j]) E++;
			}
			//fprintf(debug,"\n");
        }
        E /= 2;
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
        fprintf(debug,"N = %d\tE = %d\n",N,E);

        for (int i=0; i<totaliter; i++) {
			tpre[totaliter-i] = maxt * pow(mint/maxt, (i)*1.0/totaliter);
        }

        for (int i=0; i<N; i++) {
			bp[i] = i;
        }

        int sa = 0;
        while (++sa) {
			fprintf(stderr,"SA restart %d\n",sa);
			fprintf(debug,"SA restart %d\n",sa);
			SA();
			fprintf(stderr," New best: %.4f\n",bsco); fflush(stderr);
			fprintf(stderr," ops: %lld   ops2: %lld   ops3: %lld\n",ops,ops2,ops3);
			//if (sa == 1) break;
			if (runtime() > TL) break;
        }

        vector<int> ret(N);
        for (int i = 0; i < N; ++i) {
            ret[i] = bp[i];
        }
        return ret;
    }
};


// -------8<------- end of solution submitted to the website -------8<-------

template<class T> void getVector(vector<T>& v) {
    for (int i = 0; i < v.size(); ++i)
        cin >> v[i];
}

int main() {
    PointsOnTheCircle pc;
    debug=fopen("debug.txt","w");
    int M;
    cin >> M;
    vector<int> matrix(M);
    getVector(matrix);

    vector<int> ret = pc.permute(matrix);
    cout << ret.size() << endl;
    for (int i = 0; i < (int)ret.size(); ++i)
        cout << ret[i] << endl;
    cout.flush();

    fflush(stdout);
    fflush(debug);
    fflush(stderr);
}
