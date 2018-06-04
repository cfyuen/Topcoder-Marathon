#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
using namespace std;

vector<int> freq;

int main () {
    freopen("alls.txt","r",stdin);
    freopen("analysis.txt","w",stdout);
    string str;
    int sho=100000,tot=0,lon=0,sen=0;
    while (getline(cin,str)) {
       int l=str.length();
       if (l<sho) { sho=l; printf("%s\n",str.c_str()); }
       if (l>lon) { lon=l; printf("%s\n",str.c_str()); }
       tot+=l;
       freq.push_back(l);
       sen++;
       }
    printf("Sen: %d\n",sen);
    printf("TotalCh: %d      Average: %lf0.4\n",tot,tot*1.0/sen);
    printf("Longest: %d\n",lon);
    printf("Shortest: %d\n",sho);
    sort(freq.begin(),freq.end());
    printf("Freq: \n");
    int pos=0;
    for (int i=1; i<=21; i++) {
        while (freq[pos]<=i*100 && pos<freq.size())
           pos++;
        printf("%d: %d\n",i*100,pos-1);
        }
    fflush(stdout);
    return 0;
}
