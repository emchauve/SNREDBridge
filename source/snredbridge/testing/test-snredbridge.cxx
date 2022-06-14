#include <iostream>
#include <cstdlib>
#include <exception>
#include <string>

// SNREDBridge:
#include <snredbridge/snredbridge.h>

int main(void)
{
  snredbridge::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    if (snredbridge::is_initialized())
      std::clog << "SNREDBridge module is initialized !" << std::endl;
  } catch (std::exception & err) {
    std::cerr << "[error] " << err.what() << std::endl;
  } catch (...) {
    std::cerr << "[error] " << "unexpected error!" << std::endl;
  }
  snredbridge::terminate();
  return error_code;
}
