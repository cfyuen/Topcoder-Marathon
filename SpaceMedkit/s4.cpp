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

/************************approach****************
1.    Parse all resource, event and missions into appropriate format
         when parsing resource, express the mass and volume in terms of cost
         when parsing missions, only consider the mission where there are gain in evacuation
            v1.4 sepearate a mission into parts of '1 gain' in evaluation
1.1   Set target  
2.    In phrase 1 (v1.3 bin packing)
         calculate the cost of each mission in order to eliminate the evacuation cost
            v1.1 concern only about the key resource in eliminating the evacuation cost
         sort the missions in ascending order of cost
      Interphrase
         try to include the most urgent resource (among the expensive ones)
      In phrase 2
         for each mission from low cost to high cost, use the required resource to eliminate the cost
            avoid increasing too much at a time (v1.3 now set at 0.5)
         v1.2 if not fulfilled target, repeat phrase 1
3.    Parse all the information obtained into ret
*************************************************/

/****Score***

*********/

typedef pair<pair<int,int>,int> PII;

vector<string> ret;
int consum[10005],alluntreat,target,noofmis,resolved,small=0;
double curruse[10005],nowcost;
double P,C,rcost[10005];
vector<double> bevent[10005],wevent[10005];
vector<pair<double,pair<vector<PII>,vector<int> > > > handle,save;
//     amount       Rid
vector<double> bin[10005];

int cntop;

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

class SpaceMedkit {
   public:
      void parseResource (vector<string> ar) {
         char par1[1000],par2[1000],par3[1000],par4[1000],tc;
         int rid;
         double m,v;
         for (int i=0; i<ar.size(); i++) {
             sscanf(ar[i].c_str(),"%s%s%s%s",par1,par2,par3,par4);
             sscanf(par1,"%c%d",&tc,&rid);
             sscanf(par2,"%d",&consum[rid]);
             sscanf(par3,"%lf",&m);
             sscanf(par4,"%lf",&v);
             rcost[rid]=m+C*v;
             }
         }
      void parseEvent (vector<string> rr) {
           int ptr=0,prev=-1,rid,mid;
           double tmp[10005];
           char par1[1000],par2[1000],par3[1000],par4[1000],tc;
           for (int i=0; i<rr.size(); i++) {
               sscanf(rr[i].c_str(),"%s%s%s%s",par1,par2,par3,par4);
               sscanf(par1,"%c%c%d",&tc,&tc,&mid);
               sscanf(par2,"%c%d",&tc,&rid);
               //fprintf(debug,"%s %s\n",par1,par2);
               //fprintf(debug,"%d %d\n",Mid,Rid);
               if (mid!=prev && prev!=-1) {
                  bevent[prev].resize(ptr/3*2);
                  wevent[prev].resize(ptr/3*2);
                  for (int j=0; j<ptr; j+=3) {
                      bevent[prev][j/3*2]=tmp[j];
                      wevent[prev][j/3*2]=tmp[j];
                      bevent[prev][j/3*2+1]=tmp[j+1];
                      wevent[prev][j/3*2+1]=tmp[j+2];
                      }
                  ptr=0;
                  /*
                  for (int j=0; j<Best[prev].size(); j++)
                      fprintf(debug,"%d: [%d]%0.4lf\n",prev,j,Best[prev][j]); */
                  //printf("%d\n",prev); fflush(stdout);
                  }
               prev=mid;
               tmp[ptr]=rid; ptr++; 
               sscanf(par3,"%lf",&tmp[ptr]); ptr++;
               sscanf(par4,"%lf",&tmp[ptr]); ptr++;
               }
           bevent[prev].resize(ptr/3*2);
           wevent[prev].resize(ptr/3*2);
           for (int j=0; j<ptr; j+=3) {
               bevent[prev][j/3*2]=tmp[j];
               wevent[prev][j/3*2]=tmp[j];
               bevent[prev][j/3*2+1]=tmp[j+1];
               wevent[prev][j/3*2+1]=tmp[j+2];
               }
           }
      void parseMis (vector<string> mis) {
         int tid,ti,mid,worst,tre,utre,prev=-1,ok=-1,prevat=-1;
         char par1[1000],tc;
         vector<PII> tevent;
         vector<int> concern;
         vector<double> req;
         for (int i=0; i<mis.size(); i++) {
         //for (int i=0; i<21589; i++) {    
             sscanf(mis[i].c_str(),"%d%d%s%d%d%d",&tid,&ti,&par1,&worst,&tre,&utre);
             sscanf(par1,"%c%c%d",&tc,&tc,&mid);
             if (prev!=tid) {
                if (ok!=-1) {
                   //fprintf(debug,"%d\n",prev);
                   tevent.resize(ok-prevat);
                   //printf("%d %d\n",ok,prevat);
                   //printf("%d\n",tevent[tevent.size()-1].second);
                   if (tevent[tevent.size()-1].second!=0)
                      handle.push_back(make_pair(0,make_pair(tevent,concern)));
                   /*
                   for (int k=0; k<concern.size(); k++)
                       fprintf(debug,"%d: %d\n",handle.size()-1,concern[k]);
                   */
                   concern.clear();
                   }
                ok=-1;
                tevent.clear();
                prev=tid;
                prevat=i-1;
                }
             alluntreat+=utre;
             if (utre>tre) {
                ok=i;
                cntop++;
                if (worst==1) req=wevent[mid];
                   else req=bevent[mid];
                
                for (int j=0; j<req.size(); j+=2)
                    if (req[j+1]>1e-12) concern.push_back((int)req[j]);
                
                }
             tevent.push_back(make_pair(make_pair(mid,worst),utre-tre));
             if (utre>tre) {
                handle.push_back(make_pair(0,make_pair(tevent,concern)));
                tevent[tevent.size()-1].second=0;
                }
             }
         if (ok!=-1) {
            tevent.resize(ok-prevat);
            handle.push_back(make_pair(0,make_pair(tevent,concern)));
            }
         noofmis=tid;
         }
      
      void phrase1 () { 
         vector<double> req;
         int keyres[10005]; memset(keyres,0,sizeof(keyres));
         double totres[10005];
         for (int i=0; i<handle.size(); i++) {
             double cost=0;
             for (int j=0; j<handle[i].second.second.size(); j++) {
                 keyres[handle[i].second.second[j]]=1;
                 totres[handle[i].second.second[j]]=0;
                 //fprintf(debug,"%d -- need %d\n",i,handle[i].second.second[j]);
                 }
             for (int j=0; j<handle[i].second.first.size(); j++) {
                 //evaluate cost
                 //
                 //handle[i].second.first[j].#
                 //   #first.first = event related
                 //   #first.second = is it worst case
                 //   #second = gain (-1,0,1,2...)
                 int mid=handle[i].second.first[j].first.first,worst=handle[i].second.first[j].first.second;
                 if (worst==1) {
                    req=wevent[mid];
                    }
                    else req=bevent[mid];
                 for (int k=0; k<req.size(); k+=2) //here consider curruse[] also
                     if (keyres[(int)req[k]]==1 && totres[(int)req[k]]+req[k+1]>curruse[(int)req[k]]) {//optimize, only consider key resources
                        cost+=rcost[(int)req[k]]*req[k+1];
                        if (consum[(int)req[k]]==1) totres[(int)req[k]]+=req[k+1];
                           else totres[(int)req[k]]=max(req[k+1],totres[(int)req[k]]);
                        //fprintf(debug,"%d -- cost: %d  %0.4lf*%0.4lf\n",i,(int)req[k],rcost[(int)req[k]],req[k+1]);
                        }
                 //printf("%d: %0.4lf\n",mid,cost); fflush(stdout);
                 }
             handle[i].first=cost;
             int putbin=0;
             double costlimit=0.5;
             for (int j=0; j<handle[i].second.second.size(); j++) {
                 keyres[handle[i].second.second[j]]=0;
                 if (totres[handle[i].second.second[j]]*rcost[handle[i].second.second[j]]>costlimit)
                    putbin++;
                    
                 }
             if (putbin==1) {
                for (int j=0; j<handle[i].second.second.size(); j++)
                    if (totres[handle[i].second.second[j]]*rcost[handle[i].second.second[j]]>costlimit)
                       bin[handle[i].second.second[j]].push_back(totres[handle[i].second.second[j]]);
                }
             }
         printf("**%0.3lf\n",runtime());
         sort(handle.begin(),handle.end());
         printf("**%0.3lf\n",runtime());
         
         for (int i=0; i<handle.size(); i++) {
             //fprintf(debug,"%d: %0.4lf\n",i,handle[i].first); 
             if (handle[i].first<1.0) //can change
                small++;
             }
         }    
      void phrase2 (int times) {
         vector<int> usedres;
         double need[10005]; //need[] store the key resource needed for each mission
         vector<double> req;
         int keyres[10005]; memset(keyres,0,sizeof(keyres));
         for (int j=0; j<=10000; j++) need[j]=0;
         for (int i=0; i<handle.size(); i++) {
             //printf("%d/%d: %d  %d\n",i,handle.size(),resolved,usedres.size());
             int solved=0; //here, solved is only a temporary var to store current reduction of evaluation
             //fprintf(debug,"%d/%d: %d  %0.4lf\n",i,handle.size(),resolved,nowcost);
             if (alluntreat+resolved<=target) {
                //fprintf(debug,"stopat : %d\n",i);
                break;
                }
             for (int j=0; j<usedres.size(); j++) need[usedres[j]]=0;
             usedres.clear();
             for (int j=0; j<handle[i].second.second.size(); j++)
                 keyres[handle[i].second.second[j]]=1;
             for (int j=0; j<handle[i].second.first.size(); j++) {
                 int mid=handle[i].second.first[j].first.first,worst=handle[i].second.first[j].first.second;
                 if (worst==1) {
                    req=wevent[mid];
                    }
                    else req=bevent[mid];
                 for (int k=0; k<req.size(); k+=2) 
                     if (keyres[(int)req[k]]==1) {
                        usedres.push_back((int)req[k]);
                        if (consum[(int)req[k]]==1)
                           need[(int)req[k]]+=req[k+1];
                           else need[(int)req[k]]=max(need[(int)req[k]],req[k+1]);
                        }
                 solved+=handle[i].second.first[j].second;
                 }
             //find out whether it is worth to add additional resource
             int additem=1;
             double totcost=0;
             for (int j=0; j<usedres.size(); j++) {
                 if (curruse[usedres[j]]<need[usedres[j]]) 
                    totcost+=(need[usedres[j]]-curruse[usedres[j]])*rcost[usedres[j]];
                 if (curruse[usedres[j]]<need[usedres[j]] && nowcost>times*5) 
                    if ( ((need[usedres[j]]-curruse[usedres[j]])*rcost[usedres[j]])>0.5 ) {
                       additem=0;
                       //fprintf(debug,"%d -- exceed\n",i);
                       }
                 }
             //fprintf(debug,"%0.4lf\n",totcost);
             if (totcost>0.5) {
                additem=0;
                //fprintf(debug,"%d -- exceed %0.4lf\n",i,totcost);
                }
             //if (i>0) fprintf(debug,"%0.4lf - %0.4lf\n",handle[i].first,handle[i-1].first);
             for (int j=0; j<handle[i].second.second.size(); j++)
                 keyres[handle[i].second.second[j]]=0;
             //additem==1 -> ko, additem==0 -> save until the next stage
             if (additem==1 || times>=1000) {
                for (int j=0; j<usedres.size(); j++) {
                    if (curruse[usedres[j]]<need[usedres[j]]) {
                       nowcost+=(need[usedres[j]]-curruse[usedres[j]])*rcost[usedres[j]];
                       //fprintf(debug,"%d -- %d: %0.4lf\n",i,usedres[j],need[usedres[j]]);
                       }
                    curruse[usedres[j]]=max(curruse[usedres[j]],need[usedres[j]]);
                    }
                resolved-=solved;
                }
                else {
                //put unsolved mission to a new vector 'save'
                //fprintf(debug,"%d->%d\n",i,save.size());
                save.push_back(handle[i]);                
                }
             }
         } 
      vector <string> getMedkit(vector <string> ar, vector <string> rr, vector <string> mis, double _P, double _C) {
         start();
         cntop=0;
         P=_P; C=_C;
         alluntreat=0; noofmis=0; nowcost=0; resolved=0;
         memset(consum,-1,sizeof(consum));
         for (int i=0; i<=10000; i++) {
             curruse[i]=0;
             }
         handle.clear();
         //fprintf(debug,"%0.4lf %0.4lf\n",P,C);
         parseResource(ar);
         parseEvent(rr);
         parseMis(mis);
         target=(int)(P*noofmis);
         //target=(int)(P*noofmis)+(int)(P*12000);
         //fprintf(debug,"handle size = %d   alluntreat = %d   target = %d\n",handle.size(),alluntreat,target);
         
         printf("%0.3lf\n",runtime());
         printf("opreation: %d\n",cntop);
         int rerun=1,manual=-1;
         double amount=0,freq;
         do {
            //fprintf(debug,"RERUN = %d\n",rerun);
            small=0;
            phrase1();
            manual=-1; freq=1e9;
            if (alluntreat+resolved-small>target) {
               for (int i=0; i<=10000; i++)
                   if (bin[i].size()>0) {
                      //fprintf(debug,"R%d %d %0.4lf\n",i,bin[i].size(),rcost[i]);
                      sort(bin[i].begin(),bin[i].end());
                      //if (alluntreat+resolved+bin[i].size()<target && bin[i].size()>1000) manual=i;
                      if (rcost[i]/bin[i].size()<freq && bin[i].size()>50) {
                         freq=rcost[i]/bin[i].size();
                         manual=i;
                         amount=bin[i][0];
                         }
                      //for (int j=0; j<bin[i].size(); j++)
                      //    fprintf(debug,"%0.4lf ",bin[i][j]);
                      //fprintf(debug,"\n");
                      bin[i].clear();
                      }
               //fprintf(debug,"RManual: %d\n",manual);
            
               nowcost+=(amount-curruse[manual])*rcost[manual];
               curruse[manual]=amount;
               }
               else rerun=1000;
            if (rerun>20) rerun=1000;
            //if (rerun==1000) fprintf(debug,"RERUN 1000\n");
            phrase2(rerun);
            rerun++;
            handle=save;
            save.clear();
            printf("%0.3lf\n",runtime());
            } while (alluntreat+resolved>target);
         printf("%0.3lf\n",runtime());
         /*
         for (int i=0; i<=10000; i++) {
             if (consum[i]==1) {
                string s;
                char str[1000];
                sprintf(str,"R%d %0.4lf\0",i,35.0);
                s=str;
                ret.push_back(s);
                }
             if (consum[i]==0) {
                string s;
                char str[1000];
                sprintf(str,"R%d %d\0",i,1);
                s=str;
                ret.push_back(s);
                }
             }
         */
         for (int i=0; i<=10000; i++) {
             if (consum[i]==1) {
                string s;
                char str[1000];
                sprintf(str,"R%d %lf\0",i,curruse[i]);
                s=str;
                ret.push_back(s);
                }
             if (consum[i]==0) {
                string s;
                char str[1000];
                sprintf(str,"R%d %lf\0",i,curruse[i]);
                s=str;
                ret.push_back(s);
                }
             }
         /*
         for (int i=0; i<ret.size(); i++)
             fprintf(debug,"%s\n",ret[i].c_str());
         */
         
         return ret;
      }
};
