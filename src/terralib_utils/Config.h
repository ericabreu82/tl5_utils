/*!
  \file src/terralib_utils/Config.h

  \brief Configuration flags for the TerraLib Utils module.
*/

#ifndef __TERRALIB_UTILS_INTERNAL_CONFIG_H
#define __TERRALIB_UTILS_INTERNAL_CONFIG_H

/** @name DLL/LIB Module
 *  Flags for building TerraLib Utils as a DLL or as a Static Library
 */
//@{

/*!
  \def TETERRALIBUTILSEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib Utils as DLL in Windows, remember to insert TETERRALIBUTILSDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TETERRALIBUTILSSTATIC flag into the project list of defines.
*/
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4290 )
  #endif

  #ifdef TETERRALIBUTILSSTATIC
    #define TETERRALIBUTILSEXPORT                          // Don't need to export/import... it is a static library
  #elif TETERRALIBUTILSDLL
    #define TETERRALIBUTILSEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TETERRALIBUTILSEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
    #define TETERRALIBUTILSEXPORT
#endif

//@}

#endif  // __TERRALIB_UTILS_INTERNAL_CONFIG_H
