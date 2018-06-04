#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<map>
using namespace std;

vector<pair<int,string> > v;
map<string,int> m;

int main () {
    freopen("alls.txt","r",stdin);
    freopen("12letternps.txt","w",stdout);
    int nl=12,cnt=0;
    string str;
    while (getline(cin,str)) {
       string w="";
       //if (cnt%10000==0) cout << cnt << endl;
       for (int i=0; i<nl-1; i++)
           w+=str[i];
       for (int i=nl-1; i<min(50,max(25,(int)str.length()/2)); i++) {
           w+=str[i];
           int num=0,ok=1;
           for (int j=0; j<nl; j++) {
               if (w[j]=='.') num=0;
                  else num++;
               if (num>6) ok=0;
               }
           //if (!(i==nl-1 || str[i-nl]=='.')) { w.erase(w.begin()); continue; }
           //if (!(i+1==str.length() || str[i+1]=='.')) { w.erase(w.begin()); continue; }
           //printf("%s\n",w.c_str());
           if (ok==1) {
              //printf("%s\n",w.c_str());
              if (m.find(w)==m.end()) m[w]=1;
                 else m[w]++;
              }
           w.erase(w.begin());
           }
       //printf("%s\n",w.c_str());
       if (m.find(w)==m.end()) m[w]=1;
          else m[w]++;
       cnt++;
       }
    for (map<string,int>::iterator it=m.begin(); it!=m.end(); it++)
        if ((*it).second>=10) v.push_back(make_pair((*it).second,(*it).first));
    sort(v.begin(),v.end());
    for (int j=v.size()-1; j>=0; j--)
        printf("%s %d\n",(v[j].second).c_str(),v[j].first);
    return 0;
}
