#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <cstring>
extern "C" {
#include <time.h>
};

using namespace std;

// Placeholder function for command parsing (not yet implemented)
void parse_cmd(string cmd)
{
	// XXX - Future implementation for command analysis
}

// Structure representing a single command from ZSH history
struct cmd {
	string htime;    // Timestamp as string from history file
	string hflag;    // ZSH history flags (typically "0")
	string cmdline;  // The actual command that was executed
	time_t ltime;    // Parsed Unix timestamp

	// Constructor that initializes the command and parses the timestamp
	cmd (string htime, string hflag, string cmdline ) : htime(htime), hflag(hflag), cmdline(cmdline)
	{
		parse_time ();
	}

	// Getter for the command line
	string get_cmd()
	{
		return cmdline;
	}

	// Parse the string timestamp into a Unix timestamp
	void parse_time ()
	{
		ltime=strtold(htime.c_str(), NULL);
	}
	
	// Extract year from timestamp
	uint get_year ()
	{
		struct tm* tm=tm_time();
		return tm->tm_year+1900;
	}
	
	// Extract weekday from timestamp (0=Sunday, 6=Saturday)
	uint get_week_day ()
	{
		struct tm* tm=tm_time();
		return tm->tm_wday;
	}
	
	// Extract hour from timestamp (0-23)
	uint get_hour ()
	{
		struct tm* tm=tm_time();
		return tm->tm_hour;
	}
	
	// Extract month from timestamp (0=January, 11=December)
	uint get_month ()
	{
		struct tm* tm=tm_time();
		return tm->tm_mon;
	}

	private:
	// Helper function to convert Unix timestamp to tm structure
	struct tm* tm_time()
	{
		return localtime (&ltime);
	}
};
// Type alias for list of command pointers
using cmd_list=list<cmd*>;

int main ()
{
	string line;
	vector<cmd> history{};           // Storage for all parsed commands
	map<uint,cmd_list> years{};      // Commands grouped by year
	map<uint,cmd_list> week_days{};  // Commands grouped by weekday
	map<uint,cmd_list> hours{};      // Commands grouped by hour
	map<uint,cmd_list> months{};     // Commands grouped by month

	string *cmdline{nullptr};        // Pointer to current command being built

	// Read ZSH history from standard input
	while (getline(cin, line))
	{
		// Check if line starts with ':' (new history entry)
		if (line[0]==':')
		{
			line=line.substr(1);           // Remove the leading ':'
			int sep1=line.find(':');       // Find first separator (after timestamp)
			int sep2=line.find(';');       // Find second separator (before command)

			// Skip malformed lines
			if (sep1==-1 || sep2==-1) continue;
			
			// Parse the three components: timestamp, flags, command
			string ltime{line.substr(1, sep1-1)};        // Extract timestamp
			string hflag{line.substr(sep1+1, sep2-sep1-1)}; // Extract flags  
			string commandline=line.substr (sep2+1);     // Extract command

			// Add to history and set current command pointer
			history.push_back({ltime, hflag, commandline});
			cmdline=&history.back().cmdline;
		}else{
			// This is a continuation line for a multi-line command
			if(cmdline)
			(*cmdline)+=line; // Append to the previous history entry
		}
	}
	
	// Categorize all commands by time periods
	for (cmd& cmd: history)
	{
		// Get references to the appropriate lists for this command's time
		cmd_list& l_cmds=years[cmd.get_year()];
		cmd_list& lw_cmds=week_days[cmd.get_week_day()];
		cmd_list& lh_cmds=hours[cmd.get_hour()];
		cmd_list& lm_cmds=months[cmd.get_month()];

		// Add command pointer to each time-based category
		l_cmds.push_back ({&cmd});
		lw_cmds.push_back ({&cmd});
		lh_cmds.push_back ({&cmd});
		lm_cmds.push_back ({&cmd});
	};
	
	// Output statistics (currently only months)
	for (auto& year_stats: months)
	{
		cout<<year_stats.first<<" size("<<year_stats.second.size()<<")"<<endl;
	}
	return 0;
}
