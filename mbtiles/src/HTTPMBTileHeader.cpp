#include "HTTPMBTileHeader.h"
#include "MBTilesDbFactory.h"
#include "MBTileLiterals.h"
#include "Utils.h"
#include "Types.h"
#include <type_traits>

namespace                         htl   {
namespace                         tiles {

HTTPMBTileHeader::HTTPMBTileHeader()
  : HTTPMBTileHeader::base_class_t()
{

}

HTTPMBTileHeader::HTTPMBTileHeader(const std::string& req)
  : HTTPMBTileHeader::base_class_t( req )
{
  HTTPMBTileHeader::PrepareTile();
}

void HTTPMBTileHeader::SetRequest(const std::string& req)
{
  HTTPMBTileHeader::base_class_t::SetRequest( req );
  PrepareTile();
}

MBTileInfo
HTTPMBTileHeader::GetTileInfo()
const
{
  return m_tile_info;
}

bool
HTTPMBTileHeader::IsGZip()
{
  return Get( HTTP_KEY_A_ENCODING) == HTTP_VAL_A_ENCODING_GZ;
}

bool
HTTPMBTileHeader::IsDefalte()
{
  return Get( HTTP_KEY_A_ENCODING) == HTTP_VAL_A_ENCODING_DF;
}

void
HTTPMBTileHeader::PrepareTile()
noexcept
{
//  BEGIN_LOCK_SECTION_SELF_;

  auto dest( GetDestinition() );
  Set( dest.empty() ? HttpStatus::NoContent : HttpStatus::OK );
  if( IsCodeValid() and ( not dest.empty() ) )
  {
    auto strings = htl::utils::NormalisationStrings( htl::utils::StrSplit( dest, "/") );
    auto s_size( strings.size() );
    Set( s_size == MBTile_Max_Idx ? HttpStatus::OK : HttpStatus::NoContent );
    if( IsCodeValid() )
    {
      m_tile_info.SetCoordinates(
            static_cast<uint32_t>(std::atoi( strings.at( MBTile_Z_Idx ).c_str() ) ),
            static_cast<uint64_t>(std::atoi( strings.at( MBTile_X_Idx ).c_str() ) ),
            static_cast<uint64_t>(std::atoi( htl::utils::StrSplit( strings.back(), ".").front().c_str() ) )
      );

      Set(
            htl::tiles::MBTilesDbFactory::Instance().
            PrepareTile( strings.at( MBTile_DbName_Idx ), m_tile_info )
            ? HttpStatus::OK
            : HttpStatus::NoContent);
    }
  }
  //  END_LOCK_SECTION_;
}

MBTileInfo &
HTTPMBTileHeader::TileInfoRef()
{
  return std::ref( m_tile_info );
}

}// namespace                     tiles
}// namespace                     htl
