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
#include <bayeux/datatools/things.h>
#include <bayeux/dpp/input_module.h>

// - Falaise:
#include <falaise/snemo/datamodels/unified_digitized_data.h>
#include <falaise/snemo/datamodels/event_header.h>

// - SNFEE:
#include <snfee/snfee.h>
#include <snfee/io/multifile_data_reader.h>
#include <snfee/data/raw_event_data.h>


bool compare_red_event_record(const snfee::data::raw_event_data &,
                              const datatools::things &,
                              const datatools::logger::priority &);


//----------------------------------------------------------------------
// MAIN PROGRAM
//----------------------------------------------------------------------

int main (int argc, char *argv[])
{
  datatools::logger::priority logging = datatools::logger::PRIO_WARNING;
  int error_code = EXIT_SUCCESS;
  try {
    std::string input_red_filename = "";
    std::string input_udd_filename = "";
    size_t data_count = 100000000;

    for (int iarg=1; iarg<argc; ++iarg)
      {
        std::string arg (argv[iarg]);
        if (arg[0] == '-')
          {
            if ((arg == "-d") || (arg == "--debug"))
              logging = datatools::logger::PRIO_DEBUG;

            else if ((arg == "-v") || (arg == "--verbose"))
              logging = datatools::logger::PRIO_INFORMATION;

            else if (arg=="-ired" || arg=="--input-red")
              input_red_filename = std::string(argv[++iarg]);

            else if (arg=="-iudd" || arg=="--input-udd")
              input_udd_filename = std::string(argv[++iarg]);

            else if ((arg == "-n") || (arg == "--max-events"))
              data_count = std::strtol(argv[++iarg], NULL, 10);

            else if (arg=="-h" || arg=="--help")
              {
                std::cout << std::endl;
                std::cout << "Usage:   " << argv[0] << " [options]" << std::endl;
                std::cout << std::endl;
                std::cout << "Options:   -h    / --help" << std::endl;
                std::cout << "           -ired / --input-red   RED_FILE" << std::endl;
                std::cout << "           -iudd / --input-udd   UDD_FILE" << std::endl;
                std::cout << "           -n    / --max-events  Max number of events" << std::endl;
                std::cout << std::endl;
                return 0;
              }

            else
              std::cerr << "*** unkown option " << arg << std::endl;
          }
      }

    if (input_red_filename.empty() || input_udd_filename.empty())
      {
        std::cerr << "*** ERROR: missing input RED or UDD filename !" << std::endl;
        return 1;
      }

    snfee::initialize();


    /// Configuration for raw data reader
    snfee::io::multifile_data_reader::config_type reader_cfg;
    reader_cfg.filenames.push_back(input_red_filename);

    // Declare the reader
    DT_LOG_DEBUG(logging, "Instantiate the RED reader");
    snfee::io::multifile_data_reader red_source(reader_cfg);
    DT_LOG_DEBUG(logging, "Initialization of the RED input module is done.");


    // The input module for the datatools event record:
    DT_LOG_DEBUG(logging, "Instantiate the UDD reader");
    dpp::input_module reader;
    reader.set_logging_priority(datatools::logger::PRIO_FATAL);
    reader.set_name("Reader input module");
    reader.set_description("Input module for the datatools::things event_record");
    reader.set_single_input_file(input_udd_filename);
    reader.initialize_simple();
    DT_LOG_DEBUG(logging, "Initialization of the UDD input module is done.");

    std::string EH_tag  = "EH";
    std::string UDD_tag = "UDD";

    // RED counter
    std::size_t red_counter = 0;

    // ER counter
    std::size_t er_counter = 0;

    // EH counter
    std::size_t eh_counter = 0;

    // UDD counter
    std::size_t udd_counter = 0;

    // Missing event counter (for debug purpose)
    std::size_t missing_event_counter = 0;

    // Non equal events counter during comparison function (for debug purpose)
    std::size_t non_equal_event_counter = 0;

    // Check number of events in each data format
    while (red_source.has_record_tag() && red_counter < data_count)
      {
        // Check and analyze 1 RED event and 1 UDD event
        // For 1 RED event, must have 1 event record with 1 event header and 1 UDD event for a given RUN ID, same EVENT ID

        // Empty working RED object
        snfee::data::raw_event_data red;
        red_source.load(red);
        red_counter++;

        int32_t red_run_id   = red.get_run_id();
        int32_t red_event_id = red.get_event_id();

        bool find_corresponding_udd_event = false;
        datatools::things event_record;

        while (!reader.is_terminated() || !find_corresponding_udd_event) {
          // Empty working UDD object
          dpp::base_module::process_status status = reader.process(event_record);
          if (status != dpp::base_module::PROCESS_OK) {
            DT_LOG_DEBUG(logging, "Cannot process another event record, status is " << status);
            break;
          }
          // event_record.tree_dump(std::clog, "An event record:");

          auto & EH  = event_record.get<snemo::datamodel::event_header>(EH_tag);
          auto & UDD = event_record.get<snemo::datamodel::unified_digitized_data>(UDD_tag);

          // Search for corresponding ER event with proper runID and eventID
          if (EH.get_id().get_run_number() == red_run_id && EH.get_id().get_event_number() == red_event_id
              && UDD.get_run_id() == red_run_id && UDD.get_event_id() == red_event_id) {
            DT_LOG_DEBUG(logging, "Find corresponding EH/UDD event for run #" << red_run_id <<  " event #" << red_event_id);
            find_corresponding_udd_event = true;
          }
        } // end of while UDD

        if (find_corresponding_udd_event) {
          er_counter++;
          bool is_valid = compare_red_event_record(red, event_record, logging);
          if (is_valid) {
            eh_counter++;
            udd_counter++;
          }
          else non_equal_event_counter++;
        }

        else {
          DT_LOG_WARNING(logging, "Did not find corresponding EH/UDD event for run #" << red_run_id <<  " event #" << red_event_id);
          missing_event_counter++;
        }

        event_record.clear();
      }


    std::cout << "Results :" << std::endl;
    std::cout << "- Worker #0 (input RED)" << std::endl;
    std::cout << "  - RED events    : " << red_counter << std::endl;
    std::cout << "- Worker #1 (output ER)" << std::endl;
    std::cout << "  - Event Records : " << er_counter << std::endl;
    std::cout << "  - Contains (EH and UDD banks)" << std::endl;
    std::cout << "    - Event header : " << eh_counter << std::endl;
    std::cout << "    - UDD events   : " << udd_counter << std::endl;
    std::cout << "- Missing events     : " << missing_event_counter << std::endl;
    std::cout << "- Non equal events   : " << non_equal_event_counter << std::endl;


    snfee::terminate();

    DT_LOG_INFORMATION(logging, "The end.");
  }

  catch (std::exception & x) {
    DT_LOG_FATAL(logging, x.what());
    error_code = EXIT_FAILURE;
  }
  catch (...) {
    DT_LOG_FATAL(logging, "unexpected error !");
    error_code = EXIT_FAILURE;
  }
  return (error_code);
}



bool compare_red_event_record(const snfee::data::raw_event_data & red_,
                              const datatools::things & event_record_,
                              const datatools::logger::priority & logging_)
{
  DT_LOG_DEBUG(logging_, "Entering compare_red_event_record.");
  bool red_er_is_equivalent = false;
  // event_record_.tree_dump(std::clog, "An event record:");

  std::string EH_tag  = "EH";
  std::string UDD_tag = "UDD";
  auto & EH  = event_record_.get<snemo::datamodel::event_header>(EH_tag);
  auto & UDD = event_record_.get<snemo::datamodel::unified_digitized_data>(UDD_tag);

  // red_.print_tree(std::clog);
  // EH.tree_dump(std::clog, "Event header('EH'): ");
  // UDD.tree_dump(std::clog, "Unified Digitized Data('UDD'): ");

  // Compare RED attributes with EH / UDD ones
  bool is_event_header_equivalent = false;
  // Check Run ID, Event ID and Generation
  if (EH.get_id().get_run_number() == red_.get_run_id()
      && EH.get_id().get_event_number() == red_.get_event_id()
      && EH.is_real()) is_event_header_equivalent = true;

  bool is_udd_global_equivalent = false;
  if (UDD.get_run_id() == red_.get_run_id()
      && UDD.get_event_id() == red_.get_event_id()
      && UDD.get_reference_timestamp() == red_.get_reference_time().get_ticks()
      && UDD.get_origin_trigger_ids() == red_.get_origin_trigger_ids())
    is_udd_global_equivalent = true;

  bool is_calo_equivalent = false;

  // RED Digitized calo hits
  const std::vector<snfee::data::calo_digitized_hit> red_calo_hits = red_.get_calo_hits();

  std::vector<bool> list_calos_corresponding;

  for (std::size_t ihit = 0; ihit < red_calo_hits.size(); ihit++) {
    snfee::data::calo_digitized_hit red_calo_hit = red_calo_hits[ihit];
    bool is_corresponding_udd_calo_find = false;
    std::size_t udd_calo_counter = 0;

    snemo::datamodel::calorimeter_digitized_hit udd_calo_hit;

    while (!is_corresponding_udd_calo_find) {
      udd_calo_hit = UDD.get_calorimeter_hits()[udd_calo_counter].get();

      if (udd_calo_hit.get_geom_id() ==  red_calo_hit.get_geom_id()
          && udd_calo_hit.get_hit_id() == red_calo_hit.get_hit_id()) is_corresponding_udd_calo_find = true;
      udd_calo_counter++;
    }

    bool is_corresponding_calo_valid = false;

    // Compare calo hit per attributes
    // create a vector of a boolean for each calo hit already checked
    if (is_corresponding_udd_calo_find) {

      if (udd_calo_hit.get_geom_id() == red_calo_hit.get_geom_id()
          && udd_calo_hit.get_hit_id()  == red_calo_hit.get_hit_id()
          && udd_calo_hit.get_timestamp() == red_calo_hit.get_reference_time().get_ticks()
          && udd_calo_hit.get_waveform()  == red_calo_hit.get_waveform()
          && udd_calo_hit.is_low_threshold_only() == red_calo_hit.is_low_threshold_only()
          && udd_calo_hit.is_high_threshold() == red_calo_hit.is_high_threshold()
          && udd_calo_hit.get_fcr() == red_calo_hit.get_fcr()
          && udd_calo_hit.get_lt_trigger_counter() == red_calo_hit.get_lt_trigger_counter()
          && udd_calo_hit.get_lt_time_counter() == red_calo_hit.get_lt_time_counter()
          && udd_calo_hit.get_fwmeas_baseline() == red_calo_hit.get_fwmeas_baseline()
          && udd_calo_hit.get_fwmeas_peak_amplitude() == red_calo_hit.get_fwmeas_peak_amplitude()
          && udd_calo_hit.get_fwmeas_peak_cell() == red_calo_hit.get_fwmeas_peak_cell()
          && udd_calo_hit.get_fwmeas_charge() == red_calo_hit.get_fwmeas_charge()
          && udd_calo_hit.get_fwmeas_rising_cell() == red_calo_hit.get_fwmeas_rising_cell()
          && udd_calo_hit.get_fwmeas_falling_cell() == red_calo_hit.get_fwmeas_falling_cell()
          && udd_calo_hit.get_origin().get_hit_number() == red_calo_hit.get_origin().get_hit_number()
          && udd_calo_hit.get_origin().get_trigger_id() == red_calo_hit.get_origin().get_trigger_id()) {
          DT_LOG_DEBUG(logging_, "Corresponding UDD calo is valid.");
          is_corresponding_calo_valid = true;
      }
    }

    list_calos_corresponding.push_back(is_corresponding_calo_valid);

  } // end of calo red ihit

  is_calo_equivalent = std::all_of(list_calos_corresponding.begin(), list_calos_corresponding.end(), [](bool v) { return v; });

  bool is_tracker_equivalent = false;

  // RED Digitized tracker hits
  const std::vector<snfee::data::tracker_digitized_hit> red_tracker_hits = red_.get_tracker_hits();

  std::vector<bool> list_trackers_corresponding;

  for (std::size_t ihit = 0; ihit < red_tracker_hits.size(); ihit++) {
    snfee::data::tracker_digitized_hit red_tracker_hit = red_tracker_hits[ihit];
    bool is_corresponding_udd_tracker_find = false;
    std::size_t udd_tracker_counter = 0;

    snemo::datamodel::tracker_digitized_hit udd_tracker_hit;

    while (!is_corresponding_udd_tracker_find) {
      udd_tracker_hit = UDD.get_tracker_hits()[udd_tracker_counter].get();

      if (udd_tracker_hit.get_geom_id() ==  red_tracker_hit.get_geom_id()
          && udd_tracker_hit.get_hit_id() == red_tracker_hit.get_hit_id()) is_corresponding_udd_tracker_find = true;
      udd_tracker_counter++;
    }

    bool is_corresponding_tracker_valid = false;

    // Compare tracker hit per attributes
    // create a vector of a boolean for each tracker hit already checked

    // GO Note/Warning, number of GG times are the same for now between RED and UDD but it might not be the case in a near future
    // if we change the event builder algorithm and decide to remove the 'deduplication' for tracker hits.
    // Not sure how it will impact RED format and then get propagated to UDD format
    if (is_corresponding_udd_tracker_find
        && red_tracker_hit.get_times().size() == udd_tracker_hit.get_times().size()) {

      std::vector<bool> list_timestamps_corresponding;
      bool is_corresponding_timestamp_valid = false;
      for (std::size_t iggtime = 0; iggtime < red_tracker_hit.get_times().size(); iggtime++)
        {
          DT_LOG_DEBUG(logging_, "Corresponding UDD tracker is valid.");
          // Retrieve RED and UDD GG timestamps
          const snfee::data::tracker_digitized_hit::gg_times      & red_gg_timestamp = red_tracker_hit.get_times()[iggtime];
          const snemo::datamodel::tracker_digitized_hit::gg_times & udd_gg_timestamp = udd_tracker_hit.get_times()[iggtime];
          is_corresponding_tracker_valid = true;
          if (udd_tracker_hit.get_geom_id() ==  red_tracker_hit.get_geom_id()
              && udd_tracker_hit.get_hit_id() == red_tracker_hit.get_hit_id()
              && udd_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R0).get_hit_number() == red_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R0).get_hit_number()
              && udd_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R0).get_trigger_id() == red_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R0).get_trigger_id()
              && udd_gg_timestamp.get_anode_time(snemo::datamodel::tracker_digitized_hit::ANODE_R0) == red_gg_timestamp.get_anode_time(snemo::datamodel::tracker_digitized_hit::ANODE_R0).get_ticks()
              && udd_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R1).get_hit_number() == red_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R1).get_hit_number()
              && udd_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R1).get_trigger_id() == red_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R1).get_trigger_id()
              && udd_gg_timestamp.get_anode_time(snemo::datamodel::tracker_digitized_hit::ANODE_R1) == red_gg_timestamp.get_anode_time(snemo::datamodel::tracker_digitized_hit::ANODE_R1).get_ticks()
              && udd_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R2).get_hit_number() == red_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R2).get_hit_number()
              && udd_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R2).get_trigger_id() == red_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R2).get_trigger_id()
              && udd_gg_timestamp.get_anode_time(snemo::datamodel::tracker_digitized_hit::ANODE_R2) == red_gg_timestamp.get_anode_time(snemo::datamodel::tracker_digitized_hit::ANODE_R2).get_ticks()
              && udd_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R3).get_hit_number() == red_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R3).get_hit_number()
              && udd_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R3).get_trigger_id() == red_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R3).get_trigger_id()
              && udd_gg_timestamp.get_anode_time(snemo::datamodel::tracker_digitized_hit::ANODE_R3) == red_gg_timestamp.get_anode_time(snemo::datamodel::tracker_digitized_hit::ANODE_R3).get_ticks()
              && udd_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R4).get_hit_number() == red_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R4).get_hit_number()
              && udd_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R4).get_trigger_id() == red_gg_timestamp.get_anode_origin(snemo::datamodel::tracker_digitized_hit::ANODE_R4).get_trigger_id()
              && udd_gg_timestamp.get_anode_time(snemo::datamodel::tracker_digitized_hit::ANODE_R4) == red_gg_timestamp.get_anode_time(snemo::datamodel::tracker_digitized_hit::ANODE_R4).get_ticks()
              && udd_gg_timestamp.get_bottom_cathode_origin().get_hit_number() == red_gg_timestamp.get_bottom_cathode_origin().get_hit_number()
              && udd_gg_timestamp.get_bottom_cathode_origin().get_trigger_id() == red_gg_timestamp.get_bottom_cathode_origin().get_trigger_id()
              && udd_gg_timestamp.get_bottom_cathode_time() == red_gg_timestamp.get_bottom_cathode_time().get_ticks()
              && udd_gg_timestamp.get_top_cathode_origin().get_hit_number() == red_gg_timestamp.get_top_cathode_origin().get_hit_number()
              && udd_gg_timestamp.get_top_cathode_origin().get_trigger_id() == red_gg_timestamp.get_top_cathode_origin().get_trigger_id()
              && udd_gg_timestamp.get_top_cathode_time() == red_gg_timestamp.get_top_cathode_time().get_ticks())
            is_corresponding_timestamp_valid = true;
        }
      list_timestamps_corresponding.push_back(is_corresponding_timestamp_valid);
      is_corresponding_tracker_valid = std::all_of(list_timestamps_corresponding.begin(), list_timestamps_corresponding.end(), [](bool v) { return v; });
      list_trackers_corresponding.push_back(is_corresponding_tracker_valid);
    } // end of is_corresponding_udd

  } // end of tracker red ihit

  is_tracker_equivalent = std::all_of(list_trackers_corresponding.begin(), list_trackers_corresponding.end(), [](bool v) { return v; });


  DT_LOG_DEBUG(logging_, "EH is equivalent" << is_event_header_equivalent
               << " UDD global is equivalent = " << is_udd_global_equivalent
               << " UDD Calo is equivalent = " << is_calo_equivalent
               << " UDD Tracker is equivalent = " << is_tracker_equivalent);
  if (is_event_header_equivalent && is_udd_global_equivalent && is_calo_equivalent && is_tracker_equivalent) red_er_is_equivalent = true;

  return red_er_is_equivalent;
}
