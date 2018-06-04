#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<cmath>
#include<set>
#include <sys/time.h> 
using namespace std;

int MaxSearch=20000;
//insert longphr when submit
string longphr="";
string stdphr[25000];
int dx[]={1,1,-1,-1},dy[]={1,-1,1,-1};
vector<string> dict;
int n,v[100][100],cv[100][100],cntapp,maxpos,curpos,lx,ly;
string ctext,recstr;
vector<int> delta;
set<string> vphr,cand;
vector<string> accphr;

//delete when submit
FILE *debug;

int sttime;
void start() {
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

bool cmp (string a,string b) {
     return a.length()>b.length();
}

class KnightsMoveCipher {
   public:
      vector<string> getWords() {
         FILE *WORD; WORD=fopen("words.txt","r");
         //FILE *WORD; WORD=fopen("allphrw.txt","r");
         string Tstr; char Tc1[1000],Tc2[10];
         vector<string> Ret;
         while (fscanf(WORD,"%s %s",Tc1,Tc2)==2) {
            Tstr=Tc1; Tstr+=" "; Tstr+=Tc2;
            Ret.push_back(Tstr);
            }
         return Ret;
      }
      int dfsacc (string word,int pos,int x,int y,int ind) {
          if (pos==word.length()) {
             fprintf(debug,"Acc: %s\n",word.c_str());
             //if (pos>50)
             for (int i=0; i<n; i++) {
                for (int j=0; j<n; j++)
                    if (cv[i][j]==1) fprintf(debug,"#");
                       else fprintf(debug,".");
                fprintf(debug,"\n");
                }
             lx=x; ly=y;
             return 1;
             }
          int d=delta[ind+pos-1],ret=0;
          for (int i=1; i<d; i++)
              for (int j=0; j<4; j++) {
                  int nx=x+dx[j]*i,ny=y+dy[j]*(d-i);
                  if (nx>=0 && ny>=0 && nx<n && ny<n && cv[nx][ny]==0 && ctext[nx*n+ny]==word[pos]) {
                     cv[nx][ny]=1;
                     if (ret==1) return 1;
                     ret=dfsacc(word,pos+1,nx,ny,ind);
                     if (ret==1) return 1;
                     cv[nx][ny]=0;
                     }
                  }
          return ret;
          }
      void dfs (string word,int pos,int x,int y,int ind,string totstr) {
         if (pos==word.length()) {
            if (vphr.find(totstr)!=vphr.end()) return;
               else vphr.insert(totstr);
            int contacc=0;
            //fprintf(debug,"%s\n",totstr.c_str());
            //printf("%s\n",totstr.c_str());
            memset(cv,0,sizeof(cv));
            for (int j=0; j<n; j++)
                for (int k=0; k<n; k++)
                    if (ctext[j*n+k]==totstr[0]) {
                       cv[j][k]=1;
                       if (dfsacc(totstr,1,j,k,curpos)==1) contacc=1;
                       /*
                       if (contacc==1) 
                          for (int x=0; x<n; x++) {
                           for (int y=0; y<n; y++)
                               if (cv[x][y]==1) fprintf(debug,"#");
                                  else fprintf(debug,".");
                           fprintf(debug,"\n");
                           }
                       */
                       if (contacc==1) { j=n; k=n; } //double break
                          else cv[j][k]=0;
                       }
            if (contacc==0) return;
             else accphr.push_back(totstr);
            fprintf(debug,"%s is found at pos: %d\n",totstr.c_str(),ind+pos);
            cntapp++;
            if (ind+pos>maxpos) {
               //record the record made
               maxpos=ind+pos;
               }
            int wl=word.length();
            /*
            for (int i=0; i<n; i++) {
                for (int j=0; j<n; j++)
                    if (v[i][j]==1) fprintf(debug,"#");
                       else fprintf(debug,".");
                fprintf(debug,"\n");
                }
            */
            memset(v,0,sizeof(v)); v[x][y]=1;
            for (int i=-1; i<wl; i++)
                if (i==-1 || word[i]=='.') {
                   for (int j=0; j<MaxSearch; j++) {
                       if (stdphr[j].length()<wl-i+1) continue;
                       int ok=1;
                       for (int k=i+1; k<wl; k++)
                           if (word[k]!=stdphr[j][k-i-1]) {
                              ok=0; break;
                              }
                       if (ok==1) {
                          string nword=stdphr[j];
                          //fprintf(debug,"dfs: %s pos: %d  x,y=%d,%d  ind: %d\n",nword.c_str(),wl-i-2,x,y,ind+word.length()-(wl-i-1));
                          dfs(nword,wl-i-1,x,y,ind+word.length()-(wl-i-1),totstr);
                          }
                       }
                   }
            return;
            }
         int d=delta[ind+pos-1];
         for (int i=1; i<d; i++)
             for (int j=0; j<4; j++) {
                 int nx=x+dx[j]*i,ny=y+dy[j]*(d-i);
                 if (nx>=0 && ny>=0 && nx<n && ny<n && v[nx][ny]==0 && ctext[nx*n+ny]==word[pos]) {
                    v[nx][ny]=1;
                    
                    dfs(word,pos+1,nx,ny,ind,totstr+word[pos]);
                    if (v[nx][ny]==0) return;
                       v[nx][ny]=0;
                    }
                 }
         }
            
      string decipher(string ciphertext, vector <int> Delta) {
         ctext=ciphertext;
         delta=Delta;
         accphr.clear();
         accphr.push_back("");
         vphr.clear();
         vphr.insert("");
         start();
         int phrcnt=0,phrpos=0;
         while (phrcnt<MaxSearch) {
            string tstr="";
            while (longphr[phrpos]!='#') {
               tstr+=longphr[phrpos];
               phrpos++;
               }
            phrpos++;
            stdphr[phrcnt]=tstr;
            phrcnt++;
            }
         printf("%0.4lf\n",runtime());
         maxpos=0;
         int len=delta.size();
         n=(int)ceil(sqrt(delta.size()+1.0));
         vector<string> tdict=getWords();
         int dictsz=tdict.size();
         cout << tdict.size() << endl;
         for (int i=0; i<dictsz; i++) {
             char tw[1000]; int ti;
             sscanf(tdict[i].c_str(),"%s %d",tw,&ti);
             dict.push_back(tw);
             }
         //cout << dict[0].first << " " << dict[0].second << endl;
         memset(v,0,sizeof(v));
         memset(cv,0,sizeof(cv));
         curpos=0;
         
             for (int i=0; i<MaxSearch; i++) {
                 int ok=0,outa=0;
                 if (stdphr[i].length()+curpos<=delta.size()+1) ok=1; 
                 if (ok==1) {
                    for (int j=0; j<n; j++)
                        for (int k=0; k<n; k++)
                            if (ctext[j*n+k]==stdphr[i][0]) {
                               cntapp=0;
                               v[j][k]=1;
                               string parastr="";
                               parastr+=stdphr[i][0];
                               dfs(stdphr[i],1,j,k,curpos,parastr);
                               v[j][k]=0;
                               if (cntapp!=0 && outa==0) {
                                  //fprintf(debug,"p:%d %s is found %d times\n",curpos,dict[i].first.c_str(),cntapp);
                                  outa=1;
                                  }
                               }
                    }
                 }
         
         for (int i=0; i<accphr.size(); i++) {
             string minus=accphr[i];
             int pos=minus.length()-1;
             while (pos>=0 && minus[pos]!='.') { minus.erase(minus.end()-1); pos--; }
             if (minus.length()>0) minus.erase(minus.end()-1);
             if (vphr.find(minus)==vphr.end() && minus.length()>5) {
                vphr.insert(minus);
                accphr.push_back(minus);
                }
                else continue;
             }
         sort(accphr.begin(),accphr.end(),cmp);
         cand.clear();
         for (int i=0; i<accphr.size(); i++)
             cand.insert(accphr[i]);
         int canmax=accphr[0].length();
         
         if (len<2000) {      
         while (accphr[0].length()<len) {
         vector<string> append=accphr;
         set<string> newphr;
         for (int j=0; j<min((int)append.size(),5); j++)
             printf("%d: %s\n",j,append[j].c_str());
         printf("\n");
         for (int i=0; i<min((int)append.size(),5); i++) {
             int verdot=0;
             for (int j=append[i].length()-1; j>=0; j--) {
                 if (append[i][j]=='.') verdot++;
                 if (j==(int)append[i].length()-8) break;
                 }
             if (verdot>=3) continue;
             memset(cv,0,sizeof(cv));
             memset(v,0,sizeof(v));
             vphr.clear();
             accphr.clear();
             fprintf(debug,"%s\n",append[i].c_str());
             //printf("%s\n",append[i].c_str());
             
             /*
             for (int j=0; j<n; j++)
                 for (int k=0; k<n; k++)
                     if (ctext[j*n+k]==append[i][0]) {
                        
                        v[j][k]=1;
                        int done=0;
                        done=dfsacc(append[i],1,j,k,0);
                        if (done==1) { j=n; k=n; }
                           else { v[j][k]=0; }
                        }
             for (int x=0; x<n; x++) {
                 for (int y=0; y<n; y++)
                     if (cv[x][y]==1) fprintf(debug,"#");
                        else fprintf(debug,".");
                     fprintf(debug,"\n");
                 }
             */
             //curpos=append[i].length();
             for (int j=0; j<7000; j++) {
                 if (curpos+dict[j].length()<=delta.size()+1 && (dict[j].length()!=1 || j<100) && (dict[j].length()!=2 || j<800) && (dict[j].length()!=3 || j<2000)) {
                    for (int k=0; k<n; k++)
                        for (int m=0; m<n; m++)
                            if ((append[i].length()>0 && ctext[k*n+m]=='.') || (append[i].length()==0 && ctext[k*n+m]==dict[j][0])) {
                               //fprintf(debug,"try: %s\n",dict[j].c_str());
                               v[k][m]=1;
                               cntapp=0;
                               string parastr=append[i];
                               if (append[i].length()!=0) parastr+=".";
                                  else parastr+=dict[j][0];
                               if (append[i].length()>0) dfs("."+dict[j],1,k,m,append[i].length(),parastr);
                                  else dfs(dict[j],1,k,m,append[i].length(),parastr);
                               if (cntapp>0) {
                                  fprintf(debug,"%s BACK\n",dict[j].c_str());
                                  }
                               v[k][m]=0;
                               }
                    }
                 }
             for (int j=0; j<accphr.size(); j++) {
                 newphr.insert(accphr[j]);
                 }
             if (runtime()>19) break;
             }
         
         for (int i=5; i<append.size(); i++)
             newphr.insert(append[i]);
         for (int i=0; i<min((int)append.size(),5); i++)
             if (newphr.find(append[i])!=newphr.end()) newphr.erase(append[i]);
         
         accphr.clear();
         for (set<string>::iterator it=newphr.begin(); it!=newphr.end(); it++) {
             accphr.push_back((*it));
             if ((int)(*it).length()>canmax-5) {
                cand.insert((*it));
                canmax=max(canmax,(int)(*it).length());
                }
             }
         sort(accphr.begin(),accphr.end(),cmp);
         if (runtime()>19) break;
         }
         }

         fprintf(debug,"maxpos: %d\n",maxpos);
         fprintf(debug,"\n\n\n");
         
         accphr.clear();
         for (set<string>::iterator it=cand.begin(); it!=cand.end(); it++)
             accphr.push_back((*it));
         sort(accphr.begin(),accphr.end(),cmp);
         string ret="";
         ret=accphr[0];
         for (int i=1; i<accphr.size(); i++) {
             int j;
             for (j=0; j<i; j++) {
                 int k;
                 for (k=0; k<min(accphr[i].length(),accphr[j].length()); k++)
                     if (accphr[i][k]!=accphr[j][k]) break;
                 if (k>8) break;
                 }
             if (j==i) {
                ret+='.';
                ret+=accphr[i];
                }
             if (ret.length()>len+1) {
                ret=ret.substr(0,len+1); break;
                }
             }
         for (int i=0; i<MaxSearch; i++) {
             ret+=stdphr[i];
             ret+='.';
             if (ret.length()>len+1) {
                ret=ret.substr(0,len+1); break;
                }
             }
         fprintf(debug,"OUT = %s\n",ret.c_str());
         return ret;
      }
};

int LCS[2500][2500];

int main () {
    KnightsMoveCipher KMC;
    debug=fopen("debug.txt","w");
    
    FILE *getphr; getphr=fopen("allphrmod.txt","r");
    for (int i=0; i<MaxSearch; i++) {
        char tc[1000]; int ti;
        fscanf(getphr,"%s %d",tc,&ti);
        //stdphr[i]=tc;
        longphr+=tc;
        longphr+="#";
        }
    
    /*
    string CT;
    cin >> CT;
    int LEN;
    cin >> LEN;
    vector<int> DEL(LEN);
    for (int i=0; i<LEN; i++)
        cin >> DEL[i];
    string OUT=KMC.decipher(CT,DEL);
    fflush(debug);
    cout << OUT;
    fflush(stdout);
    */
    
    int TOT=79627;
    double sum=0;
    string allstr[80000];
    FILE *alls,*out; alls=fopen("alls.txt","r"); out=fopen("output.txt","w");
    for (int i=0; i<TOT; i++) {
        char tstr[5000];
        fscanf(alls,"%s",tstr);
        allstr[i]=tstr;
        //printf("%s\n",allstr[i].c_str());
        }
    fclose(alls);
    srand(1234567891);
    int testno=200,sttest=100,nowtest=0;
    while (nowtest<testno) {
    printf("Seed: %d\n",nowtest);
    if (nowtest>=sttest) {
       fprintf(out,"Seed: %d\n",nowtest);
       fprintf(debug,"Seed: %d\n",nowtest);
       }
    string fullsen;
    int senno=rand()%TOT;
    fullsen=allstr[senno];
    //fprintf(out,"%d %s\n",senno,fullsen.c_str());
    int LEN=fullsen.length();
    LEN=rand()%(LEN-25+1)+25;
    //LEN=180;
    string AT="";
    for (int i=0; (i<LEN || fullsen[i]!='.') && i<fullsen.length(); i++)
        AT+=fullsen[i];
    int N=(int)ceil(sqrt(AT.length()*1.0)),proceed=0;
    if (nowtest>=sttest) {
       fprintf(out,"Message Length = %d\n",AT.length());
       fprintf(out,"Message = %s\n",AT.c_str());
       fprintf(out,"N = %d\n",N);
       }
    string CT="";
    vector<int> DEL;
    do {
       CT=""; DEL.clear();
       proceed=1;
       for (int i=0; i<N*N; i++)
           CT+='-';
       int Pos=rand()%(N*N),Row=Pos%N,Col=Pos/N;
       
       CT[Pos]=AT[0];
       //cout << Col << " " << Row << endl;
       for (int i=1; i<AT.length(); i++) {
           int maxd=max(Row,N-1-Row)+max(Col,N-1-Col),dis;
           vector<pair<int,int> > Vp;
           for (dis=3; dis<=maxd; dis++) {
               int Rcon=rand()%10;
               if (Rcon<2) { continue; }
               for (int k=1; k<dis; k++)
                   for (int dv=0; dv<4; dv++) {
                       int nR=Row+dx[dv]*k,nC=Col+dy[dv]*(dis-k);
                       //cout << nR << " " << nC << endl;
                       if (nR>=0 && nC>=0 && nR<N && nC<N && CT[nC*N+nR]=='-')
                          Vp.push_back(make_pair(nR,nC));
                       }
               if (!Vp.empty()) {
                  DEL.push_back(dis);
                  break;
                  }
               
               }
           if (dis==maxd+1) { proceed=0; break; }
              else {
                   int Rpos=rand()%(Vp.size());
                   Row=Vp[Rpos].first; Col=Vp[Rpos].second;
                   CT[Col*N+Row]=AT[i];
                   //cout << DEL[i-1] << "  " << Col << " " << Row << endl;
                   if (i==AT.length()-1 && nowtest>=sttest) {
                      for (int j=0; j<N; j++) {
                          for (int k=0; k<N; k++)
                              fprintf(out,"%c",CT[j*N+k]);
                          fprintf(out,"\n");
                          }
                      }
                   }
           }
       } while (proceed==0);
    for (int i=0; i<CT.length(); i++)
        if (CT[i]=='-') CT[i]='.';
    if (nowtest>=sttest) {
       fprintf(out,"Cipher = %s\n",CT.c_str());
       fprintf(out,"Delta = (%d",DEL[0]);
       for (int i=1; i<DEL.size(); i++)
           fprintf(out,",%d",DEL[i]);
       fprintf(out,")\n");
       string OUT=KMC.decipher(CT,DEL);
       fprintf(out,"Decoded = %s\n",OUT.c_str());
       int mxstr=0;
       for (int i=0; i<AT.length()+1; i++) {
           LCS[0][i]=0; LCS[i][0]=0;
           }
       for (int i=1; i<AT.length()+1; i++)
           for (int j=1; j<AT.length()+1; j++)
               if (AT[i-1]==OUT[j-1]) {
                  LCS[i][j]=LCS[i-1][j-1]+1;
                  if (LCS[i][j]>mxstr) {
                     mxstr=LCS[i][j];
                     //fprintf(out,"%d: %d %d\n",mxstr,i,j);
                     }
                  }
                  else LCS[i][j]=0;
       if (OUT.length()!=AT.length()) mxstr=0;
       fprintf(out,"Score = %0.4lf         Max Length: %d\n",mxstr*1.0/AT.length(),mxstr);
       fprintf(out,"Runtime = %0.3lf\n",runtime());
       sum+=mxstr*1.0/AT.length();
       fprintf(out,"\n");
       }
    fflush(out); fflush(debug);
    nowtest++;
    }
    fprintf(out,"\nTotal Score: %0.4lf\n",sum);
    
    return 0;
}
