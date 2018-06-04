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
    freopen("seed.txt","w",stdout);
    srand(9997);
    for (int i=0; i<1000; i++) {
        int n=rand()%51+10;
        int r=rand()%6+5;
        printf("%d\n",i+1);
        printf("%d\n",n+1);
        printf("%d\n",rand()%(500-2*r+1)+r);
        for (int j=0; j<n; j++)
            printf("%d\n",rand()%(500-2*r+1)+r);
        printf("%d\n",490);
        for (int j=0; j<n; j++)
            printf("%d\n",rand()%(490-2*r+1)+r);
        printf("%d\n",r);
        printf("\n");
        }
    return 0;
}
