#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
using namespace std;

class Klondike {
   public:
      string move(vector <string> stack, vector <string> deck, vector <int> complete, int pos) {
         return "ADVANCE";
         }
};

char suit[]={'C','D','H','S'};
char rank[]={'A','2','3','4','5','6','7','8','9','T','J','Q','K'};

void Vis(vector <string> Vs, vector <string> Vd, vector <int> Vc, int Vp) {
     system("CLS");
     
     printf("DECK:\n");
     for (int i=0; i<Vd.size(); i++) cout << Vd[i] << " ";
     printf("\n");
     for (int i=0; i<Vp*3; i++) printf(" ");
     printf("/\\\n");
     printf("Completed: ");
     for (int i=0; i<4; i++) printf("%d ",Vc[i]);
     printf("\nStack:\n");
     vector<int> cnt; int ma=0;
     for (int i=0; i<7; i++) {
         cnt.push_back(Vs[i][0]-'0'+(Vs[i].length()-1)/3);
         if (cnt[i]>ma) ma=cnt[i];
         }
     
     for (int i=0; i<ma; i++) {
         for (int j=0; j<7; j++) {
             if (i==0 && cnt[j]==0) { printf(" -- "); continue; }
             if (cnt[j]<=i) { printf("    "); continue; }
             if (i<Vs[j][0]-'0') { printf(" ## "); continue; }
             printf(" %c%c ",Vs[j][(i-Vs[j][0]+'0')*3+2],Vs[j][(i-Vs[j][0]+'0')*3+3]);
             }
         printf("\n");
         }
     
     //system("PAUSE");
}

int c2n (string C) {
    int ret=0;
    for (int i=0; i<13; i++)
        if (C[0]==rank[i]) { ret+=i; break; }
    for (int i=0; i<4; i++)
        if (C[1]==suit[i]) { ret+=i*13; break; }
    return ret;
}

string n2c (int N) {
    string ret="";
    ret+=rank[N%13];
    ret+=suit[N/13];
    return ret;
}

int suitcheck (int s,int d) {
    if ((s/13)%3!=(d/13)%3) return 1;
    return 0;
}

void illegal () {
   //system("CLS");
   printf("ILLEGAL");
   system("PAUSE");
}

int main() {
    Klondike kl;
    
    vector<int> vc;
    for (int i=0; i<52; i++) vc.push_back(i);
    random_shuffle(vc.begin(),vc.end());
    //for (int i=0; i<52; i++) printf("%d ",vc[i]);
    //printf("\n");
    //system("PAUSE");
    vector<string> Rs,Rd;
    vector<int> Rc;
    int Rp=2;
    int tc=0,tm=0;
    while (tm<52) {
       string ts=""; ts+=tc+'0';
       ts+=" "+n2c(vc[tm]);
       Rs.push_back(ts);
       tc++;
       tm+=tc;
       }
    for (int i=28; i<52; i++) Rd.push_back(n2c(vc[i]));
    for (int i=0; i<4; i++) Rc.push_back(0);
    while (true) {
       Vis(Rs,Rd,Rc,Rp);
       string cmd;
       cmd=kl.move(Rs,Rd,Rc,Rp);
       //getline(cin,cmd);
       if (cmd=="DONE") break;
       if (cmd=="ADVANCE") {
          if (Rp==Rd.size()-1) {
             if (Rd.size()>=3) Rp=2;
                else Rp=Rd.size()-1;
             }
             else {
                  Rp+=3;
                  if (Rp>Rd.size()-1) Rp=Rd.size()-1;
                  }
          continue;
          }
       int ns=cmd[1]-'1',nd=cmd[4]-'1';
       if (cmd.length()==7) {
          nd=cmd[6]-'1';
          if (cmd[5]=='S') {
             int l=Rs[nd].length(),sci=c2n(Rd[Rp]);
             if (l==1) {
                if (sci%13==12) {
                   Rd.erase(Rd.begin()+Rp);
                   Rs[nd].append(" ");
                   Rs[nd].append(n2c(sci));
                   if (Rp!=0) Rp--;
                   continue;
                   }
                   else { illegal(); break; }
                }
             string dcs=""; dcs+=Rs[nd][l-2]; dcs+=Rs[nd][l-1];
             int dci=c2n(dcs);
             if (suitcheck(dci,sci) && sci%13==dci%13-1) {
                Rd.erase(Rd.begin()+Rp);
                Rs[nd].append(" ");
                Rs[nd].append(n2c(sci));
                if (Rp!=0) Rp--;
                }
                else { illegal(); break; }
             }
             else {
             int sci=c2n(Rd[Rp]);
             if (sci/13==nd && sci%13==Rc[nd]) {
                Rc[nd]++;
                Rd.erase(Rd.begin()+Rp);
                if (Rc[0]+Rc[1]+Rc[2]+Rc[3]==52) break;
                if (Rp!=0) Rp--;
                }
                else { illegal(); break; }
             }
          continue;
          }
       
       if (cmd[0]=='S' && cmd[3]=='C') {
          int l=Rs[ns].length();
          if (l==1) { illegal(); break; }
          string scs=Rs[ns][l-2]+""+Rs[ns][l-1];
          int sci=c2n(scs);
          if (sci/13==nd && sci%13==Rc[nd]) {
             Rc[nd]++;
             Rs[nd].erase(Rs[nd].end()-3,Rs[nd].end());
             if (Rc[0]+Rc[1]+Rc[2]+Rc[3]==52) break;
             if (Rs[ns].length()==1 && Rs[ns][0]!='0') {
                Rs[ns].append(" ");
                int nci=vc[(ns*(ns+1))/2+ns-Rs[ns][0]+'0'+1];
                Rs[ns].append(n2c(nci));
                }
             }
             else { illegal(); break; }
          continue;
          }
       if (cmd[0]=='C' && cmd[3]=='S') {
          int l=Rs[nd].length();
          string dcs=Rs[nd][l-2]+""+Rs[nd][l-1];
          int dci=c2n(dcs),sci=ns*13+Rc[ns]-1;
          if (l==1) {
             if (sci%13==12) {
                Rc[ns]--;
                Rs[nd].append(" ");
                Rs[nd].append(n2c(sci));
                continue;
                }
                else { illegal(); break; }
             }
          if (suitcheck(dci,sci) && sci%13==dci%13-1) {
             Rc[ns]--;
             Rs[nd].append(" ");
             Rs[nd].append(n2c(sci));
             }
             else { illegal(); break; }
          continue;
          }
       if (cmd[0]=='S' && cmd[3]=='S') {
          int l=Rs[nd].length();
          if (l==1) { illegal(); break; }
          string dcs=Rs[nd][l-2]+""+Rs[nd][l-1];
          int dci=c2n(dcs),ip;
          for (ip=Rs[ns].length()-2; ip>0; ip-=3) {
              string scs=Rs[ns][ip]+""+Rs[ns][ip+1];
              int sci=c2n(scs);
              if (suitcheck(dci,sci) && sci%13==dci%13-1) {
                 string ts=Rs[ns].substr(ip-1,Rs[ns].length()-ip+1);
                 Rs[ns].erase(Rs[ns].end()-ip-1,Rs[ns].end());
                 Rs[nd].append(ts);
                 if (Rs[ns].length()==1 && Rs[ns][0]!='0') {
                    Rs[ns].append(" ");
                    int nci=vc[(ns*(ns+1))/2+ns-Rs[ns][0]+'0'+1];
                    Rs[ns].append(n2c(nci));
                    }
                 break;
                 }
              }
          if (ip<=0) { illegal(); break; }
          continue;
          }
       illegal(); break;
       }
    Vis(Rs,Rd,Rc,Rp);
    system("PAUSE");
}
