#include<fstream>
using namespace std;
 
int main()
{
	int i;
	fstream f;
	string executable,palette,tester;
	f.open("runvis.bat",ios::out);
	executable = """D:\\Documents\\Computer\\TopCoder\\BookSelection\\v7.2.exe""";
	tester="""D:\\Documents\\Computer\\TopCoder\\BookSelection\\BookSelectionVis.jar""";
	for(i=1;i<=300;i++)
		f<<"java -jar "<<tester<<" -exec "<<executable<<" -seed "<<i<<" -novis"<<endl;
	
	f.close();
	return 0;
}
