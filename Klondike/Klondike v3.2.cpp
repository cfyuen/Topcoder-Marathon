#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<queue>
#include<ctime>
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
                       /*
                       int canmove=0;
                       
                       if (stack[i][0]=='0' && bot[i]%13!=12) {
                          for (int k=0; k<7; k++)
                              if (bot[k]%13==12 && stack[k][0]!='0') canmove=1;
                          }
                          else canmove=1;
                       //if (deck[pos][0]=='K') canmove=1;
                       if (canmove==1) {
                       */
                          if (suitcheck(bot[i],top[j]) && bot[i]%13==top[j]%13-1) {
                             string ret="S"; ret+='1'+i; ret+=" S"; ret+='1'+j;
                             return ret;
                             }
                       //   }
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
            //cout << "BFS is coming" << endl;
            queue<Game> q; int end=0;
            Game now;
            now.Pos=pos; now.Top=top; now.Com=complete;
            for (int i=0; i<decksz; i++) now.Deck.push_back(c2n(deck[i]));
            now.Cho.clear();
            q.push(now);
            while (!q.empty()) {
               now=q.front();
               q.pop();
               //cout << q.size() << endl;
               int terminate=0;
               for (int i=0; i<7; i++)
                   for (int j=0; j<7; j++) 
                       if (stack[i][0]!='0') {
                          if (suitcheck(bot[i],now.Top[j]) && bot[i]%13==now.Top[j]%13-1) terminate=1;
                          if (bot[i]==now.Top[j] && bot[i]%13==now.Com[bot[i]/13]) terminate=1;
                          }
               if (last==1 && !now.Cho.empty()) {
                  chosen.push_back(now.Cho[0]);
                  //cout << now.Cho[1].first << " " << now.Cho[0].second << endl;
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
                  //cout << "-----------------" << endl;
                  //for (int i=0; i<now.Cho.size(); i++) cout << now.Cho[i].first << " " << now.Cho[i].second << endl;
                  end=1;
                  //break;
                  }
               decksz=now.Deck.size();
               int v[25],tpos=now.Pos,card; memset(v,0,sizeof(v));
               
               /*
               for (int i=0; i<decksz; i++) cout << n2c(now.Deck[i]) << " ";
               cout << endl;
               for (int i=0; i<4; i++) cout << now.Com[i] << " ";
               cout << endl;
               for (int i=0; i<7; i++) cout << n2c(now.Top[i]) << " ";
               cout << endl;
               
               system("PAUSE");
               */
               while (v[tpos]==0 && !now.Deck.empty()) {
                  v[tpos]=1;
                  //cout << tpos << " ";
                  
                  card=now.Deck[tpos];
                  for (int i=0; i<7; i++) {
                      //   A King is put on empty stack       A normal card put on stack
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
               //cout << endl;
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
                                  //cout << i << " " << j << endl;
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



void Vis(vector <string> Vs, vector <string> Vd, vector <int> Vc, int Vp) {
     //system("CLS");
     
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





void illegal () {
   //system("CLS");
   printf("ILLEGAL");
   system("PAUSE");
}

int main() {
    Klondike kl;
    freopen("mfile.txt","w",stdout);
    int game=0,win=0;
    vector<int> vc;
    for (int i=0; i<52; i++) vc.push_back(i);
    while (game<20000) {
    clock_t st; st=clock();
    game++;
    random_shuffle(vc.begin(),vc.end());
    while (game<10001) {
       game++;
       random_shuffle(vc.begin(),vc.end());
       }
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
       //Vis(Rs,Rd,Rc,Rp);
       string cmd;
       cmd=kl.move(Rs,Rd,Rc,Rp);
       //cout << cmd << endl;
       //system("PAUSE");
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
                   else Rp=min((int)(Rd.size()-1),2);
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
                   else Rp=min((int)(Rd.size()-1),2);
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
    clock_t end; end=clock();
    double dif=(end-st)/1000.0;
    //printf("NEW GAME\n");
    printf("Game %d finished in %0.3lfs\n",game,dif);
    if (Rc[0]+Rc[1]+Rc[2]+Rc[3]==52) {
       printf("Game %d is WON!\n",game);
       win++;
       }
    printf("\n");
    //system("PAUSE");
    fflush(stdout);
    }
    printf("TOTAL WIN: %d\n",win);
}
