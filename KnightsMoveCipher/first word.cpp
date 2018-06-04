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
    freopen("first25.txt","w",stdout);
    string str;
    while (getline(cin,str)) {
       string w="";
       for (int i=0; i<25 || str[i]!='.'; i++) {
           if (str[i]=='.') {
              if (m.find(w)==m.end()) m[w]=1;
                 else m[w]++;
              //cout << w << endl;
              w="";
              }
              else w+=str[i];
           }
       //printf("%s\n",w.c_str());
       if (w[w.length()-1]!='.') {
          if (m.find(w)==m.end()) m[w]=1;
             else m[w]++;
          //cout << w << endl;
          }
          else printf("OHOHOH! %s\n",w.c_str());
       }
    for (map<string,int>::iterator it=m.begin(); it!=m.end(); it++)
        v.push_back(make_pair((*it).second,(*it).first));
    sort(v.begin(),v.end());
    for (int j=v.size()-1; j>=0; j--)
        printf("%s %d\n",(v[j].second).c_str(),v[j].first);
    return 0;
}
