#include "processor.h"
#include "linux_parser.h"


// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
    // access and call the CpuUtilization() function from the LinuxParser namespace
    // to get the CPU utilization
    cpuUtilization_ = LinuxParser::CpuUtilization();

    // calculate the CPU utilization
    float idle = stof(cpuUtilization_[LinuxParser::kIdle_]) + stof(cpuUtilization_[LinuxParser::kIOwait_]);
    float nonIdle = stof(cpuUtilization_[LinuxParser::kUser_]) + stof(cpuUtilization_[LinuxParser::kNice_]) + stof(cpuUtilization_[LinuxParser::kSystem_]) + stof(cpuUtilization_[LinuxParser::kIRQ_]) + stof(cpuUtilization_[LinuxParser::kSoftIRQ_]) + stof(cpuUtilization_[LinuxParser::kSteal_]);
    float total = idle + nonIdle;

    return (total - idle) / total;
}