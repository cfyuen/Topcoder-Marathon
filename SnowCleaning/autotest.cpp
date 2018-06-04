#include<fstream>
using namespace std;

int main()
{
	int i;
	fstream f;
	string executable,palette,tester;
	f.open("runvis.bat",ios::out);
	executable = """D:\\Documents\\Computer\\TopCoder\\SnowCleaning\\v5.1.exe""";
	tester="""D:\\Documents\\Computer\\TopCoder\\SnowCleaning\\SnowCleaningVis.jar""";
	for(i=10001;i<=10200;i++) {
		f<<"java -jar "<<tester<<" -exec "<<executable<<" -seed "<<i<<" -novis"<<endl;
	}

	f.close();
	return 0;
}
