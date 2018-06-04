#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
using namespace std;

char suit[]={'C','D','H','S'};
char rank[]={'A','2','3','4','5','6','7','8','9','T','J','Q','K'};

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
    if ((s/13)%3==(d/13)%3) return 0;
    if ((s/13)%3!=0 && (d/13)%3!=0) return 0;
    return 1;
}

class Klondike {
   public:
      string move(vector <string> stack, vector <string> deck, vector <int> complete, int pos) {
         vector<int> bot,top;
         
         //create bot and top
         for (int i=0; i<7; i++) {
             int stl=stack[i].length();
             if (stl>1) {
                string tbot=""; tbot+=stack[i][2]; tbot+=stack[i][3];
                bot.push_back(c2n(tbot));
                }
                else bot.push_back(-1);
             if (stl>1) {
                string ttop=""; ttop+=stack[i][stl-2]; ttop+=stack[i][stl-1];
                top.push_back(c2n(ttop));
                }
                else top.push_back(-1);
             }
         
         //check condition 1
         for (int i=0; i<7; i++)
             for (int j=0; j<7; j++)
                 if (bot[i]!=-1) {
                    if (top[j]!=-1) {
                       if (suitcheck(bot[i],top[j]) && bot[i]%13==top[j]%13-1) {
                          string ret="S"; ret+='1'+i; ret+=" S"; ret+='1'+j;
                          return ret;
                          }
                       }
                       else if (bot[i]%13==12 && stack[i][0]!='0') {
                               string ret="S"; ret+='1'+i; ret+=" S"; ret+='1'+j;
                               return ret;
                               }
                    }
         
         //check condition 2
         vector<int> com;
         for (int i=0; i<4; i++)
             if (complete[i]!=0) com.push_back(complete[i]-1+13*i);
                else com.push_back(-1);
         for (int i=0; i<4; i++)
             if (com[i]!=-1) {
                for (int j=0; j<7; j++)
                    if (suitcheck(com[i],bot[j]) && bot[j]%13==com[i]%13-1) {
                       for (int k=0; k<7; k++)
                           if (suitcheck(com[i],top[j]) && com[i]%13==top[j]%13-1) {
                              string ret="C"; ret+='1'+i; ret+=" S"; ret+='1'+k;
                              return ret;
                              }
                       }
                }
         
         //check condition 3
         for (int i=0; i<4; i++)
             for (int j=0; j<7; j++) {
                 if (top[j]%13==0 && com[i]==-1 && i==top[j]/13) {
                    string ret="S"; ret+='1'+j; ret+=" C"; ret+='1'+i;
                    return ret;
                    }        
                 if (com[i]/13==top[j]/13 && com[i]+1==top[j]) {
                    string ret="S"; ret+='1'+j; ret+=" C"; ret+='1'+i;
                    return ret;
                    }
                 }
         
         return "ADVANCE";
         //return "DONE";
         }
};



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
     
     system("PAUSE");
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
    while (true) {
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
       cout << cmd << endl;
       system("PAUSE");
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
          string scs=""; scs+=Rs[ns][l-2]; scs+=Rs[ns][l-1];
          int sci=c2n(scs);
          if (sci/13==nd && sci%13==Rc[nd]) {
             Rc[nd]++;
             Rs[ns].erase(Rs[ns].end()-3,Rs[ns].end());
             if (Rc[0]+Rc[1]+Rc[2]+Rc[3]==52) break;
             if (Rs[ns].length()==1 && Rs[ns][0]!='0') {
                Rs[ns].append(" ");
                int nci=vc[(ns*(ns+1))/2+ns-Rs[ns][0]+'0'+1];
                Rs[ns].append(n2c(nci));
                Rs[ns][0]=Rs[ns][0]-1;
                }
             }
             else { illegal(); break; }
          continue;
          }
       if (cmd[0]=='C' && cmd[3]=='S') {
          int l=Rs[nd].length();
          string dcs=""; dcs+=Rs[nd][l-2]; dcs+=Rs[nd][l-1];
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
          //cout << Rs[ns] << endl;
          //cout << Rs[nd] << endl;
          if (l==1) {
             string scs=""; scs+=Rs[ns][2]; scs+=Rs[ns][3];
             int sci=c2n(scs);
             if (sci%13==12) {
                string ts=Rs[ns].substr(1,Rs[ns].length()-1);
                Rs[ns].erase(Rs[ns].begin()+1,Rs[ns].end());
                Rs[nd].append(ts);
                if (Rs[ns].length()==1 && Rs[ns][0]!='0') {
                    Rs[ns].append(" ");
                    int nci=vc[(ns*(ns+1))/2+ns-Rs[ns][0]+'0'+1];
                    Rs[ns].append(n2c(nci));
                    Rs[ns][0]=Rs[ns][0]-1;
                    }
                continue;
                }
                else { illegal(); break; }
             }
          string dcs=""; dcs+=Rs[nd][l-2]; dcs+=Rs[nd][l-1];
          int dci=c2n(dcs),ip;
          for (ip=Rs[ns].length()-2; ip>0; ip-=3) {
              string scs=""; scs+=Rs[ns][ip]; scs+=Rs[ns][ip+1];
              int sci=c2n(scs);
              //cout << ip << endl;
              //cout << dci << " " << sci << endl;
              if (suitcheck(dci,sci) && sci%13==dci%13-1) {
                 string ts=Rs[ns].substr(ip-1,Rs[ns].length()-ip+1);
                 //cout << ts << endl;
                 Rs[ns].erase(Rs[ns].begin()+ip-1,Rs[ns].end());
                 Rs[nd].append(ts);
                 if (Rs[ns].length()==1 && Rs[ns][0]!='0') {
                    Rs[ns].append(" ");
                    int nci=vc[(ns*(ns+1))/2+ns-Rs[ns][0]+'0'+1];
                    Rs[ns].append(n2c(nci));
                    Rs[ns][0]=Rs[ns][0]-1;
                    }
                 //system("PAUSE");
                 break;
                 }
              }
          if (ip<=0) { illegal(); break; }
          continue;
          }
       illegal(); break;
       }
    Vis(Rs,Rd,Rc,Rp);
    printf("NEW GAME\n");
    system("PAUSE");
    }
}
