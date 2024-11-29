#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <cstring>
extern "C" {
#include <time.h>
};

using namespace std;

void parse_cmd(string cmd)
{
	// XXX
}

struct cmd {
	string htime;
	string hflag;
	string cmdline;
	time_t ltime;

	cmd (string htime, string hflag, string cmdline ) : htime(htime), hflag(hflag), cmdline(cmdline)
	{
		parse_time ();
	}

	string get_cmd()
	{
		return cmdline;
	}

	void parse_time ()
	{
		ltime=strtold(htime.c_str(), NULL);
	}
	int get_year ()
	{
		struct tm* tm=tm_time();
		return tm->tm_year-1900;
	}

	private:
	struct tm* tm_time()
	{
		return localtime (&ltime);
	}
};

int main ()
{
	string line;
	vector<cmd> history{};
	map<int,list<cmd*>> years{};
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
	for (cmd& cmd: history)
	{
		list<cmd*>& l_cmds=years[cmd.get_year()];
		l_cmds.push_back ({&cmd});
	};
	for (auto& year_stats: years)
	{
		cout<<year_stats.first<<" size("<<year_stats.second.size()<<")"<<endl;
	}
	return 0;
}
