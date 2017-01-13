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
  \file wtss-cxx/exception.hpp

  \brief Implementation of util class in WTSS.CXX.

  \author Matheus Cavassan Zaglia
 */

// wtss.cxx
#include "utils.hpp"
#include "exception.hpp"

// CURL
#include <curl/curl.h>

// Boost
#include <boost/format.hpp>

size_t write_response_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
  ((std::string *)data)->append((char *)ptr, size * nmemb);
  return size * nmemb;
}

std::string wtss::cxx::request(const std::string &server_uri)
{
  CURL *curl;
  CURLcode res;
  struct curl_slist *header = NULL;

  header = curl_slist_append(header, "Accept: application/json");
  header = curl_slist_append(header, "Content-Type: application/json");

  curl = curl_easy_init();
  char err_buffer[CURL_ERROR_SIZE];
  std::string response;

  if (curl)
  {
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &err_buffer);
    curl_easy_setopt(curl, CURLOPT_URL, server_uri.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
      boost::format err_msg(
          "WTSS server response error.\n"
          "The following error has ocurred: %1%.");
      curl_slist_free_all(header);
      curl_easy_cleanup(curl);
      throw http_response_error()
          << error_description((err_msg % err_buffer).str());
    }

    char *content_type;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);

    if (std::string(content_type).find("application/json") == std::string::npos)
    {
      std::string content_type_str = std::string(content_type);

      boost::format err_msg(
          "WTSS server response error.\n"
          "Expected an 'application/json' Content-Type.\n"
          "Received: %1%");

      curl_slist_free_all(header);
      
      curl_easy_cleanup(curl);

      throw http_response_error()
          << error_description((err_msg % content_type_str).str());
    }

    int response_code = 0;
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    if (response_code != 200)
    {
      std::string response_code_str = std::to_string(response_code);

      boost::format err_msg(
          "WTSS server response error.\n Expected response code 200.\n"
          "Received: '%1%'.");

      curl_slist_free_all(header);
      curl_easy_cleanup(curl);

      throw http_response_error()
          << error_description((err_msg % response_code_str).str());
    }

    curl_slist_free_all(header);
    curl_easy_cleanup(curl);
  }

  return response.c_str();
}
