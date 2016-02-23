/*
  Copyright (C) 2016 National Institute For Space Research (INPE) - Brazil.

  This file is part of the WTSS.CXX.

  WTSS.CXX is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3 as
  published by the Free Software Foundation.

  WTSS.CXX is distributed  "AS-IS" in the hope that it will be useful,
  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along
  with WTSS.CXX. See COPYING. If not, see <http://www.gnu.org/licenses/lgpl-3.0.html>.
 */

/*!
  \file wtss-cxx/wtss.cpp

  \brief The Web Time Series Service Client API for C++.

  \author Matheus Cavassan Zaglia
  \author Gilberto Ribeiro de Queiroz

 */

// WTSS.CXX
#include "wtss.hpp"
#include "exception.hpp"
#include "utils.hpp"
#include "data_types.hpp"

// RapidJSON
#include <rapidjson/document.h>
#include <rapidjson/filestream.h>

//Boost
#include <boost/lexical_cast.hpp>

//STL
#include <string>

wtss_cxx::wtss::wtss(const std::string& server_uri)
{
    wtss_cxx::wtss::server_uri = server_uri;
}

wtss_cxx::wtss::~wtss()
{
}

std::vector<std::string>
wtss_cxx::wtss::list_coverages() const
{
  std::vector<std::string> result;

  rapidjson::Document doc(wtss_cxx::json_request(wtss_cxx::wtss::server_uri+"/mds/product_list?output_format=json"));
  rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

//  if(!doc.IsObject())
//  {
//      throw ;
//  }
//  if(!doc.HasMember("products"))
//  {
//      throw;
//  }
//  if(!doc["products"].IsArray()){
//      throw;
//  }

  for (rapidjson::Value::ConstValueIterator itr = doc["products"].Begin(); itr != doc["products"].End(); ++itr)
    result.push_back(itr->GetString());

  return result;
}

wtss_cxx::geoarray_t
wtss_cxx::wtss::describe_coverage(const std::string& cv_name) const
{
  wtss_cxx::geoarray_t result;
  
  result.name = cv_name;

  rapidjson::Document doc(wtss_cxx::json_request(wtss_cxx::wtss::server_uri+"/mds/dataset_list?product="
                                                 +cv_name+"&output_format=json"));
  rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

//  if(!doc.IsObject())
//  {
//      throw ;
//  }
//  if(!doc.HasMember("datasets"))
//  {
//      throw;
//  }
//  if(!doc["datasets"].IsArray())
//  {
//      throw;
//  }
  for (rapidjson::Value::ConstValueIterator itr = doc["datasets"].Begin(); itr != doc["datasets"].End(); ++itr)
  {
    rapidjson::Document c_desc(wtss_cxx::json_request(wtss_cxx::wtss::server_uri+"/mds/dataset_metadata?product="
                                                      +cv_name+"&dataset="+itr->GetString()+"&output_format=json"));
    if(!c_desc.IsObject())
    {
        continue ;
    }
    wtss_cxx::attribute_t attribute_t;
//    wtss_cxx::dimension_t dimension_t;

    attribute_t.name = itr->GetString();
    attribute_t.description = c_desc["src_subdataset_info"]["description"].GetString();
    attribute_t.scale_factor = c_desc["band_metadata"]["scale_factor"].GetDouble();
    attribute_t.missing_value = c_desc["band_metadata"]["nodata"].GetDouble();
    attribute_t.datatype = wtss_cxx::datatype_t::from_string(c_desc["band_metadata"]["datatype"].GetString());

//    result.dimensions.push_back(dimension_t);
    result.attributes.push_back(attribute_t);
  }
  
  return result;
}

wtss_cxx::timeseries_query_result_t
wtss_cxx::wtss::time_series(const timeseries_query_t& query) const
{
  timeseries_query_result_t result;
  std::string datasets;
  wtss_cxx::queried_attribute_t queried_attribute_t;
  for (std::vector<std::string>::const_iterator it = query.attributes.begin(); it != query.attributes.end();++it)
  {
      datasets.append(*it );
      if(query.attributes.end() != it + 1)
        datasets.append(",") ;
  }

  rapidjson::Document doc(wtss_cxx::json_request(wtss_cxx::wtss::server_uri+"/mds/query?product="
                                                  +query.coverage_name+"&datasets="+datasets+
                                                  "&longitude="+boost::lexical_cast<std::string>(query.longitude)+
                                                  "&latitude="+boost::lexical_cast<std::string>(query.latitude)+
                                                  "&output_format=json"));

  for (rapidjson::Value::ConstValueIterator itr = doc["result"]["datasets"].Begin(); itr != doc["result"]["datasets"].End(); ++itr)
  {
     const rapidjson::Value& dataset = (*itr);
     queried_attribute_t.name = dataset["dataset"].GetString();
     if(!dataset["values"].IsNull())
     {
       if(dataset["values"].IsArray())
         for (rapidjson::Value::ConstValueIterator itr_ = dataset["values"].Begin(); itr_ != dataset["values"].End(); ++itr_)
           queried_attribute_t.values.push_back(itr_->GetDouble());
       else
         queried_attribute_t.values.push_back(dataset["values"].GetDouble());
     }
     result.coverage.queried_attributes.push_back(queried_attribute_t);
  }
  result.query = query;
  return result;
}
