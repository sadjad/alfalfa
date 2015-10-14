#include "frame_data.hh"

#include <sstream>

FrameData::FrameData( std::string frame_name, std::string ivf_filename,
  size_t offset, size_t length )
  : frame_name( frame_name ),
    ivf_filename( ivf_filename ),
    offset( offset ),
    length( length ),
    source_hash( frame_name ),
    target_hash( frame_name )
{}

FrameData::FrameData( std::string ivf_filename, size_t offset, size_t length,
  SourceHash source_hash, TargetHash target_hash )
  : frame_name(),
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
