/*!
\file src/terralib_utils/ExchangeService.h

\brief This file has class used to Exchange data between data sourcers.
*/

#ifndef __TERRALIB_UTILS_INTERNAL_EXCHANGESERVICE_H
#define __TERRALIB_UTILS_INTERNAL_EXCHANGESERVICE_H

#include "Config.h"

//STL
#include <string>

namespace te
{
  namespace utils
  {
    class TETERRALIBUTILSEXPORT ExchangeService
    {
       public:

        ExchangeService();

        ~ExchangeService();

      public:

        void toDatabase(const std::string& filePath, const std::string& connInfo, const std::string& dsType, const std::string& tableName, const int& srid, const std::string& charEncoding, const std::string& whereClause = "");

        void toMosaic(const std::string& filePath, const std::string& connInfo, const std::string& dsType, const std::string& charEncoding, const std::string& tableName, const int& srid, const std::string& whereClause = "");

        void toFile(const std::string& connInfo, const std::string& dsType, const std::string& tableName, const std::string& filePath, const std::string& charEncoding, const int& srid, const std::string& whereClause = "");

    };
  } //utils
}   //te

#endif  // __TERRALIB_UTILS_INTERNAL_EXCHANGESERVICE_H
