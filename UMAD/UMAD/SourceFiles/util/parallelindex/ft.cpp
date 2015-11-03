#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

int
main(int argc, char **argv)
{
	fstream ifs("../data/ufile.txt", ios::in);
	if (!ifs)
	{
		cout << "failed to open file ufile.txt" << endl;
		return -1;	
	}

	long fileloc = 0;
	
	char buffer[1024];

	for (int i=0; i < 3; i++)
	{
		int num = 0;
		if (i == 2)
		{
			num = 20;
		}
		else
		{
			num = (i + (20 / 3));
		}
		
		fileloc = ifs.tellg();
		cout << "[" << i << "] start location:" << fileloc << endl;
		for (int j=i; j<num; j++)
		{
			bzero(buffer, 1024);
			ifs.getline(buffer, 1024);
			cout << "linelen:" << strlen(buffer) << endl;
		}
		fileloc = ifs.tellg();
		cout << "[" << i << "] start location:" << fileloc << endl;
	}
	ifs.clear();
	ifs.close();
}
