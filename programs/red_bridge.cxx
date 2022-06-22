// Standard library:
#include <cstdio>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <memory>
#include <string>
#include <vector>

// Third party:
// - Bayeux:
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/io_factory.h>
// - Boost:
#include <boost/program_options.hpp>

// - Falaise:
#include <falaise/snemo/datamodels/unified_digitized_data.h>

// - SNCabling
#include <sncabling/sncabling.h>
#include <sncabling/gg_cell_id.h>
#include <sncabling/om_id.h>

// - SNFEE:
#include <snfee/data/calo_hit_record.h>
#include <snfee/data/raw_event_data.h>
