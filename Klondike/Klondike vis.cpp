#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<queue>
using namespace std;

FILE *mfile;

char suit[]={'C','D','H','S'};
char rank[]={'A','2','3','4','5','6','7','8','9','T','J','Q','K'};

struct Game {
    int Pos;
    vector<int> Deck,Top,Com;
    vector<pair<int,string> > Cho;
};

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
      vector<pair<int,string> > chosen;
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
         
         //BFS
         int decksz=deck.size();
         if (chosen.empty()) {
            queue<Game> q;
            Game now;
            now.Pos=pos; now.Top=top; now.Com=complete;
            for (int i=0; i<decksz; i++) now.Deck.push_back(c2n(deck[i]));
            now.Cho.clear();
            q.push(now);
            while (!q.empty()) {
               now=q.front();
               //cout << q.size() << endl;
               int terminate=0;
               for (int i=0; i<7; i++)
                   for (int j=0; j<7; j++) {
                       if (suitcheck(bot[i],now.Top[j]) && bot[i]%13==now.Top[j]%13-1) terminate=1;
                       if (bot[i]==now.Top[j] && bot[i]%13==now.Com[bot[i]/13]) terminate=1;
                       }
               if (terminate==1) {
                  chosen=now.Cho;
                  break;
                  }
               q.pop();
               decksz=now.Deck.size();
               int v[25],tpos=now.Pos,card; memset(v,0,sizeof(v));
               /*
               for (int i=0; i<decksz; i++) cout << n2c(now.Deck[i]) << " ";
               cout << endl;
               for (int i=0; i<4; i++) cout << now.Com[i] << " ";
               cout << endl;
               for (int i=0; i<7; i++) cout << n2c(now.Top[i]) << " ";
               cout << endl;
               for (int i=0; i<now.Cho.size(); i++) cout << now.Cho[i].first << " " << now.Cho[i].second << endl;
               system("PAUSE");
               */
               while (v[tpos]==0) {
                  v[tpos]=1;
                  //cout << tpos << " ";
                  //cout << endl;
                  card=now.Deck[tpos];
                  for (int i=0; i<7; i++) {
                      //   A King is put on empty stack       A normal card put on stack
                      if ((now.Top[i]==-1 && card%13==12) || (suitcheck(now.Top[i],card) && card%13==now.Top[i]%13-1)) {
                         Game next;
                         if (tpos!=0) next.Pos=tpos-1;
                         next.Top=now.Top; next.Top[i]=card;
                         next.Deck=now.Deck; next.Deck.erase(next.Deck.begin()+tpos,next.Deck.begin()+tpos+1);
                         next.Com=now.Com;
                         string ts="S"; ts+='1'+i;
                         next.Cho=now.Cho; next.Cho.push_back(make_pair(tpos,ts));
                         q.push(next);
                         break;
                         }
                      }
                  if (now.Com[card/13]==card%13) {
                     Game next;
                     if (tpos!=0) next.Pos=tpos-1;
                     next.Top=now.Top;
                     next.Deck=now.Deck; next.Deck.erase(next.Deck.begin()+tpos,next.Deck.begin()+tpos+1);
                     next.Com=now.Com; next.Com[card/13]++;
                     string ts="C"; ts+='1'+card/13;
                     next.Cho=now.Cho; next.Cho.push_back(make_pair(tpos,ts));
                     q.push(next);
                     }
                  if (tpos==decksz) {
                     if (decksz<3) tpos=decksz-1;
                        else tpos=2;
                     }
                     else {
                          tpos+=3;
                          if (tpos>=decksz) tpos=decksz-1;
                          }
                  }
               }
            if (q.empty()) return "DONE";
            }
         //Anyway i have to do this, because BFS must return a result
         if (chosen[0].first==pos) {
            string ret=chosen[0].second;
            chosen.erase(chosen.begin(),chosen.begin()+1);
            ret="DECK "+ret;
            return ret;
            }
            else return "ADVANCE";
         }
};

int main() {
    Klondike kl;
    
    mfile=fopen("mfile.txt","w");
    while (true) {
       vector<string> Rs,Rd;
       vector<int> Rc;
       int Rp;
       int Sz;
       string S;
       scanf("%d\n",&Sz);
       for (int i=0; i<Sz; i++) {
           getline(cin,S);
           S.erase(S.end()-1,S.end());
           Rs.push_back(S);
           }
       scanf("%d\n",&Sz);
       for (int i=0; i<Sz; i++) {
           getline(cin,S);
           S.erase(S.end()-1,S.end());
           Rd.push_back(S);
           }
       for (int i=0; i<4; i++) {
           scanf("%d",&Sz);
           Rc.push_back(Sz);
           }
       scanf("\n");
       scanf("%d",&Rp);
       string cmd=kl.move(Rs,Rd,Rc,Rp);
       fprintf(mfile,"%s\n",cmd.c_str());
       fflush(mfile);
       cout << cmd << endl;
       fflush(stdout);
       }
}
