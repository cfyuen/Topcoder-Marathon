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
    freopen("allphr.txt","w",stdout);
    string str;
    while (getline(cin,str)) {
       string w="";
       int dot=0;
       for (int i=0; i<str.length(); i++) {
           
           if (str[i]=='.') {
              if (w.length()>=10 && dot>=1) {
                 
                 
                 
                 //cout << w << endl;
                 if (m.find(w)==m.end()) m[w]=1;
                    else m[w]++;
                 /*
                 while (!(w.find('.')==string::npos || w.length()-(int)(w.find('.'))-1<10)) {
                    while (w[0]!='.') w.erase(w.begin());
                    w.erase(w.begin());
                    }
                 */
                 while (w.length()>=10 && w.find('.')!=string::npos) {
                    while (w[0]!='.') w.erase(w.begin());
                    dot--;
                    w.erase(w.begin());
                    }
                 
                 }
              dot++;
              }
           w+=str[i];
           }
       //cout << w << endl;
       if (m.find(w)==m.end()) m[w]=1;
          else m[w]++;
       }
    for (map<string,int>::iterator it=m.begin(); it!=m.end(); it++) {
        if ((*it).second>=20) v.push_back(make_pair((*it).second,(*it).first));
        }
    sort(v.begin(),v.end());
    for (int j=v.size()-1; j>=0; j--)
        printf("%s %d\n",(v[j].second).c_str(),v[j].first);
    return 0;
}
