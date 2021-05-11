#include "mbtiles/vector_tile_config.h"

namespace                         htl   {
namespace                         tiles {

//------------------------------------------------------------------------------

tile_pattern_t                    tile_format_patterns
{
  { TileFormat::GZIP  , { { 0x1f, 0x8b } } }, // this masks PBF format too
  { TileFormat::ZLIB  , { { 0x78/*, 0x9c*/ } } },
  { TileFormat::PNG   , { { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A } } },
  { TileFormat::JPG   , { { 0xFF, 0xD8, 0xFF } } },
  { TileFormat::WEBP  , { { 0x52, 0x49, 0x46, 0x46, 0xc0, 0x00, 0x00, 0x00, 0x57, 0x45, 0x42, 0x50, 0x56, 0x50 } } },
};

template< size_t High >
inline bool is_valid( TileFormat f, const char* data )
{
  bool result{ nullptr != data };
  auto & patern_ref = tile_format_patterns.at(f);
  for( size_t i(0); result and ( i < High ); i++ )
    result = static_cast<uint8_t>(data[i]) == patern_ref.at(i);
  return result;
}

//------------------------------------------------------------------------------
bool is_zlib(const char* data, std::size_t size)
{
  return
      (data != nullptr )
      and
      (size > 2)
      and
      is_valid<1>( TileFormat::ZLIB, data )
//      static_cast<uint8_t>(data[0]) == tile_format_patterns.at(TileFormat::ZLIB).at(0)
      and
      (
        ( static_cast<uint8_t>(data[1]) == 0x9C)
        or
        (static_cast<uint8_t>(data[1]) == 0x01)
        or
        (static_cast<uint8_t>(data[1]) == 0xDA)
        or
        (static_cast<uint8_t>(data[1]) == 0x5E)
      );
}

bool is_gzip(const char* data, std::size_t size)
{
  return
      (data != nullptr )
      and
      ( size > 2 )
      and
      is_valid<2>( TileFormat::GZIP, data )
//      data[0] == tile_format_patterns.at( TileFormat::GZIP ).at(0)
//      and
//      data[1] == tile_format_patterns.at( TileFormat::GZIP ).at(1)
      ;
}

bool is_png(const char* data, std::size_t size)
{
//  auto & p_ref = tile_format_patterns.at(TileFormat::PNG);
  return
      (size > 7)
      and
      is_valid<8>( TileFormat::PNG, data )
//      static_cast<uint8_t>(data[0]) == static_cast<uint8_t>(p_ref.at(0))
//      and
//      static_cast<uint8_t>(data[1]) == static_cast<uint8_t>(p_ref.at(1))
//      and
//      static_cast<uint8_t>(data[2]) == static_cast<uint8_t>(p_ref.at(2))
//      and
//      static_cast<uint8_t>(data[3]) == static_cast<uint8_t>(p_ref.at(3))
//      and
//      static_cast<uint8_t>(data[4]) == static_cast<uint8_t>(p_ref.at(4))
//      and
//      static_cast<uint8_t>(data[5]) == static_cast<uint8_t>(p_ref.at(5))
//      and
//      static_cast<uint8_t>(data[6]) == static_cast<uint8_t>(p_ref.at(6))
//      and
//      static_cast<uint8_t>(data[7]) == static_cast<uint8_t>(p_ref.at(7))
      ;
}

bool is_jpeg(const char* data, std::size_t size)
{
//  auto & p_ref = tile_format_patterns.at(TileFormat::JPG);
  return
      (size > 2)
      and
      is_valid<3>( TileFormat::JPG, data )
//      static_cast<uint8_t>(data[0]) == static_cast<uint8_t>(p_ref.at(0))
//      and
//      static_cast<uint8_t>(data[1]) == static_cast<uint8_t>(p_ref.at(1))
//      and
//      static_cast<uint8_t>(data[2]) == static_cast<uint8_t>(p_ref.at(2))
      ;
}

bool is_webp(const char* data, std::size_t size)
{
//  auto & p_ref = tile_format_patterns.at(TileFormat::WEBP);
  return
      (size > 2)
      and
      is_valid<14>( TileFormat::WEBP, data )
//      static_cast<uint8_t>(data[0]) == static_cast<uint8_t>(p_ref.at(0))
//      and
//      static_cast<uint8_t>(data[1]) == static_cast<uint8_t>(p_ref.at(1))
//      and
//      static_cast<uint8_t>(data[2]) == static_cast<uint8_t>(p_ref.at(2))
//      and
//      static_cast<uint8_t>(data[3]) == static_cast<uint8_t>(p_ref.at(3))
//      and
//      static_cast<uint8_t>(data[4]) == static_cast<uint8_t>(p_ref.at(4))
//      and
//      static_cast<uint8_t>(data[5]) == static_cast<uint8_t>(p_ref.at(5))
//      and
//      static_cast<uint8_t>(data[6]) == static_cast<uint8_t>(p_ref.at(6))
//      and
//      static_cast<uint8_t>(data[7]) == static_cast<uint8_t>(p_ref.at(7))
//      and
//      static_cast<uint8_t>(data[8]) == static_cast<uint8_t>(p_ref.at(8))
//      and
//      static_cast<uint8_t>(data[9]) == static_cast<uint8_t>(p_ref.at(9))
//      and
//      static_cast<uint8_t>(data[10]) == static_cast<uint8_t>(p_ref.at(10))
//      and
//      static_cast<uint8_t>(data[11]) == static_cast<uint8_t>(p_ref.at(11))
//      and
//      static_cast<uint8_t>(data[12]) == static_cast<uint8_t>(p_ref.at(12))
//      and
//      static_cast<uint8_t>(data[13]) == static_cast<uint8_t>(p_ref.at(13))
      ;
}

//------------------------------------------------------------------------------

}// namespace                     tiles
}// namespace                     htl
