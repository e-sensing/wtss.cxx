#
#  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.
#
#  This file is part of the TerraLib Web Services.
#
#  TerraLib Web Services is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License version 3 as
#  published by the Free Software Foundation.
#
#  TerraLib Web Services is distributed  "AS-IS" in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY OF ANY KIND; without even the implied warranty
#  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
#  with TerraLib Web Services. See COPYING. If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
#
#
#  CMake scripts for TerraLib Web Services
#
#  Author: Gilberto Ribeiro de Queiroz <gribeiro@dpi.inpe.br>
#
#  Description: Find RapidJSON include directory.
#
#  RAPIDJSON_INCLUDE_DIR - where to find rapidjson/document.h and other header files.
#  RAPIDJSON_FOUND       - True if RapidJSON found.
#

find_path(RAPIDJSON_INCLUDE_DIR rapidjson/document.h
          PATHS /usr
                /usr/local
          PATH_SUFFIXES include
                        include/rapidjson)

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(RapidJSON DEFAULT_MSG RAPIDJSON_INCLUDE_DIR)

mark_as_advanced(RAPIDJSON_INCLUDE_DIR)
