#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

using std::vector;
using std::string;
class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

    

  // TODO: Declare any necessary private members
 private:
    vector<string> cpuUtilization_;

};

#endif