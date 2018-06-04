#include<cstdio>
using namespace std;
 
int main() {
	FILE *score,*out;
	score=fopen("output.txt","r");
	out=fopen("score v4.3.txt","w");
	double tot=0;
	int st=1,end=30;
	//int st=1,end=100;
	for (int i=st; i<=end; i++) {
        char str[500],ts[500];
        fgets(str,500,score); 
        fgets(str,500,score);
        fgets(str,500,score);
        int v,e;
        sscanf(str,"%s%s%d",ts,ts,&v);
        fgets(str,500,score);
        fgets(str,500,score);
        sscanf(str,"%s%s%d",ts,ts,&e);
        double sco;
        fgets(str,500,score);
        sscanf(str,"%s%s%lf",ts,ts,&sco);
        fprintf(out,"(Seed %3d)   V:%4d  E:%4d   Score = %0.0lf\n",i,v,e,sco);
        tot+=sco;
        
        }
    //fprintf(out,"Average = %0.4lf\n",tot/(end-st+1));
}
