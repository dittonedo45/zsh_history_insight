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
	uint get_year ()
	{
		struct tm* tm=tm_time();
		return tm->tm_year+1900;
	}
	uint get_week_day ()
	{
		struct tm* tm=tm_time();
		return tm->tm_wday;
	}
	uint get_hour ()
	{
		struct tm* tm=tm_time();
		return tm->tm_hour;
	}
	uint get_month ()
	{
		struct tm* tm=tm_time();
		return tm->tm_mon;
	}

	private:
	struct tm* tm_time()
	{
		return localtime (&ltime);
	}
};
using cmd_list=list<cmd*>;

int main ()
{
	string line;
	vector<cmd> history{};
	map<uint,cmd_list> years{};
	map<uint,cmd_list> week_days{};
	map<uint,cmd_list> hours{};
	map<uint,cmd_list> months{};

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
		cmd_list& l_cmds=years[cmd.get_year()];
		cmd_list& lw_cmds=week_days[cmd.get_week_day()];
		cmd_list& lh_cmds=hours[cmd.get_hour()];
		cmd_list& lm_cmds=months[cmd.get_month()];

		l_cmds.push_back ({&cmd});
		lw_cmds.push_back ({&cmd});
		lh_cmds.push_back ({&cmd});
		lm_cmds.push_back ({&cmd});
	};
	for (auto& year_stats: months)
	{
		cout<<year_stats.first<<" size("<<year_stats.second.size()<<")"<<endl;
	}
	return 0;
}
