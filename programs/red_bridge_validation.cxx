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

// - Falaise:
#include <falaise/snemo/datamodels/unified_digitized_data.h>
#include <falaise/snemo/datamodels/event_header.h>

// - SNFEE:
#include <snfee/snfee.h>
#include <snfee/io/multifile_data_reader.h>
#include <snfee/data/raw_event_data.h>


void compare_red_udd_event(const snfee::data::raw_event_data,
                           const snemo::datamodel::unified_digitized_data);

//----------------------------------------------------------------------
// MAIN PROGRAM
//----------------------------------------------------------------------

int main (int argc, char *argv[])
{
  std::string input_red_filename = "";
  std::string input_udd_filename = "";

  for (int iarg=1; iarg<argc; ++iarg)
    {
      std::string arg (argv[iarg]);
      if (arg[0] == '-')
        {
          if (arg=="-ired" || arg=="--input-red")
            input_red_filename = std::string(argv[++iarg]);

          else if (arg=="-iudd" || arg=="--input-udd")
            input_udd_filename = std::string(argv[++iarg]);

          else if (arg=="-h" || arg=="--help")
            {
              std::cout << std::endl;
              std::cout << "Usage:   " << argv[0] << " [options]" << std::endl;
              std::cout << std::endl;
              std::cout << "Options:   -h / --help" << std::endl;
              std::cout << "           -ired / --input-red   RED_FILE" << std::endl;
              std::cout << "           -iudd / --input-udd  UDD_FILE" << std::endl;
              std::cout << std::endl;
              return 0;
            }

          else
            std::cerr << "*** unkown option " << arg << std::endl;
        }
    }

  if (input_red_filename.empty() || input_udd_filename.empty())
    {
      std::cerr << "*** missing input RED or UDD filename !" << std::endl;
      return 1;
    }

  snfee::initialize();

  // RED counter
  std::size_t red_counter = 0;

  // UDD counter
  std::size_t udd_events = 0;


  std::cout << "Results :" << std::endl;
  std::cout << "- Worker #0 (input RED)" << std::endl;
  std::cout << "  - Processed records : " << red_counter << std::endl;
  std::cout << "- Worker #1 (output UDD)" << std::endl;
  std::cout << "  - Stored records : " << udd_events << std::endl;

  snfee::terminate();

  return 0;
}
