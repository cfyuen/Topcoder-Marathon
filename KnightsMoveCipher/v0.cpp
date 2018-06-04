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

class Statistics {
   public:
      vector<string> getWords() {
         FILE *WORD; WORD=fopen("words.txt","r");
         string Tstr; char Tc1[1000],Tc2[10];
         vector<string> Ret;
         while (fscanf(WORD,"%s %s",Tc1,Tc2)==2) {
            Tstr=Tc1; Tstr+=" "; Tstr+=Tc2;
            Ret.push_back(Tstr);
            }
         return Ret;
      }
};

class KnightsMoveCipher {
   public:
      string decipher(string ciphertext, vector <int> delta) {
         int len=delta.size();
         vector<string> tdict=Statistics::getWords();
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
    freopen("output.txt","w",stdout);
    int TOT=79627;
    string allstr[80000];
    FILE *alls; alls=fopen("alls.txt","r");
    for (int i=0; i<TOT; i++) {
        char tstr[5000];
        fscanf(alls,"%s",tstr);
        allstr[i]=tstr;
        //printf("%s\n",allstr[i].c_str());
        }
    fclose(alls);
    srand(1234567891);
    int testno=20,nowtest=0;
    while (nowtest<testno) {
    printf("Seed: %d\n",nowtest);
    string fullsen;
    int senno=rand()%TOT;
    fullsen=allstr[senno];
    cout << senno << " " << fullsen << endl;
    int LEN=fullsen.length();
    LEN=rand()%(LEN-25+1)+25;
    string AT="";
    for (int i=0; (i<LEN || fullsen[i]!='.') && i<fullsen.length(); i++)
        AT+=fullsen[i];
    cout << "Message Length = " << AT.length() << endl;
    cout << "Message = " << AT << endl;
    int N=(int)ceil(sqrt(AT.length()*1.0)),proceed=0;
    cout << "N = " << N << endl;
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
                              cout << CT[j*N+k];
                          cout << endl;
                          }
                      }
                   }
           }
       } while (proceed==0);
    for (int i=0; i<CT.length(); i++)
        if (CT[i]=='-') CT[i]='.';
    cout << "Cipher = " << CT << endl;
    string OUT=KMC.decipher(CT,DEL);
    cout << "Decoded = " << OUT << endl;
    cout << endl;
    nowtest++;
    }
    return 0;
}
