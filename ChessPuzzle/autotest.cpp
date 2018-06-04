#include<fstream>
using namespace std;
 
int main()
{
	int i;
	fstream f;
	string executable,palette,tester;
	f.open("runvis.bat",ios::out);
	executable = """D:\\Documents\\Computer\\TopCoder\\ChessPuzzle\\v2.3.exe""";
	tester="""D:\\Documents\\Computer\\TopCoder\\ChessPuzzle\\Tester.jar""";
	palette="""D:\\Documents\\Computer\\TopCoder\\ChessPuzzle\\palette.txt""";
	for(i=42522;i<=42800;i++)
		f<<"java -jar "<<tester<<" -palette "<<palette<<" -exec "<<executable<<" -seed "<<i<<" -novis"<<endl;
	
	f.close();
	return 0;
}
