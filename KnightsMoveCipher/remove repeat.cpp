#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<set>
using namespace std;

vector<string> v1;

int main () {
    freopen("tallphr.txt","r",stdin);
    freopen("allphrm.txt","w",stdout);
    string str; int ti,cnt=0;
    char tstr[1000];
    while (scanf("%s %d",tstr,&ti)==2) {
       v1.push_back(tstr);
       }
    for (int i=0; i<v1.size(); i++) {
        if (i<10000)
           for (int j=i+1; j<v1.size(); j++)
               if (v1[i]==v1[j])
                  v1[j]="?";
        if (v1[i]!="?") printf("%s %d\n",v1[i].c_str(),i+1);
        if (i%100==0) fflush(stdout);
        }
    return 0;
}
