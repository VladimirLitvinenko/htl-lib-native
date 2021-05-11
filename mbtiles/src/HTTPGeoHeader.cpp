#include "mbtiles/HTTPGeoHeader.h"
#include "mbtiles/MBTilesDbFactory.h"
#include "mbtiles/MBTileLiterals.h"
#include "common/Utils.h"
#include "common/Types.h"

namespace                         htl   {
namespace                         tiles {

HTTPGeoHeader::HTTPGeoHeader()
  : HTTPGeoHeader::base_class_t()
{

}

HTTPGeoHeader::HTTPGeoHeader(const std::string& req)
  : HTTPGeoHeader::base_class_t( req )
{
  HTTPGeoHeader::PrepareGeoJSON();
}

void
HTTPGeoHeader::SetRequest(const std::string& req)
{
  HTTPGeoHeader::base_class_t::SetRequest( req );
  PrepareGeoJSON();
}

std::string
HTTPGeoHeader::GetGeoJSON()
const
{
  return m_geo_json;
}

void
HTTPGeoHeader::PrepareGeoJSON()
noexcept
{
  auto dest( GetDestinition() );
  Set( dest.empty() ? HttpStatus::NoContent : HttpStatus::OK );
  if( IsCodeValid() and ( not dest.empty() ) )
  {
    auto strings = htl::utils::NormalisationStrings( htl::utils::StrSplit( dest, "/") );
    auto s_size( strings.size() );
    Set( s_size == MBTile_Max_Idx ? HttpStatus::OK : HttpStatus::NoContent );
    if( IsCodeValid() )
    {
      Set(
            htl::tiles::MBTilesDbFactory::Instance().
            PrepareGeoJSON( strings.at( MBTile_DbName_Idx ), m_geo_json )
            ? HttpStatus::OK
            : HttpStatus::NoContent);
    }
  }
}

}// namespace                     tiles
}// namespace                     htl
