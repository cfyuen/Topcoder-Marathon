#include<cstdio>
#include<string>
using namespace std;

int main() {
    freopen("v3.2 Record.txt","r",stdin);
    freopen("win.txt","w",stdout);
    int game=1;
    while (game<=1000) {
       string str;
       do {
           char ts[1000];
           scanf("%s",ts);
           str=ts;
           } while (str!="finished");
       do {
           char ts[1000];
           scanf("%s",ts);
           str=ts;
           } while (str!="WON!" && str!="DECK:");
       if (str=="WON!") printf("%d\n",game);
       game++;
       }
}
