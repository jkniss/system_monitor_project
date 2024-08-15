#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


// TODO: Return the system's CPU
Processor& System::Cpu() 
{
    // return the cpu_ object
    return cpu_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() 
{
    // clear the processes vector
    processes_.clear();

    // get the list of PIDs
    vector<int> pids = LinuxParser::Pids();

    // iterate through the list of PIDs
    for (int pid : pids)
    {
        // create a new process object
        Process process;

        // set the PID of the process object
        process.setPid(pid);

        // add the process object to the processes vector
        processes_.push_back(process);
    }

    // return the processes vector
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel();
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {   
    return LinuxParser::MemoryUtilization();
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem();
 }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() 
{
    // call the RunningProcesses() function from the LinuxParser namespace
    return LinuxParser::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() 
{
    // call the TotalProcesses() function from the LinuxParser namespace
    return LinuxParser::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() 
{
    // call the UpTime() function from the LinuxParser namespace
    long l = LinuxParser::UpTime();

    // cast the long to a long int
    return static_cast<long int>(l);

}