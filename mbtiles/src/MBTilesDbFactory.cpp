#include "MBTilesDbFactory.h"
#include "SqliteDatabase.h"
#include "Logger.h"

#ifdef IS_SAILFISH
#   include <json/json.h>
#else
#   include <jsoncpp/json/json.h>
#endif

namespace                         htl   {
namespace                         tiles {

MBTilesDbFactory::MBTilesDbFactory()
  : MBTilesDbFactory::base_class_t( htl::MAIN_CONF_DB )
{
  Prepare<htl::tiles::MBTileDatabase>();
}

MBTilesDbFactory::MBTilesDbFactory(const std::string& config_path)
  : MBTilesDbFactory::base_class_t( config_path )
{
  Prepare<htl::tiles::MBTileDatabase>();
}

//MBTilesDbFactory&
//MBTilesDbFactory::Instance(const std::string& config_path)
//{
//  static MBTilesDbFactory factory( config_path );
//  if( config_path != factory.GetConfig() )
//  {
//    factory.SetPath( config_path );
//    factory.Prepare<htl::tiles::MBTileDatabase>();
//  }
//  return std::ref( factory );
//}

bool
MBTilesDbFactory::PrepareTile( const std::string& db_name, MBTileInfo& ti )
{
  bool retval( IsValid() );
  if( retval )
  {
    auto & coll_ref = CollectionRef();
    auto iter( coll_ref.find( db_name ) );
    retval = ( iter != coll_ref.end() ) and ( iter->second );
    if( retval )
    {
      retval = iter->second->IsValid();
      if( retval )
      {
        std::dynamic_pointer_cast<htl::tiles::MBTileDatabase>(iter->second)->ReadTile( ti );
        retval = iter->second->IsOk();
      }
    }
  }
  return retval;
}

bool MBTilesDbFactory::PrepareGeoJSON(const std::string& db_name, std::string& geo)
{
  geo.clear();
  bool retval( IsValid() );
  if( retval )
  {
    auto & coll_ref = CollectionRef();
    auto iter( coll_ref.find( db_name ) );
    retval = ( iter != coll_ref.end() ) and ( iter->second );
    if( retval )
    {
      retval = iter->second->IsValid();
      if( retval )
      {
        auto geo_opt = std::dynamic_pointer_cast<htl::tiles::MBTileDatabase>(iter->second)->ReadGeoJSON();
        retval = iter->second->IsOk() and ( geo_opt );
        if( retval )
          geo = geo_opt.value();
      }
    }
  }
  return retval;
}

}// namespace                     tiles
}// namespace                     htl
