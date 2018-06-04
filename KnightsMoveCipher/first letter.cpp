#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
using namespace std;

int cum[30];

int main () {
    freopen("alls.txt","r",stdin);
    freopen("firstletter.txt","w",stdout);
    string str;
    memset(cum,0,sizeof(cum));
    while (getline(cin,str)) {
       //printf("%c",str[0]);
       if (str[0]!='.' && str[0]>='A') cum[str[0]-'A']++;
          else {
                cum[26]++;
                cout << str << endl;
                }
       }
    int tot=0;
    for (int i=0; i<26; i++) {
        printf("%c %d\n",i+'A',cum[i]);
        tot+=cum[i];
        }
    printf(". %d\n",cum[26]);
    tot+=cum[26];
    printf("TOT: %d\n",tot);
    return 0;
}
