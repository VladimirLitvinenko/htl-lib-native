#include "mbtiles/MBTileInfo.h"
#include "mbtiles/MBTileLiterals.h"
#include "mbtiles/vector_tile_compression.h"
#include "common/Utils.h"
#include <boost/filesystem.hpp>

namespace                         htl   {
namespace                         tiles {

MBTileInfo::MBTileInfo(const MBTileInfo& ti)
  : MBTileInfo::MBTileInfo()
{
  *this = ti;
}

MBTileInfo::MBTileInfo(MBTileInfo&& ti)
  : MBTileInfo::MBTileInfo()
{
  *this = std::move( ti );
}

MBTileInfo&
MBTileInfo::operator=(const MBTileInfo& ti )
noexcept
{
  m_z       = ti.m_z;
  m_x       = ti.m_x;
  m_y       = ti.m_y;
  SetData( ti.m_data );
  return std::ref( * this );
}

MBTileInfo&
MBTileInfo::operator=( MBTileInfo && ti )
noexcept
{
  BEGIN_LOCK_SECTION_SELF_;
  m_z       = ti.m_z;
  m_x       = ti.m_x;
  m_y       = ti.m_y;
  m_data    . swap( ti.m_data );
  DetectFormat();

  ti.ClearData();
  ti.m_z = 0;
  ti.m_x = ti.m_y = 0;
  return std::ref( * this );
  END_LOCK_SECTION_;
}

bool
MBTileInfo::IsEmpty()
const
{
  return m_data.empty();
}

vectors_data_t::value_type
MBTileInfo::GetData()
const
{
  return m_data;
}

std::string
MBTileInfo::DataToString() const
{
  std::string retval;
  if( not IsEmpty() )
  {
    if( is_zlib( m_data.data(), m_data.size() ) or is_gzip( m_data.data(), m_data.size() ) )
    {
      zlib_decompress(  m_data.data(), m_data.size(), retval);
    }
    else
    {
      retval.assign( m_data.begin(), m_data.end() );
    }
  }
  return retval;
}

vectors_data_t::value_type &
MBTileInfo::GetDataRef()
{
  return std::ref( m_data );
}

void
MBTileInfo::SetData( const vectors_data_t::value_type & data )
{
  BEGIN_LOCK_SECTION_SELF_;
  m_data.assign( data.begin(), data.end() );
  DetectFormat();
  END_LOCK_SECTION_;
}

void
MBTileInfo::SetData( vectors_data_t::value_type && data )
{
  BEGIN_LOCK_SECTION_SELF_;
  m_data.swap( data );
  data.clear();
  DetectFormat();
  END_LOCK_SECTION_;
}

void
MBTileInfo::ClearData()
{
  BEGIN_LOCK_SECTION_SELF_;
  m_data.clear();
  END_LOCK_SECTION_;
}

void
MBTileInfo::ResetAll()
{
  BEGIN_LOCK_SECTION_SELF_;
  m_z = 0;
  m_x = m_y = 0;
  ClearData();
  END_LOCK_SECTION_;
}

void
MBTileInfo::SetCoordinates(uint32_t z, uint64_t x, uint64_t y, bool transform_y)
{
  BEGIN_LOCK_SECTION_SELF_;
  m_z = z;
  m_x = x;
  m_y = transform_y ? htl::utils::TileRowTransform(z,y) : y;
  END_LOCK_SECTION_
}

TileFormat
MBTileInfo::GetFormat()
const
{
  return m_format;
}

std::string
MBTileInfo::GetFormatStr()
const
{
  switch ( GetFormat() )
  {
    case TileFormat::PNG:   return MBTile_Format_Png;
    case TileFormat::JPG:   return MBTile_Format_Jpg;
    case TileFormat::PBF:   return MBTile_Format_Pbf;
    case TileFormat::GZIP:  return MBTile_Format_Deflate;
    case TileFormat::WEBP:  return MBTile_Format_Webp;
    default:;
  };
  return "<unknown>";
}

std::string
MBTileInfo::GetFormatMime()
const
{
  switch ( GetFormat() )
  {
    case TileFormat::PNG:   return MBTile_Mime_Png;
    case TileFormat::JPG:   return MBTile_Mime_Jpg;
    case TileFormat::PBF:
    case TileFormat::GZIP:  return MBTile_Mime_Pbf;
    case TileFormat::WEBP:  return MBTile_Mime_Webp;
    default:;
  };
  return "";
}

std::pair<bool, std::string>
MBTileInfo::ToPath(const std::string& output_path , bool decompress)
{
  std::pair<bool, std::string> retval{ not IsEmpty(), "" };
  if( retval.first )
  {
    boost::filesystem::path p( output_path + DIR_SEPARATOR +
                                           std::to_string( GetZ() ) + DIR_SEPARATOR +
                                           std::to_string( GetX() ) + DIR_SEPARATOR );
    retval.first = boost::filesystem::exists( p );
    if( not retval.first )
      retval.first = boost::filesystem::create_directories( p );
    if( retval.first )
    {
      retval.second = p.string() + std::to_string( GetY() ) + MBTile_Proto_Suffix;
      std::ofstream ofs( retval.second, std::ios::binary | std::ios::trunc );
      if( ofs.is_open() )
      {
        if( decompress and ( IsGzip() or IsZlib() ) )
        {
          std::string str_out;
          zlib_decompress( m_data.data(), m_data.size(), str_out );
          ofs.write( str_out.data(), str_out.size() );
        }
        else
        {
          ofs.write( m_data.data(), m_data.size() );
        }
        ofs.close();
      }
    }
  }
  return retval;
}

bool MBTileInfo::IsGzip() const
{
  return is_gzip(m_data.data(), 3 );
}

bool MBTileInfo::IsZlib() const
{
  return is_zlib( m_data.data(), 3 );
}

void
MBTileInfo::DetectFormat()
noexcept
{
  BEGIN_LOCK_SECTION_SELF_;
  m_format =
     IsEmpty()
     ? TileFormat::UNKNOWN
     : htl::tiles::is_gzip( m_data.data(), 3 )
        ? TileFormat::GZIP
        : htl::tiles::is_zlib( m_data.data(), 3 )
          ? TileFormat::ZLIB
          : htl::tiles::is_jpeg( m_data.data(), 3 )
            ? TileFormat::JPG
            : htl::tiles::is_png( m_data.data(), 7 )
              ? TileFormat::PNG
              : htl::tiles::is_webp( m_data.data(), 14 )
                ? TileFormat::WEBP
                : TileFormat::UNKNOWN;
 END_LOCK_SECTION_;
}

}// namespace                     tiles
}// namespace                     htl
