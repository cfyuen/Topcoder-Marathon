#include<cstdio>
using namespace std;
 
int main() {
	freopen("runvis.bat","w",stdout);
	for (int i=101; i<=200; i++) printf("java -jar Tester.jar -exec v5.1.exe -seed %d\n",i);
}
