#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  stream.close();
  return kernel;
  
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  // Create a vector of integers called pids
  vector<int> pids;

  // Use the directory class method opendir to open the /proc/ directory
  DIR* directory = opendir(kProcDirectory.c_str());

  // Create a pointer called file
  struct dirent* file;

  // While there is a directory to read and not equal to a nullpt
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);

      // If it is, store that filename inside the PID vector
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

  // variables to store info
  string line;
  string key, value;
  float free_memory =1.0, total_memory =1.0;
  int count = 0;

  // read the /proc/meminfo file
  std::ifstream meminfo_file(kProcDirectory + kMeminfoFilename);

  // loop through the file
  while (std::getline(meminfo_file, line)) {
    std::istringstream is(line);
    is >> key >> value;
    if (key == "MemTotal:") {
      total_memory = std::stof(value);
      count++;
    }
    if (key == "MemFree:") {
      free_memory = std::stof(value);
      count++;
    }
    if (count == 2) {
      break;
    }
  }

  return (total_memory - free_memory) / total_memory;
  
}

// NOT WORKING: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  // variables to store info
  long uptime, idle_time;
  string line;

  // Open the /proc/uptime file
  std::ifstream uptime_file(kProcDirectory + kUptimeFilename);

  // Get the first line of the file
  std::getline(uptime_file, line);

  // Create a string stream object
  std::istringstream is(line);

  // Get the uptime and idle time
  is >> uptime >> idle_time;

  // Close the file
  uptime_file.close();

  // Return the uptime
  return uptime;

}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{
   return UpTime() * sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{
  // variables to store info
  string line;
  string value;
  // total time = utime + stime + cutime + cstime
  long utime, stime, cutime, cstime, total_time;

  // Open the /proc/[pid]/stat file
  std::ifstream stat_file(kProcDirectory + std::to_string(pid) + kStatFilename);

  // Read in the file
  if (stat_file.is_open()) {
    std::getline(stat_file, line);
    std::istringstream is(line);
    for (int i = 0; i < 17; i++) {
      is >> value;
      if (i == 13) {
        utime = std::stol(value);
      }
      if (i == 14) {
        stime = std::stol(value);
      }
      if (i == 15) {
        cutime = std::stol(value);
      }
      if (i == 16) {
        cstime = std::stol(value);
      }
    }
  }

  // Close the file
  stat_file.close();

  // Calculate the total time
  total_time = utime + stime + cutime + cstime;

  // Return the total time
  return total_time;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
  // Using answer found on stackoverflow: https://bit.ly/3cPFsrg
  auto cpu_load = LinuxParser::CpuUtilization();
  vector<int> vecStates = {
    LinuxParser::kUser_,
    LinuxParser::kNice_,
    LinuxParser::kSystem_,
    LinuxParser::kIRQ_,
    LinuxParser::kSoftIRQ_,
    LinuxParser::kSteal_,
    LinuxParser::kGuest_,
    LinuxParser::kGuestNice_
  };

  // Calculate the nonidle time
  float nonidle = 0.0;
  for (int i : vecStates) {
    nonidle += std::stof(cpu_load[i]);
  }

  // Return the number of active jiffies
  return nonidle;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{
  // Using answer found on stackoverflow: https://bit.ly/3cPFsrg
  //variables to store info
  auto cpu_load = LinuxParser::CpuUtilization();
  vector<int> vecStates = {
    LinuxParser::kIdle_,
    LinuxParser::kIOwait_
  };

  // Calculate the idle time
  float idle = 0.0;
  for (int i : vecStates) {
    idle += std::stof(cpu_load[i]);
  }

  // Return the number of idle jiffies
  return idle;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{
  // variables to store info
  string line;
  string key, value;
  vector<string> cpu_utilization;

  // Open the /proc/stat file
  std::ifstream stat_file(kProcDirectory + kStatFilename);

  // Get the first line of the file
  std::getline(stat_file, line);

  // Create a string stream object
  std::istringstream is(line);

  // Loop through the file
  while (is >> key) {
    if (key == "cpu") {
      while (is >> value) {
        cpu_utilization.push_back(value);
      }
    }
  }

  // Close the file
  stat_file.close();

  // Return the CPU utilization
  return cpu_utilization;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{
  // variables to store info
  string line;
  string key, value;
  int total_processes = 0;

  // Open the /proc/stat file
  std::ifstream stat_file(kProcDirectory + kStatFilename);

  // Loop through the file
  while (std::getline(stat_file, line)) {
    std::istringstream is(line);
    is >> key >> value;
    if (key == "processes") {
      total_processes = std::stoi(value);
      break;
    }
  }

  // Close the file
  stat_file.close();

  // Return the total number of processes
  return total_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{
  // variables to store info
  string line;
  string key, value;
  int running_processes;

  // Open the /proc/stat file
  std::ifstream stat_file(kProcDirectory + kStatFilename);

  // Loop through the file
  while (std::getline(stat_file, line)) {
    std::istringstream is(line);
    is >> key >> value;
    if (key == "procs_running") {
      running_processes = std::stoi(value);
      break;
    }
  }

  // Close the file
  stat_file.close();

  // Return the number of running processes
  return running_processes;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) 
{ 
  // variables to store info
  string line;
  string command;

  // Open the /proc/[pid]/cmdline file
  std::ifstream command_file(kProcDirectory + std::to_string(pid) + kCmdlineFilename);

  // Get the first line of the file
  std::getline(command_file, line);

  // Close the file
  command_file.close();

  // Return the command
  return line;

}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{ 
  // variables to store info
  string line;
  string key, value;

  // Open the /proc/[pid]/status file
  std::ifstream status_file(kProcDirectory + std::to_string(pid) + kStatusFilename);

  // Loop through the file
  while (std::getline(status_file, line)) {
    std::istringstream is(line);
    is >> key >> value;
    if (key == "VmSize:") {
      return value;
    }
  }

  // Close the file
  status_file.close();

  // Return the memory used by the process
  return value;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{ 
  // variables to store info
  string line;
  string key, value;

  // Open the /proc/[pid]/status file
  std::ifstream status_file(kProcDirectory + std::to_string(pid) + kStatusFilename);

  // Loop through the file
  while (std::getline(status_file, line)) {
    std::istringstream is(line);
    is >> key >> value;
    if (key == "Uid:") {
      return value;
    }
  }

  // Close the file
  status_file.close();

  // Return the user ID
  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
  // variables to store info
  string line;
  string user, x, uid;
  string key, value;

  // Get the user ID
  uid = Uid(pid);

  // Open the /etc/passwd file
  std::ifstream passwd_file(kPasswordPath);

  // Loop through the file
  while (std::getline(passwd_file, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream is(line);
    is >> user >> x >> key;
    if (key == uid) {
      return user;
    }
  }

  // Close the file
  passwd_file.close();

  // Return the user
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{ 
  // variables to store info
  string line;
  string value;
  string pid_str = Uid(pid);
  float jiffies = 0.0;

  // Open the /proc/[pid]/stat file
  std::ifstream stat_file(kProcDirectory + std::to_string(pid) + kStatFilename);

  // Loop through the file
  std::getline(stat_file, line);
  std::istringstream is(line);
  for (int i = 0; i < 22; i++) {
    is >> value;
    if (i == 21) {
      jiffies = std::stof(value);
    }
  }

  // Close the file
  stat_file.close();

  // Return the uptime of the process
  return (UpTime() - (jiffies / sysconf(_SC_CLK_TCK)));
}
