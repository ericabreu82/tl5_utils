/*!
\file terralib_utils_app/main.cpp

\brief It contains the main routine of terralib_utils_app.
*/

// TerraLib Utils
#include "../terralib_utils/ExchangeService.h"

// STL
#include <cstdlib>

int main(int argc, char** argv)
{
  int waitVal = EXIT_FAILURE;

  //get arguments
  std::string filePath = "";
  std::string connInfo = "";
  std::string charEncoding = "";

  try
  {
    te::utils::ExchangeService es;

    es.execute(filePath, connInfo, charEncoding);

    waitVal = EXIT_SUCCESS;
  }
  catch (const std::exception&)
  {
    return EXIT_FAILURE;
  }
  catch (...)
  {
    return EXIT_FAILURE;
  }

  return waitVal;
}
