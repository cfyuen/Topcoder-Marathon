#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<set>
using namespace std;

vector<string> v1,v2;
set<string> s,out;

int main () {
    FILE *f1,*f2; f1=fopen("allphr.txt","r"); f2=fopen("allphrw.txt","r");
    freopen("compare.txt","w",stdout);
    string str; int ti,cnt=0;
    char tstr[1000];
    while (fscanf(f1,"%s %d",tstr,&ti) && cnt<10000) {
       str=tstr; s.insert(str); cnt++;
       }
    cnt=0;
    while (fscanf(f2,"%s %d",tstr,&ti) && cnt<10000) {
       str=tstr;
       if (s.find(str)!=s.end()) s.erase(str);
          else out.insert(str);
       cnt++;
       }
    printf("%d\nf1 unique:\n",s.size());
    for (set<string>::iterator it=s.begin(); it!=s.end(); it++)
        printf("%s\n",(*it).c_str());
    printf("\n\nf2 unique:\n");
    for (set<string>::iterator it=out.begin(); it!=out.end(); it++)
        printf("%s\n",(*it).c_str());
    return 0;
}
