#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
using namespace std;

int main () {
    freopen("all.txt","r",stdin);
    freopen("alls.txt","w",stdout);
    string str;
    while (getline(cin,str)) {
       //cout << str << endl;
       int l=str.length();
       string out="";
       for (int i=2; i<l; i++)
           if (str[i]>='A' && str[i]<='Z') out+=str[i];
              else if (str[i]>='a' && str[i]<='z') out+=str[i]-'a'+'A';
              else if (str[i]==' ') out+=".";
       for (int i=1; i<out.length(); i++)
           if (out[i]=='.' && out[i-1]=='.') {
              out.erase(out.begin()+i);
              i--;
              }
       while (out[0]=='.') out.erase(out.begin());
       while (out[out.length()-1]=='.') out.erase(out.end()-1);
       if (out.length()>=25) printf("%s\n",out.c_str());
       }
    fflush(stdout);
    return 0;
}
