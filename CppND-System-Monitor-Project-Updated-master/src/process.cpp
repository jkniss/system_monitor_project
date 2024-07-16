#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;
using std::stof;

// TODO: Return this process's ID
int Process::Pid() { return 0; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() { return string(); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  // variables
  //     #14     #15    #16      #17      #22 
  string utime, stime, cutime, cstime, starttime, line, str_pid;
  long utime_val, stime_val, cutime_val, cstime_val, starttime_val, total_time;
  int line_number = 0;

  // Use the given pid to open the /proc/[PID]/stat file to read
  // str_pid = std::to_string(pid);
  // std::ifstream file_stream(kProcDirectory + str_pid + kStatFilename);

  // // Find the values for information above (see numbering) and store values
  // while (std::getline(file_stream, line)) {
  //   line_number++;
  //   if (line_number == 13){
  //     utime = line;
  //   } else if (line_number == 14) {
  //     stime = line;
  //   } else if (line_number == 15) {
  //     cutime = line;
  //   } else if (line_number == 16) {
  //     cstime = line;
  //   } else if (line_number == 22) {
  //     starttime = line;
  //   }
  //   if (line_number >=22) {
  //     break;
  //   } 
  // }
  // Calculate the average utilization of each process since process launched
  // Make sure to get clock ticks to make the conversion
  // return the value for that PID
  return 0; 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }