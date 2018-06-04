#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<cmath>
using namespace std;

void getLet () {
	char fname[10],tstr[55];
	FILE *flet;
	string cs;
	int fn,fm,hv;
	for (int i=0; i<26; i++) 
		for (int j=0; j<2; j++) {
			for (int k=0; k<21; k++) {
				if (j==0) sprintf(fname,"letters/%cB%d.txt",i+'A',k+8);
				else sprintf(fname,"letters/%cP%d.txt",i+'A',k+8);
				flet=fopen(fname,"r");
				fscanf(flet,"%d%d",&fn,&fm);
				printf("%d,%d",fn,fm);
				for (int x=0; x<fn; x++) {
					fscanf(flet,"%s",tstr);
					hv=0;
					for (int y=0; y<fm; y++) {
						hv*=2;
						hv+=tstr[y]-'0';
					}
					printf(",%d",hv);
				}
				printf(",");
				fclose(flet);
			}
			printf("\n");
		}
}

int main () {
	getLet();
}

