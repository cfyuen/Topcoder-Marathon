#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<queue>
using namespace std;

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
      int prevpos,midmove[8][8];
      string move(vector <string> stack, vector <string> deck, vector <int> complete, int pos) {
         for (int i=0; i<7; i++) {
             int stl=stack[i].length();
             if (stack[i][stl-1]==' ') stack[i].erase(stack[i].end()-1);
             }
         vector<int> bot,top;
         int last=1,chance=0;
         if (deck.size()==24) {
            int newgame=1;
            for (int i=0; i<7; i++) if (stack[i].length()!=4) newgame=0;
            if (newgame==1) memset(midmove,0,sizeof(midmove));
            }
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
             if (stack[i][0]!='0') last=0;
             }
         //create com
         vector<int> com;
         for (int i=0; i<4; i++)
             if (complete[i]!=0) com.push_back(complete[i]-1+13*i);
                else com.push_back(-1);
         //check condition 1
         for (int i=0; i<7; i++)
             for (int j=0; j<7; j++)
                 if (bot[i]!=-1 && i!=j) {
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
         
         for (int i=0; i<4; i++)
             if (com[i]!=-1) {
                for (int j=0; j<7; j++)
                    if (suitcheck(com[i],bot[j]) && bot[j]%13==com[i]%13-1) {
                       for (int k=0; k<7; k++)
                           if (suitcheck(com[i],top[j]) && com[i]%13==top[j]%13-1) {
                              string ret="C"; ret+='1'+i; ret+=" S"; ret+='1'+k;
                              memset(midmove,0,sizeof(midmove));
                              return ret;
                              }
                       }
                }
         
         //check condition 3
         if (!(last==1 && deck.size()>0)) {
            for (int i=0; i<4; i++)
                for (int j=0; j<7; j++) {
                    if (top[j]%13==0 && com[i]==-1 && i==top[j]/13) {
                       string ret="S"; ret+='1'+j; ret+=" C"; ret+='1'+i;
                       memset(midmove,0,sizeof(midmove));
                       return ret;
                       }        
                    if (com[i]/13==top[j]/13 && com[i]+1==top[j]) {
                       string ret="S"; ret+='1'+j; ret+=" C"; ret+='1'+i;
                       memset(midmove,0,sizeof(midmove));
                       return ret;
                       }
                    }
            }
         //BFS
         int decksz=deck.size(),minstep[25];
         memset(minstep,-1,sizeof(minstep));
         if (chosen.empty()) {
            queue<Game> q; int end=0;
            Game now;
            now.Pos=pos; now.Top=top; now.Com=complete;
            for (int i=0; i<decksz; i++) now.Deck.push_back(c2n(deck[i]));
            now.Cho.clear();
            q.push(now);
            while (!q.empty()) {
               now=q.front();
               q.pop();
               int terminate=0;
               for (int i=0; i<7; i++)
                   for (int j=0; j<7; j++) 
                       if (stack[i][0]!='0') {
                          if (suitcheck(bot[i],now.Top[j]) && bot[i]%13==now.Top[j]%13-1) terminate=1;
                          if (bot[i]==now.Top[j] && bot[i]%13==now.Com[bot[i]/13]) terminate=1;
                          }
               if (last==1 && !now.Cho.empty()) {
                  chosen.push_back(now.Cho[0]);
                  end=1;
                  break;
                  }
               int chosz=chosen.size(),have=0;
               for (have=0; have<chosz; have++)
                   if (chosen[have].first==now.Cho[0].first) {
                      if (terminate==1 && now.Cho.size()<minstep[now.Cho[0].first])
                         minstep[now.Cho[0].first]=now.Cho.size();
                      break;
                      }
               
               if (have!=chosz) continue;
               if (q.size()>30000) break;
               if (terminate==1 && last!=1) {
                  chosen.push_back(now.Cho[0]);
                  minstep[now.Cho[0].first]=now.Cho.size();
                  end=1;
                  }
               decksz=now.Deck.size();
               int v[25],tpos=now.Pos,card; memset(v,0,sizeof(v));
               while (v[tpos]==0 && !now.Deck.empty()) {
                  v[tpos]=1;
                  card=now.Deck[tpos];
                  for (int i=0; i<7; i++) {
                      if ((now.Top[i]==-1 && card%13==12) || (suitcheck(now.Top[i],card) && card%13==now.Top[i]%13-1)) {
                         Game next;
                         if (tpos!=0) next.Pos=tpos-1;
                            else next.Pos=min((int)(now.Deck.size())-2,2);
                         next.Top=now.Top; next.Top[i]=card;
                         next.Deck=now.Deck; next.Deck.erase(next.Deck.begin()+tpos);
                         next.Com=now.Com;
                         string ts="DECK S"; ts+='1'+i;
                         next.Cho=now.Cho; next.Cho.push_back(make_pair(tpos,ts));
                         q.push(next);
                         break;
                         }
                      }
                  if (now.Com[card/13]==card%13) {
                     Game next;
                     if (tpos!=0) next.Pos=tpos-1;
                        else next.Pos=min((int)(now.Deck.size())-2,2);
                     next.Top=now.Top;
                     next.Deck=now.Deck; next.Deck.erase(next.Deck.begin()+tpos);
                     next.Com=now.Com; next.Com[card/13]++;
                     string ts="DECK C"; ts+='1'+card/13;
                     next.Cho=now.Cho; next.Cho.push_back(make_pair(tpos,ts));
                     q.push(next);
                     }
                  
                  if (tpos==decksz-1) {
                     if (decksz<3) tpos=decksz-1;
                        else tpos=2;
                     }
                     else {
                          tpos+=3;
                          if (tpos>=decksz) tpos=decksz-1;
                          }
                  }
               }

            if (end==0) {
               if (last==1) {
                  for (int i=0; i<4; i++)
                      for (int j=0; j<7; j++) {
                          if (top[j]%13==0 && com[i]==-1 && i==top[j]/13) {
                             string ret="S"; ret+='1'+j; ret+=" C"; ret+='1'+i;
                             memset(midmove,0,sizeof(midmove));
                             return ret;
                             }        
                          if (com[i]/13==top[j]/13 && com[i]+1==top[j]) {
                             string ret="S"; ret+='1'+j; ret+=" C"; ret+='1'+i;
                             memset(midmove,0,sizeof(midmove));
                             return ret;
                             }
                          }
                  }
               if (!now.Cho.empty()) {
                  chosen.push_back(now.Cho[0]);
                  }
                  else {
                       for (int i=0; i<7; i++)
                           for (int j=0; j<7; j++) {
                               if (top[i]!=-1 && top[j]!=-1 && i!=j && midmove[i][j]==0 && top[i]%13<top[j]%13) {
                                  if ((top[j]%13-top[i]%13+suitcheck(top[j],top[i]))%2==0 && bot[i]%13>=top[j]%13) {
                                     string ret="S";
                                     ret+=i+'1'; ret+=" S";
                                     ret+=j+'1';
                                     midmove[i][j]=1;
                                     return ret;
                                     }
                                  }
                               midmove[i][j]=1;
                               }
                       return "DONE";
                       }
               }
            }

         for (int i=1; i<chosen.size(); i++) {             
             if (deck.size()==24) {
                if (chosen[i]>chosen[0]) swap(chosen[i],chosen[0]);
                }
                else {
                if (chosen[i].first>=prevpos-1) {
                   if (chosen[0].first<prevpos-1) swap(chosen[i],chosen[0]);
                      else {
                           if (minstep[chosen[i].first]<minstep[chosen[0].first]) swap(chosen[i],chosen[0]);
                              else if (minstep[chosen[i].first]==minstep[chosen[0].first] && chosen[i]>chosen[0]) 
                                      swap(chosen[i],chosen[0]);
                           }
                   }
                   else if (chosen[0].first<prevpos-1 && chosen[i]>chosen[0]) swap(chosen[i],chosen[0]);
                }
             
             }
         //Anyway i have to do this, because BFS must return a result
         if (chosen[0].first==pos) {
            string ret=chosen[0].second;
            prevpos=chosen[0].first;
            chosen.erase(chosen.begin());
            chosen.clear();
            memset(midmove,0,sizeof(midmove));
            return ret;
            }
            else return "ADVANCE";
         }
};

int main() {}
