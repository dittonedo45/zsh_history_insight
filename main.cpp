#include <iostream>

using namespace std;

void parse_cmd(string cmd)
{

}

int main ()
{
	string line;
	string cmdline{};

	while (getline(cin, line))
	{
		if (line[0]==':')
		{
			line=line.substr(1);
			int sep1=line.find(':');
			int sep2=line.find(';');

			if (sep1==-1 || sep2==-1) continue;
			string ltime{line.substr(1, sep1-1)};

			string hflag{line.substr(sep1+1, sep2-sep1-1)};

			cout<<ltime<<" "<<hflag<<endl;
		}
	}
	return 0;
}
