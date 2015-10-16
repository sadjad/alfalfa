#ifndef MANIFESTS_HH
#define MANIFESTS_HH

#include <string>
#include <vector>

#include "kvstore.hh"

/* sequence of hashes of the original rasters that the video approximates */

class RasterList
{
public:
  RasterList( const std::string & filename );
};

struct QualityData
{
  size_t approximate_raster;
  double quality;

  friend std::ostream & operator<<(std::ostream & stream, QualityData const & qd) {
    stream << qd.approximate_raster << " " << qd.quality;
    return stream;
  }

  friend std::istream & operator>>(std::istream & stream, QualityData & qd) {
    stream >> qd.approximate_raster >> qd.quality;
    return stream;
  }
};

class TrackDataVector : public vector<std::string>
{
  friend std::ostream & operator<<(std::ostream & stream, TrackDataVector const & tdv) {
    stream << tdv.size() << " ";

    for ( auto it = tdv.begin(); it != tdv.end(); it++ ) {
      stream << *it;

      if ( it + 1 != tdv.end() ) {
        stream << " ";
      }
    }

    return stream;
  }

  friend std::istream & operator>>(std::istream & stream, TrackDataVector & tdv) {
    int count;
    stream >> count;

    tdv.clear();
    tdv.reserve(count);

    for ( int i = 0; i < count; i++ ) {
      std::string temp;
      stream >> temp;
      tdv.emplace_back( temp );
    }

    return stream;
  }
};

/* relation:
   original raster / approximate raster / quality */

using QualityDB = KVStore< size_t, QualityData >;

/* relation:
   ID => { sequence of frame names } */

using TrackDB = KVStore< size_t, TrackDataVector >;

#endif /* MANIFESTS_HH */
