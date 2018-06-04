#include<cstdio>
#include<algorithm>
#include<vector>
using namespace std;

vector<double> a;
 
int main() {
	FILE *score1,*out,*score2;
	score1=fopen("score v2.3.txt","r");
	score2=fopen("score v1.8.txt","r");
	out=fopen("highest.txt","r");
	double tot=0;
	int st=1,end=100;
	for (int i=st; i<=end; i++) {
        char str[500],ts[500];
        fgets(str,500,score1);
        //fprintf(out,"%s\n",str);
        double sco1,sco2,sco3;
        sscanf(str,"%s%s%s%lf",ts,ts,ts,&sco1);
        fgets(str,500,score2);
        sscanf(str,"%s%s%s%lf",ts,ts,ts,&sco2);
        fgets(str,500,out);
        sscanf(str,"%s%s%s%lf",ts,ts,ts,&sco3);
        a.push_back(max(sco1,max(sco2,sco3)));
        }
    out=fopen("highest.txt","w");
    for (int i=st; i<=end; i++) {
        fprintf(out,"(Seed %d) = %0.4lf\n",i,a[i-st]);
        tot+=a[i-st];
        }
    fprintf(out,"Average = %0.4lf\n",tot/(end-st+1));
}
