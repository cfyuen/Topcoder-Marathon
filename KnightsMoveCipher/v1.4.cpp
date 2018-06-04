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
using namespace std;

int MaxSearch=10000;
int dx[]={1,1,-1,-1},dy[]={1,-1,1,-1};
vector<pair<string,int> > dict;
int n,v[100][100],cv[100][100],cntapp,maxpos,curpos;
string ctext,recstr;
vector<int> delta;
set<string> vphr;
vector<string> accphr;
FILE *debug;

bool cmp (string a,string b) {
     return a.length()>b.length();
}

class KnightsMoveCipher {
   public:
      vector<string> getWords() {
         //FILE *WORD; WORD=fopen("words.txt","r");
         FILE *WORD; WORD=fopen("allphrw.txt","r");
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
             //fprintf(debug,"Acc: %s\n",word.c_str());
             if (pos>50)
             for (int i=0; i<n; i++) {
                for (int j=0; j<n; j++)
                    if (cv[i][j]==1) fprintf(debug,"#");
                       else fprintf(debug,".");
                fprintf(debug,"\n");
                }
             return 1;
             }
          //fprintf(debug,"%d: %d %d\n",pos,x,y);
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
            memset(cv,0,sizeof(cv));
            for (int j=0; j<n; j++)
                for (int k=0; k<n; k++)
                    if (ctext[j*n+k]==totstr[0]) {
                       cv[j][k]=1;
                       if (dfsacc(totstr,1,j,k,curpos)==1) contacc=1;
                       cv[j][k]=0;
                       if (contacc==1) { j=n; k=n; } //double break
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
                       if (dict[j].first.length()<wl-i+1) continue;
                       int ok=1;
                       for (int k=i+1; k<wl; k++)
                           if (word[k]!=dict[j].first[k-i-1]) {
                              ok=0; break;
                              }
                       //if (i==-1 && word==dict[j].first) ok=0;
                       if (ok==1) {
                          string nword=dict[j].first;
                          /*
                          for (int k=0; k<wl-i-2; k++)
                              nword.erase(nword.begin());
                          */
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
         maxpos=0;
         int len=delta.size();
         n=(int)ceil(sqrt(delta.size()+1.0));
         vector<string> tdict=getWords();
         int dictsz=tdict.size();
         //cout << tdict.size() << endl;
         for (int i=0; i<dictsz; i++) {
             char tw[1000]; int ti;
             sscanf(tdict[i].c_str(),"%s %d",tw,&ti);
             dict.push_back(make_pair(tw,ti));
             }
         //cout << dict[0].first << " " << dict[0].second << endl;
         memset(v,0,sizeof(v));
         memset(cv,0,sizeof(cv));
         curpos=0;
         
             for (int i=0; i<MaxSearch; i++) {
                 int ok=0,outa=0;
                 if (dict[i].first.length()+curpos<=delta.size()+1) ok=1; 
                 if (ok==1) {
                    for (int j=0; j<n; j++)
                        for (int k=0; k<n; k++)
                            if (ctext[j*n+k]==dict[i].first[0]) {
                               cntapp=0;
                               v[j][k]=1;
                               string parastr="";
                               parastr+=dict[i].first[0];
                               dfs(dict[i].first,1,j,k,curpos,parastr);
                               v[j][k]=0;
                               if (cntapp!=0 && outa==0) {
                                  //fprintf(debug,"p:%d %s is found %d times\n",curpos,dict[i].first.c_str(),cntapp);
                                  outa=1;
                                  }
                               }
                    }
                 }
         fprintf(debug,"maxpos: %d\n",maxpos);
         fprintf(debug,"\n\n\n");
         if (!accphr.empty()) sort(accphr.begin(),accphr.end(),cmp);
         string ret="";
         if (accphr.empty()) {
            for (int i=0; i<MaxSearch; i++) {
                ret+=dict[i].first;
                ret+='.';
                if (ret.length()>len+1) {
                   ret=ret.substr(0,len+1); break;
                   }
                }
            }
            else {
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
                ret+=dict[i].first;
                ret+='.';
                if (ret.length()>len+1) {
                   ret=ret.substr(0,len+1); break;
                   }
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
    
    /*
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
    int testno=100,nowtest=0;
    while (nowtest<testno) {
    printf("Seed: %d\n",nowtest);
    fprintf(out,"Seed: %d\n",nowtest);
    fprintf(debug,"Seed: %d\n",nowtest);
    string fullsen;
    int senno=rand()%TOT;
    fullsen=allstr[senno];
    //fprintf(out,"%d %s\n",senno,fullsen.c_str());
    int LEN=fullsen.length();
    LEN=rand()%(LEN-25+1)+25;
    //LEN=100;
    string AT="";
    for (int i=0; (i<LEN || fullsen[i]!='.') && i<fullsen.length(); i++)
        AT+=fullsen[i];
    fprintf(out,"Message Length = %d\n",AT.length());
    fprintf(out,"Message = %s\n",AT.c_str());
    int N=(int)ceil(sqrt(AT.length()*1.0)),proceed=0;
    fprintf(out,"N = %d\n",N);
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
                   if (i==AT.length()-1) {
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
    sum+=mxstr*1.0/AT.length();
    fprintf(out,"\n");
    fflush(out); fflush(debug);
    nowtest++;
    }
    fprintf(out,"\nTotal Score: %0.4lf\n",sum);
    */
    return 0;
}
