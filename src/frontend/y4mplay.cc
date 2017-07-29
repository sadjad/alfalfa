/* -*-mode:c++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <iostream>
#include <unistd.h>

#include "player.hh"
#include "display.hh"
#include "yuv4mpeg.hh"
#include "frame_input.hh"

using namespace std;

int main( int argc, char *argv[] )
{
  try {
    if ( argc < 2 ) {
      cerr << "Usage: " << argv[ 0 ] << " FILENAME" << endl;
      return EXIT_FAILURE;
    }

    string filename { argv[ 1 ] };

    shared_ptr<FrameInput> input_reader;

    if ( filename == "-" ) {
      input_reader = make_shared<YUV4MPEGReader>( FileDescriptor( STDIN_FILENO ) );
    }
    else {
      input_reader = make_shared<YUV4MPEGReader>( filename );
    }

    VideoDisplay display { input_reader->get_next_frame().get() };

    while ( true ) {
      display.draw( input_reader->get_next_frame().get() );
    }
  } catch ( const exception & e ) {
    print_exception( argv[ 0 ], e );
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
