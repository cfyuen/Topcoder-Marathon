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
    freopen("allphr12w.txt","w",stdout);
    string str;
    while (getline(cin,str)) {
       string w="";
       int dot=0;
       for (int i=0; i<str.length(); i++) {
           
           if (str[i]=='.') {
              if (w.length()>=12 && dot>=1) {
                 int fac;
                 if (i<=50) fac=500;
                    else if (i<=100) fac=400;
                    else if (i<=150) fac=300;
                    else if (i<=200) fac=180;
                    else if (i<=300) fac=100;
                    else if (i<=400) fac=80;
                    else if (i<=500) fac=60;
                    else if (i<=600) fac=50;
                    else if (i<=700) fac=40;
                    else if (i<=800) fac=24;
                    else if (i<=900) fac=15;
                    else if (i<=1000) fac=12;
                    else if (i<=1100) fac=9;
                    else if (i<=1200) fac=6;
                    else if (i<=1400) fac=3;
                    else fac=2;
                 
                 //cout << w << endl;
                 if (m.find(w)==m.end()) m[w]=fac;
                    else m[w]+=fac;
                 /*
                 while (!(w.find('.')==string::npos || w.length()-(int)(w.find('.'))-1<10)) {
                    while (w[0]!='.') w.erase(w.begin());
                    w.erase(w.begin());
                    }
                 */
                 
                 while (w.length()>=12 && w.find('.')!=string::npos) {
                    while (w[0]!='.') w.erase(w.begin());
                    w.erase(w.begin());
                    }
                 
                 }
              dot++;
              }
           w+=str[i];
           }
       //cout << w << endl;
       int fac,i=str.length();
       if (i<=50) fac=500;
          else if (i<=100) fac=400;
          else if (i<=150) fac=300;
          else if (i<=200) fac=180;
          else if (i<=300) fac=100;
          else if (i<=400) fac=80;
          else if (i<=500) fac=60;
          else if (i<=600) fac=50;
          else if (i<=700) fac=40;
          else if (i<=800) fac=24;
          else if (i<=900) fac=15;
          else if (i<=1000) fac=12;
          else if (i<=1100) fac=9;
          else if (i<=1200) fac=6;
          else if (i<=1400) fac=3;
          else fac=2;
       if (m.find(w)==m.end()) m[w]=fac;
          else m[w]+=fac;
       }
    for (map<string,int>::iterator it=m.begin(); it!=m.end(); it++)
        if ((*it).second>=3000) v.push_back(make_pair((*it).second,(*it).first));
    sort(v.begin(),v.end());
    for (int j=v.size()-1; j>=0; j--)
        printf("%s %d\n",(v[j].second).c_str(),v[j].first);
    return 0;
}
