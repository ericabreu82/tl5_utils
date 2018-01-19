/*!
\file terralib_utils_app/main.cpp

\brief It contains the main routine of terralib_utils_app.
*/

// TerraLib Utils
#include "../terralib_utils/ExchangeService.h"
#include "../terralib_utils/TerraLibService.h"

// STL
#include <cstdlib>
#include <iostream>

int main(int argc, char** argv)
{
  int waitVal = EXIT_FAILURE;

  //get arguments
  std::string inputFilePath = "D:\\Temp\\br_estados.shp";
  std::string outputFilePath = "D:\\Temp\\br_estados_out.shp";
  std::string connInfo = "pgsql://postgres:tdk696@localhost:5432/bira?";
  std::string dsType = "POSTGIS";
  std::string outputTableName = "br_estados";
  std::string inputTableName = "br_estados";
  std::string charEncoding = "UTF-8";
  std::string whereClause = "";
  int srid = 4674; 

  try
  {
    //start terralib
    te::utils::TerraLibService::startTerraLib();

    //load plugins
    te::utils::TerraLibService::loadModules();
        
    //export example
    te::utils::ExchangeService es;

    //export to database
    es.toDatabase(inputFilePath, connInfo, dsType, outputTableName, srid, charEncoding, whereClause);

    //export to file
    es.toFile(connInfo, dsType, inputTableName, outputFilePath, charEncoding, srid, whereClause);

    //finalize terralib
    te::utils::TerraLibService::finalizeTerraLib();

    waitVal = EXIT_SUCCESS;
  }
  catch (const std::exception& e)
  {
    std::cout << e.what() << std::endl;

    return EXIT_FAILURE;
  }
  catch (...)
  {
    return EXIT_FAILURE;
  }

  return waitVal;
}
