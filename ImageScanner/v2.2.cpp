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

struct LID {
	int a,b,sz;
	int sx,sy;
	int mat;
	bool operator < (const LID &t) const {
		if (t.sx!=sx) return sx<t.sx;
		if (t.sy!=sy) return sy<t.sy;
		if (t.a!=a) return a<t.a;
		if (t.b!=b) return b<t.b;
		return sz<t.sz;
	}
};

const int HHSZ=1000003;

int N,M,numb,numlet;
int useh[HHSZ];
vector<string> let[26][2][21];
int dif[1100][1100];
vector<string> ret;
int epn[305][305];
int scaned[305],scansco[305];
vector<LID> pref;

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

void getLet () {
	char fname[10],tstr[55];
	FILE *flet;
	string cs;
	int fn,fm;
	for (int i=0; i<26; i++)
		for (int j=0; j<2; j++) 
			for (int k=0; k<21; k++) {
				if (j==0) sprintf(fname,"letters/%cB%d.txt",i+'A',k+8);
				else sprintf(fname,"letters/%cP%d.txt",i+'A',k+8);
				//fprintf(debug,"%s\n",fname);
				flet=fopen(fname,"r");
				fscanf(flet,"%d%d",&fn,&fm);
				//fprintf(debug,"%d %d\n",fn,fm);
				fflush(debug);
				for (int x=0; x<fn; x++) {
					fscanf(flet,"%s",tstr);
					cs=tstr;
					let[i][j][k].push_back(cs);
				}
				/*
				for (int x=0; x<fn; x++) {
					for (int y=0; y<fm; y++)
						if (let[i][j][k][x][y]=='0') fprintf(debug,".");
						else fprintf(debug,"#");
					fprintf(debug,"\n");
				}
				*/
				fflush(debug);
				fclose(flet);
			}
}

void ltos (LID l) {
	if (l.b==0) fprintf(debug,"%cB%d",l.a+'A',l.sz+8);
	else fprintf(debug,"%cP%d",l.a+'A',l.sz+8);
}

void ltos (int a,int b,int sz) {
	if (b==0) fprintf(debug,"%cB%d",a+'A',sz+8);
	else fprintf(debug,"%cP%d",a+'A',sz+8);
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

ImageToScan Lib;


int hash (int a,int b,int sz) {
	return a*42+b*21+sz;
}

int hashLID (LID l) {
	return (hash(l.a,l.b,l.sz)*90000+l.sx*300+l.sy)%HHSZ;
}

class ImageScanner {
	public:
		int finddif (int a1,int b1,int sz1,int a2,int b2,int sz2) {
			int pn1=let[a1][b1][sz1].size();
			int pm1=let[a1][b1][sz1][0].length();
			int pn2=let[a2][b2][sz2].size();
			int pm2=let[a2][b2][sz2][0].length();
			if (pn1*pm1-pn2*pm2>50) return 100;
			//if (pn1!=pn2 || pm1!=pm2) return 100;
			if (pn1*pm1-pn2*pm2<0) return -1;
			int match,best=0,cnt=0;
			/*
			for (int i=0; i<pn1; i++)
				for (int j=0; j<pm1; j++)
					if (let[a1][b1][sz1][i][j]=='1' && let[a2][b2][sz2][i][j]=='0') return 100;
			*/
			
			for (int i=0; i<2; i++)
				for (int j=0; j<2; j++) {
					match=0;
					for (int x=0; x<pn1; x++)
						for (int y=0; y<pm1; y++)
							if (i+x<pn2 && j+y<pm2 && let[a1][b1][sz1][x][y]=='1' && let[a2][b2][sz2][i+x][j+y]=='1') 
								match++;
					best=max(best,match);
				}
			
			for (int i=0; i<pn1; i++)
				for (int j=0; j<pm1; j++)
					if (let[a1][b1][sz1][i][j]=='1') cnt++;
			for (int i=0; i<pn2; i++)
				for (int j=0; j<pm2; j++)
					if (let[a2][b2][sz2][i][j]=='1') cnt++;
			
			//fprintf(debug,"%d - %d\n",cnt,best);
			//return 0;
			return cnt-2*best;
		}
			
		int choosescan () {
			int bestsco=-(1<<30),toscan=-1;
			
			for (int i=0; i<N; i++) {
				fprintf(debug,"scansco %d = %d\n",i,scansco[i]);
				if (scaned[i]==0 && scansco[i]>bestsco) {
					bestsco=scansco[i];
					toscan=i;
				}
			}
			if (toscan==-1) return toscan;
			ret[toscan]=Lib.scan(toscan);
			scaned[toscan]=1;
			return toscan;
		}
		void matching (int row) {
			//pref.clear();
			//insert new things into pref
			for (int y=0; y<M; y++) {
				int ok=1;
				LID cand;
				for (int i=0; i<26; i++)
					for (int j=0; j<2; j++)
						for (int k=0; k<21; k++) {
							for (int x=0; x<let[i][j][k].size(); x++) {
								if (row-x<0 || row+let[i][j][k].size()-x>N) continue;
								ok=1;
								for (int d=0; d<let[i][j][k][0].length(); d++)
									if (let[i][j][k][x][d]!=ret[row][y+d]) {
										ok=0; break;
									}
								if (ok==1) {
									cand.a=i; cand.b=j; cand.sz=k;
									cand.sx=row-x; cand.sy=y;
									cand.mat=0;
									if (useh[hashLID(cand)]==0) {
										useh[hashLID(cand)]=1;
										pref.push_back(cand);
									}
									else {
										//fprintf(debug,"FailHash "); ltos(cand); fprintf(debug,"\n");
									}
								}
							}
						}
			}
			fprintf(stderr,"Sub: %.4f (%d)\n",runtime(),pref.size()); fflush(stderr);
			//clear wrong
			for (int i=0; i<pref.size(); i++) {
				int ok=1,nepn=0;
				pref[i].mat=0;
				for (int x=0; x<let[pref[i].a][pref[i].b][pref[i].sz].size(); x++)
					if (scaned[pref[i].sx+x]==1) {
						ok=1;
						for (int y=0; y<let[pref[i].a][pref[i].b][pref[i].sz][0].length(); y++) 
							if (let[pref[i].a][pref[i].b][pref[i].sz][x][y]=='1') {
								if (ret[pref[i].sx+x][pref[i].sy+y]=='0') {
									ok=0; goto fail;
								}
								pref[i].mat++;
								if (epn[pref[i].sx+x][pref[i].sy+y]==0) nepn=1;
							}
					}
				fail:
				if (ok==0 || nepn==0) {
					useh[hashLID(pref[i])]=0;
					pref.erase(pref.begin()+i);
					i--;
				}
				
			}
			fprintf(stderr,"Sub: %.4f (%d)\n",runtime(),pref.size()); fflush(stderr);
			
			fprintf(debug,"Match %d:\n",pref.size());
			for (int i=0; i<pref.size(); i++) {
				ltos(pref[i]);
				fprintf(debug," %d %d\n",pref[i].sx,pref[i].sy);
			}
		}
		void confirm (int ind) {
			fprintf(debug,"confirm: %d\n",ind);
			ltos(pref[ind]);
			fprintf(debug," %d %d\n",pref[ind].sx,pref[ind].sy);
			int pn,pm;
			pn=let[pref[ind].a][pref[ind].b][pref[ind].sz].size();
			pm=let[pref[ind].a][pref[ind].b][pref[ind].sz][0].length();
			for (int i=0; i<pn; i++)
				for (int j=0; j<pm; j++) 
					if (let[pref[ind].a][pref[ind].b][pref[ind].sz][i][j]=='1') {
						ret[pref[ind].sx+i][pref[ind].sy+j]='1';
						epn[pref[ind].sx+i][pref[ind].sy+j]=1;
					}
		}
		void updatesco (LID la,LID lb) {
			//update scansco
			int pn,pm,pn1,pm1,pn2,pm2;
			fprintf(debug,"Update "); ltos(la); fprintf(debug," %d %d  "); ltos(lb); fprintf(debug," %d %d\n"); fflush(debug);
			pn1=let[la.a][la.b][la.sz].size(); pn2=let[lb.a][lb.b][lb.sz].size();
			pm1=let[la.a][la.b][la.sz][0].length(); pm2=let[lb.a][lb.b][lb.sz][0].length();
			pn=max(la.sx+pn1,lb.sx+pn2);
			pm=max(la.sy+pm1,lb.sy+pm2);
			int is1,is2;
			for (int x=min(la.sx,lb.sx); x<pn; x++)
				for (int y=min(la.sy,lb.sy); y<pm; y++) {
					is1=is2=0;
					if (x-la.sx>=0 && y-la.sy>=0 && la.sx+pn1-x>0 && la.sy+pm1-y>0 && let[la.a][la.b][la.sz][x-la.sx][y-la.sy]=='1') is1=1;
					if (x-lb.sx>=0 && y-lb.sy>=0 && lb.sx+pn2-x>0 && lb.sy+pm2-y>0 && let[lb.a][lb.b][lb.sz][x-lb.sx][y-lb.sy]=='1') is2=1;
					if (is1+is2==1) scansco[x]++;
				}
		}
		void processpref () {
			//should also consider connected components, not only match
			int pn,pm;
			for (int i=0; i<N; i++)
				if (scaned[i]==1) {
					for (int j=0; j<M; j++)
						if (ret[i][j]=='1' && (j==0 || (j>0 && ret[i][j-1]=='0')) && epn[i][j]==0) {
							fprintf(debug,"To explain: %d,%d\n",i,j);
							int bind=-1,best=1,secbest=1,sind=-1;
							for (int k=0; k<pref.size(); k++) {
								pn=let[pref[k].a][pref[k].b][pref[k].sz].size();
								pm=let[pref[k].a][pref[k].b][pref[k].sz][0].length();
								if (pref[k].sx<=i && pref[k].sy<=j && pref[k].sx+pn-1>=i && pref[k].sy+pm-1>=j) {
									if (pref[k].mat>5) {
										fprintf(debug,"  "); ltos(pref[k]);
										fprintf(debug," %d %d (%d)\n",pref[k].sx,pref[k].sy,pref[k].mat);
									}
									if (pref[k].mat>best) {
										if (bind>=0 && dif[hash(pref[bind].a,pref[bind].b,pref[bind].sz)][hash(pref[k].a,pref[k].b,pref[k].sz)]>20)
											secbest=best,sind=bind;
										best=pref[k].mat;
										bind=k;
									}
									else if (pref[k].mat>secbest && dif[hash(pref[bind].a,pref[bind].b,pref[bind].sz)][hash(pref[k].a,pref[k].b,pref[k].sz)]>20) {
										secbest=pref[k].mat;
										sind=k;
										fprintf(debug," 2best"); ltos(pref[bind]); fprintf(debug," "); ltos(pref[k]); fprintf(debug,"\n");
									}
								}
							}
							fprintf(debug,"%d/%d - ",best,secbest);
							ltos(pref[bind]);
							fprintf(debug," %d %d\n",pref[bind].sx,pref[bind].sy);
							if (best/secbest>=2 || best-secbest>10 || (bind!=-1 && sind==-1)) {
								confirm(bind);
							}
							else {
								if (bind!=-1 && sind!=-1) updatesco(pref[bind],pref[sind]);
							}
						}
				}
				
		}
		
		void doscan () {
			for (int i=0; i<N/3; i++) {
				fprintf(debug,"Choose %d\n",i+1);
				fprintf(stderr,"Choose %d  (%.4f)\n",i+1,runtime()); fflush(stderr);
				int chosen=choosescan();
				matching(chosen);
				fprintf(stderr,"Match done %.4f\n",runtime()); fflush(stderr);
				memset(scansco,0,sizeof(scansco));
				processpref();
				for (int x=0; x<N; x++)
					for (int y=0; y<M; y++)
						if (ret[x][y]=='1') scansco[x]-=10;
				fprintf(stderr,"Process done %.4f\n",runtime()); fflush(stderr);
				fflush(debug);
			}
		}
		vector <string> restore(int H, int W, int nb, int totlet) {
			N=H; M=W;
			numlet=totlet;
			numb=nb;
			getLet();
			string ts="";
			for (int i=0; i<M; i++) ts+='?';
			for (int i=0; i<N; i++) ret.push_back(ts);
			memset(scansco,0,sizeof(scansco));
			memset(scaned,0,sizeof(scaned));
			memset(epn,0,sizeof(epn));
			memset(useh,0,sizeof(useh));
			//calculate difference
			memset(dif,1,sizeof(dif));
			int ti=0;
			for (int i1=0; i1<26; i1++)
			for (int j1=0; j1<2; j1++)
			for (int k1=0; k1<21; k1++)
				for (int i2=0; i2<26; i2++)
				for (int j2=0; j2<2; j2++)
				for (int k2=max(0,k1-2); k2<min(21,k1+2); k2++) {
					ti=finddif(i1,j1,k1,i2,j2,k2);
					if (ti>=0) {
						dif[hash(i1,j1,k1)][hash(i2,j2,k2)]=ti;
						dif[hash(i2,j2,k2)][hash(i1,j1,k1)]=ti;
						
						if (ti<=20  && hash(i1,j1,k1)!=hash(i2,j2,k2)) {
							ltos(i1,j1,k1); fprintf(debug,"  ");
							ltos(i2,j2,k2); fprintf(debug,"  : %d\n",ti);
							fflush(debug);
						}
						
					}
				}
			
			doscan();	
			for (int i=0; i<N; i++)
				for (int j=0; j<M; j++)
					if (ret[i][j]=='?') ret[i][j]='0';
			/*
			for (int i=0; i<N; i+=2) {
				ret[i]=Lib.scan(i);
			}
			for (int i=1; i<N; i+=2) {
				for (int j=0; j<M; j++)
					if (ret[i-1][j]=='1' && i+1<N && ret[i+1][j]=='1') ret[i][j]='1';
					else ret[i][j]='0';
			}
			*/
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

