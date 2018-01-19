/*!
\file src/terralib_utils/TerraLibService.h

\brief This file has class used to start TerraLib services.
*/

#ifndef __TERRALIB_UTILS_INTERNAL_TERRALIBSERVICE_H
#define __TERRALIB_UTILS_INTERNAL_TERRALIBSERVICE_H

#include "Config.h"

//STL
#include <string>

namespace te
{
  namespace utils
  {
    class TETERRALIBUTILSEXPORT TerraLibService
    {
       public:

         TerraLibService();

         ~TerraLibService();

      public:

        static void startTerraLib();

        static void loadModules();

        static void finalizeTerraLib();

    };
  } //utils
}   //te

#endif  // __TERRALIB_UTILS_INTERNAL_EXCHANGESERVICE_H
