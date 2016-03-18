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
  \file unittest/main.cpp

  \brief Unit-test for WTSS.CXX.

  \author Matheus Cavassan Zaglia
 */

// WTSS.CXX
#include <wtss-cxx/wtss.hpp>
#include <wtss-cxx/exception.hpp>

// STL
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[])
{
  wtss_cxx::wtss chronos("http://localhost:7654/wtss");
  
  try
  {
// listing the available coverages
    std::vector<std::string> coverages = chronos.list_coverages();
  
    if(coverages.empty())
      return EXIT_SUCCESS;
  
// describing first coverage
    wtss_cxx::geoarray_t cv = chronos.describe_coverage(coverages.front());
  
    if(cv.attributes.empty())
      return EXIT_SUCCESS;
  
// retrieving timeseries for first coverage and its first attribute
    wtss_cxx::timeseries_query_t q;
    q.coverage_name = coverages.front();
    q.attributes.push_back(cv.attributes.front().name);
    q.longitude = -54.0;
    q.latitude = -12;
  
    wtss_cxx::timeseries_query_result_t result = chronos.time_series(q);

    if(result.coverage.name.empty());
  }
  catch(const boost::exception& e)
  {
    if(const std::string* d = boost::get_error_info<wtss_cxx::error_description>(e))
      std::cout << "\n\nthe following error has occurried: " << *d << std::endl;
    else
      std::cout << "\n\nan unknown error has occurried" << std::endl;
    
    return EXIT_FAILURE;
  }
  catch(const std::exception& e)
  {
    std::cout << "\n\nthe following error has occurried: " << e.what() << std::endl;
    
    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << "\n\nan unknown error has occurried." << std::endl;
    
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
