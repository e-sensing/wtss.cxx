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
  rapidjson::Document doc(wtss_cxx::json_request(wtss_cxx::wtss::server_uri+"/list_coverages?output_format=json"));

  if(!doc.IsObject())
      throw parse_error() << error_description("Invalid JSON document: expecting a object!");

  if(!doc.HasMember("coverages"))
    throw parse_error() << error_description("Invalid JSON document: expecting a member named \"coverages\"!");

  const rapidjson::Value& j_coverages = doc["coverages"];

  if(!j_coverages.IsArray())
    throw parse_error() << error_description("Invalid JSON document: member named \"coverages\" must be an array!");

  std::vector<std::string> result;

  for (rapidjson::Value::ConstValueIterator itr = j_coverages.Begin(); itr != j_coverages.End(); ++itr)
    result.push_back(itr->GetString());

  return result;
}

wtss_cxx::geoarray_t
wtss_cxx::wtss::describe_coverage(const std::string& cv_name) const
{

  std::string query_string("/describe_coverage?coverage="+ cv_name);
  rapidjson::Document doc(wtss_cxx::json_request(wtss_cxx::wtss::server_uri + query_string + "&output_format=json"));

  if(!doc.IsObject())
    throw parse_error() << error_description("Invalid JSON document: expecting a object!");

  wtss_cxx::geoarray_t result;

  if(!doc.HasMember("coverage"))
    throw parse_error() << error_description("Invalid JSON document: expecting a member named \"coverage\"!");

  result.name = doc["coverage"].GetString();

  if(!doc.HasMember("description"))
    throw parse_error() << error_description("Invalid JSON document: expecting a member named \"description\"!");

  result.description = doc["description"].GetString();

  if(!doc.HasMember("detail"))
    throw parse_error() << error_description("Invalid JSON document: expecting a member named \"detail\"!");

  result.detail = doc["detail"].GetString();

  if(!doc.HasMember("dimensions"))
    throw parse_error() << error_description("Invalid JSON document: expecting a member named \"dimensions\"!");

  const rapidjson::Value& j_dimensions = doc["dimensions"];

  if(!doc.HasMember("attributes"))
    throw parse_error() << error_description("Invalid JSON document: expecting a member named \"attributes\"!");

  const rapidjson::Value& j_attributes = doc["attributes"];

  if(!j_dimensions.IsArray())
    throw parse_error() << error_description("Invalid JSON document: member named \"dimensions\" must be an array!");

  if(!j_attributes.IsArray())
    throw parse_error() << error_description("Invalid JSON document: member named \"attributes\" must be an array!");

   for (rapidjson::Value::ConstValueIterator itr = j_dimensions.Begin(); itr != j_dimensions.End(); ++itr)
   {
     wtss_cxx::dimension_t dimension;

     dimension.name = (*itr)["name"].GetString();
     dimension.description = (*itr)["description"].GetString();
     dimension.max_idx = (*itr)["max_idx"].GetInt64();
     dimension.min_idx = (*itr)["min_idx"].GetInt64();

     result.dimensions.push_back(dimension);
   }

  for (rapidjson::Value::ConstValueIterator itr = j_attributes.Begin(); itr != j_attributes.End(); ++itr)
  {
    wtss_cxx::attribute_t attribute;

    attribute.name = (*itr)["name"].GetString();
    attribute.description = (*itr)["description"].GetString();
    attribute.scale_factor = (*itr)["scale_factor"].GetDouble();
    attribute.missing_value = (*itr)["missing_value"].GetDouble();
    attribute.datatype = wtss_cxx::datatype_t::from_string((*itr)["datatype"].GetString());
    attribute.valid_range.max_val = (*itr)["valid_range"]["max"].GetDouble();
    attribute.valid_range.min_val = (*itr)["valid_range"]["min"].GetDouble();

    result.attributes.push_back(attribute);
  }

  return result;
}

wtss_cxx::timeseries_query_result_t
wtss_cxx::wtss::time_series(const timeseries_query_t& query) const
{
  timeseries_query_result_t result;  
  wtss_cxx::queried_attribute_t attribute;
  std::string query_string("/time_series?coverage="+query.coverage_name+
                           "&attributes="+boost::algorithm::join(query.attributes, ",")+
                           "&longitude="+boost::lexical_cast<std::string>(query.longitude)+
                           "&latitude="+boost::lexical_cast<std::string>(query.latitude));

  result.coverage.name = query.coverage_name;
  result.query = query;

  if(!query.start_date.empty())
     query_string.append("&start="+query.start_date);

  if(!query.end_date.empty())
     query_string.append("&end="+query.start_date);

  rapidjson::Document doc(wtss_cxx::json_request(wtss_cxx::wtss::server_uri+query_string+"&output_format=json"));

  if(!doc.HasMember("result"))
    throw parse_error() << error_description("Invalid JSON document: expecting a member named \"result\"!");

  const rapidjson::Value& j_result = doc["result"];

  if(!j_result.HasMember("attributes"))
    throw parse_error() << error_description("Invalid JSON document: expecting a member named \"attributes\"!");

  const rapidjson::Value& j_attributes = j_result["attributes"];

  for (rapidjson::Value::ConstValueIterator itr = j_attributes.Begin(); itr != j_attributes.End(); ++itr)
  {
     attribute.name = (*itr)["attribute"].GetString();
     attribute.values.clear();
     if(!(*itr)["values"].IsNull())
       if((*itr)["values"].IsArray())
         for (rapidjson::Value::ConstValueIterator itr_v = (*itr)["values"].Begin(); itr_v != (*itr)["values"].End(); ++itr_v)
           attribute.values.push_back(itr_v->GetDouble());

     result.coverage.queried_attributes.push_back(attribute);
  }

  if(!j_result.HasMember("timeline"))
    throw parse_error() << error_description("Invalid JSON document: expecting a member named \"timeline\"!");

  const rapidjson::Value& j_timeline = j_result["timeline"];

  if(!j_timeline.IsArray())
    throw parse_error() << error_description("Invalid JSON document: member named \"timeline\" must be an array!");

  for(rapidjson::Value::ConstValueIterator itr = j_timeline.Begin(); itr != j_timeline.End(); ++itr)
  {
    const rapidjson::Value& j_date = (*itr);
    std::vector<std::string> date_split;
    std::string timeline = j_date.GetString();//j_date.GetString();
    boost::algorithm::split(date_split,timeline,boost::algorithm::is_any_of("-"));
    date d;
    d.day   = boost::lexical_cast<int>(date_split[2]);
    d.month = boost::lexical_cast<int>(date_split[1]);
    d.year  = boost::lexical_cast<int>(date_split[0]);
    result.coverage.timeline.push_back(d);
  }

  return result;
}
