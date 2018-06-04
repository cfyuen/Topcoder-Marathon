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
    FILE *f1,*f2; f1=fopen("tallphr.txt","r");
    freopen("constant.txt","w",stdout);
    int mxele=28000;
    string str; char tc[1000]; int ti,cnt=0;
    /*
    fscanf(f1,"%s %d",tc,&ti);
    printf("{\"%s\"",tc);
    while (fscanf(f1,"%s %d",tc,&ti) && cnt<mxele) {
       printf(",");
       printf("\"%s\"",tc);
       cnt++;
       }
    printf("}");
    */
    fscanf(f1,"%s %d",tc,&ti);
    printf("\"%s",tc);
    while (fscanf(f1,"%s %d",tc,&ti) && cnt<mxele) {
       printf("#%s",tc);
       cnt++;
       }
    printf("#\"");
    return 0;
}
