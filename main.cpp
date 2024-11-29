#include <iostream>

using namespace std;

void parse_cmd(string cmd)
{

}

int main ()
{
	string line;
	while (getline(cin, line))
	{
		if (line[0]==':')
		{
			int sep1=line.find(':');
			int sep2=line.find(';');
			if (sep1==-1 || sep2==-1) continue;
			cout<<line<<endl;

		}
	}
	return 0;
}
