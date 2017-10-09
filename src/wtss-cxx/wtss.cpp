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
  with WTSS.CXX. See COPYING. If not, see
  <http://www.gnu.org/licenses/lgpl-3.0.html>.
 */

/*!
  \file wtss-cxx/wtss.cpp

  \brief The Web Time Series Service Client API for C++.

  \author Matheus Cavassan Zaglia
  \author Gilberto Ribeiro de Queiroz

 */

// WTSS.CXX
#include "wtss.hpp"
#include "data_types.hpp"
#include "exception.hpp"
#include "utils.hpp"

// Boost
#include <boost/format.hpp>

// RapidJSON
#include <rapidjson/document.h>
#include <rapidjson/filestream.h>

// STL
#include <numeric>
#include <sstream>
#include <string>

wtss::cxx::client::client(const std::string& server_uri)
{
  wtss::cxx::client::server_uri = server_uri;
}

wtss::cxx::client::~client() {}
std::vector<std::string> wtss::cxx::client::list_coverages() const
{

  rapidjson::Document doc;
  doc.Parse<0>(wtss::cxx::request(
    wtss::cxx::client::server_uri + "/list_coverages").c_str());

  if (doc.HasParseError())
  {
    boost::format err_msg("Error parsing requested document '%1%/list_coverages: %2%.");

    throw parse_error() << error_description(
      (err_msg % server_uri %doc.GetParseError()).str());
  }

  if (!doc.IsObject())
    throw parse_error() << error_description(
        "Invalid JSON document: expecting a object!");

  if (!doc.HasMember("coverages"))
    throw parse_error() << error_description(
        "Invalid JSON document: expecting a member named \"coverages\"!");

  const rapidjson::Value& j_coverages = doc["coverages"];

  if (!j_coverages.IsArray())
    throw parse_error() << error_description(
        "Invalid JSON document: member named \"coverages\" must be an array!");

  std::vector<std::string> result;

  for (rapidjson::Value::ConstValueIterator itr = j_coverages.Begin();
       itr != j_coverages.End(); ++itr)
    result.push_back(itr->GetString());

  return result;
}

wtss::cxx::geoarray_t wtss::cxx::client::describe_coverage(
    const std::string& cv_name) const
{


  std::string query_string("/describe_coverage?name=" + cv_name);
  rapidjson::Document doc;
  doc.Parse<0>(wtss::cxx::request(
    wtss::cxx::client::server_uri + query_string).c_str());

  if (doc.HasParseError())
  {
    boost::format err_msg("Error parsing requested document '%1%%2%': %3%.");

    throw parse_error() << error_description(
      (err_msg % server_uri % query_string %doc.GetParseError()).str());
  }

  if (!doc.IsObject())
    throw parse_error() << error_description(
        "Invalid JSON document: expecting a object!");

  wtss::cxx::geoarray_t result;

  if (!doc.HasMember("name"))
    throw parse_error() << error_description(
        "Invalid JSON document: expecting a member named \"coverage\"!");

  result.name = doc["name"].GetString();

  if (!doc.HasMember("description"))
    throw parse_error() << error_description(
        "Invalid JSON document: expecting a member named \"description\"!");

  result.description = doc["description"].GetString();

  if (!doc.HasMember("detail"))
    throw parse_error() << error_description(
        "Invalid JSON document: expecting a member named \"detail\"!");

  result.detail = doc["detail"].GetString();

  if (!doc.HasMember("dimensions"))
    throw parse_error() << error_description(
        "Invalid JSON document: expecting a member named \"dimensions\"!");

  const rapidjson::Value& j_dimensions = doc["dimensions"];

  if (!doc.HasMember("attributes"))
    throw parse_error() << error_description(
        "Invalid JSON document: expecting a member named \"attributes\"!");

  const rapidjson::Value& j_attributes = doc["attributes"];

  if (!j_dimensions.IsObject())
    throw parse_error() << error_description(
        "Invalid JSON document: member named \"dimensions\" must be an object!");

  if (!j_attributes.IsArray())
    throw parse_error() << error_description(
        "Invalid JSON document: member named \"attributes\" must be an array!");

  for (rapidjson::Value::ConstMemberIterator itr = j_dimensions.MemberBegin();
       itr != j_dimensions.MemberEnd(); ++itr)
  {
    wtss::cxx::dimension_t dimension;

    dimension.name = itr->value["name"].GetString();
    dimension.max_idx = itr->value["max_idx"].GetInt64();
    dimension.min_idx = itr->value["min_idx"].GetInt64();

    result.dimensions.push_back(dimension);
  }

  for (rapidjson::Value::ConstValueIterator itr = j_attributes.Begin();
       itr != j_attributes.End(); ++itr)
  {
    wtss::cxx::attribute_t attribute;

    attribute.name = (*itr)["name"].GetString();
    attribute.description = (*itr)["description"].GetString();
    attribute.scale_factor = (*itr)["scale_factor"].GetDouble();
    attribute.missing_value = (*itr)["missing_value"].GetDouble();
    attribute.datatype =
        wtss::cxx::datatype_t::from_string((*itr)["datatype"].GetString());
    attribute.valid_range.max_val = (*itr)["valid_range"]["max"].GetDouble();
    attribute.valid_range.min_val = (*itr)["valid_range"]["min"].GetDouble();

    result.attributes.push_back(attribute);
  }

  wtss::cxx::spatial_extent_t spatial_extent;

  spatial_extent.extent.xmin =
      doc["spatial_extent"]["xmin"].GetDouble();
  spatial_extent.extent.xmax =
       doc["spatial_extent"]["xmax"].GetDouble();
  spatial_extent.extent.ymin =
      doc["spatial_extent"]["ymin"].GetDouble();
  spatial_extent.extent.ymax =
       doc["spatial_extent"]["ymax"].GetDouble();
  spatial_extent.resolution.x =
      doc["spatial_resolution"]["x"].GetDouble();
  spatial_extent.resolution.y =
      doc["spatial_resolution"]["y"].GetDouble();

  result.spatial_extent = spatial_extent;

  if (!doc.HasMember("crs"))
    throw parse_error() << error_description(
        "Invalid JSON document: expecting a member named \"crs\"!");

  result.crs.proj4 = doc["crs"]["proj4"].GetString();
  result.crs.wkt = doc["crs"]["wkt"].GetString();

  if (!doc.HasMember("timeline"))
    throw parse_error() << error_description(
        "Invalid JSON document: expecting a member named \"timeline\"!");

  const rapidjson::Value& j_timeline = doc["timeline"];

  if (!j_timeline.IsArray())
    throw parse_error() << error_description(
        "Invalid JSON document: member named \"timeline\" must be an array!");

  for (rapidjson::Value::ConstValueIterator itr = j_timeline.Begin();
       itr != j_timeline.End(); ++itr)
  {
    const rapidjson::Value& j_date = (*itr);
    std::vector<std::string> date_split;
    std::string timeline = j_date.GetString();
    std::string delimiter = "-";
    size_t pos = 0;
    std::string token;
    while ((pos = timeline.find(delimiter)) != std::string::npos)
    {
      token = timeline.substr(0, pos);
      date_split.push_back(token);
      timeline.erase(0, pos + delimiter.length());
    }
    date_split.push_back(timeline);
    date d;

    if(date_split.size() > 0) d.year = std::stoi(date_split[0]);
    if(date_split.size() > 1) d.month = std::stoi(date_split[1]);
    if(date_split.size() > 2) d.day = std::stoi(date_split[2]);

    result.timeline.push_back(d);
  }

  return result;
}

wtss::cxx::timeseries_query_result_t wtss::cxx::client::time_series(
    const timeseries_query_t& query) const
{
  timeseries_query_result_t result;
  queried_attribute_t attribute;
  std::string attributes;
  attributes =
      std::accumulate(query.attributes.begin(), query.attributes.end(), attributes,
                 [](const std::string& a, const std::string& b) -> std::string {
                   return a.empty() ? b : a + "," + b;
                 });

  std::ostringstream strs;
  strs << query.latitude;
  std::string latitude = strs.str();
  strs.str("");
  strs << query.longitude;
  std::string longitude = strs.str();

  strs.str("");
  strs << query.start_date;
  std::string startDate = strs.str();
  strs.str("");
  strs << query.end_date;
  std::string endDate = strs.str();

  std::string query_string = "/time_series?coverage=" + query.coverage_name +
                             "&attributes=" + attributes + "&longitude=" +
                             longitude + "&latitude=" + latitude + "&start_date=" +
                             startDate + "&end_date=" + endDate;

  result.coverage.name = query.coverage_name;
  result.query = query;

  rapidjson::Document doc;
  doc.Parse<0>(wtss::cxx::request(
      wtss::cxx::client::server_uri + query_string).c_str());

  if (doc.HasParseError())
  {
    boost::format err_msg("Error parsing requested document '%1%%2%': %3%.");

    throw parse_error() << error_description(
      (err_msg % server_uri % query_string %doc.GetParseError()).str());
  }

  if (!doc.HasMember("result"))
    throw parse_error() << error_description(
        "Invalid JSON document: expecting a member named \"result\"!");
  

  const rapidjson::Value& j_result = doc["result"];

  if (!j_result.HasMember("attributes"))
    throw parse_error() << error_description(
        "Invalid JSON document: expecting a member named \"attributes\"!");

  const rapidjson::Value& j_attributes = j_result["attributes"];

  for (rapidjson::Value::ConstValueIterator itr = j_attributes.Begin();
       itr != j_attributes.End(); ++itr)
  {
    attribute.name = (*itr)["attribute"].GetString();
    attribute.values.clear();
    if (!(*itr)["values"].IsNull())
      if ((*itr)["values"].IsArray())
        for (rapidjson::Value::ConstValueIterator itr_v =
                 (*itr)["values"].Begin();
             itr_v != (*itr)["values"].End(); ++itr_v)
          attribute.values.push_back(itr_v->GetDouble());

    result.coverage.queried_attributes.push_back(attribute);
  }

  if (!j_result.HasMember("timeline"))
    throw parse_error() << error_description(
        "Invalid JSON document: expecting a member named \"timeline\"!");

  const rapidjson::Value& j_timeline = j_result["timeline"];

  if (!j_timeline.IsArray())
    throw parse_error() << error_description(
        "Invalid JSON document: member named \"timeline\" must be an array!");

  for (rapidjson::Value::ConstValueIterator itr = j_timeline.Begin();
       itr != j_timeline.End(); ++itr)
  {
    const rapidjson::Value& j_date = (*itr);
    std::vector<std::string> date_split;
    std::string timeline = j_date.GetString();
    std::string delimiter = "-";
    size_t pos = 0;
    std::string token;
    while ((pos = timeline.find(delimiter)) != std::string::npos)
    {
      token = timeline.substr(0, pos);
      date_split.push_back(token);
      timeline.erase(0, pos + delimiter.length());
    }
    date_split.push_back(timeline);
    date d;

    if(date_split.size() > 0) d.year = std::stoi(date_split[0]);
    if(date_split.size() > 1) d.month = std::stoi(date_split[1]);
    if(date_split.size() > 2) d.day = std::stoi(date_split[2]);

    result.coverage.timeline.push_back(d);
  }

  if (!j_result.HasMember("coordinates"))
    throw parse_error() << error_description(
        "Invalid JSON document: expecting a member named \"coordinates\"!");

  const rapidjson::Value& j_coordinates = j_result["coordinates"];

  result.coverage.coordinates.longitude =
      j_coordinates["longitude"].GetDouble();
  result.coverage.coordinates.latitude = j_coordinates["latitude"].GetDouble();
  result.coverage.coordinates.col = j_coordinates["col"].GetDouble();
  result.coverage.coordinates.row = j_coordinates["row"].GetDouble();

  return result;
}
