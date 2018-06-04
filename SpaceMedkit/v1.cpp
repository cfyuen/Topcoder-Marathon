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

/************************approach****************
1.    Parse all resource, event and missions into appropriate format
         when parsing resource, express the mass and volume in terms of cost
         when parsing missions, only consider the mission where there are gain in evacuation
1.1   Set target  
2.    In phrase 1
         calculate the cost of each mission in order to eliminate the evacuation cost
         sort the missions in ascending order of cost
         for each mission from low cost to high cost, use the required resource to eliminate the cost
3.    Parse all the information obtained into ret
*************************************************/

/****Score***
Seed = 1
Total evacuations = 246     Limit = 319
Total mass = 57.3752     Total volume = 167313.7774     Score = 156.5663
Score = 6.3871

Seed = 2
Total evacuations = 63     Limit = 124
Total mass = 62.8727     Total volume = 179620.3249     Score = 163.2792
Score = 6.1245

Seed = 3
Total evacuations = 271     Limit = 300
Total mass = 57.4440     Total volume = 167577.9599     Score = 78.1595
Score = 12.7944

Seed = 4
Total evacuations = 336     Limit = 451
Total mass = 48.9505     Total volume = 133336.6714     Score = 71.7228
Score = 13.9426

Seed = 5
Total evacuations = 54     Limit = 237
Total mass = 62.8274     Total volume = 179601.9749     Score = 162.9841
Score = 6.1356

Seed = 6
Total evacuations = 50     Limit = 249
Total mass = 62.8274     Total volume = 179601.9749     Score = 175.9561
Score = 5.6832

Seed = 7
Total evacuations = 63     Limit = 115
Total mass = 65.1680     Total volume = 185706.5694     Score = 226.2806
Score = 4.4193

Seed = 8
Total evacuations = 53     Limit = 101
Total mass = 64.7852     Total volume = 183749.2739     Score = 115.2765
Score = 8.6748

Seed = 9
Total evacuations = 218     Limit = 424
Total mass = 50.6583     Total volume = 138985.4386     Score = 187.2927
Score = 5.3392

Seed = 10
Total evacuations = 340     Limit = 468
Total mass = 47.5802     Total volume = 133229.9214     Score = 62.0270
Score = 16.1220
*********/

typedef pair<pair<int,int>,int> PII;

vector<string> ret;
int consum[10005],alluntreat,target,noofmis;
double curruse[10005];
double P,C,rcost[10005];
vector<double> bevent[10005],wevent[10005];
vector<pair<double,vector<PII> > > handle;

//delete when submit
FILE *debug,*result;
int local=1,stseed=1,endseed=10;

int sttime;
int tcnt=0;
void start() {
    sttime=clock();
}

double runtime() {
    return (clock()-sttime)/1000.0;
}

// ********************* MERSENNE TWISTER (from ploh) **********************

typedef unsigned uint32;

#define MT_N 624
#define MT_M 397
#define MT_MSB 0x80000000U
#define MT_LS31B 0x7FFFFFFFU
#define MT_A 2567483615U

class MersenneTwister { // pseudo-random number generator
  uint32 twistory[MT_N]; // history (i.e., previous states) of the generator
  int pos;
public:
  double pairGaussian;
  MersenneTwister(uint32 seed = 0) {
    twistory[0] = seed;
    for (int i = 1; i < MT_N; i++)
      twistory[i] = 1812433253U * (twistory[i-1] ^ (twistory[i-1]>>30)) + i;
    pos = 0;
  }
  void generate(void) {
    uint32 tmp; int i;
    for (i = 0; i < MT_N-MT_M; i++) {
      tmp = (twistory[i]&MT_MSB) + (twistory[i+1]&MT_LS31B);
      twistory[i] = twistory[i+MT_M] ^ (tmp>>1) ^ (MT_A & -(tmp&1));
    }
    for (; i < MT_N-1; i++) {
      tmp = (twistory[i]&MT_MSB) + (twistory[i+1]&MT_LS31B);
      twistory[i] = twistory[i+MT_M-MT_N] ^ (tmp>>1) ^ (MT_A & -(tmp&1));
    }
    tmp = (twistory[i]&MT_MSB) + (twistory[0]&MT_LS31B);
    twistory[i] = twistory[MT_M-1] ^ (tmp>>1) ^ (MT_A & -(tmp&1));
  }
  uint32 rand_unsigned(void) {
    if (pos == 0) generate();
    uint32 ans = twistory[pos++];
    pos &= -(pos!=624); // if (pos == 624) pos = 0;
    ans ^= ans>>11;
    ans ^= (ans<<7) & 2636928640U;
    ans ^= (ans<<15) & 4022730752U;
    ans ^= ans>>18;
    return ans;
  }
  int rand_signed(void) {
    return rand_unsigned()>>1;
  }

  int nextInt(int n) {
    return rand_unsigned() % n; // close enough
  }
  double nextDouble(void) {
    return (rand_unsigned()+0.5) / 4294967296.0; // close enough... div slow?
  }
  double nextGaussian(void) { // not optimized; throw away a value
    double x, y, r2;
    do {
      x = -1.0 + 2.0 * nextDouble();
      y = -1.0 + 2.0 * nextDouble();
      r2 = x*x + y*y;
    } while (r2 > 1.0);
    double factor = sqrt(-2.0 * log(r2) / r2);
    pairGaussian = y * factor;
    return x * factor;
  }
};

// ****************** END MERSENNE TWISTER ***************************

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

class SpaceMedkit {
   public:
      void parseResource (vector<string> ar) {
         char par1[1000],par2[1000],par3[1000],par4[1000],tc;
         int rid;
         double m,v;
         for (int i=0; i<ar.size(); i++) {
             sscanf(ar[i].c_str(),"%s%s%s%s",par1,par2,par3,par4);
             sscanf(par1,"%c%d",&tc,&rid);
             sscanf(par2,"%d",&consum[rid]);
             sscanf(par3,"%lf",&m);
             sscanf(par4,"%lf",&v);
             rcost[rid]=m+C*v;
             }
         }
      void parseEvent (vector<string> rr) {
           int ptr=0,prev=-1,rid,mid;
           double tmp[10005];
           char par1[1000],par2[1000],par3[1000],par4[1000],tc;
           for (int i=0; i<rr.size(); i++) {
               sscanf(rr[i].c_str(),"%s%s%s%s",par1,par2,par3,par4);
               sscanf(par1,"%c%c%d",&tc,&tc,&mid);
               sscanf(par2,"%c%d",&tc,&rid);
               //fprintf(debug,"%s %s\n",par1,par2);
               //fprintf(debug,"%d %d\n",Mid,Rid);
               if (mid!=prev && prev!=-1) {
                  bevent[prev].resize(ptr/3*2);
                  wevent[prev].resize(ptr/3*2);
                  for (int j=0; j<ptr; j+=3) {
                      bevent[prev][j/3*2]=tmp[j];
                      wevent[prev][j/3*2]=tmp[j];
                      bevent[prev][j/3*2+1]=tmp[j+1];
                      wevent[prev][j/3*2+1]=tmp[j+2];
                      }
                  ptr=0;
                  /*
                  for (int j=0; j<Best[prev].size(); j++)
                      fprintf(debug,"%d: [%d]%0.4lf\n",prev,j,Best[prev][j]); */
                  //printf("%d\n",prev); fflush(stdout);
                  }
               prev=mid;
               tmp[ptr]=rid; ptr++; 
               sscanf(par3,"%lf",&tmp[ptr]); ptr++;
               sscanf(par4,"%lf",&tmp[ptr]); ptr++;
               }
           bevent[prev].resize(ptr/3*2);
           wevent[prev].resize(ptr/3*2);
           for (int j=0; j<ptr; j+=3) {
               bevent[prev][j/3*2]=tmp[j];
               wevent[prev][j/3*2]=tmp[j];
               bevent[prev][j/3*2+1]=tmp[j+1];
               wevent[prev][j/3*2+1]=tmp[j+2];
               }
           }
      void parseMis (vector<string> mis) {
         int tid,ti,mid,worst,tre,utre,prev=-1,ok=-1,prevat=-1;
         char par1[1000],tc;
         vector<PII> tevent;
         for (int i=0; i<mis.size(); i++) {
             
             sscanf(mis[i].c_str(),"%d%d%s%d%d%d",&tid,&ti,&par1,&worst,&tre,&utre);
             sscanf(par1,"%c%c%d",&tc,&tc,&mid);
             if (prev!=tid) {
                if (ok!=-1) {
                   //fprintf(debug,"%d\n",prev);
                   tevent.resize(ok-prevat);
                   //printf("%d %d\n",ok,prevat);
                   //printf("%d\n",tevent[tevent.size()-1].second);
                   handle.push_back(make_pair(0,tevent));
                   }
                ok=-1;
                tevent.clear();
                prev=tid;
                prevat=i-1;
                }
             alluntreat+=utre;
             if (utre>tre) ok=i;
             tevent.push_back(make_pair(make_pair(mid,worst),utre-tre));
             }
         if (ok!=-1) {
            tevent.resize(ok-prevat);
            handle.push_back(make_pair(0,tevent));
            }
         noofmis=tid;
         }
      
      void phrase1 () {
         vector<double> req;
         for (int i=0; i<handle.size(); i++) {
             double cost=0;
             for (int j=0; j<handle[i].second.size(); j++) {
                 //evaluate cost
                 //
                 //handle[i].second.#
                 //   #first.first = event related
                 //   #first.second = is it worst case
                 //   #second = gain (-1,0,1,2...)
                 int mid=handle[i].second[j].first.first,worst=handle[i].second[j].first.second;
                 if (worst==1) {
                    req=wevent[mid];
                    }
                    else req=bevent[mid];
                 for (int k=0; k<req.size(); k+=2)
                     cost+=rcost[(int)req[k]]*req[k+1];
                 //optimize, only conside key resources
                 //printf("%d: %0.4lf\n",mid,cost); fflush(stdout);
                 }
             handle[i].first=cost;
             }
         printf("**%0.3lf\n",runtime());
         sort(handle.begin(),handle.end());
         printf("**%0.3lf\n",runtime());
         /*
         for (int i=0; i<handle.size(); i++)
             fprintf(debug,"%d: %0.4lf\n",i+1,handle[i].first);     
         */
         int resolved=0;
         vector<int> usedres;
         double need[10005];        
         for (int j=0; j<=10000; j++) need[j]=0;
         for (int i=0; i<handle.size(); i++) {
             //printf("%d/%d: %d  %d\n",i,handle.size(),resolved,usedres.size());
             if (alluntreat+resolved<target) {
                fprintf(debug,"stopat : %d\n",i);
                break;
                }
             for (int j=0; j<usedres.size(); j++) need[usedres[j]]=0;
             usedres.clear();
             for (int j=0; j<handle[i].second.size(); j++) {
                 int mid=handle[i].second[j].first.first,worst=handle[i].second[j].first.second;
                 if (worst==1) {
                    req=wevent[mid];
                    }
                    else req=bevent[mid];
                 for (int k=0; k<req.size(); k+=2) {
                     usedres.push_back((int)req[k]);
                     if (consum[(int)req[k]]==1)
                        need[(int)req[k]]+=req[k+1];
                        else need[(int)req[k]]=max(need[(int)req[k]],req[k+1]);
                     }
                 resolved-=handle[i].second[j].second;
                 }
             for (int j=0; j<usedres.size(); j++) curruse[usedres[j]]=max(curruse[usedres[j]],need[usedres[j]]);
             }
         } 
      vector <string> getMedkit(vector <string> ar, vector <string> rr, vector <string> mis, double _P, double _C) {
         start();
         P=_P; C=_C;
         alluntreat=0; noofmis=0;
         memset(consum,-1,sizeof(consum));
         for (int i=0; i<=10000; i++) {
             curruse[i]=0;
             }
         handle.clear();
         fprintf(debug,"%0.4lf %0.4lf\n",P,C);
         //ret.push_back("R7274 30");
         //fprintf(debug,"%d %d %d %0.4lf %0.4lf\n",ar.size(),rr.size(),mis.size(),P,C);
         //fprintf(debug,"%s\n%s\n%s\n",ar[0].c_str(),rr[0].c_str(),mis[0].c_str());
         parseResource(ar);
         parseEvent(rr);
         parseMis(mis);
         target=(int)(P*noofmis)-100;
         fprintf(debug,"handle size = %d   alluntreat = %d   target = %d\n",handle.size(),alluntreat,target);
         
         printf("%0.3lf\n",runtime());
         
         phrase1();
         printf("%0.3lf\n",runtime());
         /*
         for (int i=0; i<=10000; i++) {
             if (consum[i]==1) {
                string s;
                char str[1000];
                sprintf(str,"R%d %0.4lf\0",i,35.0);
                s=str;
                ret.push_back(s);
                }
             if (consum[i]==0) {
                string s;
                char str[1000];
                sprintf(str,"R%d %d\0",i,1);
                s=str;
                ret.push_back(s);
                }
             }
         */
         for (int i=0; i<=10000; i++) {
             if (consum[i]==1) {
                string s;
                char str[1000];
                sprintf(str,"R%d %lf\0",i,curruse[i]);
                s=str;
                ret.push_back(s);
                }
             if (consum[i]==0) {
                string s;
                char str[1000];
                sprintf(str,"R%d %lf\0",i,curruse[i]);
                s=str;
                ret.push_back(s);
                }
             }
         /*
         for (int i=0; i<ret.size(); i++)
             fprintf(debug,"%s\n",ret[i].c_str());
         */
         
         return ret;
      }
};

int main () {
    SpaceMedkit SMK;
    debug=fopen("debug.txt","w");
    result=fopen("result.txt","w");
    int N;
    vector<string> aR,rR,M;
    double inP,inC;
    //local starts
    if (local==0) {
    scanf("%d\n",&N);
    aR.resize(N); 
    for (int i=0; i<N; i++) {
        char tstr[1000];
        gets(tstr);
        aR[i]=tstr;
        }
    scanf("%d\n",&N);
    rR.resize(N); 
    for (int i=0; i<N; i++) {
        char tstr[1000];
        gets(tstr);
        rR[i]=tstr;
        }
    scanf("%d\n",&N);
    M.resize(N); 
    for (int i=0; i<N; i++) {
        char tstr[1000];
        gets(tstr);
        M[i]=tstr;
        }
    scanf("%lf %lf",&inP,&inC);
    vector<string> RET;
    RET=SMK.getMedkit(aR,rR,M,inP,inC);
    printf("%d\n",RET.size());
    for (int i=0; i<RET.size(); i++)
        cout << RET[i] << endl;
    fflush(stdout);
    }
    else {
    //local tester begins
    FILE *train,*resource,*event;
    double Mass[10005],Vol[10005];
    for (int i=0; i<=10001; i++) {
        Mass[i]=0.0; Vol[i]=0.0;
        }
    int Consum[10005],Rid,Mid;
    vector<double> Best[10005],Worst[10005];
    char tc,str[1000];
    char par1[1000],par2[1000],par3[1000],par4[1000];
    MersenneTwister r;
    train=fopen("train.txt","r");
    event=fopen("events.txt","r");
    resource=fopen("resources.txt","r");
    //parse resource
    printf("parse resource\n");
    while (fgets(str,1000,resource)) {
       aR.push_back(str);
       }
    for (int i=0; i<aR.size(); i++) {
       sscanf(aR[i].c_str(),"%s%s%s%s",par1,par2,par3,par4);
       sscanf(par1,"%c%d",&tc,&Rid);
       sscanf(par2,"%d",&Consum[Rid]);
       sscanf(par3,"%lf",&Mass[Rid]);
       sscanf(par4,"%lf",&Vol[Rid]);
       }
    //parse event
    printf("parse event\n");
    int ptr=0,prev=-1;
    double tmp[10005];
    while (fgets(str,1000,event)) {
       rR.push_back(str);
       }
    for (int i=0; i<rR.size(); i++) {
       sscanf(rR[i].c_str(),"%s%s%s%s",par1,par2,par3,par4);
       sscanf(par1,"%c%c%d",&tc,&tc,&Mid);
       sscanf(par2,"%c%d",&tc,&Rid);
       //fprintf(debug,"%s %s\n",par1,par2);
       //fprintf(debug,"%d %d\n",Mid,Rid);
       if (Mid!=prev && prev!=-1) {
          Best[prev].resize(ptr/3*2);
          Worst[prev].resize(ptr/3*2);
          for (int j=0; j<ptr; j+=3) {
              Best[prev][j/3*2]=tmp[j];
              Worst[prev][j/3*2]=tmp[j];
              Best[prev][j/3*2+1]=tmp[j+1];
              Worst[prev][j/3*2+1]=tmp[j+2];
              }
          ptr=0;
          /*
          for (int j=0; j<Best[prev].size(); j++)
           fprintf(debug,"%d: [%d]%0.4lf\n",prev,j,Best[prev][j]); */
           //printf("%d\n",prev); fflush(stdout);
          
          }
       
       prev=Mid;
       tmp[ptr]=Rid; ptr++; 
       sscanf(par3,"%lf",&tmp[ptr]); ptr++;
       sscanf(par4,"%lf",&tmp[ptr]); ptr++;
       }
    Best[prev].resize(ptr/3*2);
    Worst[prev].resize(ptr/3*2);
    for (int j=0; j<ptr; j+=3) {
        Best[prev][j/3*2]=tmp[j];
        Worst[prev][j/3*2]=tmp[j];
        Best[prev][j/3*2+1]=tmp[j+1];
        Worst[prev][j/3*2+1]=tmp[j+2];
        }
    //read mission
    printf("read mission\n");
    while (fgets(str,1000,train)) {
       M.push_back(str);
       //if (M.size()%1000==0) printf("%d\n",M.size());
       }
    while (stseed<=endseed) {
    //random P,C
    inP=r.nextDouble()*0.04+0.01;
    inC=r.nextDouble()*1e-3;
    //pass to do
    printf("%d Start %0.4lf %0.6lf\n",stseed,inP,inC);
    vector<string> RET;
    RET=SMK.getMedkit(aR,rR,M,inP,inC);
    //evaluate
    printf("Evaluate\n");
    vector<double> mk; mk.resize(10005);
    for (int i=0; i<RET.size(); i++) {
        sscanf(RET[i].c_str(),"%s%s",par1,par2);
        sscanf(par1,"%c%d",&tc,&Rid);
        sscanf(par2,"%lf",&mk[Rid]);
        }
    vector<int> Sample;
    int used[100005]; memset(used,0,sizeof(used));
    for (int i=1; i<=100000; i++) Sample.push_back(i);
    for (int i=0; i<100000; i++) swap(Sample[i],Sample[r.nextInt(100000)]);
    for (int i=0; i<10000; i++) used[Sample[i]]=1;
    int evac=0; prev=-1;
    vector<double> req,has;
    for (int i=0; i<M.size(); i++) {
        int Sam,ti,W,Tre,Utre,supply=1;
        sscanf(M[i].c_str(),"%d%d%s%d%d%d",&Sam,&ti,par1,&W,&Tre,&Utre);
        sscanf(par1,"%c%c%d",&tc,&tc,&Mid);
        if (used[Sam]==1) {
           if (prev!=Sam) {
              prev=Sam;
              has=mk;
              }
           
           if (W==1) req=Worst[Mid];
              else req=Best[Mid];
           for (int j=0; j<req.size(); j+=2) {
               Rid=(int)req[j];
               if (has[Rid]+1e-12<req[j+1]) supply=0;
               }
           if (supply==1) {
              for (int j=0; j<req.size(); j+=2) {
                  Rid=(int)req[j];
                  if (Consum[Rid]==1) has[Rid]-=req[j+1];
                  }
              evac+=Tre;
              }
              else evac+=Utre;
                     
           }
        }
    printf("Finish!!\n");
    fprintf(result,"Seed = %d\n",stseed);
    fprintf(result,"Total evacuations = %d     Limit = %d\n",evac,(int)(inP*10000));
    if (evac<=inP*10000) {
       double score=0,m=0,v=0;
       for (int i=0; i<=10000; i++) {
           m+=Mass[i]*mk[i];
           v+=Vol[i]*mk[i];
           }
       score=inC*v+m;
       fprintf(result,"Total mass = %0.4lf     Total volume = %0.4lf     Score = %0.4lf\n",m,v,score);
       fprintf(result,"Score = %0.4lf\n",1000/score);
       }
       else {
       fprintf(result,"Evacuation exceeds!!\n");
       fprintf(result,"Score = 0\n");
       }  
    fprintf(result,"\n");           
    fflush(result);     
    stseed++;
    }
    }
    return 0;
}