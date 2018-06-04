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

int dx[]={1,1,-1,-1},dy[]={1,-1,1,-1};
vector<pair<string,int> > dict;
int n,v[100][100],cntapp;
string ctext;
FILE *debug;

class KnightsMoveCipher {
   public:
      vector<string> getWords() {
         //FILE *WORD; WORD=fopen("words.txt","r");
         FILE *WORD; WORD=fopen("10letternp.txt","r");
         string Tstr; char Tc1[1000],Tc2[10];
         vector<string> Ret;
         while (fscanf(WORD,"%s %s",Tc1,Tc2)==2) {
            Tstr=Tc1; Tstr+=" "; Tstr+=Tc2;
            Ret.push_back(Tstr);
            }
         return Ret;
      }
      void dfs (string word,int pos,int x,int y) {
         if (pos==word.length()) {
            //if (cntapp==0) fprintf(debug,"%s is found\n",word.c_str());
            cntapp++;
            return;
            }
         for (int d=3; d<=4; d++) {
             for (int i=1; i<d; i++)
                 for (int j=0; j<4; j++) {
                     int nx=x+dx[j]*i,ny=y+dy[j]*(d-i);
                     if (nx>=0 && ny>=0 && nx<n && ny<n && v[nx][ny]==0 && ctext[nx*n+ny]==word[pos]) {
                        v[nx][ny]=1;
                        dfs(word,pos+1,nx,ny);
                        v[nx][ny]=0;
                        }
                     }
             }
         }
            
      string decipher(string ciphertext, vector <int> delta) {
         ctext=ciphertext;
         int len=delta.size();
         n=(int)ceil(sqrt(delta.size()+1.0));
         vector<string> tdict=getWords();
         int dictsz=tdict.size();
         cout << tdict.size() << endl;
         for (int i=0; i<dictsz; i++) {
             char tw[1000]; int ti;
             sscanf(tdict[i].c_str(),"%s %d",tw,&ti);
             dict.push_back(make_pair(tw,ti));
             }
         //cout << dict[0].first << " " << dict[0].second << endl;
         memset(v,0,sizeof(v));
         for (int i=0; i<5000; i++) {
             int ok=1,outa=0;
             /*
             for (int j=0; j<dict[i].first.length(); j++)
                 if (dict[i].first[j]=='Q' || dict[i].first[j]=='X' || dict[i].first[j]=='Z') ok=1;
             */
             //if (dict[i].first.length()>=10) ok=1;
             if (ok==1) {
                for (int j=0; j<n; j++)
                    for (int k=0; k<n; k++)
                        if (ctext[j*n+k]==dict[i].first[0]) {
                           cntapp=0;
                           v[j][k]=1;
                           dfs(dict[i].first,1,j,k);
                           v[j][k]=0;
                           if (cntapp!=0 && outa==0) {
                              fprintf(debug,"%s is found %d times\n",dict[i].first.c_str(),cntapp);
                              outa=1;
                              }
                           }
                }
             }
         fprintf(debug,"\n\n\n");
         return ciphertext.substr(0,len+1);
      }
};

int main () {
    KnightsMoveCipher KMC;
    /*
    string CT;
    cin >> CT;
    int LEN;
    cin >> LEN;
    vector<int> DEL(LEN);
    for (int i=0; i<LEN; i++)
        cin >> DEL[i];
    string OUT=KMC.decipher(CT,DEL);
    cout << OUT;
    fflush(stdout);
    */
    int TOT=79627;
    string allstr[80000];
    FILE *alls,*out; alls=fopen("alls.txt","r"); out=fopen("output.txt","w");
    debug=fopen("debug.txt","w");
    for (int i=0; i<TOT; i++) {
        char tstr[5000];
        fscanf(alls,"%s",tstr);
        allstr[i]=tstr;
        //printf("%s\n",allstr[i].c_str());
        }
    fclose(alls);
    srand(1234567891);
    int testno=5,nowtest=0;
    while (nowtest<testno) {
    printf("Seed: %d\n",nowtest);
    fprintf(out,"Seed: %d\n",nowtest);
    string fullsen;
    int senno=rand()%TOT;
    fullsen=allstr[senno];
    //fprintf(out,"%d %s\n",senno,fullsen.c_str());
    int LEN=fullsen.length();
    //LEN=rand()%(LEN-25+1)+25;
    LEN=80;
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
    fprintf(out,"\n");
    fflush(out); fflush(debug);
    nowtest++;
    }
    return 0;
}
