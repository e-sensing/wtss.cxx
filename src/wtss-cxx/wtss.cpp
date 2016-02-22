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

//cpp-netlib

#include <boost/network/protocol/http/client.hpp>

// RapidJSON

#include <rapidjson/document.h>
#include <rapidjson/filestream.h>

//Boost
#include <boost/format.hpp>

namespace http = boost::network::http;

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

  http::client client;
  http::client::request request(wtss_cxx::wtss::server_uri+"/mds/product_list?output_format=json");
  http::client::response response = client.get(request);

  std::string json = body(response);

  rapidjson::Document doc;
  rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

  doc.Parse<0>(json.c_str());
  if(doc.HasParseError()){
      throw ;
  }
  if(!doc.IsObject()){
      throw ;
  }
  if(!doc.HasMember("products")){
      throw;
  }
  if(!doc["products"].IsArray()){
      throw;
  }

  for (rapidjson::Value::ConstValueIterator itr = doc["products"].Begin(); itr != doc["products"].End(); ++itr)

    result.push_back(itr->GetString());

  return result;
}

wtss_cxx::geoarray_t
wtss_cxx::wtss::describe_coverage(const std::string& cv_name) const
{

  wtss_cxx::geoarray_t result;

  return result;
}

wtss_cxx::timeseries_query_result_t
wtss_cxx::wtss::time_series(const timeseries_query_t& /*query*/) const
{
  timeseries_query_result_t result;

  return result;
}
