#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<sstream>
#include<fstream>
#include<set>
#include<queue>
#include<cmath>
using namespace std;

template <class STR> int s2i (STR S) { istringstream IS(S); int I; IS >> I; return I; }
template <class INT> string i2s (INT I) { ostringstream OS; OS << I; return OS.str(); }

int sz,callnr=0,term,cnttest=0,prevbest=0,curbest=0;
vector<int> allsco,totsco;
vector<set<int> > combin,addcom;
int cx[150],cy[150],vx[150],vy[150];
priority_queue<pair<int,int> > pq;
char alldet[300][20][20];
int prev[110][15][15],K,M,allin;
vector<int> sucx,sucy,final,seq;
set<int> hull,used,origin,bestset;
vector<string> suc;
ofstream fout("test.txt");

int cross (int x1,int y1,int x2,int y2) {
    return x1*y2-y1*x2;
}

void processgrid() {
     for (int m=0; m<10; m++) {
         for (int n=0; n<10; n++) {
             int empty=1,i,j;
             for (i=0; i<sz; i++)
                 for (j=0; j<sz; j++) {
                     if (suc[m*sz+i+2][n*sz+j]!='0') empty=0;
                     }
             if (empty==0) {
                int k;
                for (k=0; k<allsco.size(); k++) {
                    int dif=0;
                    for (i=0; i<sz; i++)
                        for (j=0; j<sz; j++)
                            if (alldet[k][i][j]!=suc[m*sz+i+2][n*sz+j]) dif++;
                    if (dif<=8) {
                       prev[callnr][m][n]=k;
                       break;
                       }
                    }
                if (k==allsco.size()) {
                   allsco.push_back(0);
                   totsco.push_back(0);
                   for (i=0; i<sz; i++)
                       for (j=0; j<sz; j++)
                           alldet[k][i][j]=suc[m*sz+i+2][n*sz+j];
                   prev[callnr][m][n]=k;
                   }
                }
                else prev[callnr][m][n]=-1;
             }
         }
}

void updatescore () {
     hull.clear();
     while (!pq.empty()) pq.pop();
     for (int i=0; i<allsco.size(); i++) {
         pq.push(make_pair(allsco[i],i));
         //fout << i << "  sco: " << all[i].sco << endl;
         }
     for (int i=0; i<M; i++)
         if (!pq.empty() && pq.top().first>0) {
            hull.insert(pq.top().second);
            //fout << pq.top().second << "  sco: " << pq.top().first << endl;
            pq.pop();
            }
     if (hull.empty()) {
        if (prev[seq[callnr-1]][sucx[seq[callnr-1]]/sz][sucy[seq[callnr-1]]/sz]==-1) return;
        allsco[prev[seq[callnr-1]][sucx[seq[callnr-1]]/sz][sucy[seq[callnr-1]]/sz]]=10;
        return;
        }
     //**************************************************************************************************************
            
     allin=1;
     for (int k=callnr-1; k>=0; k--) {
         //fout << "BEGIN SEARCH" << endl;
         hull.clear();
                
         while (!pq.empty()) pq.pop();
         for (int i=0; i<allsco.size(); i++) {
             //fout << i << " " << allsco[i] << endl;
             pq.push(make_pair(allsco[i],i));
             }
                
         for (int i=0; i<M; i++)
             if (!pq.empty() && pq.top().first>0) {
                hull.insert(pq.top().second);
                //fout << pq.top().second << "  sco: " << pq.top().first << endl;
                pq.pop();
                }
                
         /*
         for (set<int>::iterator it=hull.begin(); it!=hull.end(); it++)
             fout << *it << " ";
         fout << endl;
         */
         //make convex hull
         used.clear();
         int pnr=0,inhull=1,cax=0,cay=0,vnr=0;
         for (int i=0; i<10; i++)
             for (int j=0; j<10; j++) {
                 if (hull.find(prev[seq[k]][i][j])!=hull.end()) {
                    cx[pnr]=i*sz; cy[pnr]=j*sz; pnr++;
                    cx[pnr]=i*sz+sz-1; cy[pnr]=j*sz; pnr++;
                    cx[pnr]=i*sz; cy[pnr]=j*sz+sz-1; pnr++;
                    cx[pnr]=i*sz+sz-1; cy[pnr]=j*sz+sz-1; pnr++;
                    used.insert(prev[seq[k]][i][j]);
                    }
                 }
         if (used.find(prev[seq[k]][sucx[seq[k]]/sz][sucy[seq[k]]/sz])!=used.end() && k==callnr-1) {
            allsco[prev[seq[k]][sucx[seq[k]]/sz][sucy[seq[k]]/sz]]+=10; continue;
            }
         //fout << pnr << endl;
                
         if (pnr==0) {
            if (prev[seq[k]][sucx[seq[k]]/sz][sucy[seq[k]]/sz]!=-1)
               allsco[prev[seq[k]][sucx[seq[k]]/sz][sucy[seq[k]]/sz]]+=10;
            continue;
            }
         //core part of convex hull
         int st=0,now=0;
         for (int i=0; i<pnr; i++)
             if (cx[i]<cx[now] || (cx[i]==cx[now] && cy[i]<cy[now])) now=i;
         st=now;
         do {
            vx[vnr]=cx[now]; vy[vnr]=cy[now]; vnr++;
            int tpt=-1;
            for (int i=0; i<pnr; i++)
                if (i!=now) {
                   if (tpt==-1) tpt=i;
                   int ti1=cx[i]-cx[now],ti2=cy[i]-cy[now];
                   int ti3=cx[tpt]-cx[now],ti4=cy[tpt]-cy[now];
                   int crs=cross(ti1,ti2,ti3,ti4);
                   if (crs<0) tpt=i;
                   }
            now=tpt;
            } while (now!=st);
         /*
         for (int i=0; i<vnr; i++)
             fout << vx[i] << " " << vy[i] << endl;
         */
         for (int i=0; i<vnr; i++) {
             cax+=vx[i]; cay+=vy[i];
             }   
         cax/=vnr; cay/=vnr;
         int p1,p2;
         for (int i=0; i<vnr; i++) {
             p1=vx[(i+1)%vnr]-vx[i];
             p2=vy[(i+1)%vnr]-vy[i];
             int cpc=cross(p1,p2,cax-vx[i],cay-vy[i]);
             int cps=cross(p1,p2,sucx[seq[k]]-vx[i],sucy[seq[k]]-vy[i]);
             if (cpc*cps<0) inhull=0;
             }
         if (inhull==1) {
            //fout << "INHULL" << endl;
            if (hull.size()==M) {
               int area=0;
               for (int i=0; i<vnr; i++)
                   area+=vx[i]*vy[(i+1)%vnr]-vy[i]*vx[(i+1)%vnr];
               area=abs(area)/2;
               //fout << "area: " << area << endl;
                      
               if (area<325) {
                  for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                      allsco[*it]+=51;
                  }
                  else if (area<800) {
                  for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                      allsco[*it]+=16;
                  }
                  else if (area<2000) {
                  for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                      allsco[*it]+=7;
                  }
                  else if (area<5000) {
                  for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                      allsco[*it]+=3;
                  }
                  else if (area<10000) {
                  for (set<int>::iterator it=used.begin(); it!=used.end(); it++)
                      allsco[*it]+=1;
                  }
               }
            }
            else {
            //fout << "OUTHULL" << endl;
            allin=0;
                   
            //see if adding a image cover point
            vector<int> canadd;
            if (prev[seq[k]][sucx[seq[k]]/sz][sucy[seq[k]]/sz]!=-1)
               canadd.push_back(prev[seq[k]][sucx[seq[k]]/sz][sucy[seq[k]]/sz]);
            for (int i=0; i<10; i++)
                for (int j=0; j<10; j++)
                    if (prev[seq[k]][i][j]!=-1) {
                       cx[pnr]=i*sz; cy[pnr]=j*sz;
                       cx[pnr]=i*sz+sz-1; cy[pnr]=j*sz;
                       cx[pnr]=i*sz; cy[pnr]=j*sz+sz-1;
                       cx[pnr]=i*sz+sz-1; cy[pnr]=j*sz+sz-1;
                       int st=0,now=0;
                       vnr=0;
                       for (int l=0; l<pnr+4; l++)
                           if (cx[l]<cx[now] || (cx[l]==cx[now] && cy[l]<cy[now])) now=l;
                       st=now;
                       do {
                          vx[vnr]=cx[now]; vy[vnr]=cy[now]; vnr++;
                          int tpt=-1;
                          for (int l=0; l<pnr+4; l++)
                              if (l!=now) {
                                 if (tpt==-1) tpt=l;
                                 int ti1=cx[l]-cx[now],ti2=cy[l]-cy[now];
                                 int ti3=cx[tpt]-cx[now],ti4=cy[tpt]-cy[now];
                                 int crs=cross(ti1,ti2,ti3,ti4);
                                 if (crs<0) tpt=l;
                                 }
                          now=tpt;
                          } while (now!=st);
                       inhull=1;
                       int ti1,ti2;
                       for (int l=0; l<vnr; l++) {
                           ti1=vx[(l+1)%vnr]-vx[l];
                           ti2=vy[(l+1)%vnr]-vy[l];
                           int cpc=cross(ti1,ti2,cax-vx[l],cay-vy[l]);
                           int cps=cross(ti1,ti2,sucx[seq[k]]-vx[l],sucy[seq[k]]-vy[l]);
                           if (cpc*cps<0) inhull=0;
                           }
                       if (inhull==1) canadd.push_back(prev[seq[k]][i][j]);
                       }
            for (int i=0; i<canadd.size(); i++)
                if (hull.find(canadd[i])==hull.end()) {
                   if (canadd.size()<=1) allsco[canadd[i]]+=41;
                      else if (canadd.size()<=3) allsco[canadd[i]]+=12;
                      else if (canadd.size()<=5) allsco[canadd[i]]+=6;
                      else if (canadd.size()<=10) allsco[canadd[i]]+=3;
                      else if (canadd.size()<=20) allsco[canadd[i]]+=1;
                   }
            }
         /*
         for (int i=0; i<allsco.size(); i++)
             fout << i << " " << allsco[i] << endl;
         */
         }
}

int allinhull () {
     int resa=0;
     for (int k=min(callnr-1,M*5); k>=0; k--) {
         //fout << "BEGIN SEARCH" << endl;
                
         //make convex hull
         used.clear();
         int pnr=0,inhull=1,cax=0,cay=0,vnr=0;
         for (int i=0; i<10; i++)
             for (int j=0; j<10; j++) {
                 if (hull.find(prev[k][i][j])!=hull.end()) {
                    cx[pnr]=i*sz; cy[pnr]=j*sz; pnr++;
                    cx[pnr]=i*sz+sz-1; cy[pnr]=j*sz; pnr++;
                    cx[pnr]=i*sz; cy[pnr]=j*sz+sz-1; pnr++;
                    cx[pnr]=i*sz+sz-1; cy[pnr]=j*sz+sz-1; pnr++;
                    used.insert(prev[k][i][j]);
                    }
                 }
         //fout << pnr << endl;
                
         if (pnr==0 || pnr>=24) {
            continue;
            }
         //core part of convex hull
         int st=0,now=0;
         for (int i=0; i<pnr; i++)
             if (cx[i]<cx[now] || (cx[i]==cx[now] && cy[i]<cy[now])) now=i;
         st=now;
         do {
            vx[vnr]=cx[now]; vy[vnr]=cy[now]; vnr++;
            int tpt=-1;
            for (int i=0; i<pnr; i++)
                if (i!=now) {
                   if (tpt==-1) tpt=i;
                   int ti1=cx[i]-cx[now],ti2=cy[i]-cy[now];
                   int ti3=cx[tpt]-cx[now],ti4=cy[tpt]-cy[now];
                   int crs=cross(ti1,ti2,ti3,ti4);
                   if (crs<0) tpt=i;
                   }
            now=tpt;
            } while (now!=st);
         /*
         for (int i=0; i<vnr; i++)
             fout << vx[i] << " " << vy[i] << endl;
         */
         for (int i=0; i<vnr; i++) {
             cax+=vx[i]; cay+=vy[i];
             }   
         cax/=vnr; cay/=vnr;
         int p1,p2;
         for (int i=0; i<vnr; i++) {
             p1=vx[(i+1)%vnr]-vx[i];
             p2=vy[(i+1)%vnr]-vy[i];
             int cpc=cross(p1,p2,cax-vx[i],cay-vy[i]);
             int cps=cross(p1,p2,sucx[k]-vx[i],sucy[k]-vy[i]);
             if (cpc*cps<0) inhull=0;
             }
         if (inhull==1) {
            resa++;
            }
         }
     return resa;
}

void exhaust () {
     int sz=combin.size();
     for (int i=0; i<sz; i++) {
         hull=combin[i];
         int match=allinhull();
         /*
         fout << "TEST ";
         for (set<int>::iterator it=hull.begin(); it!=hull.end(); it++)
             fout << *it << " ";
         fout << endl;
         fout << match << endl;
         */
         cnttest++;
         if (match>prevbest) {
            if (match>curbest) { curbest=match; bestset=hull; }
            if (curbest==min(callnr,M*5+1)) { term=1; return; }
            for (int j=M; j<min(K,50); j++)
                if (hull.find(final[j])==hull.end()) {
                   hull.insert(final[j]);
                   for (set<int>::iterator it=hull.begin(); it!=hull.end(); it++) {
                       if (origin.find(*it)!=origin.end()) {
                          set<int> newhull=hull;
                          newhull.erase(*it);
                          addcom.push_back(newhull);
                          }
                       }
                   hull.erase(final[j]);
                   }
            }
         if (cnttest>20000) return;
         }
}

//visualizer

vector<string> successfulLogin () {
   cout << 1 << endl; fflush(stdout);
   vector<string> vs(10*sz+2);
   for (int i=0; i<10*sz+2; i++)
       cin >> vs[i];
   return vs;
}

//local machine
/*
vector<pair<int,vector<string> > > chosen,pass;
vector<pair<int,int> > finx;
ofstream res("res.txt");
int cntcall;
vector<string> successfulLogin () {
   cout << cntcall+1 << " CALL" << endl;
   int tar,ran=rand()%10;
   if (ran<1) tar=1;
      else if (ran<3) tar=2;
      else if (ran<7) tar=3;
      else if (ran<9) tar=4;
      else if (ran<10) tar=5;
   cout << "tar: " << tar << endl;
   int psz=pass.size(),csz=chosen.size();
   vector<pair<int,vector<string> > > tot;
   set<int> selp;
   for (int i=0; i<tar; i++) {
       ran=rand()%psz;
       tot.push_back(pass[ran]);
       //cout << pass[ran].first << endl;
       selp.insert(pass[ran].first);
       }
   int decoy=rand()%((100-tar)-(100-tar)/2+1)+(100-tar)/2;
   cout << "decoy: " << decoy << endl;
   for (int i=0; i<decoy; i++) {
       tot.push_back(chosen[rand()%csz]);
       }
   vector<string> emp;
   for (int i=0; i<18; i++)
       emp.push_back("000000000000000000");
   for (int i=0; i<100-decoy-tar; i++)
       tot.push_back(make_pair(-1,emp));
   random_shuffle(tot.begin(),tot.end());
   vector<pair<int,int> > pts;
   finx.clear();
   for (int i=0; i<100; i++)
       if (selp.find(tot[i].first)!=selp.end()) {
          pts.push_back(make_pair(i%10*18,i/10*18));
          pts.push_back(make_pair(i%10*18+17,i/10*18));
          pts.push_back(make_pair(i%10*18,i/10*18+17));
          pts.push_back(make_pair(i%10*18+17,i/10*18+17));
          }
   //cout << "pts: " << pts.size() << endl;
   int st=0,now=0;
   for (int i=0; i<pts.size(); i++)
       if (pts[i]<pts[now]) now=i;
       st=now;
       do {
          finx.push_back(pts[now]);
          int tpt=-1;
          for (int i=0; i<pts.size(); i++)
              if (i!=now) {
                 if (tpt==-1) tpt=i;
                 int crs=cross(pts[i].first-pts[now].first,pts[i].second-pts[now].second,pts[tpt].first-pts[now].first,pts[tpt].second-pts[now].second);
                 if (crs<0) tpt=i;
                 }
          now=tpt;
          } while (now!=st);
   int inhull=1,cx=0,cy=0,vnr=finx.size();
   for (int i=0; i<vnr; i++) {
       cx+=finx[i].first; cy+=finx[i].second;
       }
   cx/=vnr; cy/=vnr;
   //cout << "(" << cx << "," << cy << ")" << endl;
   int vx,vy;
   do {
       vx=rand()%180;
       vy=rand()%180;
       inhull=1;
       for (int i=0; i<vnr; i++) {
           int cpc=cross(finx[(i+1)%vnr].first-finx[i].first,finx[(i+1)%vnr].second-finx[i].second,cx-finx[i].first,cy-finx[i].second);
           int cps=cross(finx[(i+1)%vnr].first-finx[i].first,finx[(i+1)%vnr].second-finx[i].second,vx-finx[i].first,vy-finx[i].second);
           if (cpc*cps<0) inhull=0;
           }
       } while (inhull==0);
   vector<string> ret;
   ret.push_back(i2s(vx));
   ret.push_back(i2s(vy));
   for (int i=0; i<10; i++)
       for (int k=0; k<18; k++) {
           string ts="";
           for (int j=0; j<10; j++)
               ts+=tot[i*10+j].second[k];
           ret.push_back(ts);
           }
   cntcall++;
   return ret;
}
*/

//#######################################################################################################################
class GraphicalAuthentication {
   public:
      int shoulderSurfing(int K1, int M1, int SZ) {
         sz=SZ; K=K1; M=M1;
         fout << "K:" << K << " M:" << M << endl;
         for (int i=0; i<100; i++) seq.push_back(i);
         while (true) {
            //fout << "RESTART " << callnr << endl;
            suc=successfulLogin();
            sucx.push_back(s2i(suc[1]));
            sucy.push_back(s2i(suc[0]));
            //fout << sucx[sucx.size()-1] << " " << sucy[sucy.size()-1] << " " << gx << " " << gy << endl;
            processgrid();
            callnr++;
            //calculate score
            updatescore();
            /*
            for (int i=0; i<allsco.size(); i++) {
                fout << i << " " << allsco[i] << endl;
                }
            fout << flush;
            */
            //fout << "allin: " << allin << endl;
            if (callnr>M*8-1 || (allin==1 && callnr>M*4-1)) break;
            
            }
         for (int i=0; i<allsco.size(); i++) {
             totsco[i]+=allsco[i];
             //fout << i << " " << allsco[i] << " " << totsco[i] << endl;
             }
         int shuf=0;
         while (allin!=1 && shuf<40) {
            callnr=0; 
            fout<< "ATTEMPT " << shuf << endl;
            for (int i=0; i<allsco.size(); i++) allsco[i]=0;
            hull.clear(); used.clear();
            random_shuffle(seq.begin(),seq.begin()+M*8); 
            while (true) {
               callnr++;
               updatescore();
               if (callnr>M*8-1 || (allin==1 && callnr>M*5-1)) break;
               }
            for (int i=0; i<allsco.size(); i++) {
                totsco[i]+=allsco[i];
                }
            shuf++;
            }
         if (allin==0) {
            for (int i=0; i<allsco.size(); i++)
                allsco[i]=totsco[i];
            }
         while (!pq.empty()) pq.pop();
         for (int i=0; i<allsco.size(); i++)
             if (allsco[i]>0) pq.push(make_pair(allsco[i],i));
         for (int i=0; i<allsco.size(); i++)
             if (!pq.empty()) {
                fout << pq.top().second << "  sco: " << pq.top().first << endl;
                /*
                if (i<50) {
                   for (int j=0; j<sz; j++) {
                       for (int k=0; k<sz; k++)  
                           if (alldet[pq.top().second][j][k]=='1') fout << "#";
                              else fout << ".";                    
                       fout << endl;
                       }
                    }
                */
                final.push_back(pq.top().second);
                pq.pop();
                }
         hull.clear();
         for (int i=0; i<M; i++) hull.insert(final[i]);
         combin.push_back(hull);
         origin=hull;
         for (int i=M; i>=0; i--) {
             term=0;
             fout << "exhaust " << i << " " << M-i << endl;
             exhaust();
             fout << "best: " << curbest << "  sz: " << addcom.size() << endl;
             prevbest=curbest;
             combin=addcom; addcom.clear();
             if (term==1 || cnttest>20000) break;
             }
         fout << "NO TEST: " << cnttest << endl;
         final.clear();
         for (set<int>::iterator it=bestset.begin(); it!=bestset.end(); it++) {
             final.push_back(*it);
             for (int j=0; j<sz; j++) {
                 for (int k=0; k<sz; k++)  
                     if (alldet[*it][j][k]=='1') fout << "#";
                        else fout << ".";                    
                 fout << endl;
                 }
             }
         return 1;
         }
      vector <int> loginAttempt(vector <string> scr) {
         vector<int> ret;
         ret.push_back(90); ret.push_back(90);
         /*
         for (int i=0; i<scr.size(); i++) {
             for (int j=0; j<scr[0].length(); j++)
                 if (scr[i][j]=='1') fout << "#"; else fout << ".";
             fout << endl;
             }
         */
         /*
         for (int i=0; i<final.size(); i++)
             fout << final[i] << " ";
         fout << endl;
         fout << ret.size() << endl;
         */
         fout << "New" << endl;
         int prior=0;
         for (int m=0; m<10; m++) {
             for (int n=0; n<10; n++) {
                 int empty=1;
                 for (int i=0; i<sz; i++)
                     for (int j=0; j<sz; j++) {
                         if (scr[m*sz+i][n*sz+j]!='0') empty=0;
                         }
                 if (empty==0) {
                    for (int k=0; k<final.size(); k++) {
                        int dif=0;
                        for (int i=0; i<sz; i++)
                            for (int j=0; j<sz; j++)
                                if (alldet[final[k]][i][j]!=scr[m*sz+i][n*sz+j]) dif++;
                        if (dif<=8 && allsco[final[k]]>prior) {
                           //fout << m << " " << n << endl << flush;
                           ret[0]=(n*sz+9); ret[1]=(m*sz+9);
                           prior=k;
                           //fout << k << " dif: " << dif << endl;
                           }
                        }
                    }
                 }
             }
         fout << ret[0] << " " << ret[1] << endl << flush;
         return ret;
         }
};

int main () {
    GraphicalAuthentication GA;
    //visualizer
    
    int t1,t2,t3;
    cin >> t1 >> t2 >> t3;
    t1=GA.shoulderSurfing(t1,t2,t3);
    cout << 0 << endl; fflush(stdout);
    while (true) {
       vector<string> vs(10*sz);
       for (int i=0; i<10*sz; i++)
           cin >> vs[i];
       vector<int> res;
       res=GA.loginAttempt(vs);
       cout << res[0] << endl;
       cout << res[1] << endl;
       fflush(stdout);
       }
    
    //local machine
    /*
    int rept=25,stseed=0,crep=0;
    srand(997);
    while (crep<rept+stseed) {
       ifstream pic("pic.txt");
       sucx.clear(); sucy.clear(); final.clear(); hull.clear(); used.clear(); callnr=0; allsco.clear();
       chosen.clear(); pass.clear(); cntcall=0;
       string str;
       for (int i=0; i<256; i++)
           if (rand()%10<8) {
              vector<string> vs;
              for (int j=0; j<18; j++) {
                  pic >> str;
                  vs.push_back(str);
                  }
              chosen.push_back(make_pair(i,vs));
              }
              else for (int j=0; j<18; j++) pic >> str;
       random_shuffle(chosen.begin(),chosen.end());
       int sel=rand()%5+5;
       for (int i=0; i<sel; i++)
           pass.push_back(chosen[i]);
       chosen.erase(chosen.begin(),chosen.begin()+sel);
       
       if (crep>=stseed) {
          res << "Seed " << crep << endl;
          res << "K: " << chosen.size()+pass.size() << " M: " << pass.size() << endl;
          sel=GA.shoulderSurfing(chosen.size()+pass.size(),pass.size(),18);
          }
       double sco=(1+1.0/(1+cntcall));
       vector<string> rub;
       int callmade=cntcall;
       for (int i=cntcall; i<=100; i++)
           rub=successfulLogin();
       if (crep>=stseed) {
          int suclogin=0;
          vector<int> realpos;
          set<int> mispos;
          for (int i=0; i<pass.size(); i++) mispos.insert(i);
          for (int i=0; i<pass.size(); i++) {
              int equal=0;
              for (int j=0; j<pass.size(); j++) {
                  equal=1;
                  for (int k=0; k<18; k++)
                      for (int l=0; l<18; l++)
                          if (alldet[final[j]][k][l]!=pass[i].second[k][l]) equal=0;
                  if (equal==1) { mispos.erase(j); break; }
                  }
              suclogin+=equal;
              if (equal==0) {
                 for (int j=0; j<final.size(); j++) {
                     equal=1;
                     for (int k=0; k<18; k++)
                         for (int l=0; l<18; l++)
                             if (alldet[final[j]][k][l]!=pass[i].second[k][l]) equal=0;
                     if (equal==1) { realpos.push_back(j+1); break; }
                     }
                 }
              }
          if (suclogin==pass.size()) {
             res << "Calls: " << callmade<< " Score: " << sco << endl;
             }
             else {
                  res << "Calls: " << callmade << " Score: " << 0 << endl;
                  res << "     right: " << suclogin << "/" << pass.size() << endl;
                  sort(realpos.begin(),realpos.end());
                  res << "     ";
                  for (set<int>::iterator it=mispos.begin(); it!=mispos.end(); it++)
                      res << *it << " ";
                  res << endl;
                  res << "     ";
                  for (int i=0; i<realpos.size(); i++)
                      res << realpos[i] << " ";
                  res << endl;
                  }
          res << endl;
          }
       crep++;
       }
    */
}
