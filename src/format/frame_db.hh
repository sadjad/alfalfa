#ifndef FRAME_DB_HH
#define FRAME_DB_HH

#include <fstream>
#include <iostream>
#include <iterator>
#include <boost/functional/hash.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>

#include "frame_info.hh"
#include "dependency_tracking.hh"

using namespace std;
using namespace boost;
using namespace boost::multi_index;
using boost::multi_index_container;

struct FrameData_SourceHashHash
{
  size_t operator()( const SourceHash & hash ) const
  {
    size_t hash_val = 0;

    boost::hash_combine( hash_val, hash.state_hash.initialized() ? hash.state_hash.get() : 0 );
    boost::hash_combine( hash_val, hash.continuation_hash.initialized() ? hash.continuation_hash.get() : 0 );
    boost::hash_combine( hash_val, hash.last_hash.initialized() ? hash.last_hash.get() : 0 );
    boost::hash_combine( hash_val, hash.golden_hash.initialized() ? hash.golden_hash.get() : 0 );
    boost::hash_combine( hash_val, hash.alt_hash.initialized() ? hash.alt_hash.get() : 0 );

    return hash_val;
  }
};

struct FrameData_SourceHashEqualTo
{
  bool operator()( const SourceHash & lhs, const SourceHash & rhs ) const
  {
    return ( lhs.state_hash == rhs.state_hash and
      lhs.continuation_hash == rhs.continuation_hash and
      lhs.last_hash         == rhs.last_hash and
      lhs.golden_hash       == rhs.golden_hash and
      lhs.alt_hash          == rhs.alt_hash );
  }
};

struct FrameData_OutputHashExtractor
{
  typedef size_t result_type;

  const result_type & operator()( const FrameInfo & fd ) const { return fd.target_hash.output_hash; }
  result_type & operator()( FrameInfo * fd ) { return fd->target_hash.output_hash; }
};

struct FrameData_SourceHashExtractor
{
  typedef SourceHash result_type;

  const result_type & operator()( const FrameInfo & fd ) const { return fd.source_hash; }
  result_type & operator()( FrameInfo * fd ) { return fd->source_hash; }
};

typedef multi_index_container<
  FrameInfo,
  indexed_by<
    hashed_unique<member<FrameInfo, std::string, &FrameInfo::frame_name> >,
    hashed_non_unique<FrameData_OutputHashExtractor>,
    hashed_non_unique<FrameData_SourceHashExtractor,
      FrameData_SourceHashHash,
      FrameData_SourceHashEqualTo>,
    sequenced<>
  >
> FrameDataSet;

typedef FrameDataSet::nth_index<0>::type FrameDataSetByFrameName;
typedef FrameDataSet::nth_index<1>::type FrameDataSetByOutputHash;
typedef FrameDataSet::nth_index<2>::type FrameDataSetBySourceHash;
typedef FrameDataSet::nth_index<3>::type FrameDataSetRandomAccess;

class FrameDataSetSourceHashSearch
{
private:
  class FrameDataSetSourceHashSearchIterator
    : public std::iterator<std::forward_iterator_tag, FrameInfo>
  {
  private:
    size_t stage_;
    SourceHash source_hash_;
    FrameDataSetBySourceHash & data_set_;
    FrameDataSetBySourceHash::iterator itr_;
    FrameDataSetBySourceHash::iterator begin_, current_end_;

  public:
    FrameDataSetSourceHashSearchIterator( FrameDataSetBySourceHash & data_set,
      SourceHash source_hash, bool end );

    FrameDataSetSourceHashSearchIterator( const FrameDataSetSourceHashSearchIterator & it );

    FrameDataSetSourceHashSearchIterator & operator++();
    FrameDataSetSourceHashSearchIterator operator++( int );

    bool operator==( const FrameDataSetSourceHashSearchIterator & rhs ) const;
    bool operator!=( const FrameDataSetSourceHashSearchIterator & rhs ) const;

    const FrameInfo & operator*() const;
    const FrameInfo * operator->() const;
  };

  SourceHash source_hash_;
  FrameDataSetBySourceHash & data_set_;
  FrameDataSetSourceHashSearchIterator begin_iterator_, end_iterator_;

public:
  typedef FrameDataSetSourceHashSearchIterator iterator;

  FrameDataSetSourceHashSearch( FrameDataSetBySourceHash & data_set, SourceHash source_hash );

  iterator begin() { return begin_iterator_; }
  iterator end() { return end_iterator_; }
};

class FrameDB
{
private:
  std::string filename_;
  FrameDataSet data_;
  void load( ifstream & fin );

public:
  FrameDB( const std::string & filename );

  void insert( FrameInfo fd );

  std::pair<FrameDataSetByFrameName::iterator, FrameDataSetByFrameName::iterator>
  search_by_frame_name( std::string & frame_name );

  std::pair<FrameDataSetByOutputHash::iterator, FrameDataSetByOutputHash::iterator>
  search_by_output_hash( const size_t & output_hash );

  std::pair<FrameDataSetSourceHashSearch::iterator, FrameDataSetSourceHashSearch::iterator>
  search_by_decoder_hash( const DecoderHash & decoder_hash );

  FrameDataSetRandomAccess::iterator begin() { return data_.get<3>().begin(); }
  FrameDataSetRandomAccess::iterator end() { return data_.get<3>().end(); }

  void save();
};

#endif /* FRAME_DB */
