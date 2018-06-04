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
    freopen("firstphr.txt","w",stdout);
    string str;
    while (getline(cin,str)) {
       string w="";
       int dot=0;
       for (int i=0; i<str.length(); i++) {
           if (str[i]=='.') {
              if (dot>=1 && i>=10) break;
              dot++;
              }
           w+=str[i];
           }
       //cout << w << endl;
       if (m.find(w)==m.end()) m[w]=1;
          else m[w]++;
       }
    for (map<string,int>::iterator it=m.begin(); it!=m.end(); it++)
        v.push_back(make_pair((*it).second,(*it).first));
    sort(v.begin(),v.end());
    m.clear();
    for (int j=v.size()-1; j>=0; j--)
        printf("%s %d\n",(v[j].second).c_str(),v[j].first);
    return 0;
}
