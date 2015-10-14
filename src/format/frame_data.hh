#ifndef FRAME_DATA_HH
#define FRAME_DATA_HH

#include <string>

#include "dependency_tracking.hh"

using namespace std;

struct FrameData
{
  std::string frame_name;

  std::string ivf_filename;
  size_t offset;
  size_t length;

  SourceHash source_hash;
  TargetHash target_hash;

  FrameData( std::string frame_name, std::string ivf_filename, size_t offset,
    size_t length );

  FrameData( std::string ivf_filename, size_t offset, size_t length,
    SourceHash source_hash, TargetHash target_hash );
};

#endif
