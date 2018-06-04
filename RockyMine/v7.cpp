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
/*********************
v7 Approach:
	1. Rank the gold to take
	    a. Gold value
	    b. Surrounding gold value
	    c. Estimated round of bomb to get the gold
	    d. Number of steps required to take the gold
	2. Get the gold efficiently
		Now use IDA* approach to locate the bomb
		Reduce to sliding window 20x20
	3. Augmented route
	4. Use efficient saferoute()
	5. Speed up
	Extra: use bomb to step ratio
 *********************/
 
#define winsz 21

struct Bomb {
	int left;
	int eff[6][6];
} b[10];

struct Grid {
	int gold,rock;
} og[105][105],g[105][105];

struct Coor {
	int x,y;
	bool operator == (Coor const &a) const{
		return (x==a.x && y==a.y);
	}
};

struct PutBomb {
	Coor c;
	int typ,tl;
};

struct State {
	Coor c;
	vector<PutBomb> pb;
	int gold[winsz][winsz],rock[winsz][winsz];
	int tgc,tgd,sco;
	int bleft[10];
	bool operator < (State const &a) const {
		return a.sco>sco;
	}
};

struct Route {
	Coor fr,to;
};

struct Pathway {
	Coor c;
	int cntg,gv,gold[300];
	int bp,be;
	string rec;
	bool operator < (Pathway const &a) const {
		if (a.rec.length()!=rec.length()) return a.rec.length()<rec.length();
		if (a.gv!=gv) return a.gv>gv;
		if (a.bp!=bp) return a.bp>bp;
		return a.be>be;
	}
};

int n,m,dn,move;
int dx[]={1,0,-1,0,0},dy[]={0,1,0,-1,0};
int reach[105][105];
int oleft[10],omove;
PutBomb cur[300],fin[300];
int bplace=0,bbsco[300],bfin=0;
double timenow=0;
Coor q[10005];
priority_queue<State> stq[2];
int bestsco=-1000000,backup;
vector<Route> rou;
Coor pick[300];
int pcnt=0,opn[300],clos[300],forb[100][300][300];
int mxmv[30];
string pd[300][300];
Pathway pat;
const Pathway *ppt;
priority_queue<Pathway> pq;
int cbest[50][8][1<<8][1<<5];
int bdt=2;
string rec[10005];
int vb[2][105][105];
int cntopr=0,cntst=0;

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

void disbd () {
	for (int i=0; i<n; i++) {
	    for (int j=0; j<m; j++)
	        fprintf(debug,"%d(%d) ",g[i][j].rock,g[i][j].gold);
		fprintf(debug,"\n");
	}
	fflush(debug);
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

string dir (int d) {
	if (d==0) return "S";
	if (d==1) return "E";
	if (d==2) return "N";
	if (d==3) return "W";
	return "-";
}

void fill (Coor at) {
	int q1=0,q2=0;
	memset(reach,0,sizeof(reach));
	q[0]=at;
	reach[at.x][at.y]=1;
	while (q1<=q2) {
		at=q[q1];
		for (int i=0; i<4; i++) {
			Coor nat;
			nat.x=at.x+dx[i]; nat.y=at.y+dy[i];
			if (nat.x>=0 && nat.y>=0 && nat.x<n && nat.y<m && reach[nat.x][nat.y]==0 && g[nat.x][nat.y].rock<=0) {
				reach[nat.x][nat.y]=reach[at.x][at.y]+1;
				q2++;
				q[q2]=nat;
			}
		}
		q1++;
	}
}

void fillst (const State &st) {
	int q1=0,q2=0;
	memset(reach,0,sizeof(reach));
	q[0].x=10; q[0].y=10;
	reach[winsz/2][winsz/2]=1;
	while (q1<=q2) {
		for (int i=0; i<4; i++) {
			Coor nat;
			nat.x=q[q1].x+dx[i]; nat.y=q[q1].y+dy[i];
			if (nat.x>=0 && nat.y>=0 && nat.x<winsz && nat.y<winsz && nat.x+st.c.x-winsz/2>=0 && nat.y+st.c.y-winsz/2>=0 &&  nat.x+st.c.x-winsz/2<n && nat.y+st.c.y-winsz/2<m && reach[nat.x][nat.y]==0 && st.rock[nat.x][nat.y]<=0) {
				reach[nat.x][nat.y]=reach[q[q1].x][q[q1].y]+1;
				q2++;
				q[q2]=nat;
			}
		}
		q1++;
	}
}

class RockyMine {
	public:
		Coor target (Coor at) {
			Coor bc;
			double bestsco=-1e9;
			for (int i=0; i<n; i++)
			    for (int j=0; j<m; j++) {
					double cursco=0.0;
					cursco+=g[i][j].gold;
					if (g[i][j].gold==0 || g[i][j].rock<0) continue;
					for (int x=i-5; x<=i+5; x++)
					    for (int y=j-5; y<=j+5; y++)
					        if (x>=0 && y>=0 && x<n && y<m && !(x==i && y==j)) {
								cursco+=g[x][y].gold*0.6/(abs(x-i)+abs(y-j));
							}
					cursco-=abs(at.x-i)+abs(at.y-j);
					if (cursco>bestsco) {
						bestsco=cursco;
						bc.x=i; bc.y=j;
					}
					//fprintf(debug,"(%d,%d)[%d] %0.4lf\n",i,j,g[i][j].gold,cursco);
				}
			//disbd();
			return bc;
		}
		int scorest (const State &st) {
			return st.tgc*8-st.tgd-st.pb.size()*max(40,max(n,m));
		}
		void placebomb (Coor fr,Coor tar,int limit) {
			//improvement: note that new bomb must be placed near the old bomb
			//				only store free space near the old bomb
			//				consider new bomb there
			//				does not need the whole coor grid, only need the bomb placed area
			State at;
			const State *ptr;
			int cntlim=0,step=0;
			int swch=0,worst=-100000;
			backup=-1000000;
			PutBomb nb;
			at.c=fr;
			at.tgc=0; at.tgd=0;
			int sx=at.c.x-winsz/2,sy=at.c.y-winsz/2;
			for (int i=sx; i<sx+winsz; i++)
				for (int j=sy; j<sy+winsz; j++) 
					if (i>=0 && j>=0 && i<n && j<m) {
						at.gold[i-sx][j-sy]=g[i][j].gold;
						at.rock[i-sx][j-sy]=g[i][j].rock;
					}
					else {
						at.gold[i-sx][j-sy]=-1;
						at.rock[i-sx][j-sy]=9;
					}
			for (int i=0; i<dn; i++)
				at.bleft[i]=b[i].left;
			at.sco=scorest(at);
			stq[swch].push(at);
			do {
				cntlim=0;
				fprintf(debug,"Step %d: \n",step); fflush(debug);
				while (!stq[swch].empty()) {
					ptr=&(stq[swch].top());
					
					if (cntlim<5) fprintf(debug," %d(%d): (%d,%d): (%d,%d) %d\n",cntlim,stq[1-swch].size(),(*ptr).c.x,(*ptr).c.y,(*ptr).tgc,(*ptr).tgd,(*ptr).sco);
					/*
					for (int i=0; i<winsz; i++) {
						for (int j=0; j<winsz; j++)
							if ((*ptr).rock[i][j]==0 && (*ptr).gold[i][j]>0) fprintf(debug,"%2d ",(*ptr).gold[i][j]);
								else if ((*ptr).rock[i][j]<=0) fprintf(debug,".. ");
								else fprintf(debug,"## ");
						fprintf(debug,"\n");
					}
					*/
					if (cntlim==0 && bestsco-100>(*ptr).sco) break;
					cntlim++;
					if (cntlim>limit) {
						worst=(*ptr).sco;
						break;
					}
					fillst(*ptr);
					/*
					for (int i=0; i<winsz; i++) {
						for (int j=0; j<winsz; j++)
							fprintf(debug,"%2d ",reach[i][j]);
						fprintf(debug,"\n");
					}
					*/
					//arrive at target
					if (bestsco==-1000000 && (*ptr).sco>backup) {
						backup=(*ptr).sco;
						bfin=(*ptr).pb.size();
						for (int i=0; i<bfin; i++)
							fin[i]=(*ptr).pb[i];
					}
					if (abs(tar.x-(*ptr).c.x)<=winsz/2 && abs(tar.y-(*ptr).c.y)<=winsz/2 && reach[tar.x-(*ptr).c.x+winsz/2][tar.y-(*ptr).c.y+winsz/2]>=1) {
						if (scorest(*ptr)>bestsco) {
							bestsco=scorest(*ptr);
							bfin=(*ptr).pb.size();
							for (int i=0; i<bfin; i++)
								fin[i]=(*ptr).pb[i];
						}
						stq[swch].pop();
						continue;
					}
					
					int mindis=10000,chx,chy;
					sx=(*ptr).c.x-winsz/2; sy=(*ptr).c.y-winsz/2;
					for (int i=max(0,sx); i<min(n,sx+winsz); i++)
						for (int j=max(0,sy); j<min(m,sy+winsz); j++) 
							if (reach[i-sx][j-sy]>=1) {
								mindis=min(abs(i-tar.x)+abs(j-tar.y),mindis);
							}
					//fprintf(debug,"Mindist %d\n",mindis);
					for (int i=max(0,sx); i<min(n,sx+winsz); i++)
						for (int j=max(0,sy); j<min(m,sy+winsz); j++) 
							if (reach[i-sx][j-sy]>=1 && abs(i-tar.x)+abs(j-tar.y)<=mindis+1) {
								//new bomb will be placed here
								//fprintf(debug," add (%d,%d)(%d,%d) %d\n",i,j,i-sx,j-sy,abs(i-tar.x)+abs(j-tar.y));
								int tx,ty;
								for (int k=0; k<dn; k++)
									if ((*ptr).bleft[k]>0) {
										at.c.x=i; at.c.y=j;
										chx=i-(*ptr).c.x; chy=j-(*ptr).c.y;
										at.tgc=(*ptr).tgc; at.tgd=(*ptr).tgd;
										tx=at.c.x-winsz/2; ty=at.c.y-winsz/2;
										//fprintf(debug," (%d,%d) + (%d,%d) = (%d,%d)\n",(*ptr).c.x,(*ptr).c.y,chx,chy,i,j);
										for (int xx=tx; xx<tx+winsz; xx++)
											for (int yy=ty; yy<ty+winsz; yy++) {
												if (xx>=0 && yy>=0 && xx<n && yy<m) {
													if (abs(xx-(*ptr).c.x)<=winsz/2 && abs(yy-(*ptr).c.y)<=winsz/2) {
														at.gold[xx-tx][yy-ty]=(*ptr).gold[xx-tx+chx][yy-ty+chy];
														//fprintf(debug,"   [%d,%d](%d,%d) -> (%d,%d)\n",xx,yy,xx-tx+chx,yy-ty+chy,xx-tx,yy-ty);
														at.rock[xx-tx][yy-ty]=(*ptr).rock[xx-tx+chx][yy-ty+chy];
													}
													else {
														at.gold[xx-tx][yy-ty]=g[xx][yy].gold;
														at.rock[xx-tx][yy-ty]=g[xx][yy].rock;
													}
													if (xx>=i-2 && xx<=i+2 && yy>=j-2 && yy<=j+2) {
														at.rock[xx-tx][yy-ty]-=b[k].eff[xx-i+2][yy-j+2];
														if (at.rock[xx-tx][yy-ty]==0) {
															at.tgc+=at.gold[xx-tx][yy-ty];
															at.gold[xx-tx][yy-ty]=0;
														}
														if (at.rock[xx-tx][yy-ty]<0) {
															at.tgd+=at.gold[xx-tx][yy-ty];
															at.gold[xx-tx][yy-ty]=0;
														}
													}
												}
												else {
													at.gold[xx-tx][yy-ty]=-1;
													at.rock[xx-tx][yy-ty]=9;
												}
											}
										//fprintf(debug,"  c %d d %d\n",at.tgc,at.tgd);
										for (int bd=0; bd<dn; bd++)
											at.bleft[bd]=(*ptr).bleft[bd];
										at.bleft[k]--;
										nb.c.x=i; nb.c.y=j; nb.typ=k;
										at.pb=(*ptr).pb;
										at.pb.push_back(nb);
										at.sco=scorest(at);
										//if (at.sco>worst) 
										stq[1-swch].push(at);
									}
							}
					stq[swch].pop();
				}
			while (!stq[swch].empty()) stq[swch].pop();
			swch=1-swch;
			step++;
			} while (!stq[swch].empty());
		}
		string walkmove (Coor fr,Coor to) {
			int q1,q2,v[105][105];
			memset(v,0,sizeof(v));
			Coor nat;
			q1=0; q2=0;
			q[0].x=fr.x; q[0].y=fr.y;
			rec[0]="";
			v[fr.x][fr.y]=1;
			while (q1<=q2) {
				q1++;
				if (q[q1-1].x==to.x && q[q1-1].y==to.y) break;
				for (int i=0; i<4; i++) {
					nat.x=q[q1-1].x+dx[i]; nat.y=q[q1-1].y+dy[i];
					if (nat.x>=0 && nat.y>=0 && nat.x<n && nat.y<m && v[nat.x][nat.y]==0 && g[nat.x][nat.y].rock<=0) {
						v[nat.x][nat.y]=1;
						q2++;
						rec[q2]=rec[q1-1]+dir(i);
						q[q2].x=nat.x; q[q2].y=nat.y;
					}
				}
			}
			return rec[q1-1];
		}
		pair<Coor,string> saferoute (Coor at,int bbid) {
			//fprintf(debug,"safe (%d,%d) -> (%d,%d)\n",at.x,at.y,fin[bbid].c.x,fin[bbid].c.y); fflush(debug);
			//go to place the bomb
			string ret=walkmove(at,fin[bbid].c);
			//fprintf(debug,"goto place done\n"); fflush(debug);
			//place the bomb
			ret+=fin[bbid].typ+'0';
			ret+="-----";
			int sx,sy;
			sx=fin[bbid].c.x-2; sy=fin[bbid].c.y-2;
			for (int xx=max(0,sx); xx<=min(n-1,sx+4); xx++)
				for (int yy=max(0,sy); yy<=min(m-1,sy+4); yy++) {
					g[xx][yy].rock-=b[fin[bbid].typ].eff[xx-sx][yy-sy];
				}
			b[fin[bbid].typ].left--;
			//fprintf(debug,"place bomb done\n"); fflush(debug);
			//escape from the bomb
			pcnt=0;
			Coor bf=fin[bbid].c;
			pick[pcnt]=bf; pcnt++;
			fill(bf);
            for (int i=max(0,bf.x-4); i<=min(n-1,bf.x+4); i++)
                for (int j=max(0,bf.y-4); j<=min(m-1,bf.y+4); j++)
                    if (reach[i][j]<=10 && reach[i][j]>=1 && g[i][j].rock==0 && g[i][j].gold>0) {
						pick[pcnt].x=i; pick[pcnt].y=j;
						pcnt++;
					}
			if (bbid!=bfin-1) { pick[pcnt]=fin[bbid+1].c; pcnt++; }
			vector<int> perv;
			for (int i=0; i<pcnt; i++)
			    for (int j=0; j<pcnt; j++) {
					pd[i][j]=walkmove(pick[i],pick[j]);
					//fprintf(debug,"(%d,%d) -> (%d,%d) : %s\n",pick[i].x,pick[i].y,pick[j].x,pick[j].y,pd[i][j].c_str());
				}
			for (int i=0; i<pcnt; i++) {
				perv.push_back(i);
				//fprintf(debug,"Pick (%d,%d)[%d] - %d\n",pick[i].x,pick[i].y,g[pick[i].x][pick[i].y].gold,g[pick[i].x][pick[i].y].rock);
			}
			fflush(debug);
			Coor endc=pick[0];
			if (perv.size()>1) {
				string bpick="",curpick="";
				int bpstep=100000;
				do {
					curpick="";
					for (int i=1; i<perv.size(); i++) {
						curpick+=pd[perv[i-1]][perv[i]];
						if (curpick.size()>bpstep) break;
					}
					if (curpick.size()<bpstep) {
						bpstep=curpick.size();
						endc=pick[perv[perv.size()-1]];
						bpick=curpick;
						/*
						for (int i=0; i<pcnt; i++)
						    fprintf(debug,"-> (%d,%d) ",pick[perv[i]].x,pick[perv[i]].y);
						fprintf(debug,"\n%d: %s\n",bpstep,bpick.c_str()); fflush(debug);
						*/
					}
				} while (next_permutation(perv.begin()+1,perv.end()-1));
				for (int i=0; i<pcnt; i++)
					g[pick[i].x][pick[i].y].gold=0;
				ret+=bpick;
			}
			//fprintf(debug,"escape done\n"); fflush(debug);
			mxmv[bbid]=ret.length();
			return make_pair(endc,ret);
		}
		void getgoldpos () {
			pcnt=0;
			memset(opn,-1,sizeof(opn));
			memset(clos,-1,sizeof(clos));
			memset(forb,0,sizeof(forb));
			for (int i=0; i<n; i++)
				for (int j=0; j<m; j++) {
					if (g[i][j].rock>0) forb[0][i][j]=1;
				}
			int orch[105][105];
			for (int k=0; k<bfin; k++) {
				int sx,sy;
				fill(fin[k].c);
				for (int i=0; i<n; i++)
					for (int j=0; j<m; j++)
						orch[i][j]=reach[i][j];
				sx=fin[k].c.x-2; sy=fin[k].c.y-2;
				for (int xx=max(0,sx); xx<=min(n-1,sx+4); xx++)
					for (int yy=max(0,sy); yy<=min(m-1,sy+4); yy++) {
						g[xx][yy].rock-=b[fin[k].typ].eff[xx-sx][yy-sy];
						for (int i=0; i<pcnt; i++)
							if (pick[i].x==xx && pick[i].y==yy && clos[i]==-1 && g[xx][yy].rock<0) {
								clos[i]=k;
							}
					}
				b[fin[k].typ].left--;
				for (int i=0; i<n; i++)
					for (int j=0; j<m; j++) {
						if (g[i][j].rock>0) {
							forb[k*2+2][i][j]=1;
							forb[k*2+1][i][j]=1;
						}
						if (abs(i-fin[k].c.x)<=2 && abs(j-fin[k].c.y)<=2) {
							if (b[fin[k].typ].eff[i-sx][j-sy]>0) forb[k*2+1][i][j]=1;
						}
					}
				//escape from the bomb
				
				Coor bf=fin[k].c;
				fill(bf);
	            for (int i=max(0,bf.x-4); i<=min(n-1,bf.x+4); i++)
	                for (int j=max(0,bf.y-4); j<=min(m-1,bf.y+4); j++)
	                    if (reach[i][j]<=10 && reach[i][j]>=1 && g[i][j].rock==0 && orch[i][j]==0 && g[i][j].gold>0) {
							pick[pcnt].x=i; pick[pcnt].y=j;
							g[i][j].gold=0;
							opn[pcnt]=k;
							pcnt++;
						}
				
				for (int i=0; i<pcnt; i++)
				    for (int j=0; j<pcnt; j++) {
						pd[i][j]=walkmove(pick[i],pick[j]);
						//fprintf(debug,"(%d,%d) -> (%d,%d) : %s\n",pick[i].x,pick[i].y,pick[j].x,pick[j].y,pd[i][j].c_str());
					}
			}
			for (int i=0; i<pcnt; i++)
				if (clos[i]==-1) clos[i]=bfin;
		}
		//objective
		//1. get a unget gold (plus wait)
		//2. place a new bomb
		void goldbfs (int fb) {
			int q1,q2,tg[105][105],cntt=0,curl,obe=pat.be;
			memset(vb,0,sizeof(vb));
			memset(tg,-1,sizeof(tg));
			for (int i=0; i<pcnt; i++) {
				if (opn[i]>=pat.bp) break;
				if (pat.gold[i]==-1) {
					tg[pick[i].x][pick[i].y]=i;
					//fprintf(debug,"T %d,%d\n",pick[i].x,pick[i].y);
					cntt++;
				}
			}
			if (cntt==0) return;
			Coor nat;
			q1=0; q2=0;
			q[0]=pat.c;
			rec[0]="";
			curl=0;
			if (pat.rec.length()>=6 && pat.rec[pat.rec.length()-6]>='0' && pat.rec[pat.rec.length()-6]<='9') fb--,pat.be--;
			vb[curl%2][q[0].x][q[0].y]=1;
			string trec;
			while (q1<=q2) {
				q1++;
				cntopr++;
				if (rec[q1-1].length()!=curl || rec[q1-1].length()==0) {
					memset(vb[curl%2],0,sizeof(vb[curl%2]));
					trec=pat.rec+rec[q1-1];
					if (trec.length()>=5 && trec[trec.length()-5]>='0' && trec[trec.length()-5]<='9') fb++;
					if (trec.length()>=6 && trec[trec.length()-6]>='0' && trec[trec.length()-6]<='9') { fb++; pat.be++; }
					if (rec[q1-1].length()>0) curl++;
				}
				//fprintf(debug," (%d,%d) %d/%d %d[%d]:%s\n",q[q1-1].x,q[q1-1].y,pat.bp,pat.be,curl,fb,(pat.rec+rec[q1-1]).c_str()); fflush(debug);
				if (tg[q[q1-1].x][q[q1-1].y]!=-1) {
					//fprintf(debug,"Arrive %d\n",tg[q[q1-1].x][q[q1-1].y]);
					//fprintf(debug," (%d,%d) %d[%d]:%s\n",q[q1-1].x,q[q1-1].y,curl,fb,(pat.rec+rec[q1-1]).c_str()); fflush(debug);
					if (pat.be<=clos[tg[q[q1-1].x][q[q1-1].y]]) {
						trec=pat.rec; Coor tc=pat.c;
						pat.rec+=rec[q1-1];
						pat.c=q[q1-1];
						pat.gold[tg[q[q1-1].x][q[q1-1].y]]=1;
						pat.cntg++;
						pat.gv+=og[q[q1-1].x][q[q1-1].y].gold;
						pq.push(pat);
						pat.gv-=og[q[q1-1].x][q[q1-1].y].gold;
						pat.c=tc;
						pat.rec=trec;
						pat.gold[tg[q[q1-1].x][q[q1-1].y]]=-1;
						pat.cntg--;
						tg[q[q1-1].x][q[q1-1].y]=-1;
					}
					//else fprintf(debug,"END\n");
					cntt--;
					if (cntt==0) break;
				} 
				/*
				for (int i=0; i<n; i++) {
					for (int j=0; j<m; j++)
						fprintf(debug,"%d",forb[fb][i][j]);
					fprintf(debug,"\n");
				}
				*/
				/*
				for (int i=0; i<n; i++) {
					for (int j=0; j<m; j++)
						fprintf(debug,"%d",v[1-curl%2][i][j]);
					fprintf(debug,"\n");
				}
				*/
				for (int i=0; i<5; i++) {
					nat.x=q[q1-1].x+dx[i]; nat.y=q[q1-1].y+dy[i];
					if (nat.x>=0 && nat.y>=0 && nat.x<n && nat.y<m && vb[1-curl%2][nat.x][nat.y]==0 && forb[fb][nat.x][nat.y]==0) {
						vb[1-curl%2][nat.x][nat.y]=1;
						q2++;
						rec[q2]=rec[q1-1]+dir(i);
						q[q2].x=nat.x; q[q2].y=nat.y;
					}
				}
			}
			pat.be=obe;
		}
		void bombbfs (int fb) {
			int q1,q2,curl;
			memset(vb,0,sizeof(vb));
			Coor nat;
			q1=0; q2=0;
			q[0]=pat.c;
			rec[0]="";
			curl=0;
			if (pat.rec.length()>=6 && pat.rec[pat.rec.length()-6]>='0' && pat.rec[pat.rec.length()-6]<='9') fb--,pat.be--;
			vb[curl%2][q[0].x][q[0].y]=1;
			string trec;
			while (q1<=q2) {
				q1++;
				cntopr++;
				if (rec[q1-1].length()!=curl || rec[q1-1].length()==0) {
					memset(vb[curl%2],0,sizeof(vb[curl%2]));
					trec=pat.rec+rec[q1-1];
					if (trec.length()>=5 && trec[trec.length()-5]>='0' && trec[trec.length()-5]<='9') fb++;
					if (trec.length()>=6 && trec[trec.length()-6]>='0' && trec[trec.length()-6]<='9') { fb++; pat.be++; }
					if (rec[q1-1].length()>0) curl++;
				}
				//fprintf(debug," (%d,%d) %d/%d %d[%d]:%s\n",q[q1-1].x,q[q1-1].y,pat.bp,pat.be,curl,fb,(pat.rec+rec[q1-1]).c_str()); fflush(debug);
				if (pat.bp==bfin) {
					if (pat.bp==pat.be) {
						trec=pat.rec; Coor tc=pat.c;
						pat.rec+=rec[q1-1];
						pat.c=q[q1-1];
						pq.push(pat);
						pat.c=tc;
						pat.rec=trec;
						break;
					}
				}
				else {
					if (fin[pat.bp].c==q[q1-1] && forb[fb][q[q1-1].x][q[q1-1].y]==0) {
						//fprintf(debug,"Hi Bomb %d\n",pat.bp);
						//fprintf(debug," (%d,%d) %d/%d %d[%d]:%s\n",q[q1-1].x,q[q1-1].y,pat.bp,pat.be,curl,fb,(pat.rec+rec[q1-1]).c_str()); fflush(debug);
						trec=pat.rec; Coor tc=pat.c;
						pat.rec+=rec[q1-1];
						pat.rec+=fin[pat.bp].typ+'0';
						if (pat.rec.length()>=6 && pat.rec[pat.rec.length()-6]>='0' && pat.rec[pat.rec.length()-6]<='9') pat.be++;
						pat.c=q[q1-1];
						pat.bp++;
						pq.push(pat);
						if (pat.rec.length()>=6 && pat.rec[pat.rec.length()-6]>='0' && pat.rec[pat.rec.length()-6]<='9') pat.be--;
						pat.c=tc;
						pat.rec=trec;
						pat.bp--;
						if (pat.bp==pat.be) break;
					} 
					for (int i=0; i<5; i++) {
						nat.x=q[q1-1].x+dx[i]; nat.y=q[q1-1].y+dy[i];
						if (nat.x>=0 && nat.y>=0 && nat.x<n && nat.y<m && vb[1-curl%2][nat.x][nat.y]==0 && forb[fb][nat.x][nat.y]==0) {
							vb[1-curl%2][nat.x][nat.y]=1;
							q2++;
							rec[q2]=rec[q1-1]+dir(i);
							q[q2].x=nat.x; q[q2].y=nat.y;
						}
					}
				}
			}
		}
		
		pair<Coor,string> greedyman () {
			getgoldpos();
			int totg=0,curl=0,curbest=0,lim=15,cnt=0;
			int bhigh=0,bbb=0;
			string bret="";
			Coor bat=fin[0].c;
			/*
			for (int i=0; i<=bfin*2; i++) {
				fprintf(debug,"i = %d\n",i);
				for (int j=0; j<n; j++) {
					for (int k=0; k<m; k++)
						fprintf(debug,"%d",forb[i][j][k]);
					fprintf(debug,"\n");
				}
			}
			*/
			memset(cbest,-1,sizeof(cbest));
			fprintf(debug,"PICK\n");
			for (int i=0; i<pcnt; i++) {
				fprintf(debug,"pick (%d,%d)[%d] %d,%d\n",pick[i].x,pick[i].y,og[pick[i].x][pick[i].y].gold,opn[i],clos[i]);
				totg+=og[pick[i].x][pick[i].y].gold;
			}
			pat.c=fin[0].c;
			pat.rec="";
			pat.rec+=fin[0].typ+'0';
			for (int i=0; i<pcnt; i++)
				pat.gold[i]=-1;
			pat.cntg=0;
			pat.gv=0;
			pat.bp=1; pat.be=0;
			pq.push(pat);
			while (!pq.empty()) {
				ppt=&(pq.top());
				if ((*ppt).rec.length()!=curl) {
					curl=(*ppt).rec.length();
					cnt=0;
				}
				
				if ((*ppt).gv>=totg*9/10) {
					if ((*ppt).bp==(*ppt).be) {
						fprintf(debug,"DONE %s\n",(*ppt).rec.c_str());
						fprintf(debug,"pq(%d)= (%d,%d) [%d]: %d/%d  %s\n  ",pq.size(),(*ppt).c.x,(*ppt).c.y,(*ppt).gv,(*ppt).bp,(*ppt).be,(*ppt).rec.c_str());
						for (int i=0; i<pcnt; i++)
							fprintf(debug,"%d",((*ppt).gold[i]+2)/2);
						fprintf(debug,"\n");
						fflush(debug);
						for (int k=(*ppt).bp; k<bfin; k++) {
							int sx,sy;
							sx=fin[k].c.x-2; sy=fin[k].c.y-2;
							for (int xx=max(0,sx); xx<=min(n-1,sx+4); xx++)
								for (int yy=max(0,sy); yy<=min(m-1,sy+4); yy++) {
									g[xx][yy].rock+=b[fin[k].typ].eff[xx-sx][yy-sy];
								}
						}
						pat.c=(*ppt).c; pat.rec=(*ppt).rec;
						while (!pq.empty()) pq.pop();
						return make_pair(pat.c,pat.rec);
					}
				}
				int vl=0,sf=0;
				for (int i=(*ppt).bp-8; i<(*ppt).bp; i++) {
					vl*=2;
					if (i>=0) vl+=((*ppt).gold[i]+2)/2;
				}
				for (int i=(*ppt).rec.length()-5; i<(*ppt).rec.length(); i++) {
					sf*=2;
					if (i>=0 && (*ppt).rec[i]>='0' && (*ppt).rec[i]<='9') sf++;
				}
				//fprintf(debug,"%d,%d,%d,%d (%d)  %s\n",(*ppt).bp,(*ppt).be,vl,sf,cbest[(*ppt).bp][(*ppt).bp-(*ppt).be][vl][sf],(*ppt).rec.c_str());
				if ((*ppt).gv<=cbest[(*ppt).bp][(*ppt).bp-(*ppt).be][vl][sf]) { pq.pop(); continue; }
				cbest[(*ppt).bp][(*ppt).bp-(*ppt).be][vl][sf]=max((*ppt).gv,cbest[(*ppt).bp][(*ppt).bp-(*ppt).be][vl][sf]);
				int lost=0;
				for (int i=0; i<pcnt; i++)
					if ((*ppt).gold[i]==-1 && (*ppt).be>clos[i]) lost+=og[pick[i].x][pick[i].y].gold;
				if (lost>totg/10) { pq.pop(); continue; }
				if ((*ppt).bp==(*ppt).be && (*ppt).gv>bhigh && lost==0) {
					bhigh=(*ppt).gv;
					bbb=(*ppt).bp;
					bret=(*ppt).rec;
					bat=(*ppt).c;
				}
					
				cnt++;
				if (cnt>lim) { pq.pop(); continue; }
				if (runtime()>8) break;
				fprintf(debug,"pq(%d)= (%d,%d) [%d]: %d/%d  %s\n  ",pq.size(),(*ppt).c.x,(*ppt).c.y,(*ppt).gv,(*ppt).bp,(*ppt).be,(*ppt).rec.c_str());
				for (int i=0; i<pcnt; i++)
					fprintf(debug,"%d",((*ppt).gold[i]+2)/2);
				fprintf(debug,"\n");
				fflush(debug);
				pat=(*ppt);
				cntst++;
				pq.pop();
				goldbfs(pat.be*2);
				if (pat.bp!=bfin || pat.bp>pat.be) bombbfs(pat.be*2);
				
			}
			for (int k=bbb; k<bfin; k++) {
				int sx,sy;
				sx=fin[k].c.x-2; sy=fin[k].c.y-2;
				for (int xx=max(0,sx); xx<=min(n-1,sx+4); xx++)
					for (int yy=max(0,sy); yy<=min(m-1,sy+4); yy++) {
						g[xx][yy].rock+=b[fin[k].typ].eff[xx-sx][yy-sy];
					}
			}
			return make_pair(bat,bret);
			
		}
		pair<Coor,string> moveto (Coor ori,Coor at,Coor tar,int lim,int opt) {
			//Design a bombing scheme
			//  1. Get the target gold
			//  2. Do not damage other gold
			//  Score heuristically
			bestsco=-1000000;
			placebomb(at,tar,lim);
			//fprintf(stderr,"Time: %0.4lf\n",runtime()); fflush(stderr);
			if (backup==-1000000) return make_pair(ori,"");
			fprintf(debug,"best sco = %d\n",bestsco);
			for (int i=0; i<bfin; i++)
				fprintf(debug,"  (%d,%d)-%d\n",fin[i].c.x,fin[i].c.y,fin[i].typ);
			fflush(debug);
			//Design the least number of moves to go to target
			string ret="",nmv;
			pair<Coor,string> pcs;
			if (bfin<=0) return make_pair(ori,"");
			at=fin[0].c;
			ret+=walkmove(ori,at);
			if (opt==1) {
				for (int i=0; i<bfin; i++) {
					pcs=saferoute(at,i);
					at=pcs.first;
					ret+=pcs.second;
					fprintf(debug,"bomb %d(%d,%d) : %d\n",i,fin[i].c.x,fin[i].c.y,mxmv[i]);
				}
			}
			else {
				pcs=greedyman();
				ret+=pcs.second;
			}
			fflush(debug);
			return make_pair(pcs.first,ret);
		}
		
		void trial () {
			for (int i=0; i<dn; i++) {
				b[i].left=oleft[i];
			}
			for (int i=0; i<n; i++)
			    for (int j=0; j<m; j++) {
			        g[i][j].gold=og[i][j].gold;
			        g[i][j].rock=og[i][j].rock;
				}
			move=omove;
			string ret="";
			Coor tc,now;
			tc.x=tc.y=0;
			pair<Coor,string> pcs;
			int cont=1;
			while (ret.length()<=move*3/2 && cont==1) {
				timenow=runtime();
				now=target(tc);
				fprintf(debug,"Trial Best: (%d,%d) from (%d,%d)\n",now.x,now.y,tc.x,tc.y);
				int mindist=10000;
				Coor tat;
				fill(tc);
				for (int i=0; i<n; i++)
					for (int j=0; j<m; j++)
						if (reach[i][j]>=1) {
							if (abs(i-now.x)+abs(j-now.y)<mindist) {
								mindist=abs(i-now.x)+abs(j-now.y);
								tat.x=i; tat.y=j;
							}
						}
				fprintf(debug,"AT %d,%d\n",tat.x,tat.y);
				Route tr; tr.fr=tat; tr.to=now;
				rou.push_back(tr);
				pcs=moveto(tc,tat,now,10,1);
				tc.x=pcs.first.x; tc.y=pcs.first.y;
				ret+=pcs.second;
				cont=0;
				for (int i=0; i<dn; i++) {
					if (b[i].left>0) cont=1;
				}
				if (pcs.second.length()==0) {
					g[now.x][now.y].gold=0;
				}
				
			}
			if (cont==0) bdt=1;
				else bdt=2;
		}
		
		string real () {
			
			for (int i=0; i<dn; i++) {
				b[i].left=oleft[i];
			}
			for (int i=0; i<n; i++)
			    for (int j=0; j<m; j++) {
			        g[i][j].gold=og[i][j].gold;
			        g[i][j].rock=og[i][j].rock;
				}
			move=omove;
			string ret="";
			Coor tc,now;
			tc.x=tc.y=0;
			pair<Coor,string> pcs;
			for (int i=0; i<rou.size(); i++) {
				fprintf(debug,"Route %d: (%d,%d) from (%d,%d)\n",i,rou[i].to.x,rou[i].to.y,rou[i].fr.x,rou[i].fr.y);
			}
			int used[rou.size()],ind,curbest;
			memset(used,0,sizeof(used));
			for (int i=0; i<rou.size(); i++) {
				fill(tc);
				curbest=10000; ind=-1;
				Coor tat,btat;
				for (int j=0; j<rou.size(); j++)
					if (used[j]==0) {
						tat=rou[j].fr;
						int mindist=0;
						if (reach[rou[j].fr.x][rou[j].fr.y]==0) {
							mindist=10000;
							for (int xx=0; xx<n; xx++)
								for (int yy=0; yy<m; yy++)
									if (reach[xx][yy]>=1) {
										if (abs(xx-rou[j].fr.x)+abs(yy-rou[j].fr.y)<mindist) {
											mindist=abs(xx-rou[j].fr.x)+abs(yy-rou[j].fr.y);
											tat.x=xx; tat.y=yy;
										}
									}
						}
						if (mindist<5 && reach[tat.x][tat.y]<curbest) {
							curbest=reach[tat.x][tat.y];
							ind=j; btat=tat;
						}
					}
				if (ind==-1) break;
				tat=btat;
				used[ind]=1;
				timenow=runtime();
				fprintf(debug,"Real Best: [%d] (%d,%d) from (%d,%d) Now (%d,%d)\n",ind,rou[ind].to.x,rou[ind].to.y,tat.x,tat.y,tc.x,tc.y);
				if (reach[rou[ind].to.x][rou[ind].to.y]>=1) continue;
				if (runtime()>8) pcs=moveto(tc,tat,rou[ind].to,10,1);
					else if (omove>7000) pcs=moveto(tc,tat,rou[ind].to,10,bdt);
					else if (omove>5000) pcs=moveto(tc,tat,rou[ind].to,20,bdt);
					else pcs=moveto(tc,tat,rou[ind].to,20,bdt);
				tc.x=pcs.first.x; tc.y=pcs.first.y;
				ret+=pcs.second;
				fprintf(debug,"Rstring %d = %s\n",ret.size(),pcs.second.c_str());
				if (runtime()>9.8) return ret;
				//fprintf(stderr,"Oper = %d  St = %d\n",cntopr,cntst);
			}
			int cont=1;
			while (ret.length()<=move && cont==1) {
				timenow=runtime();
				now=target(tc);
				fprintf(debug,"Real Add Best: (%d,%d) from (%d,%d)\n",now.x,now.y,tc.x,tc.y);
				Coor tat;
				int mindist=10000;
				fill(tc);
				for (int i=0; i<n; i++)
					for (int j=0; j<m; j++)
						if (reach[i][j]>=1) {
							if (abs(i-now.x)+abs(j-now.y)<mindist) {
								mindist=abs(i-now.x)+abs(j-now.y);
								tat.x=i; tat.y=j;
							}
						}
				if (runtime()>8) pcs=moveto(tc,tat,now,20,1);
					else if (omove>7000) pcs=moveto(tc,tat,now,40,bdt);
					else if (omove>5000) pcs=moveto(tc,tat,now,60,bdt);
					else pcs=moveto(tc,tat,now,100,bdt);
				tc.x=pcs.first.x; tc.y=pcs.first.y;
				ret+=pcs.second;
				cont=0;
				for (int i=0; i<dn; i++) {
					if (b[i].left>0) cont=1;
				}
				if (pcs.second.length()==0) {
					g[now.x][now.y].gold=0;
				}
				if (runtime()>9.8) return ret;
			}
			return ret;
		}
		string collectGold(vector <int> dynamite, vector <int> effect, int W, vector <int> Gold, vector <int> rocks, int maxMoves) {
			//initialize
			start();
			dn=dynamite.size();
			for (int i=0; i<dn; i++) {
				b[i].left=dynamite[i];
				oleft[i]=b[i].left;
				for (int j=0; j<5; j++)
					for (int k=0; k<5; k++)
						b[i].eff[j][k]=effect[i*25+j*5+k];
			}
			m=W; n=Gold.size()/m;
			for (int i=0; i<n; i++)
			    for (int j=0; j<m; j++) {
			        og[i][j].gold=Gold[i*m+j];
			        og[i][j].rock=rocks[i*m+j];
			        g[i][j].gold=og[i][j].gold;
			        g[i][j].rock=og[i][j].rock;
				}
			move=maxMoves;
			omove=move;
			fprintf(debug,"MaxMove = %d\n",move);
			//evaluate
			//disbd();
			trial();
			string ret=real();
			if (ret.length()>move) ret.resize(move);
			for (int i=ret.length()-5; i<ret.length(); i++)
				if (ret[i]>='0' && ret[i]<='9') ret[i]='-';
			fprintf(debug,"Final: %s\n",ret.c_str());
			fflush(debug);
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

