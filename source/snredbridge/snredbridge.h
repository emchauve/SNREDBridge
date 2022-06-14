//! \file    snredbridge/snredbridge.h
//! \brief   Provide system init for SNREDBridge internals
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

#ifndef SNREDBRIDGE_SNREDBRIDGE_H
#define SNREDBRIDGE_SNREDBRIDGE_H

// Standard Library:
#include <cstdint>

//! Main namespace for the SNREDBridge library
namespace snredbridge {

  //! Check if the SNREDBridge library is initialized
  bool is_initialized();

  //! Initialize special resources of the SNREDBridge library
  void initialize(int argc_ = 0, char * argv_[] = 0, uint32_t flags_ = 0);

  //! Terminate special resources of the SNREDBridge library
  void terminate();

} // namespace snredbridge

#endif // SNREDBRIDGE_SNREDBRIDGE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
