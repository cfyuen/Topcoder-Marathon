#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
using namespace std;
#define LL long long

char s[10005];

int main() {
	int start = 10001,end = 10200;
	printf("Seed,Fine,Salary,Score\n");
	for (int i=start; i<=end; i++) {
		gets(s);
		gets(s);
		printf("%d,",i);
		gets(s);
		char ts[1005];
		int pos = 0,x;
		LL fine,sala,score;
		for (int j=0; j<2; j++) {
			sscanf(s+pos,"%s%n",ts,&x);
			pos += x;
		}
		sscanf(s+pos,"%lld\n",&fine);
		printf("%lld,",fine);

		gets(s);
		pos = 0;
		for (int j=0; j<2; j++) {
			sscanf(s+pos,"%s%n",ts,&x);
			pos += x;
		}
		sscanf(s+pos,"%lld\n",&sala);
		printf("%lld,",sala);

		gets(s);
		pos = 0;
		for (int j=0; j<2; j++) {
			sscanf(s+pos,"%s%n",ts,&x);
			pos += x;
		}
		sscanf(s+pos,"%lld\n",&score);
		printf("%lld\n",score);
	}
	return 0;
}
