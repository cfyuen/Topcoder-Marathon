#include<cstdio>
using namespace std;
 
int main() {
	FILE *score,*out;
	score=fopen("output.txt","r");
	out=fopen("score v2.2.txt","w");
	double tot=0;
	int st=1,end=30;
	//int st=1,end=100;
	for (int i=st; i<=end; i++) {
        char str[500],ts[500];
        fgets(str,500,score); 
        fgets(str,500,score);
        fgets(str,500,score);
        int r,c;
        sscanf(str,"%s%s%d, %s%s%d",ts,ts,&r,ts,ts,&c);
        fgets(str,500,score);
        char rule[100];
        sscanf(str,"%s%s%s",ts,ts,rule);
        fgets(str,500,score);
        fgets(str,500,score);
        fgets(str,500,score);
        fgets(str,500,score);
        double sco;
        sscanf(str,"%s%s%lf",ts,ts,&sco);
        fprintf(out,"(Seed %3d)   R:%4d  C:%4d  (%s)   Score = %0.5lf\n",i,r,c,rule,sco);
        tot+=sco;
        
        }
    fprintf(out,"Average = %0.4lf\n",tot/(end-st+1));
}
