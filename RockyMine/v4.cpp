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
v4 Approach:
	1. Rank the gold to take
	    a. Gold value
	    b. Surrounding gold value
	    c. Estimated round of bomb to get the gold
	    d. Number of steps required to take the gold
	2. Get the gold efficiently
		Now use IDA* approach to locate the bomb
		Reduce to sliding window 20x20
	Extra: use bomb to step ratio
 *********************/
 
#define winsz 21

struct Bomb {
	int left;
	int eff[6][6];
} b[10];

struct Grid {
	int gold,rock;
} g[105][105];

struct Coor {
	int x,y;
	bool operator == (Coor const &a) const{
		return (x==a.x && y==a.y);
	}
};

struct PutBomb {
	int x,y,typ,tl;
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

int n,m,dn,move;
int dx[]={1,0,-1,0,0},dy[]={0,1,0,-1,0};
int reach[105][105];
PutBomb cur[300],fin[300];
int bplace=0,bbsco[300],bfin=0;
double timenow=0;
Coor q[10005];
priority_queue<State> stq[2];
int bestsco=-1000000;

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
			if (nat.x>=0 && nat.y>=0 && nat.x<=winsz && nat.y<=winsz && nat.x+st.c.x-winsz/2>=0 && nat.y+st.c.y-winsz/2>=0 &&  nat.x+st.c.x-winsz/2<n && nat.y+st.c.y-winsz/2<m && reach[nat.x][nat.y]==0 && st.rock[nat.x][nat.y]<=0) {
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
			disbd();
			return bc;
		}
//		int scorepb (int tgc,int tgd,int numb) {
//			return tgc*4-tgd-numb*max(40,max(n,m));
//		}
		int scorest (const State &st) {
			return st.tgc*8-st.tgd-st.pb.size()*max(40,max(n,m));
		}
		void placebomb (Coor fr,Coor tar) {
			//improvement: note that new bomb must be placed near the old bomb
			//				only store free space near the old bomb
			//				consider new bomb there
			//				does not need the whole coor grid, only need the bomb placed area
			State at;
			const State *ptr;
			int limit=100,cntlim=0,step=0;
			if (move>5000) limit=75;
			if (move>7000) limit=60;
			if (runtime()>9) limit=40;
			int swch=0,worst=-100000;
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
										nb.x=i; nb.y=j; nb.typ=k;
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
		
		pair<Coor,string> saferoute (Coor at,int bbid) {
			fprintf(debug,"(%d,%d) -> (%d,%d)\n",at.x,at.y,fin[bbid].x,fin[bbid].y); fflush(debug);
			int q1,q2,v[105][105];
			memset(v,0,sizeof(v));
			Coor nat;
			string rec[10005];
			q1=0; q2=0;
			q[0].x=at.x; q[0].y=at.y;
			rec[0]="";
			v[at.x][at.y]=1;
			//go to place the bomb
			while (q1<=q2) {
				//fprintf(debug,"at %d,%d %s\n",q[q1].x,q[q1].y,rec[q1].c_str()); fflush(debug);
				q1++;
				if (q[q1-1].x==fin[bbid].x && q[q1-1].y==fin[bbid].y) break;
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
			string ret=rec[q1-1];
			fprintf(debug,"goto place done\n"); fflush(debug);
			//place the bomb
			ret+=fin[bbid].typ+'0';
			ret+="-----";
			int sx,sy;
			sx=fin[bbid].x-2; sy=fin[bbid].y-2;
			for (int xx=max(0,sx); xx<=min(n-1,sx+4); xx++)
				for (int yy=max(0,sy); yy<=min(m-1,sy+4); yy++) {
					g[xx][yy].rock-=b[fin[bbid].typ].eff[xx-sx][yy-sy];
				}
			b[fin[bbid].typ].left--;
			fprintf(debug,"place bomb done\n"); fflush(debug);
			//escape from the bomb
			Coor pick[30]; int pcnt=0;
			Coor bf; bf.x=fin[bbid].x; bf.y=fin[bbid].y;
			pick[pcnt]=bf; pcnt++;
			fill(bf);
            for (int i=max(0,bf.x-4); i<=min(n-1,bf.x+4); i++)
                for (int j=max(0,bf.y-4); j<=min(m-1,bf.y+4); j++)
                    if (reach[i][j]<=10 && reach[i][j]>=1 && g[i][j].rock==0 && g[i][j].gold>0) {
						pick[pcnt].x=i; pick[pcnt].y=j;
						pcnt++;
					}
			if (bbid!=bfin-1) { pick[pcnt].x=fin[bbid+1].x; pick[pcnt].y=fin[bbid+1].y; pcnt++; }
			string pd[30][30];
			vector<int> perv;
			for (int i=0; i<pcnt; i++)
			    for (int j=0; j<pcnt; j++) {
					q1=0; q2=0;
					q[0]=pick[i];
					memset(v,0,sizeof(v));
					v[q[0].x][q[0].y]=1;
					rec[0]="";
					while (q1<=q2) {
						q1++;
						if (q[q1-1]==pick[j]) break;
						for (int k=0; k<4; k++) {
							nat.x=q[q1-1].x+dx[k]; nat.y=q[q1-1].y+dy[k];
                            if (nat.x>=0 && nat.y>=0 && nat.x<n && nat.y<m && v[nat.x][nat.y]==0 && g[nat.x][nat.y].rock<=0) {
                                v[nat.x][nat.y]=1;
								q2++;
								rec[q2]=rec[q1-1]+dir(k);
								q[q2].x=nat.x; q[q2].y=nat.y;
							}
						}
					}
					pd[i][j]=rec[q1-1];
					//fprintf(debug,"(%d,%d) -> (%d,%d) : %s\n",pick[i].x,pick[i].y,pick[j].x,pick[j].y,pd[i][j].c_str());
				}
			for (int i=0; i<pcnt; i++) {
				perv.push_back(i);
				fprintf(debug,"Pick (%d,%d)[%d]\n",pick[i].x,pick[i].y,g[pick[i].x][pick[i].y].gold);
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
						for (int i=0; i<pcnt; i++)
						    fprintf(debug,"-> (%d,%d) ",pick[perv[i]].x,pick[perv[i]].y);
						fprintf(debug,"\n%d: %s\n",bpstep,bpick.c_str()); fflush(debug);
					}
				} while (next_permutation(perv.begin()+1,perv.end()-1));
				for (int i=0; i<pcnt; i++)
					g[pick[i].x][pick[i].y].gold=0;
				ret+=bpick;
			}
			fprintf(debug,"escape done\n"); fflush(debug);
			return make_pair(endc,ret);
		}
		
		pair<Coor,string> moveto (Coor at,Coor tar) {
			//Design a bombing scheme
			//  1. Get the target gold
			//  2. Do not damage other gold
			//  Score heuristically
			bestsco=-1000000;
			int mindist=10000;
			Coor tat;
			fill(at);
			for (int i=0; i<n; i++)
				for (int j=0; j<m; j++)
					if (reach[i][j]>=1) {
						if (abs(i-tar.x)+abs(j-tar.y)<mindist) {
							mindist=abs(i-tar.x)+abs(j-tar.y);
							tat.x=i; tat.y=j;
						}
					}
			fprintf(debug,"AT %d,%d\n",tat.x,tat.y);
			placebomb(tat,tar);
			//fprintf(stderr,"Time: %0.4lf\n",runtime()); fflush(stderr);
			if (bestsco==-1000000) return make_pair(at,"");
			fprintf(debug,"best sco = %d\n",bestsco);
			for (int i=0; i<bfin; i++)
				fprintf(debug,"  (%d,%d)-%d\n",fin[i].x,fin[i].y,fin[i].typ);
			fflush(debug);
			//Design the least number of moves to go to target
			string ret="",nmv;
			pair<Coor,string> pcs;
			for (int i=0; i<bfin; i++) {
				pcs=saferoute(at,i);
				at.x=pcs.first.x; at.y=pcs.first.y;
				nmv=pcs.second;
				ret+=nmv;
				//disbd();
				fprintf(debug,"bomb %d(%d,%d) : %s\n",i,fin[i].x,fin[i].y,nmv.c_str());
			}
			fflush(debug);
			return make_pair(at,ret);

		}
		string collectGold(vector <int> dynamite, vector <int> effect, int W, vector <int> Gold, vector <int> rocks, int maxMoves) {
			//initialize
			start();
			dn=dynamite.size();
			for (int i=0; i<dn; i++) {
				b[i].left=dynamite[i];
				for (int j=0; j<5; j++)
					for (int k=0; k<5; k++)
						b[i].eff[j][k]=effect[i*25+j*5+k];
			}
			m=W; n=Gold.size()/m;
			for (int i=0; i<n; i++)
			    for (int j=0; j<m; j++) {
			        g[i][j].gold=Gold[i*m+j];
			        g[i][j].rock=rocks[i*m+j];
				}
			move=maxMoves;
			fprintf(debug,"MaxMove = %d\n",move);
			Coor tc,now;
			tc.x=tc.y=0;
			fill(tc);
			//evaluate
			disbd();
			string ret;
			pair<Coor,string> pcs;
			while (ret.length()<=move) {
				timenow=runtime();
				now=target(tc);
				fprintf(debug,"Best: (%d,%d) from (%d,%d)\n",now.x,now.y,tc.x,tc.y);
				pcs=moveto(tc,now);
				tc.x=pcs.first.x; tc.y=pcs.first.y;
				ret+=pcs.second;
				if (pcs.second.length()==0) break;
			}
			if (ret.length()>move) ret.resize(move);
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

