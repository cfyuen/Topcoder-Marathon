#include<fstream>
using namespace std;
 
int main()
{
	int i;
	fstream f;
	string executable,palette,tester;
	f.open("runvis.bat",ios::out);
	executable = """D:\\Documents\\Computer\\TopCoder\\CellularAutomaton\\v2.2.exe""";
	tester="""D:\\Documents\\Computer\\TopCoder\\CellularAutomaton\\Visualizer.jar""";
	for(i=1;i<=30;i++)
		f<<"java -jar "<<tester<<" -exec "<<executable<<" -seed "<<i<< " -novis" << endl;
	
	f.close();
	return 0;
}
