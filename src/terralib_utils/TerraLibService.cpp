/*!
\file src/terralib_utils/TerraLibService.h

\brief This file has class used to start TerraLib services.
*/

#include "TerraLibService.h"

// TerraLib includes
#include <terralib/common/TerraLib.h>
#include <terralib/core/plugin.h>
#include <terralib/core/plugin/Finders.h>
#include <terralib/core/utils/Platform.h>

// STL
#include <string>

te::utils::TerraLibService::TerraLibService()
{

}

te::utils::TerraLibService::~TerraLibService()
{

}

void te::utils::TerraLibService::startTerraLib()
{
  // Initialize the Terralib support
  TerraLib::getInstance().initialize();
}

void te::utils::TerraLibService::loadModules()
{
  std::vector<te::core::PluginInfo> v_pInfo = te::core::DefaultPluginFinder();
  v_pInfo = te::core::plugin::TopologicalSort(v_pInfo);

  for (const te::core::PluginInfo& pinfo : v_pInfo)
  {
    try
    {
      te::core::PluginManager::instance().insert(pinfo);
      te::core::PluginManager::instance().load(pinfo.name);
    }
    catch (...)
    {
    }
  }
}

void te::utils::TerraLibService::finalizeTerraLib()
{
  // Finalize the Terralib support
  te::core::PluginManager::instance().clear();

  TerraLib::getInstance().finalize();
}
