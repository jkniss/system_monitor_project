#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stof;

// DONE: Return this process's ID
int Process::Pid() const
{ 
  // return the PID of the current process
  return pid_;
}

// NOT WORKING: Return this process's CPU utilization
float Process::CpuUtilization() const
{ 

  // access and call the ActiveJiffies() function from the LinuxParser namespace
  // to get the active jiffies of the current process
  long activeJiffies = LinuxParser::ActiveJiffies();

  // access and call the Jiffies() function from the LinuxParser namespace
  // to get the total jiffies 
  long idleJiffies = LinuxParser::IdleJiffies();

  // calculate the CPU utilization of the current process
  return LinuxParser::ActiveJiffies(pid_) / (activeJiffies + idleJiffies);

}

// DONE: Return the command that generated this process
string Process::Command() 
{ 
  // access and call the Command() function from the LinuxParser namespace
  // to get the command that generated this process
  return LinuxParser::Command(pid_);

}

// DONE: Return this process's memory utilization
string Process::Ram() 
{
  // access and call the Ram() function from the LinuxParser namespace
  // to get the memory utilization of this process
  return LinuxParser::Ram(pid_);
}

// DONE: Return the user (name) that generated this process
string Process::User() 
{
  // access and call the User() function from the LinuxParser namespace
  // to get the user that generated this process
  return LinuxParser::User(pid_);
}

// NOT WORKING: Return the age of this process (in seconds)
long int Process::UpTime() 
{
  // access and call the UpTime() function from the LinuxParser namespace
  // to get the age of this process
  return LinuxParser::UpTime(pid_); 
}

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{ 
  // access and call the CpuUtilization() function from the LinuxParser namespace
  // to get the CPU utilization of the current process
  float cpuUtilization = CpuUtilization();

  // access and call the CpuUtilization() function from the LinuxParser namespace
  // to get the CPU utilization of the process object
  float aCpuUtilization = a.CpuUtilization();

  // compare the CPU utilization of the current process and the process object
  return cpuUtilization > aCpuUtilization;
}