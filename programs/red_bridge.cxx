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
namespace bpo = boost::program_options;

// - Falaise:
#include <falaise/snemo/datamodels/unified_digitized_data.h>
#include <falaise/snemo/datamodels/event_header.h>

// // - SNCabling
// #include <sncabling/sncabling.h>
// #include <sncabling/calo_signal_cabling.h>
// #include <sncabling/om_id.h>
// #include <sncabling/tracker_cabling.h>
// #include <sncabling/gg_cell_id.h>
// #include <sncabling/label.h>

// - SNFEE:
#include <snfee/snfee.h>
#include <snfee/io/multifile_data_reader.h>
#include <snfee/data/raw_event_data.h>


//----------------------------------------------------------------------
// MAIN PROGRAM
//----------------------------------------------------------------------

int main (int argc, char *argv[])
{
  std::string input_filename = "";
  std::string output_filename = "";

  for (int iarg=1; iarg<argc; ++iarg)
    {
      std::string arg (argv[iarg]);
      if (arg[0] == '-')
        {
          if (arg=="-i" || arg=="--input")
            input_filename = std::string(argv[++iarg]);

          else if (arg=="-o" || arg=="--output")
            output_filename = std::string(argv[++iarg]);

          else if (arg=="-h" || arg=="--help")
            {
              std::cout << std::endl;
              std::cout << "Usage:   " << argv[0] << " [options]" << std::endl;
              std::cout << std::endl;
              std::cout << "Options:   -h / --help" << std::endl;
              std::cout << "           -i / --input   RED_FILE" << std::endl;
              std::cout << "           -o / --output  UDD_FILE" << std::endl;
              std::cout << std::endl;
              return 0;
            }

          else
            std::cerr << "*** unkown option " << arg << std::endl;
        }
    }

  if (input_filename.empty())
    {
      std::cerr << "*** missing input filename !" << std::endl;
      return 1;
    }

  snfee::initialize();

  /// Configuration for raw data reader
  snfee::io::multifile_data_reader::config_type reader_cfg;
  reader_cfg.filenames.push_back(input_filename);

  // Instantiate a reader
  snfee::io::multifile_data_reader red_source(reader_cfg);

  // Working RED object
  snfee::data::raw_event_data red;

  // Working UDD object
  snemo::datamodel::unified_digitized_data udd;

  // RED counter
  std::size_t red_counter = 0;

  while (red_source.has_record_tag())
    {
      // Check the serialization tag of the next record:
      DT_THROW_IF(!red_source.record_tag_is(snfee::data::raw_event_data::SERIAL_TAG),
                  std::logic_error, "Unexpected record tag '" << red_source.get_record_tag() << "'!");

      // Load the next RED object:
      red_source.load(red);

      // Do the RED to UDD conversion
      do_red_to_udd_conversion(red, udd);


      // Write UDD event in the output file



      // Clear working RED and working UDD



      // Increment the counter
      red_counter++;

    } // (while red_source.has_record_tag())

  std::cout << "Total RED object processed into UDD object = " << red_counter << std::endl;

  snfee::terminate();

  return 0;
}



void do_red_to_udd_conversion(snfee::data::raw_event_data red_,
                              snemo::datamodel::unified_digitized_data udd_)
{
  // Run number
  int32_t red_run_id   = red.get_run_id();

  // Event number
  int32_t red_event_id = red.get_event_id();

  // Container of merged TriggerID(s) by event builder
  const std::set<int32_t> & red_trigger_ids = red.get_origin_trigger_ids();

  // RED Digitized calo hits
  const std::vector<snfee::data::calo_digitized_hit> red_calo_hits = red.get_calo_hits();

  // RED Digitized tracker hits
  const std::vector<snfee::data::tracker_digitized_hit> red_tracker_hits = red.get_tracker_hits();

  // Print RED infos
  std::cout << "Event #" << red_event_id << " contains "
            << red_trigger_ids.size() << " TriggerID(s) with "
            << red_calo_hits.size() << " calo hit(s) and "
            << red_tracker_hits.size() << " tracker hit(s)"
            << std::endl;

  // Fill Event Header based on RED attributes
  snemo::datamodel::event_header EH;
  EH.get_id().set_run_number(red_run_id);
  EH.get_id().set_event_number(red_event_id);
  EH.set_generation(snemo::datamodel::event_header::GENERATION_REAL);

  // GO: we have to decide how we handle time and timestamp at Falaise level, commenting for now...
  // EH.get_timestamp().set_seconds(1268644034);
  // EH.get_timestamp().set_picoseconds(666);

  // GO: we can add some additional properties to the Event Header
  // EH.get_properties().store("simulation.bundle", "falaise");
  // EH.get_properties().store("simulation.version", "0.1");
  // EH.get_properties().store("author", std::string(getenv("USER")));


  // Copy RED attributes to UDD attributes
  udd_.set_run_id(red_run_id);
  udd_.set_event_id(red_event_id);
  udd_.set_reference_timestamp(red_.get_reference_time());
  udd_.set_origin_trigger_ids(red_trigger_ids);
  udd_.set_auxiliaries(red_.get_auxiliaries());

  // Copy RED calo digitized hit into UDD calo digitized hit:
  for (size_t ihit = 0; ihit < red_calo_hits.size(), ihit++)
    {
      snfee::data::calo_digitized_hit red_calo_hit = red_calo_hits[ihit];
      snemo::datamodel::calorimeter_digitized_hit & udd_calo_hit = udd_.add_calorimeter_hit();

      udd_calo_hit.set_geom_id(red_calo_hit.get_geom_id());
      udd_calo_hit.set_timestamp(red_calo_hit.get_reference_time());
      udd_calo_hit.set_waveform(red_calo_hit.get_waveform());
      udd_calo_hit.set_low_threshold_only(red_calo_hit.get_low_threshold_only());
      udd_calo_hit.set_high_threshold(red_calo_hit.get_high_threshold());
      udd_calo_hit.set_fcr(red_calo_hit.get_fcr());
      udd_calo_hit.set_lt_trigger_counter(red_calo_hit.get_lt_trigger_counter());
      udd_calo_hit.set_lt_time_counter(red_calo_hit.get_lt_time_counter());
      udd_calo_hit.set_fwmeas_baseline(red_calo_hit.get_fwmeas_baseline());
      udd_calo_hit.set_fwmeas_peak_amplitude(red_calo_hit.get_fwmeas_peak_amplitude());
      udd_calo_hit.set_fwmeas_peak_cell(red_calo_hit.get_fwmeas_peak_cell());
      udd_calo_hit.set_fwmeas_charge(red_calo_hit.get_fwmeas_charge());
      udd_calo_hit.set_fwmeas_rising_cell(red_calo_hit.get_fwmeas_rising_cell());
      udd_calo_hit.set_fwmeas_falling_cell(red_calo_hit.get_fwmeas_falling_cell());
      udd_calo_hit.set_origin(red_calo_hit.get_origin());
    } // end of for ihit



  // Copy RED tracker digitized hit into UDD calo digitized hit:
  for (size_t ihit=0; ihit < red_tracker_hits.size(), ihit++)
    {
      snfee::data::tracker_digitized_hit red_tracker_hit = red_tracker_hits[i];
      snemo::datamodel::tracker_digitized_hit & udd_tracker_hit = udd_.add_tracker_hit();
      udd_tracker_hit.set_geom_id(red_tracker_hit.get_geom_id());


      // GG timestamps
	  const std::vector<snemo::datamodel::tracker_digitized_hit::gg_times> & gg_timestamps_v = red_tracker_hit.get_times();
	  // NB: several timestamps may be read from the same GG cell (probably due to noise ?).
	  // If so, decision should be done on which one has to be used -- Looks to be rare fortunatly

	  // Scan timestamps
	  if (gg_timestamps_v.size() >= 1)
	    {
	      // Case without multiple hit in the same category
	      const snemo::datamodel::tracker_digitized_hit::gg_times & gg_timestamps = gg_timestamps_v.front();

	      // ANODE timestamps
	      const snemo::datamodel::timestamp anode_timestamp_r0 = gg_timestamps.get_anode_time(0);
	      const int64_t anode_tdc_r0 = anode_timestamp_r0.get_ticks(); // >>> 1 tracker TDC tick = 12.5E-9 sec

	      const snemo::datamodel::timestamp anode_timestamp_r1 = gg_timestamps.get_anode_time(1);
	      const int64_t anode_tdc_r1 = anode_timestamp_r1.get_ticks();

	      const snemo::datamodel::timestamp anode_timestamp_r2 = gg_timestamps.get_anode_time(2);
	      const int64_t anode_tdc_r2 = anode_timestamp_r2.get_ticks();

	      const snemo::datamodel::timestamp anode_timestamp_r3 = gg_timestamps.get_anode_time(3);
	      const int64_t anode_tdc_r3 = anode_timestamp_r3.get_ticks();

	      const snemo::datamodel::timestamp anode_timestamp_r4 = gg_timestamps.get_anode_time(4);
	      const int64_t anode_tdc_r4 = anode_timestamp_r4.get_ticks();

	      // CATHODE timestamps
	      const snemo::datamodel::timestamp bottom_cathode_timestamp = gg_timestamps.get_bottom_cathode_time();
	      const int64_t bottom_cathode_tdc = bottom_cathode_timestamp.get_ticks();

	      const snemo::datamodel::timestamp top_cathode_timestamp = gg_timestamps.get_top_cathode_time();
	      const int64_t top_cathode_tdc = top_cathode_timestamp.get_ticks();
	    }


      return;
    }
