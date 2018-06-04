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

vector<string> ret;

//delete when submit
FILE *debug,*result;
int local=1;

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
      vector <string> getMedkit(vector <string> ar, vector <string> rr, vector <string> mis, double P, double C) {
         start();
         ret.push_back("R7274 30");
         
         while (runtime()<2) { }//fprintf(stderr,"hi"); }
         fprintf(stderr,"%d %d %d %0.4lf %0.4lf\n",ar.size(),rr.size(),mis.size(),P,C);
         fprintf(stderr,"%s\n%s\n%s\n",ar[0].c_str(),rr[0].c_str(),mis[0].c_str());
         return ret;
      }
};

int main () {
    SpaceMedkit SMK;
    debug=fopen("debug.txt","w");
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
    int Consum[10005],Rid,Mid;
    vector<double> Best[10005],Worst[10005];
    char tc,str[1000];
    char par1[1000],par2[1000],par3[1000],par4[1000];
    MersenneTwister r;
    train=fopen("train.txt","r");
    event=fopen("events.txt","r");
    resource=fopen("resources.txt","r");
    inP=r.nextDouble()*0.04+0.01;
    inC=r.nextDouble()*1e-3;
    //parse resource
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
           printf("%d\n",prev); fflush(stdout);
          
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
    while (fgets(str,1000,train)) {
       M.push_back(str);
       //if (M.size()%1000==0) printf("%d\n",M.size());
       }
    //pass to do
    vector<string> RET;
    RET=SMK.getMedkit(aR,rR,M,inP,inC);
    //evaluate
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
    fprintf(debug,"Total evacuations = %d\n",evac);
    if (evac<=inP*10000) {
       double score=0,m=0,v=0;
       for (int i=0; i<mk.size(); i++) {
           m+=Mass[i]*mk[i];
           v+=Vol[i]*mk[i];
           }
       score=inC*v+m;
       fprintf(debug,"Total mass = %0.4lf\n",m);
       fprintf(debug,"Total volume = %0.4lf\n",v);
       fprintf(debug,"Score = %0.4lf\n",1000/score);
       }
       else fprintf(debug,"Evacuation exceeds!!\n");
    }
    return 0;
}
