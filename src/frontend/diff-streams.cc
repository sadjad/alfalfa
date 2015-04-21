#include <iostream>

#include "player.hh"
#include "diff_generator.hh"

using namespace std;

int main( int argc, char * argv[] )
{
  if ( argc != 3 ) {
    cerr << "Usage: " << argv[ 0 ] << " SOURCE TARGET" << endl;
    return EXIT_FAILURE;
  }

  Player<DiffGenerator> source_player( argv[ 1 ] );
  Player<DiffGenerator> target_player( argv[ 2 ] );

  while ( !source_player.eof() && !target_player.eof() ) {
    source_player.advance();
    RasterHandle target_raster = target_player.advance();

    vector< uint8_t > diff = target_player - source_player;

    cout << diff.size() << "\n";

    RasterHandle diff_raster = source_player.reconstruct_diff( diff );

    assert( diff_raster.get() == target_raster.get() );
  }
    
  return EXIT_SUCCESS;
}