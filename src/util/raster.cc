#include "raster.hh"

AbstractRaster::AbstractRaster( const unsigned int display_width, const unsigned int display_height )
  : display_width_( display_width ), display_height_( display_height )
{}

size_t AbstractRaster::raw_hash( void ) const
{
  size_t hash_val = 0;

  boost::hash_range( hash_val, Y().begin(), Y().end() );
  boost::hash_range( hash_val, U().begin(), U().end() );
  boost::hash_range( hash_val, V().begin(), V().end() );

  return hash_val;
}

double AbstractRaster::quality( const AbstractRaster & other ) const
{
  return ssim( Y(), other.Y() );
}

bool AbstractRaster::operator==( const AbstractRaster & other ) const
{
  return (Y() == other.Y()) and (U() == other.U()) and (V() == other.V());
}

bool AbstractRaster::operator!=( const AbstractRaster & other ) const
{
  return not operator==( other );
}

void AbstractRaster::copy_from( const AbstractRaster & other )
{
  Y().copy_from( other.Y() );
  U().copy_from( other.U() );
  V().copy_from( other.V() );
}

void AbstractRaster::dump( FILE * file ) const
{
  for ( unsigned int row = 0; row < display_height(); row++ ) {
    fwrite( &Y().at( 0, row ), display_width(), 1, file );
  }
  for ( unsigned int row = 0; row < (1 + display_height()) / 2; row++ ) {
    fwrite( &U().at( 0, row ), (1 + display_width()) / 2, 1, file );
  }
  for ( unsigned int row = 0; row < (1 + display_height()) / 2; row++ ) {
    fwrite( &V().at( 0, row ), (1 + display_width()) / 2, 1, file );
  }
}
