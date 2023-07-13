# system_info

system_info provides an easy-to-use API for retrieving hardware information of your system components such as CPU, RAM, Disk.

## example1

```cpp
#include <iostream>
#include "sys_info.h"

int main() {
  SystemInfo sys_info;
  std::cout << "OS Name: " << sys_info.OSName() << std::endl;
  std::cout << "OS Version: " << sys_info.OSVersion() << std::endl;
  std::cout << "OS Architecture: " << sys_info.OSArchitecture() << std::endl;
  std::cout << "Total RAM: " << sys_info.totalRAM() << std::endl;
  std::cout << "CPU Model: " << sys_info.CPUModel()  << std::endl;
  std::cout << "CPU ID: " << sys_info.CPUID() << std::endl;
  std::cout << "DISK ID: " << sys_info.DiskID() << std::endl;

  return 0;
}
```

编译：

```shell
g++ -std=c++11 example1.cpp -o example1
```

运行结果：

```
OS Name: Linux
OS Version: 3.10.0-862.el7.x86_64
OS Architecture: x86_64
Total RAM (MB): 64157
CPU Model: Intel(R) Xeon(R) CPU E5-2620 v4 @ 2.10GHz
CPU ID: F1060400FFFBEBBF
DISK ID: 600605b005d10d3022c7d6730bb72d36
```

## example2

```cpp
#define CPU_INFO_IMPLEMENTATION
#include <iostream>
#include "cpu_info.h"

int main() {
  cpui_result result;

  int info_err = cpui_get_info(&result);

  if (info_err) {
    std::cerr << "An error occured when trying to get cpu info!\n error code: " 
              << cpui_error_strings[info_err] << std::endl;
    std::exit(EXIT_FAILURE);
  }
    
  std::cout << "vendor_string: " << result.vendor_string << std::endl;
  std::cout << "brand_string: " << result.brand_string << std::endl;
  std::cout << "physical_cores: " << result.physical_cores << std::endl;
  std::cout << "logical_cores: " << result.logical_cores << std::endl;
  std::cout << "cache_line_size: " << result.cache_line_size << std::endl;
  std::cout << "l1d_cache_size: " << result.l1d_cache_size << std::endl;
  std::cout << "l1i_cache_size: " << result.l1i_cache_size << std::endl;
  std::cout << "l2_cache_size: " << result.l2_cache_size << std::endl;
  std::cout << "l3_cache_size: " << result.l3_cache_size << std::endl;

  return 0;
}
```

编译：

```shell
g++ -std=c++11 example2.cpp -o example2
```

运行结果：

```
vendor_string: GenuineIntel
brand_string: Intel(R) Xeon(R) CPU E5-2620 v4 @ 2.10GHz
physical_cores: 8
logical_cores: 16
cache_line_size: 64
l1d_cache_size: 32768
l1i_cache_size: 32768
l2_cache_size: 262144
l3_cache_size: 20971520
```








