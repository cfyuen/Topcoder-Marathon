#include<cstdio>
using namespace std;
 
int main() {
	freopen("runvis.bat","w",stdout);
	for (int i=1; i<=100; i++) printf("java -jar RockyMineVis.jar -exec v6.2.exe -novis -seed %d\n",i);
}
