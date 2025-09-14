# ZSH History Insight

A C++ tool for analyzing ZSH shell history files to provide insights about command usage patterns across different time periods.

## Overview

This program reads ZSH history data from standard input and categorizes commands by various time periods (years, months, weekdays, and hours). It provides statistical insights about command usage patterns over time.

## Features

- **Time-based Analysis**: Categorizes commands by year, month, weekday, and hour
- **ZSH History Parsing**: Correctly parses the extended ZSH history format
- **Multi-line Command Support**: Handles commands that span multiple lines
- **Statistical Output**: Provides counts of commands per time period

## Input Format

The program expects ZSH history data in the extended format, which looks like:
```
: <timestamp>:<flags>;<command>
```

Where:
- `timestamp`: Unix timestamp when the command was executed
- `flags`: ZSH history flags (typically 0)
- `command`: The actual command that was executed

Example:
```
: 1702909200:0;ls -la
: 1702909260:0;cd /home/user
: 1702909320:0;git status
```

## Compilation

Compile the program using g++:
```bash
g++ -o zsh_history_insight main.cpp
```

## Usage

### From ZSH History File
```bash
./zsh_history_insight < ~/.zsh_history
```

### From Sample Data
```bash
echo ": 1702909200:0;ls -la" | ./zsh_history_insight
```

### Using the built-in ZSH history command
```bash
history -E | ./zsh_history_insight
```

## Output Format

The program currently outputs statistics for months, showing:
```
<month_number> size(<command_count>)
```

Where:
- `month_number`: Month (0-11, where 0=January, 11=December)
- `command_count`: Number of commands executed in that month

### Example Output
```bash
$ echo -e ": 1702909200:0;ls -la\n: 1641909200:0;cd /tmp\n: 1641912800:0;pwd" | ./zsh_history_insight
0 size(2)
11 size(1)
```

This shows:
- January (month 0): 2 commands
- December (month 11): 1 command

## Code Structure

### Main Components

1. **`cmd` struct**: Represents a single command with timestamp, flags, and command line
2. **Time parsing**: Converts Unix timestamps to structured time information
3. **History parsing**: Reads and parses ZSH history format from stdin
4. **Categorization**: Groups commands by various time periods
5. **Statistics**: Outputs analysis results

### Key Functions

- `cmd::parse_time()`: Converts string timestamp to Unix time
- `cmd::get_year()`, `cmd::get_month()`, etc.: Extract time components
- Main parsing loop: Reads history entries and handles multi-line commands

## Data Structures

- `history`: Vector storing all parsed commands
- `years`, `months`, `week_days`, `hours`: Maps grouping commands by time periods
- Each map uses time period as key and list of command pointers as value

## Example Analysis

Given this sample history:
```
: 1702909200:0;ls -la          # December 2023
: 1702909260:0;cd /home/user   # December 2023
: 1641909200:0;cd /tmp         # January 2022
```

The program would output statistics showing command distribution across months and years.

## Limitations

- Currently only outputs month statistics (line 109-112)
- The `parse_cmd()` function is not implemented (marked with XXX)
- No command-level analysis or insights
- Limited error handling for malformed input
- Output format is not configurable

## Technical Details

### Dependencies
- C++ compiler with C++11 support
- Standard C++ library
- POSIX time functions

### Memory Usage
- Commands are stored in memory during processing
- Uses STL containers (vector, map, list) for efficient categorization
- Memory usage scales linearly with history file size

### Parsing Algorithm
1. Read each line from standard input
2. Detect history entries by ':' prefix
3. Parse timestamp, flags, and command using string separators
4. Handle multi-line commands by appending to previous entry
5. Convert timestamps to structured time data
6. Categorize into time-based maps
7. Output statistics

### Supported ZSH History Format
The program handles the extended ZSH history format enabled by:
```bash
setopt EXTENDED_HISTORY
```

This format includes timestamps and execution duration information.

## Future Enhancements

Potential improvements could include:
- Command frequency analysis
- Most used commands
- Time pattern analysis (busiest hours/days)
- Command correlation analysis
- Export to different formats (JSON, CSV)