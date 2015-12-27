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

    auto frames = video.get_frames( track_id );
    for ( auto frame = frames.first; frame != frames.second; frame++ ) {
      if ( not frame->target_hash().shown ) continue;
      cout << frame_index++ << ",";
      auto ts = player.get_min_track_seek( frame->target_hash().output_hash );
      auto ss = player.get_min_switch_seek( frame->target_hash().output_hash );

      cout << get<0>( ts ).cost << "," << get<0>( ss ).cost << endl;
    }

  } catch ( const exception & e ) {
    print_exception( argv[ 0 ], e );
    return EXIT_FAILURE;
  }

  return 0;
}
