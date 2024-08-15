#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

using std::vector;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid() const;                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization() const;                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  int getPid() const {
    return pid_;
  }

  void setPid(int pid) {
    pid_ = pid;
  }

  // TODO: Declare any necessary private members
 private:
    int pid_;
    float cpu_;
    std::vector<int> pids_;

};

#endif