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
  \file wtss-cxx/exception.hpp

  \brief Implementation of util class in WTSS.CXX.

  \author Matheus Cavassan Zaglia
 */

// wtss.cxx
#include "utils.hpp"
#include "exception.hpp"

//cpp-netlib
#include <boost/network/protocol/http/client.hpp>

// Boost
#include <boost/format.hpp>

struct http
{
  enum status_type
  {
    continue_http = 100,
    switching_protocols = 101,
    ok = 200,
    no_content = 204,
    bad_request = 400,
    unauthorized = 401,
    forbidden = 403,
    not_found = 404,
    method_not_allowed = 405,
    not_acceptable = 406,
    request_timeout = 408,
    internal_server_error = 500,
    not_implemented = 501,
    bad_gateway = 502,
    service_unavailable = 503,
    gateway_timeout = 504,
    http_version_not_supported = 505
  };
  
  struct headers
  {
    static const std::string content_type;
  };
  
  struct mime_type
  {
    static const std::string application_json;
  };
  
};

const std::string http::headers::content_type("Content-Type");
const std::string http::mime_type::application_json("application/json");

rapidjson::Document
wtss_cxx::json_request(const std::string& server_uri)
{
  boost::network::http::client client;
  boost::network::http::client::request request(server_uri);
  boost::network::http::client::response response = client.get(request);
  
// check response status
  uint16_t status = response.status();
  
  if(status != http::ok)
  {
    boost::format err_msg("WTSS server response error.\nHTTP Status: '%1%'\n%2%");
    throw http_response_error() << error_description((err_msg % status % response.status_message()).str());
  }
  
// check headers for a content-type of type 'application/json'
  const boost::network::http::client::response::headers_container_type& headers = response.headers();
  
  boost::network::http::client::response::headers_container_type::const_iterator it = headers.find(http::headers::content_type);
  
  if(it == headers.end())
    throw http_response_error() << error_description("WTSS server response error: missing content type.");
  
  if(it->second != http::mime_type::application_json)
  {
    boost::format err_msg("WTSS server response error.\nExpected a 'application/json' content-type.\nReceived: %1%");
    throw http_response_error() << error_description((err_msg % it->second).str());
  }
  
// get response body
  std::string json = body(response);

// valid JSON document
  rapidjson::Document doc;

  doc.Parse<0>(json.c_str());

  if(doc.HasParseError())
  {
    boost::format err_msg("error parsing requested document '%1%': %2%.");

    throw parse_error() << error_description((err_msg % server_uri % doc.GetParseError()).str());
  }

  return doc;
}
