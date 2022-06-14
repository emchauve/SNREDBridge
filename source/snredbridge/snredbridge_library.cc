// snredbridge/snredbridge_library.cc - Implementation of SNREDBridge system singleton
//

// Ourselves:
#include <snredbridge/snredbridge_library.h>

// Standard library
#include <cstdlib>
#include <memory>
#include <string>

// Third party:
// Boost:
#include <boost/filesystem.hpp>

// This project;
#include "snredbridge/snredbridge_config.h"

#if SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY == 1
// Bayeux:
#include <bayeux/datatools/kernel.h>
#include <bayeux/datatools/library_info.h>
// #include <bayeux/datatools/urn_to_path_resolver_service.h>
// #include <bayeux/datatools/urn_db_service.h>
#endif // SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY

// This project;
#include "snredbridge/version.h"
#include "snredbridge/resource_files.h"
#include "snredbridge_binreloc.h"
#include "snredbridge/exception.h"

namespace {
  //! Convert BrInitError code to a string describing the error
  //! \todo add errno to returned string
  std::string BRErrorAsString(const BrInitError& err) {
    std::string errMsg;
    switch (err) {
    case BR_INIT_ERROR_NOMEM:
      errMsg = "Unable to open /proc/self/maps";
    case BR_INIT_ERROR_OPEN_MAPS:
      errMsg = "Unable to read from /proc/self/maps";
    case BR_INIT_ERROR_READ_MAPS:
      errMsg = "The file format of /proc/self/maps";
    case BR_INIT_ERROR_INVALID_MAPS:
      errMsg = "The file format of /proc/self/maps is invalid";
    case BR_INIT_ERROR_DISABLED:
      errMsg = "Binary relocation disabled";
    default:
      BOOST_ASSERT_MSG(1,"Invalid BrInitError");
    }
    return errMsg;
  }
}

namespace snredbridge {

  directories_initialization_exception::directories_initialization_exception(const std::string & msg_)
    : std::runtime_error(msg_)
  {
  }

  void init_directories() {
    BrInitError err;
    int initSuccessful = br_init_lib(&err);
    SN_THROW_IF(initSuccessful != 1,
                snredbridge::directories_initialization_exception,
                "Initialization of SNREDBridge library's directories failed : "
                << err
                << " ("
                << BRErrorAsString(err)
                << ")");
  }

  // static
  const std::string & snredbridge_library::setup_db_name()
  {
    static const std::string _n("snredbridgeSystemSetupDb");
    return _n;
  }

  // static
  const std::string & snredbridge_library::resource_resolver_name()
  {
    static const std::string _n("snredbridgeSystemResourceResolver");
    return _n;
  }

#if SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY == 1
  datatools::logger::priority snredbridge_library::process_logging_env()
  {
    datatools::logger::priority logging = datatools::logger::PRIO_FATAL;
    char * l = getenv("SNREDBRIDGE_LIBRARY_LOGGING");
    if (l) {
      std::string level_label(l);
      ::datatools::logger::priority prio
          = ::datatools::logger::get_priority(level_label);
      if (prio != ::datatools::logger::PRIO_UNDEFINED) {
        logging = prio;
      }
    }
    return logging;
  }
#endif // SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY

  // static
  snredbridge_library * snredbridge_library::_instance_ = nullptr;

  snredbridge_library::snredbridge_library()
  {
#if SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY == 1
    _logging_ = snredbridge_library::process_logging_env();
    if (_logging_ == ::datatools::logger::PRIO_UNDEFINED) {
      DT_LOG_WARNING(::datatools::logger::PRIO_WARNING,
                     "Ignoring invalid SNREDBRIDGE_LIBRARY_LOGGING=\"" << getenv("SNREDBRIDGE_LIBRARY_LOGGING") << "\" environment!");
      _logging_ = ::datatools::logger::PRIO_FATAL;
    }
    DT_LOG_TRACE(_logging_, "Initializing SNREDBridge library's resource files directories...");
#endif // SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY
    snredbridge::init_directories();
    SN_THROW_IF(snredbridge_library::_instance_ != nullptr,
                std::logic_error,
                "SNREDBridge library system singleton is already set!");
    snredbridge_library::_instance_ = this;
    return;
  }

  snredbridge_library::~snredbridge_library()
  {
    if (is_initialized()) {
      shutdown();
    }
    snredbridge_library::_instance_ = nullptr;
    return;
  }

#if SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY == 1
  datatools::logger::priority snredbridge_library::get_logging() const
  {
    return _logging_;
  }
#endif // SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY

  bool snredbridge_library::is_initialized() const
  {
    return _initialized_;
  }

  void snredbridge_library::initialize()
  {
    SN_THROW_IF(is_initialized(), std::logic_error, "SNREDBridge library system singleton is already initialized!");

#if SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY == 1
    // Register library informations in the Bayeux/datatools' kernel:
    _libinfo_registration_();

    // Setup services:
    DT_LOG_TRACE(_logging_, "SNREDBridge library system singleton services...");
    _services_.set_name("snredbridgeLibrarySystemServices");
    _services_.set_description("SNREDBridge Library System Singleton Services");
    _services_.set_allow_dynamic_services(true);
    _services_.initialize();

    // Start URN services:
    _initialize_urn_services_();

#endif // SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY

    _initialized_ = true;
    return;
  }

  void snredbridge_library::shutdown()
  {
    SN_THROW_IF(!is_initialized(), std::logic_error, "SNREDBridge library system singleton is not initialized!");
    _initialized_ = false;

#if SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY == 1
    // Terminate services:
    if (_services_.is_initialized()) {
      _shutdown_urn_services_();
      DT_LOG_TRACE(_logging_, "Terminating SNREDBridge library system singleton services...");
      _services_.reset();
    }

    // Deregister library informations from the Bayeux/datatools' kernel:
    _libinfo_deregistration_();
#endif // SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY

    return;
  }

#if SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY == 1
  datatools::service_manager & snredbridge_library::grab_services()
  {
    return _services_;
  }

  const datatools::service_manager & snredbridge_library::get_services() const
  {
    return _services_;
  }
#endif // SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY

  // static
  bool snredbridge_library::is_instantiated()
  {
    return _instance_ != nullptr;
  }

  // static
  snredbridge_library & snredbridge_library::instance()
  {
    return *_instance_;
  }

  // static
  const snredbridge_library & snredbridge_library::const_instance()
  {
    return *_instance_;
  }

  // static
  snredbridge_library & snredbridge_library::instantiate()
  {
    if (! snredbridge_library::is_instantiated()) {
      static std::unique_ptr<snredbridge_library> _sys_handler;
      if (! _sys_handler) {
        // Allocate the SNREDBridge sys library singleton and initialize it:
        _sys_handler.reset(new snredbridge_library);
      }
    }
    return snredbridge_library::instance();
  }

#if SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY == 1
  void snredbridge_library::_libinfo_registration_()
  {
    SN_THROW_IF(!datatools::kernel::is_instantiated(),
                std::runtime_error,
                "The Bayeux/datatools' kernel is not instantiated !");
    datatools::kernel & krnl = datatools::kernel::instance();

    // Populate the library info register:
    datatools::library_info & lib_info_reg = krnl.grab_library_info_register();

    // Bundled submodules:
    {
      DT_LOG_TRACE(_logging_, "Registration of SNREDBridge library in the Bayeux/datatools' kernel...");
      // SNREDBridge itself:
      SN_THROW_IF (lib_info_reg.has("snredbridge"),
                   std::logic_error,
                   "SNREDBridge is already registered !");
      datatools::properties & snredbridge_lib_infos
        = lib_info_reg.registration("snredbridge",
                                    "SNREDBridge provides a offline and real-time software environment for the description, control,"
                                    "monitoring and running of an experiment.",
                                    snredbridge::version::get_version()
                                    );
      DT_LOG_TRACE(_logging_, "SNREDBridge library entry is now registered in the Bayeux/datatools' kernel.");

      // Register the SNREDBridge resource path in the datatools' kernel:
      // std::cerr << "[devel] ******* snredbridge_library::_libinfo_registration_ = '" << snredbridge::get_resource_files_dir() << "'" << std::endl;
      snredbridge_lib_infos.store_string(datatools::library_info::keys::install_resource_dir(),
                                       snredbridge::get_resource_files_dir());
      DT_LOG_TRACE(_logging_, "SNREDBridge resource files dir is documented.");

      // If the 'SNREDBRIDGE_RESOURCE_FILES_DIR' environment variable is set, it will supersede
      // the official registered resource path above through the 'datatools::fetch_path_with_env'
      // function:
      snredbridge_lib_infos.store_string(datatools::library_info::keys::env_resource_dir(),
                                       "SNREDBRIDGE_RESOURCE_FILES_DIR");
      DT_LOG_TRACE(_logging_, "SNREDBridge resource files dir env is documented.");
      DT_LOG_TRACE(_logging_, "SNREDBridge library has been registered in the Bayeux/datatools' kernel.");
    }

    return;
  }

  void snredbridge_library::_libinfo_deregistration_()
  {
    if (datatools::kernel::is_instantiated()) {
      datatools::kernel & krnl = datatools::kernel::instance();
      if (krnl.has_library_info_register()) {
        // Access to the datatools kernel library info register:
        datatools::library_info & lib_info_reg = krnl.grab_library_info_register();
        // Unregistration of all registered submodules from the kernel's library info register:
        if (lib_info_reg.has("snredbridge")) {
          DT_LOG_TRACE(_logging_, "Deregistration of the SNREDBridge library from the Bayeux/datatools' kernel...");
          lib_info_reg.unregistration("snredbridge");
          DT_LOG_TRACE(_logging_, "SNREDBridge library has been deregistered from the Bayeux/datatools' kernel.");
        }
      }
    }
    return;
  }

  void snredbridge_library::_initialize_urn_services_()
  {
    if (_services_.is_initialized()) {

      /*
      // Activate an URN info DB service:
      {
        datatools::urn_db_service & urnSetupDb =
          dynamic_cast<datatools::urn_db_service &>(_services_.load_no_init("snredbridgeSystemDb",
                                                                            "datatools::urn_db_service"));
        urnSetupDb.set_logging_priority(_logging_);
        std::string urn_db_conf_file = "@snredbridge:urn/db/service.conf";
        datatools::fetch_path_with_env(urn_db_conf_file);
        datatools::properties urn_db_conf;
        urn_db_conf.read_configuration(urn_db_conf_file);
        urnSetupDb.initialize_standalone(urn_db_conf);
        if (datatools::logger::is_debug(_logging_)) {
          urnSetupDb.tree_dump(std::cerr, urnSetupDb.get_name() + ": ", "[debug] ");
        }
        DT_LOG_TRACE(_logging_, "Publishing the URN info DB '" << urnSetupDb.get_name() << "' to the Bayeux/datatools' kernel...");
        urnSetupDb.kernel_push();
        DT_LOG_TRACE(_logging_, "URN info DB has been plugged in the Bayeux/datatools' kernel.");
      }
      // Activate an URN resolver service:
      {
        datatools::urn_to_path_resolver_service & urnResourceResolver =
          dynamic_cast<datatools::urn_to_path_resolver_service &>(_services_.load_no_init("snredbridgeSystemPathResolver",
                                                                                          "datatools::urn_to_path_resolver_service"));
        urnResourceResolver.set_logging_priority(_logging_);
        std::string urn_resolver_conf_file = "@snredbridge:urn/resolvers/service.conf";
        datatools::fetch_path_with_env(urn_resolver_conf_file);
        datatools::properties urn_resolver_conf;
        urn_resolver_conf.read_configuration(urn_resolver_conf_file);
        urnResourceResolver.initialize_standalone(urn_resolver_conf);
        if (datatools::logger::is_debug(_logging_)) {
          urnResourceResolver.tree_dump(std::cerr, urnResourceResolver.get_name() + ": ", "[debug] ");
        }
        DT_LOG_TRACE(_logging_, "Publishing the URN path resolver '" << urnResourceResolver.get_name() << "' to the Bayeux/datatools' kernel...");
        urnResourceResolver.kernel_push();
        DT_LOG_TRACE(_logging_, "URN path resolver has been plugged in the Bayeux/datatools' kernel.");
      }
      */
    }
    return;
  }

  void snredbridge_library::_shutdown_urn_services_()
  {
    if (_services_.is_initialized()) {

      /*
      // Deactivate the URN resolver:
      if (_services_.has(resource_resolver_name())) {
        DT_LOG_TRACE(_logging_, "Accessing URN path resolver...");
        datatools::urn_to_path_resolver_service & urnResourceResolver =
          _services_.grab<datatools::urn_to_path_resolver_service &>("snredbridgeSystemPathResolver");
        DT_LOG_TRACE(_logging_, "Removing URN path resolver '" << urnResourceResolver.get_name() << "' from the  Bayeux/datatools's kernel...");
        urnResourceResolver.kernel_pop();
        DT_LOG_TRACE(_logging_, "URN path resolver has been removed from the Bayeux/datatools kernel.");
        urnResourceResolver.reset();
      }
      // DeActivate the URN info setup DB:
      if (_services_.has(setup_db_name())){
        DT_LOG_TRACE(_logging_, "Accessing URN info setup DB...");
        datatools::urn_db_service & urnSetupDb =
          _services_.grab<datatools::urn_db_service &>("snredbridgeSystemDb");
        DT_LOG_TRACE(_logging_, "Removing URN info setup DB '" << urnSetupDb.get_name() << "' from the  Bayeux/datatools's kernel...");
        urnSetupDb.kernel_pop();
        DT_LOG_TRACE(_logging_, "URN info setup DB has been removed from the Bayeux/datatools kernel.");
        urnSetupDb.reset();
      }
      */
    }
    return;
  }
#endif // SNREDBRIDGE_WITH_BAYEUX_DEPENDENCY

} // namespace snredbridge
