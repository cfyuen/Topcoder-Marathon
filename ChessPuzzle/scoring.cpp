#include<cstdio>
using namespace std;
 
int main() {
	FILE *score,*out;
	score=fopen("output.txt","r");
	out=fopen("score v2.3 full.txt","w");
	double tot=0;
	int st=42301,end=42800;
	//int st=1,end=100;
	for (int i=st; i<=end; i++) {
        char str[500],ts[500];
        fgets(str,500,score); fgets(str,500,score);
        fgets(str,500,score);
        //fprintf(out,"%s\n",str);
        double sco;
        sscanf(str,"%s%s%lf",ts,ts,&sco);
        fprintf(out,"(Seed %d) = %0.4lf\n",i,sco);
        tot+=sco;
        }
    fprintf(out,"Average = %0.4lf\n",tot/(end-st+1));
}
