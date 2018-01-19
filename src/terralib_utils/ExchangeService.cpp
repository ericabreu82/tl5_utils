/*!
\file src/terralib_utils/ExchangeService.cpp

\brief This file has class used to Exchange data between data sourcers.
*/

#include "ExchangeService.h"

// TerraLib includes
#include <terralib/common/StringUtils.h>
#include <terralib/dataaccess/dataset/DataSetAdapter.h>
#include <terralib/dataaccess/dataset/PrimaryKey.h>
#include <terralib/dataaccess/dataset/PrimaryKey.h>
#include <terralib/dataaccess/dataset/DataSetTypeConverter.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/datasource/DataSourceInfo.h>
#include <terralib/dataaccess/datasource/DataSourceInfoManager.h>
#include <terralib/dataaccess/datasource/DataSourceManager.h>
#include <terralib/dataaccess/datasource/DataSourceTransactor.h>
#include <terralib/dataaccess/utils/Utils.h>
#include <terralib/geometry/GeometryProperty.h>
#include <terralib/srs/Config.h>

// Boost
#include <boost/algorithm/string/replace.hpp>

// STL
#include <string>

te::utils::ExchangeService::ExchangeService()
{

}

te::utils::ExchangeService::~ExchangeService()
{

}

void te::utils::ExchangeService::toDatabase(const std::string& filePath, const std::string& connInfo, const std::string& dsType, const std::string& tableName, const int& srid, const std::string& charEncoding, const std::string& whereClause)
{
  //open input data source
  std::string inputConnInfo("file://");
  inputConnInfo += filePath;

  std::unique_ptr<te::da::DataSource> inputDataSource = te::da::DataSourceFactory::make("OGR", inputConnInfo);
  inputDataSource->open();

  //get srid information
  int inputSRID = srid;
  int outputSRID = srid;

  std::auto_ptr<te::da::DataSourceTransactor> transactor;

  try
  {
    //get first dataset and create adapter
    std::vector<std::string> dataSetNames = inputDataSource->getDataSetNames();

    std::auto_ptr<te::da::DataSetType> dataSetType = inputDataSource->getDataSetType(dataSetNames[0]);

    std::unique_ptr<te::da::DataSource> outputDataSource = te::da::DataSourceFactory::make(dsType, connInfo);
    outputDataSource->open();

    transactor = outputDataSource->getTransactor();

    te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(dataSetType.get(), outputDataSource->getCapabilities(), outputDataSource->getEncoding());

    te::da::AssociateDataSetTypeConverterSRID(converter, inputSRID, outputSRID);

    te::da::DataSetType* dsTypeResult = converter->getResult();

    dsTypeResult->setName(tableName);

    // Check dataset name
    if (!outputDataSource->isDataSetNameValid(dsTypeResult->getName()))
    {
      bool aux;
      std::string newName = te::common::ReplaceSpecialChars(dsTypeResult->getName(), aux);
      dsTypeResult->setName(newName);
    }

    // Check properties names
    std::vector<te::dt::Property* > props = dsTypeResult->getProperties();
    std::map<std::size_t, std::string> invalidNames;
    for (std::size_t i = 0; i < props.size(); ++i)
    {
      if (!outputDataSource->isPropertyNameValid(props[i]->getName()))
      {
        invalidNames[i] = props[i]->getName();
      }
    }

    std::map<std::size_t, std::string>::iterator it = invalidNames.begin();

    while (it != invalidNames.end())
    {
      bool aux;
      std::string newName = te::common::ReplaceSpecialChars(it->second, aux);

      props[it->first]->setName(newName);

      ++it;
    }

    //fix repeated names
    std::set<std::string> names;

    props = dsTypeResult->getProperties();

    for (std::size_t i = 0; i < props.size(); ++i)
    {
      //check if the property name its duplicated
      std::string propName = props[i]->getName();

      int count = 1;
      while (names.find(te::common::Convert2UCase(propName)) != names.end())
      {
        propName += "_";
        propName += te::common::Convert2String(count);
      }

      names.insert(te::common::Convert2UCase(propName));

      props[i]->setName(propName);
    }

    //create index
    te::gm::GeometryProperty* p = te::da::GetFirstGeomProperty(dsTypeResult);

    if (p)
    {
      te::da::Index* idx = new te::da::Index(dsTypeResult);

      std::string name = std::string(tableName) + "_" + p->getName() + "_idx";

      boost::replace_all(name, ".", "_");

      idx->setName(name);
      idx->setIndexType(te::da::R_TREE_TYPE);

      te::dt::Property* pClone = p->clone();

      idx->add(pClone);
    }

    //create primary key
    if (dataSetType->getPrimaryKey())
    {
      te::da::PrimaryKey* pk = new te::da::PrimaryKey(dsTypeResult);

      std::string name = std::string(tableName) + "_" + dataSetType->getPrimaryKey()->getName() + "_pk";

      boost::replace_all(name, ".", "_");

      pk->setName(name);

      std::vector<te::dt::Property*> props = dataSetType->getPrimaryKey()->getProperties();

      for (size_t t = 0; t < props.size(); ++t)
      {
        te::dt::Property* p = props[t]->clone();

        pk->add(p);
      }
    }

    //exchange
    std::map<std::string, std::string> nopt;

    std::auto_ptr<te::da::DataSet> dataset = inputDataSource->getDataSet(dataSetNames[0]);

    transactor->begin();

    transactor->createDataSet(dsTypeResult, nopt);

    std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(dataset.get(), converter));

    if (dataset->moveBeforeFirst())
      transactor->add(dsTypeResult->getName(), dsAdapter.get(), nopt);

    transactor->commit();
  }
  catch (const std::exception& )
  {
    if (transactor.get())
      transactor->rollBack();
  }
}

void te::utils::ExchangeService::toMosaic(const std::string& filePath, const std::string& connInfo, const std::string& dsType, const std::string& charEncoding, const std::string& tableName, const int& srid, const std::string& whereClause)
{

}

void te::utils::ExchangeService::toFile(const std::string& connInfo, const std::string& dsType, const std::string& tableName, const std::string& filePath, const std::string& charEncoding, const int& srid, const std::string& whereClause)
{
  //open input data source
  std::string inputConnInfo = connInfo;

  std::unique_ptr<te::da::DataSource> inputDataSource = te::da::DataSourceFactory::make(dsType, inputConnInfo);
  inputDataSource->open();

  //get srid information
  int inputSRID = srid;
  int outputSRID = srid;

  try
  {
    //create adapter
    std::auto_ptr<te::da::DataSetType> dsType = inputDataSource->getDataSetType(tableName);


    //create data source
    std::string connInfo("file://" + std::string(filePath));

    std::unique_ptr<te::da::DataSource> dsOGR = te::da::DataSourceFactory::make("OGR", connInfo);

    dsOGR->open();

    te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(dsType.get(), dsOGR->getCapabilities(), dsOGR->getEncoding());

    te::da::AssociateDataSetTypeConverterSRID(converter, inputSRID, outputSRID);

    te::da::DataSetType* dsTypeResult = converter->getResult();

    dsTypeResult->setName(tableName);

    // Check dataset name
    if (!dsOGR->isDataSetNameValid(dsTypeResult->getName()))
    {
        bool aux;
        std::string newName = te::common::ReplaceSpecialChars(dsTypeResult->getName(), aux);
        dsTypeResult->setName(newName);
    }

    // Check properties names
    std::vector<te::dt::Property* > props = dsTypeResult->getProperties();
    std::map<std::size_t, std::string> invalidNames;
    for (std::size_t i = 0; i < props.size(); ++i)
    {
      if (!dsOGR->isPropertyNameValid(props[i]->getName()))
      {
        invalidNames[i] = props[i]->getName();
      }
    }

    if (!invalidNames.empty())
    {
      std::map<std::size_t, std::string>::iterator it = invalidNames.begin();

      while (it != invalidNames.end())
      {
        bool aux;
        std::string newName = te::common::ReplaceSpecialChars(it->second, aux);

        props[it->first]->setName(newName);

        ++it;
      }
    }

    //exchange
    std::map<std::string, std::string> nopt;

    std::auto_ptr<te::da::DataSet> dataset = inputDataSource->getDataSet(tableName);

    dsOGR->createDataSet(dsTypeResult, nopt);

    std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(dataset.get(), converter));

    if (dataset->moveBeforeFirst())
      dsOGR->add(dsTypeResult->getName(), dsAdapter.get(), nopt);

    dsOGR->close();
  }
  catch (const std::exception& )
  {

  }

  return;
}
