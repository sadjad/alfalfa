#ifndef FRAME_INFO_HH
#define FRAME_INFO_HH

#include <string>
#include <fstream>

#include "decoder.hh"
#include "dependency_tracking.hh"

using namespace std;

class FrameInfo
{
private:
  std::vector<uint8_t> frame_;
  Optional<RasterHandle> output_raster_;

public:
  std::string frame_name;
  std::string ivf_filename;
  size_t offset;
  size_t length;
  SourceHash source_hash;
  TargetHash target_hash;

  FrameInfo( std::string frame_name, std::string ivf_filename, size_t offset,
    size_t length );

  FrameInfo( std::string ivf_filename, size_t offset, size_t length,
    SourceHash source_hash, TargetHash target_hash );

  Chunk chunk( void ) const;
  bool validate_source( const DecoderHash & decoder_hash ) const;
  bool validate_target( const DecoderHash & decoder_hash ) const;
  bool shown( void ) const;
  RasterHandle get_output( void ) const { return output_raster_.get(); }
  double quality( const RasterHandle & original ) const;
  unsigned size( void ) const;
  void write( const std::string & path = "" ) const;
};

#endif /* FRAME_INFO_HH */
