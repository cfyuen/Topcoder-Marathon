#include<cstdio>
#include<string>
using namespace std;

int main() {
    freopen("v3 win.txt","r",stdin);
    freopen("unique.txt","w",stdout);
    int v[1500];
    for (int i=0; i<1300; i++) v[i]=0;
    int game;
    while (scanf("%d",&game)==1) {
          v[game]++;
          }
    int cnt=0;
    for (int i=1; i<=1000; i++)
        if (v[i]>0) {
           printf("GAME %d:  %d\n",i,v[i]);
           cnt++;
           }
    printf("TOTAL: %d\n",cnt);
}
