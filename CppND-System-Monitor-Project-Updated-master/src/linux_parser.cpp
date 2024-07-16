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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

  // variables to store info
  string mem_total, mem_free, mem_avail, buffers, cached;
  string line;
  string key, value;
  int line_number = 0;
  float total_mem, free_mem, avail_mem, buffer_mem, cached_mem, total_used = 0;
  float total_mem_mb;

  // Open the /proc/meminfo file
  std::ifstream mem_file(kProcDirectory + kMeminfoFilename);

  // Loop through the first 5 lines to get the required data
  while (line_number < 5 && std::getline(mem_file, line)) {
    std::istringstream is(line);
    std::getline(is, key, ':');
    std::getline(is, value);
    switch (line_number) {
      case 0:
        mem_total = key;
        total_mem = std::stof(value);
        total_mem_mb = (total_mem / 1000);
        break;
      case 1:
        mem_free = key;
        free_mem = std::stof(value);
        break;
      case 2:
        mem_avail = key;
        avail_mem = std::stof(value);
        break;
      case 3:
        buffers = key;
        buffer_mem = std::stof(value);
        break;
      case 4:
        cached = key;
        cached_mem = std::stof(value);
        break;
      default:
        break;
    }
    // Increase the line number by 1
    line_number++;
  }
  return total_mem_mb;
  
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  // Open the /proc/uptime file
  string uptime, idletime;
  string line;
  long uptime_val = 0;

  // Get the first line of data
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  stream.close();

  // Return the value
  uptime_val = std::stol(uptime);

  return uptime_val;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
