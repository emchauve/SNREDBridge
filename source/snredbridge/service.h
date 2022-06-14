//! \file    snredbridge/service.h
//! \brief   SuperNEMO Cabling Service
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

#ifndef SNREDBRIDGE_SERVICE_H
#define SNREDBRIDGE_SERVICE_H

// Standard Library:
#include <memory>

// Third Party:
#include <bayeux/datatools/base_service.h>
#include <bayeux/datatools/service_tools.h>


namespace snredbridge {

  /// \brief Cabling service
  class service
    : public datatools::base_service
  {
  public:

    /// Default constructor
    service();

    /// Destructor
    virtual ~service();

    /// Check initialization flag
    virtual bool is_initialized() const;

    /// Initialization
    virtual int initialize(const datatools::properties & config_,
                           datatools::service_dict_type & service_dict_);

    /// Termination
    virtual int reset();

  private:

    // Management:
    bool _initialized_ = false;  //!< Initialization flag

    // Configuration:

    // Working
    struct pimpl_type;
    std::unique_ptr<pimpl_type> _pimpl_; //!< Private implementation

    // Registration :
    DATATOOLS_SERVICE_REGISTRATION_INTERFACE(service)
  };

} // namespace snredbridge

#endif // SNREDBRIDGE_SERVICE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
