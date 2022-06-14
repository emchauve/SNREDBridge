//! \file    snredbridge/snredbridge_library.h
//! \brief   Provide SNREDBridge library system singleton
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

#ifndef SNREDBRIDGE_SNREDBRIDGE_LIBRARY_H
#define SNREDBRIDGE_SNREDBRIDGE_LIBRARY_H

// Standard Library
#include <string>
#include <stdexcept>

// Third party:
#include <boost/noncopyable.hpp>

// This project;
#include "snredbridge/snredbridge_config.h"

#if SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY == 1
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/service_manager.h>
#endif // SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY

namespace snredbridge {

  //! Exception class for directories initialization failure
  class directories_initialization_exception
    : public std::runtime_error
  {
  public:
    directories_initialization_exception(const std::string & msg);
  };

  //! Initialize the library resource paths
  //! \throw directories_initialization_exception when library cannot self locate
  void init_directories();

  //! \brief SNREDBridge system singleton
  class snredbridge_library
    : private boost::noncopyable
  {
  public:

    /// Return the name of the SNREDBridge library's URN database service for supported setups (geometry, simulation...)
    static const std::string & setup_db_name();

    /// Return the name of the SNREDBridge library's URN to resource path resolver service
    static const std::string & resource_resolver_name();

    /// Default constructor
    snredbridge_library();

    /// Destructor
    virtual ~snredbridge_library();

    /// Check initialization flag
    bool is_initialized() const;

    /// Initialize
    void initialize();

    /// Shutdown
    void shutdown();

#if SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY == 1
    /// Extract the verbosity from the SNREDBRIDGE_LIBRARY_LOGGING environment variable (if any)
    static datatools::logger::priority process_logging_env();

    /// Return logging priority
    datatools::logger::priority get_logging() const;

    /// Return a mutable reference to the embedded service manager
    datatools::service_manager & grab_services();

    /// Return a non mutable reference to the embedded service manager
    const datatools::service_manager & get_services() const;
#endif // SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY

    /// Check if the SNREDBridge system singleton is instantiated
    static bool is_instantiated();

    /// Return a mutable reference to the SNREDBridge system singleton instance
    static snredbridge_library & instance();

    /// Return a non-mutable reference to the SNREDBridge system singleton instance
    static const snredbridge_library & const_instance();

    /// Instantiate the SNREDBridge system singleton
    static snredbridge_library & instantiate();

  private:

#if SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY == 1
    void _libinfo_registration_();

    void _libinfo_deregistration_();

    void _initialize_urn_services_();

    void _shutdown_urn_services_();
#endif // SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY

  private:

    // Management:
    bool _initialized_ = false;            //!< Initialization flag

    // Working internal data:
#if SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY == 1
    datatools::logger::priority _logging_ = datatools::logger::PRIO_FATAL;
    datatools::service_manager _services_; //!< Embedded services
#endif // SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY

    // Singleton:
    static snredbridge_library * _instance_;  //!< SNREDBridge library system singleton handle

  };

} // namespace snredbridge

#endif // SNREDBRIDGE_SNREDBRIDGE_LIBRARY_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
