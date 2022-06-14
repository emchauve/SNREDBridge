// snredbridge/service.cc - Implementation of the SuperNEMO cabling service
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SNREDBridge. If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <snredbridge/service.h>

namespace snredbridge {

  // Registration :
  DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(service, "snredbridge::service")

  //struct service::pimpl_type
  //{
  //};

  service::service()
  {
    _pimpl_.reset(new pimpl_type);
    return;
  }

  service::~service()
  {
    if (this->service::is_initialized()) {
      this->service::reset();
    }
    _pimpl_.reset();
    return;
  }

  bool service::is_initialized() const
  {
    return _initialized_;
  }

  int service::initialize(const datatools::properties & config_,
                          datatools::service_dict_type & /* service_dict_ */)
  {
    DT_THROW_IF(is_initialized(), std::logic_error,
                "Service '" << get_name() << "' is already initialized ! ");

    base_service::common_initialize(config_);

    _initialized_ = true;
    return EXIT_SUCCESS;
  }

  int service::reset()
  {
    DT_THROW_IF(!is_initialized(), std::logic_error,
                "Service '" << get_name() << "' is not initialized ! ");
    _initialized_ = false;

    return EXIT_SUCCESS;
  }

} // namespace snredbridge
