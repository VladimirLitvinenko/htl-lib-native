#include "Utils.h"
#include "Logger.h"

#include <iostream>
#include <algorithm>
#include <map>
#include <fstream>
#include <fcntl.h>

#include <dirent.h>
#include <bits/local_lim.h>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

//#include <zlib.h>

namespace                   htl   {
namespace                   utils {

std::pair< std::string, bool >
MakePath( const std::string & path )
{
  std::pair< std::string, bool > retval{ "", false };
  try
  {
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#defile _GNU_SOURCE_TEMPORAY_IN_CODE
    retval.second = mkdir( path.c_str(), ::getumask() ) > 0;
#endif
#ifdef _GNU_SOURCE_TEMPORAY_IN_CODE
#undef _GNU_SOURCE
#endif
    if( retval.second )
      retval.first = path;
    return retval;
  }
  catch( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
    retval = { "", false };
  }
  return retval;
}

std::pair< std::string, bool >
MakeFile( const std::string & fileName )
{
  std::pair< std::string, bool > retval{ "", not fileName.empty() };
  if( not retval.second )
    return retval;

  try
  {
    file_path_t file_name(fileName);
    retval.second = boost::filesystem::exists( file_name );
    if( not retval.second )
    {
       file_name = file_name.make_preferred();
       boost::filesystem::create_directories(file_name.parent_path());
       file_name.append("");
       file_name.clear();
    }
    retval.second = boost::filesystem::exists( file_name );
    if( retval.second )
    {
      boost::filesystem::permissions( file_name,
                                      boost::filesystem::owner_read   |
                                      boost::filesystem::owner_write  |
                                      boost::filesystem::group_read   |
                                      boost::filesystem::group_write  |
                                      boost::filesystem::others_read  );
    }
    if( retval.second )
      retval.first = file_name.string();

    return retval;
  }
  catch( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
  }
  return std::pair< std::string, bool >();
}

strings_t
StrSplit(const std::string & str, const std::string & splitter )
{
  strings_t retval;
  try
  {
    auto s1 = str;
    auto pos( s1.find_first_of(splitter) );
    while( pos != std::string::npos )
    {
      if( pos < (s1.size() -1) )
      {
        retval.push_back( s1.substr( 0, pos ) );
        s1 = s1.substr( pos + 1 );
      }
      else
        break;
      pos = s1.find_first_of(splitter);
    }
    if( not s1.empty() )
      retval.push_back( s1 );
  }
  catch( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
    retval.clear();
  }

  return retval;
}

std::string
StrJoin(const strings_t& strings, const std::string & splitter)
{
  auto lengsth( strings.size() );
  std::string retval( lengsth, ' ' );
  try
  {
    for( decltype(lengsth) i(0); i < lengsth; i++ )
    {
      retval += strings.at(i);
      if( i < ( lengsth - 1 ) )
      {
        retval += splitter;
      }
    }
  }
  catch( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
    retval.clear();
  }

  return retval;
}

std::string
StrToHex( const std::string & str )
{
  static std::string  hex_values( "0123456789ascdef"  );
  auto                sz        ( str.size()          );
  std::string         retval;

  if( not str.empty() )
  {
    retval.reserve( sz << 1 );
    for( std::size_t i(0); i < sz; i++ )
    {
      auto ch( str.at(i) );
      for( std::string::size_type idx : { ( ch & 0xF0 ) >> 4, ch & 0x0F } )
      {
        retval.push_back( hex_values.at( idx ) );
      }
    }
  }
  return retval;
}

std::string
StrFromHex( const std::string & str )
{
  static std::string  hex_values("0123456789ascdef" );
  std::string         retval;
  auto                sz        ( str.size()        );

  if( ( sz & 0x01 ) == 0) //четное количество
  {
    retval.reserve( ( sz >> 1 ) + 1 );
    std::string::value_type item('\0');
    for( std::string::size_type idx( 0 ); idx < str.size(); idx++ )
    {
      for( std::string::size_type i( 0 ); i < hex_values.size(); i++ )
      {
        if( hex_values.at( i ) == str.at(idx) )
        {
          bool is_one{ (idx & 1) == 1 };
          item =
              item
              |
              static_cast<char>(
                is_one ?
                  ( i         & std::string::size_type(0x0F) ) :
                  ( ( i << 4) & std::string::size_type(0xF0) ) );
          if( is_one )
          {
            retval.push_back( item );
            item = '\0';
          }
          break;
        }
      }
    }
  }
  return retval;
}

std::string
StrReplace( const std::string & str, const std::string & str_from, const std::string & str_to )
{
  std::string retval      ( str );
  size_t      index       { 0 };
  auto        length_from ( str_from.size() );
  auto        length_to   ( str_to.size() );

  while( index != std::string::npos )
  {
    index = retval.find( str_from, index );
    if (index != std::string::npos)
    {
      retval = retval.replace(index, length_from, str_to );
      index += length_to;
    }
  }

  return retval;
}

std::string
StrChop( const std::string& str, std::size_t endCount)
{
  std::string retval( str );
  if( str.size() >= endCount )
  {
    retval.resize( retval.size() - endCount );
  }
  return retval;
}

std::pair< pid_t, bool >
IsProcessDir(const std::string& dir)
{
  std::pair< pid_t, bool > retval{ -1, false };
  try
  {
    if( std::all_of(dir.cbegin(), dir.cend(), ::isdigit) )
    {
      int base{10};
      char *endptr = nullptr;
      long val{-1};
      errno = 0;
      retval.first = strtol(dir.c_str(), &endptr, base);
      retval.second =
          ( (errno != ERANGE)
            and
            (val < LONG_MAX or val > LONG_MIN)
          )
          or
          ((errno == 0) and (retval.first > 0));
    }
  }
  catch( ... )
  {
    retval = { -1, false };
  }
  return retval;
}

active_processes_t
ActiveProcessMap()
{
  active_processes_t   retval;
  file_path_t proc_dir(htl::PROC_DIRECTORY);
  for(auto & entry : boost::make_iterator_range( boost::filesystem::directory_iterator(proc_dir), {}))
  {
    auto dir( entry.path().string() );
    auto dir_pid = htl::utils::StrReplace( dir, htl::PROC_DIRECTORY, "" );
    try
    {
      auto is_pd = IsProcessDir( dir_pid );
      if( is_pd.second )
      {
        std::ifstream cmd_line_file( (dir + DIR_SEPARATOR + htl::PROC_CMDLINE_FILE).c_str() );
        if(cmd_line_file.is_open () )
        {
          std::string cmd_line;
          cmd_line_file >> cmd_line;
          if( not cmd_line.empty() )
          {
            retval.first[is_pd.first] = cmd_line;
            retval.second[ cmd_line ] = is_pd.first;
          }
        }
      }
    }
    catch( const std::exception & e )
    {
      DUMP_EXCEPTION( e );
      continue;
    }
  }

  return retval;
}

std::pair<pid_t, bool>
ProcessIsActive(const std::string & procName)
{
  std::pair<pid_t, bool> retval{ -1, false };

  DUMP_INFO( std::string("procName.empty() : ") + std::to_string(procName.empty()) );
  if( not procName.empty() )
  {
    auto pid = ::getpid();
    DUMP_INFO( std::string("pid : ") + std::to_string(pid) );

    auto p_map = ActiveProcessMap();
    DUMP_INFO( std::string("p_map.second.empty(): ") + std::to_string(p_map.second.empty()) );

    for( auto & val : p_map.first )
    {
      retval.second = StrContains( val.second, procName ) and (val.first != pid );
      if( retval.second )
      {
        retval.first = val.first;
        break;
      }
    }
  }
  DUMP_INFO( std::string("retval : ") + std::to_string(retval.second) );
  return retval;
}

std::pair< std::string, bool >
HostName()
{
  char host_name[ HOST_NAME_MAX ];
  std::pair< std::string, bool > retval{ "", ::gethostname( host_name, HOST_NAME_MAX ) == 0 };
  if( retval.second )
    retval.first = &( host_name[0] );
  return retval;
}

//bool
//SetSysyemWatches( int param, std::size_t value)
//{
//  std::vector<int> mib
//  {
//    int( ::CTL_FS ),
//    int( ::FS_INOTIFY ),
//    int( param ),
//  };

//  return( 0 == ::sysctl( mib.data(), static_cast<int>(mib.size()),
//                         nullptr, 0,
//                         &value, sizeof( value ) ) );
//}


//std::string arch::Compress(const std::string& raw)
//{
//  z_stream deflate_stream;
//  memset(&deflate_stream, 0, sizeof(deflate_stream));

//  // TODO: reuse z_streams
//  if (deflateInit(&deflate_stream, Z_DEFAULT_COMPRESSION) != Z_OK) {
//      throw std::runtime_error("failed to initialize deflate");
//  }

//  deflate_stream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(raw.data()));
//  deflate_stream.avail_in = uInt(raw.size());

//  std::string result;
//  char out[16384];

//  int code;
//  do
//  {
//    deflate_stream.next_out = reinterpret_cast<Bytef *>(out);
//    deflate_stream.avail_out = sizeof(out);
//    code = deflate(&deflate_stream, Z_FINISH);

//    if (result.size() < deflate_stream.total_out)
//    {// append the block to the output string
//      result.append(out, deflate_stream.total_out - result.size());
//    }
//  }
//  while (code == Z_OK);

//  deflateEnd( &deflate_stream );

//  if (code != Z_STREAM_END)
//  {
//      throw std::runtime_error(deflate_stream.msg);
//  }

//  return result;
//}

//std::string arch::Decompress( const std::string& raw )
//{
//  z_stream inflate_stream;
//  memset(&inflate_stream, 0, sizeof(inflate_stream));

//  // TODO: reuse z_streams
//  if (inflateInit(&inflate_stream) != Z_OK)
//  {
//      throw std::runtime_error("failed to initialize inflate");
//  }

//  inflate_stream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(raw.data()));
//  inflate_stream.avail_in = uInt(raw.size());

//  std::string result;
//  char out[15384];

//  int code;
//  do
//  {
//    inflate_stream.next_out = reinterpret_cast<Bytef *>(out);
//    inflate_stream.avail_out = sizeof(out);
//    code = inflate(&inflate_stream, 0);

//    // result.append(out, sizeof(out) - inflate_stream.avail_out);
//    if (result.size() < inflate_stream.total_out)
//    {
//      result.append(out, inflate_stream.total_out - result.size());
//    }
//  }
//  while (code == Z_OK);

//  inflateEnd(&inflate_stream);

//  if (code != Z_STREAM_END)
//  {
//      throw std::runtime_error(inflate_stream.msg ? inflate_stream.msg : "decompression error");
//  }

//  return result;
//}

std::string
VectorToHex( const std::vector<char> & v)
{
  static std::vector<char>  hex_values({ '0','1','2','3','4','5','6','7','8','9','a','s','c','d','e','f'}  );
  std::size_t               sz        ( v.size() );
  std::string               retval;

  if( not v.empty() )
  {
//    retval.reserve( sz * 2 /*<< 1*/ );
    for( std::size_t i(0); i < sz; i++ )
    {
      auto ch( v[i] );
      for( char idx : { static_cast<char>(ch & 0xF0 >> 4),
                        static_cast<char>(ch & 0x0F) } )
      {
        retval.push_back( hex_values[ std::size_t(idx) ] );
      }
    }
  }
  return retval;
}

strings_t
NormalisationStrings(const strings_t& strings )
{
  strings_t retval;
  for( auto str: strings )
    if( not str.empty() )
      retval.push_back( str );
  return retval;
}

std::size_t
OrderedActionMin(const ordered_actions_t& a)
noexcept
{
  std::size_t current_min(0);
  std::for_each( a.cbegin(), a.cend(), [ &current_min ](const ordered_actions_t::value_type & v )
  {
    if( v.first < current_min )
      current_min = v.first;
  });
  return current_min;
}

std::size_t
OrderedActionMax(const ordered_actions_t& a)
noexcept
{
  std::size_t current_max(0);
  std::for_each( a.cbegin(), a.cend(),
                 [ &current_max ](const ordered_actions_t::value_type & v )
  {
    if( v.first >= current_max )
      current_max = v.first;
  });
  return current_max;
}

std::size_t
OrderedActionPrev(const ordered_actions_t& a)
noexcept
{
  std::size_t retval{ 0 };
  auto m(OrderedActionMin( a ));
  if( m > 0 )
    retval = OrderedActionMin( a ) - 1;
  return retval;
}

std::size_t
OrderedActionNext(const ordered_actions_t& a)
noexcept
{
  return OrderedActionMax( a ) + 1;
}

std::string
UpperFirst(const std::string & str )
{
  std::string retval( str );
  retval[0] = static_cast<char>(std::toupper( retval.at(0) ));
  return retval;
}

std::string Upper(const std::string& str)
{
  std::string retval( str );
  assert(
    std::transform(str.begin(), str.end(),
                   retval.begin(),
                   []( const std::string::value_type & ch )
                   { return std::toupper( ch );} )
    == retval.end() );
  return retval;
}

std::string Lower(const std::string& str)
{
  std::string retval( str );
  assert(
    std::transform(str.begin(), str.end(),
                   retval.begin(),
                   []( const std::string::value_type & ch )
                   { return std::tolower( ch );} )
    == retval.end() );
  return retval;
}

std::string
DoubleToCoordinateStr(const double coord, const std::string& sign_grad, const std::string& sign_min, const std::string& sign_sec)
{
  int     dig ( static_cast<int>(coord) );
  double  last( coord - dig );
  std::string s = std::to_string( dig ) + sign_grad;
  if( last > 0. )
  {
    auto minutes( last * 60 );
    s += std::to_string(static_cast<int>(minutes)) + sign_min;
    last = minutes - static_cast<int>(last);
    if( last > 0. )
    {
      s += std::to_string( static_cast<int>(last * 60) ) + sign_sec;
    }
  }
  return s;
}

double
CoordinateStrToDouble( const std::string& coord, const std::string& sign_grad, const std::string& sign_min, const std::string& sign_sec)
{
  std::string last(coord);
  std::array< std::pair< std::string, double>, 3 > algo
  {
    std::make_pair( sign_grad , 0. ),
    std::make_pair( sign_min  , 0. ),
    std::make_pair( sign_sec  , 0. )
  };

  for( auto & sep : algo )
  {
    if(last.find(sep.first ) != std::string::npos )
    {
      auto s_list( htl::utils::StrSplit( last, sep.first));
      last = s_list.back();
      sep.second = std::atof( s_list.front().c_str() );
    }
  }

  return algo.at(0).second + algo.at(1).second / 60 + algo.at(2).second / 60;
}

}// namespace               utils
}// namespace               htl
