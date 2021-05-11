#pragma once

#include <protozero/types.hpp>
#include <vector>
#include <map>

#define VT_LEGACY_IMAGE_SIZE 256.0

namespace                         htl   {
namespace                         tiles {

//------------------------------------------------------------------------------
enum class                        TileFormat
{
  UNKNOWN                       = 0 ,
  GZIP                              , // encoding = gzip
  ZLIB                              , // encoding = deflate
  PNG                               ,
  JPG                               ,
  PBF                               ,
  WEBP                              ,
};

using  tile_pattern_t            = std::map< TileFormat, std::vector<uint8_t> >;
extern tile_pattern_t              tile_format_patterns;

enum Tile_Encoding : protozero::pbf_tag_type
{
    LAYERS = 3,
};

enum Layer_Encoding : protozero::pbf_tag_type
{
    VERSION = 15,
    NAME = 1,
    FEATURES = 2,
    KEYS = 3,
    VALUES = 4,
    EXTENT = 5,
};

enum Feature_Encoding : protozero::pbf_tag_type
{
    ID = 1,
    TAGS = 2,
    TYPE = 3,
    GEOMETRY = 4,
    RASTER = 5,
};

enum Value_Encoding : protozero::pbf_tag_type
{
    STRING = 1,
    FLOAT = 2,
    DOUBLE = 3,
    INT = 4,
    UINT = 5,
    SINT = 6,
    BOOL = 7,
};

enum Geometry_Type : std::uint8_t
{
    UNKNOWN = 0,
    POINT = 1,
    LINESTRING = 2,
    POLYGON = 3,
};

enum polygon_fill_type : std::uint8_t
{
    even_odd_fill = 0,
    non_zero_fill,
    positive_fill,
    negative_fill,
    polygon_fill_type_max,
};


enum CommandType : std::uint8_t
{
    SEG_END     = 0x00,
    SEG_MOVETO  = 0x01,
    SEG_LINETO  = 0x02,
    SEG_CLOSE   = ( 0x40 | 0x0f )
};

//------------------------------------------------------------------------------
bool        is_zlib(const char * data, std::size_t size);
inline bool is_zlib(std::string const& data)
{
  return is_zlib( const_cast<std::string&>(data).c_str(), 3 );
}

bool        is_gzip(const char * data, std::size_t size);
inline bool is_gzip(std::string const& data)
{
  return is_gzip( const_cast<std::string&>(data).c_str(), 3 );
}

bool        is_png(const char * data, std::size_t size);
inline bool is_png(std::string const& data)
{
  return is_png( const_cast<std::string&>(data).c_str(), 7 );
}

bool        is_jpeg(const char * data, std::size_t size);
inline bool is_jpeg(std::string const& data)
{
  return is_jpeg( const_cast<std::string&>(data).c_str(), 3 );
}

bool        is_webp(const char * data, std::size_t size);
inline bool is_webp(std::string const& data)
{
  return is_webp( const_cast<std::string&>(data).c_str(), 14 );
}

//------------------------------------------------------------------------------

}// namespace                     tiles
}// namespace                     htl
