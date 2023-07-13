#include <iostream>
#include "sys_info.h"

int main() {
  SystemInfo sys_info;
  std::cout << "OS Name: " << sys_info.OSName() << std::endl;
  std::cout << "OS Version: " << sys_info.OSVersion() << std::endl;
  std::cout << "OS Architecture: " << sys_info.OSArchitecture() << std::endl;
  std::cout << "Total RAM (MB): " << sys_info.totalRAM() << std::endl;
  std::cout << "CPU Model: " << sys_info.CPUModel()  << std::endl;
  std::cout << "CPU ID: " << sys_info.CPUID() << std::endl;
  std::cout << "DISK ID: " << sys_info.DiskID() << std::endl;

  return 0;
}
