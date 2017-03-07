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
  \file wtss-cxx/data_types.hpp

  \brief A set of types for describing a SciDB geo-array.

  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __WTSS_CXX_DATA_TYPES_HPP__
#define __WTSS_CXX_DATA_TYPES_HPP__

// WTSS.CXX
#include "config.hpp"

// STL
#include <string>
#include <vector>
#include <stdint.h>

namespace wtss
{
  namespace cxx
  {
    //! The spatial resolution of array cells.
    struct spatial_resolution_t
    {
      double x;
      double y;
    };

    //! The range of valid values for a given attribute of an array.
    struct numeric_range_t
    {
      double min_val;
      double max_val;
    };

    //! The range of valid values for a given attribute of an array.
    struct time_interval_t
    {
      std::string start;
      std::string end;
    };

    //! The spatial extent of an array.
    struct extent_t
    {
      double xmin;
      double ymin;
      double xmax;
      double ymax;
    };

    //! The list of data types for array attributes.
    struct datatype_t
    {
      enum
      {
        int8_dt,
        uint8_dt,
        int16_dt,
        uint16_dt,
        int32_dt,
        uint32_dt,
        int64_dt,
        uint64_dt,
        float_dt,
        double_dt,
        unknown
      };

      //! Convert a data type id to its string description.
      static inline std::string to_string(int dt)
      {
        switch (dt)
        {
          case int8_dt:
            return "8-bit signed integer";
          case uint8_dt:
            return "8-bit unsigned integer";
          case int16_dt:
            return "16-bit signed integer";
          case uint16_dt:
            return "16-bit unsigned integer";
          case int32_dt:
            return "32-bit signed integer";
          case uint32_dt:
            return "32-bit unsigned integer";
          case int64_dt:
            return "64-bit signed integer";
          case uint64_dt:
            return "64-bit unsigned integer";
          case float_dt:
            return "32-bit float";
          case double_dt:
            return "64-bit float";
          default:
            return "unknown";
        }
      }

      //! Convert a data type in its string description to its numeric code.
      static inline int from_string(const std::string& data_type_name)
      {
        if (data_type_name == "8-bit signed integer")
          return int8_dt;
        else if (data_type_name == "8-bit unsigned integer")
          return uint8_dt;
        else if (data_type_name == "16-bit signed integer")
          return int16_dt;
        else if (data_type_name == "16-bit unsigned integer")
          return uint16_dt;
        else if (data_type_name == "32-bit signed integer")
          return int32_dt;
        else if (data_type_name == "32-bit unsigned integer")
          return uint32_dt;
        else if (data_type_name == "64-bit signed integer")
          return int64_dt;
        else if (data_type_name == "64-bit unsigned integer")
          return uint64_dt;
        else if (data_type_name == "32-bit float")
          return float_dt;
        else if (data_type_name == "64-bit float")
          return double_dt;
        else
          return unknown;
      }
    };

    //! The list of units.
    struct unit_t
    {
      enum
      {
        day,
        month,
        year,
        unknown
      };

      //! Convert a unit id to its string description.
      static inline std::string to_string(int u)
      {
        switch (u)
        {
          case day:
            return "day";
          case month:
            return "month";
          case year:
            return "year";
          default:
            return "unknown";
        }
      }

      //! Convert a unit type in its string description to its numeric code.
      static inline int from_string(const std::string& unit_name)
      {
        if (unit_name == "day")
          return day;
        else if (unit_name == "month")
          return month;
        else if (unit_name == "year")
          return year;
        else
          return unknown;
      }
    };

    struct date
    {
      int day;
      int month;
      int year;
    };
    //! Dimension description of an array.
    struct dimension_t
    {
      std::string name;
      int64_t min_idx;
      int64_t max_idx;
      std::size_t pos;
    };

    struct spatial_extent_t
    {
      extent_t extent;
      spatial_resolution_t resolution;
//      uint32_t crs_code;
    };

    struct temporal_extent_t
    {
      time_interval_t time_interval;
      uint64_t resolution;
      int unit;
    };

    struct geo_extent_t
    {
      spatial_extent_t spatial;
      temporal_extent_t temporal;
    };

    //! Base metadata of an array attribute.
    struct attribute_t
    {
      std::string name;
      std::string description;
      numeric_range_t valid_range;
      double scale_factor;
      double missing_value;
      int datatype;
    };

    struct crs_t
    {
      std::string proj4;
      std::string wkt;
    };

    //! Base metadata of an array.
    struct geoarray_t
    {
      std::string name;
      std::string description;
      std::string detail;
      std::vector<attribute_t> attributes;
      std::vector<dimension_t> dimensions;
      std::vector<date> timeline;
      crs_t crs;
      spatial_extent_t spatial_extent;
    };

    struct queried_attribute_t
    {
      std::string name;
      std::vector<double> values;
    };

    struct coordinates_t
    {
      double longitude;
      double latitude;
      double col;
      double row;
    };

    struct queried_coverage_t
    {
      std::string name;
      std::vector<queried_attribute_t> queried_attributes;
      std::vector<date> timeline;
      coordinates_t coordinates;
    };

    struct timeseries_query_t
    {
      std::string coverage_name;
      std::vector<std::string> attributes;
      double longitude;
      double latitude;
      std::string start_date;
      std::string end_date;
    };

    //! Time series query result type
    struct timeseries_query_result_t
    {
      queried_coverage_t coverage;
      timeseries_query_t query;
    };
  }  // end namespace cxx
}  // end namespace wtss

#endif  // __WTSS_CXX_DATA_TYPES_HPP__
