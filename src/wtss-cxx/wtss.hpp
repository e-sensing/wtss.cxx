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
  \file wtss-cxx/wtss.hpp

  \brief The Web Time Series Service Client API for C++.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

#ifndef __WTSS_CXX_WTSS_HPP__
#define __WTSS_CXX_WTSS_HPP__

// WTSS
#include "config.hpp"
#include "data_types.hpp"

namespace wtss_cxx
{
  class wtss
  {
    wtss(const std::string& server_uri);
    
    ~wtss();
    
    std::vector<std::string> list_coverages() const;
    
    geoarray_t describe_coverage(const std::string& cv_name) const;
    
    timeseries_query_result_t time_series(const timeseries_query_t& query) const;
  };

}  // end namespace wtss_cxx

#endif // __WTSS_CXX_WTSS_HPP__
