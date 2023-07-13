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
