#include "frame_info.hh"

#include <sstream>

FrameInfo::FrameInfo( std::string frame_name, std::string ivf_filename,
  size_t offset, size_t length )
  : frame_(),
    output_raster_(),
    frame_name( frame_name ),
    ivf_filename( ivf_filename ),
    offset( offset ),
    length( length ),
    source_hash( frame_name ),
    target_hash( frame_name )
{}

FrameInfo::FrameInfo( std::string ivf_filename, size_t offset, size_t length,
  SourceHash source_hash, TargetHash target_hash )
  : frame_(),
    output_raster_(),
    frame_name(),
    ivf_filename( ivf_filename ),
    offset( offset ),
    length( length ),
    source_hash( source_hash ),
    target_hash( target_hash )
{
  stringstream ss;
  ss << source_hash.str() << "#" << target_hash.str();
  frame_name = ss.str();
}

Chunk FrameInfo::chunk( void ) const
{
  return Chunk( &frame_.at( 0 ), frame_.size() );
}

bool FrameInfo::validate_source( const DecoderHash & decoder_hash ) const
{
  return decoder_hash.can_decode( source_hash );
}

bool FrameInfo::validate_target( const DecoderHash & decoder_hash ) const
{
  return target_hash.continuation_hash == decoder_hash.continuation_hash();
}

bool FrameInfo::shown( void ) const
{
  return target_hash.shown;
}

double FrameInfo::quality( const RasterHandle & original ) const
{
  assert( output_raster_.initialized() );
  return original.get().quality( output_raster_.get() );
}

unsigned FrameInfo::size( void ) const
{
  return frame_.size();
}

void FrameInfo::write( const std::string & path ) const
{
  ofstream file( path + frame_name, ofstream::binary );
  file.write( reinterpret_cast<const char *>(frame_.data()), frame_.size() );
}
