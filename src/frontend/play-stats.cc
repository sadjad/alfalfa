#include <sysexits.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <chrono>
#include <tuple>
#include <exception>
#include <boost/variant.hpp>
#include <boost/format.hpp>

#include "alfalfa_player.hh"
#include "player.hh"
#include "display.hh"
#include "2d.hh"

using namespace std;

int main( int argc, char const *argv[] )
{
  try {
    if ( argc != 2 ) {
      cerr << "Usage: " << argv[ 0 ] << " <alf-video>" << endl;
      return EX_USAGE;
    }

    const string video_dir( argv[ 1 ] );
    PlayableAlfalfaVideo video( video_dir );
    AlfalfaPlayer player( video_dir );

    size_t track_id = 0;
    size_t frame_index = 0;

    size_t num_frames_track_cheaper = 0;
    size_t num_frames_switch_cheaper = 0;

    size_t min_cost;

    auto frames = video.get_frames( track_id );
    for ( auto frame = frames.first; frame != frames.second; frame++ ) {
      if ( not frame->target_hash().shown ) continue;
      cout << frame_index++ << ",";
      auto ts_cost = get<0>( player.get_min_track_seek( frame->target_hash().output_hash ) ).cost;
      auto ss_cost = get<0>( player.get_min_switch_seek( frame->target_hash().output_hash ) ).cost;

      if ( ts_cost > ss_cost ) {
        min_cost = ss_cost;
        num_frames_switch_cheaper++;
      } else {
        min_cost = ts_cost;
        num_frames_track_cheaper++;
      }

      cout << ts_cost << "," << ss_cost << "," << min_cost << endl;
    }

    cout << endl << endl << "Switch cheaper " << num_frames_switch_cheaper << " times" << endl;
    cout << "Backtracking in track cheaper " << num_frames_track_cheaper << " times" << endl;

  } catch ( const exception & e ) {
    print_exception( argv[ 0 ], e );
    return EXIT_FAILURE;
  }

  return 0;
}
