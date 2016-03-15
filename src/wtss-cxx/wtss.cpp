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
#include <boost/algorithm/string.hpp>
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
  rapidjson::Document doc(wtss_cxx::json_request(wtss_cxx::wtss::server_uri+"/mds/product_list?output_format=json"));

  if(!doc.IsObject())
      throw parse_error() << error_description("Invalid JSON document: expecting a object!");

  if(!doc.HasMember("products"))
    throw parse_error() << error_description("Invalid JSON document: expecting a member named \"product\"!");

  const rapidjson::Value& j_products = doc["products"];

  if(!j_products.IsArray())
    throw parse_error() << error_description("Invalid JSON document: member named \"product\" must be an array!");

  std::vector<std::string> result;

  for (rapidjson::Value::ConstValueIterator itr = j_products.Begin(); itr != j_products.End(); ++itr)
    result.push_back(itr->GetString());

  return result;
}

wtss_cxx::geoarray_t
wtss_cxx::wtss::describe_coverage(const std::string& cv_name) const
{
  rapidjson::Document doc(wtss_cxx::json_request(wtss_cxx::wtss::server_uri + "/mds/dataset_list?product="
                                                 + cv_name + "&output_format=json"));

 if(!doc.IsObject())
     throw parse_error() << error_description("Invalid JSON document: expecting a object!");

  if(!doc.HasMember("datasets"))
    throw parse_error() << error_description("Invalid JSON document: expecting a member named \"datasets\"!");

  const rapidjson::Value& j_datasets = doc["datasets"];

  if(!j_datasets.IsArray())
    throw parse_error() << error_description("Invalid JSON document: member named \"datasets\" must be an array!");

  wtss_cxx::geoarray_t result;

  result.name = cv_name;

  for (rapidjson::Value::ConstValueIterator itr = j_datasets.Begin(); itr != j_datasets.End(); ++itr)
  {
    rapidjson::Document j_desc(wtss_cxx::json_request(wtss_cxx::wtss::server_uri+"/mds/dataset_metadata?product="
                                                      +cv_name+"&dataset="+itr->GetString()+"&output_format=json"));
    if(!j_desc.IsObject())
      continue;

    wtss_cxx::attribute_t attribute_t;
//    wtss_cxx::dimension_t dimension_t;

    attribute_t.name = itr->GetString();
    attribute_t.description = j_desc["src_subdataset_info"]["description"].GetString();
    attribute_t.scale_factor = j_desc["band_metadata"]["scale_factor"].GetDouble();
    attribute_t.missing_value = j_desc["band_metadata"]["nodata"].GetDouble();
    attribute_t.datatype = wtss_cxx::datatype_t::from_string(j_desc["band_metadata"]["datatype"].GetString());

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
        datasets.append(",");
  }
  rapidjson::Document doc(wtss_cxx::json_request(wtss_cxx::wtss::server_uri+"/mds/query?product="
                                                  +query.coverage_name+"&datasets="+datasets+
                                                  "&longitude="+boost::lexical_cast<std::string>(query.longitude)+
                                                  "&latitude="+boost::lexical_cast<std::string>(query.latitude)+
                                                  "&output_format=json"));

  const rapidjson::Value& j_doc = doc["result"]["datasets"];
  const rapidjson::Value& j_timeline = doc["result"]["timeline"];
  for (rapidjson::Value::ConstValueIterator itr = j_doc.Begin(); itr != j_doc.End(); ++itr)
  {
     const rapidjson::Value& j_dataset = (*itr);
     queried_attribute_t.name = j_dataset["dataset"].GetString();
     queried_attribute_t.values.clear();
     if(!j_dataset["values"].IsNull())
     {
       if(j_dataset["values"].IsArray())
         for (rapidjson::Value::ConstValueIterator itr_ = j_dataset["values"].Begin(); itr_ != j_dataset["values"].End(); ++itr_)
           queried_attribute_t.values.push_back(itr_->GetDouble());
       else
         queried_attribute_t.values.push_back(j_dataset["values"].GetDouble());
     }
     result.coverage.queried_attributes.push_back(queried_attribute_t);
  }
  for(rapidjson::Value::ConstValueIterator itr = j_timeline.Begin(); itr != j_timeline.End(); ++itr)
  {
    const rapidjson::Value& j_date = (*itr);
    std::vector<std::string> date_split;
    std::string timeline = j_date.GetString();
    boost::algorithm::split(date_split,timeline,boost::algorithm::is_any_of("-"));
    date d;
    d.day   = boost::lexical_cast<int>(date_split[2]);
    d.month = boost::lexical_cast<int>(date_split[1]);
    d.year  = boost::lexical_cast<int>(date_split[0]);
    result.coverage.timeline.push_back(d);
  }
  result.query = query;
  return result;
}
