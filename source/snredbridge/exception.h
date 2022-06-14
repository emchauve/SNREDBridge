//! \file    snredbridge/exception.h
//! \brief   Exception handling macros
//! \details
//
// Copyright (C) 2022 Guillaume Oliviero <g.oliviero@ucl.ac.uk>
//
// This file is part of SNREDBridge.
//
// SNREDBridge is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SNREDBridge is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SNREDBridge.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SNREDBRIDGE_EXCEPTION_H
#define SNREDBRIDGE_EXCEPTION_H

// Standard Library:
#include <string>
#include <iostream>
#include <sstream>

// - Boost:
#include <boost/current_function.hpp>

#define SN_THROW_IF(Condition, ExceptionType, Message)			\
  {                                                                     \
    if (Condition) {                                                    \
      std::stringstream sSN_THROW_IF_ONLY;                              \
      sSN_THROW_IF_ONLY << "[" << BOOST_CURRENT_FUNCTION << ":" << __LINE__ << ": " << Message << "]"; \
      throw ExceptionType(sSN_THROW_IF_ONLY.str());                     \
    }                                                                   \
  }

#define SN_THROW(ExceptionType, Message)				\
  {                                                                     \
    SN_THROW_IF(true,ExceptionType, Message)				\
  }

#endif // SNREDBRIDGE_EXCEPTION_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
