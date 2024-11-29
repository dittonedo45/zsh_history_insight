#include <iostream>
#include <vector>

using namespace std;

void parse_cmd(string cmd)
{
	// XXX
}

struct cmd {
	string htime;
	string hflag;
	string cmdline;

};

int main ()
{
	string line;
	vector<cmd> history{};
	string *cmdline{nullptr};

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
			string commandline=line.substr (sep2+1);

			history.push_back({ltime, hflag, commandline});
			cmdline=&history.back().cmdline;
		}else{
			if(cmdline)
			(*cmdline)+=line; // Append on the previous history entry
		}
	}
	return 0;
}
