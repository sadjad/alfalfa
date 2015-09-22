#ifndef RASTER_HH
#define RASTER_HH

#include <memory>
#include <boost/functional/hash.hpp>
#include <cmath>

#include "2d.hh"
#include "ssim.hh"
#include "safe_array.hh"
#include <config.h>

#include <iostream>

/* For an array of pixels, context and separate construction not necessary */
template<>
template< typename... Targs >
TwoDStorage<uint8_t>::TwoDStorage( const unsigned int width, const unsigned int height, Targs&&... Fargs )
  : width_( width ), height_( height ), storage_( width * height, Fargs... )
{
  assert( width > 0 );
  assert( height > 0 );
}

class SimpleRaster
{
protected:
  unsigned int display_width_, display_height_;
  size_t raw_hash( void ) const;

public:
  SimpleRaster( const unsigned int display_width, const unsigned int display_height );

  virtual TwoD< uint8_t > & Y( void ) = 0;
  virtual TwoD< uint8_t > & U( void ) = 0;
  virtual TwoD< uint8_t > & V( void ) = 0;

  virtual const TwoD< uint8_t > & Y( void ) const = 0;
  virtual const TwoD< uint8_t > & U( void ) const = 0;
  virtual const TwoD< uint8_t > & V( void ) const = 0;

  virtual unsigned int width( void ) const = 0;
  virtual unsigned int height( void ) const = 0;
  unsigned int display_width( void ) const { return display_width_; }
  unsigned int display_height( void ) const { return display_height_; }

  // SSIM as determined by libx264
  double quality( const SimpleRaster & other ) const;

  bool operator==( const SimpleRaster & other ) const;
  bool operator!=( const SimpleRaster & other ) const;

  void copy_from( const SimpleRaster & other );
  void dump( FILE * file ) const;

  virtual ~SimpleRaster() {}
};

#endif /* RASTER_HH */
