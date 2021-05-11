#include "mbtiles/MBTileSrvFactory.h"
#include "mbtiles/MBTilesDbFactory.h"
#include "mbtiles/MBTileDatabase.h"

#include "Logger.h"
#ifdef IS_SAILFISH
#   include <json/json.h>
#else
#   include <jsoncpp/json/json.h>
#endif
#include <iostream>

namespace                         htl   {
namespace                         tiles {


MBTileSrvFactory::MBTileSrvFactory()
  : MBTileSrvFactory::base_class_t( htl::MAIN_CONF_SRV )
{
}

MBTileSrvFactory::MBTileSrvFactory(const std::string& config_path )
  : MBTileSrvFactory::base_class_t( config_path )
{
}

bool
MBTileSrvFactory::IsValid()
const
{
  bool retval( not m_servers.empty()  );
  for( auto & srv : m_servers )
  {
    retval = retval and srv.second->IsValid();
  }
  return retval;
}

bool
MBTileSrvFactory::IsActive()
const
{
  bool retval( not m_servers.empty() );
  for( auto srv : m_servers )
  {
    retval = retval and srv.second->IsOpen();
  }
  return retval;
}

strings_t
MBTileSrvFactory::ServersID()
const
{
  strings_t retval;
  retval.reserve( m_servers.size() );
  for( auto iter( m_servers.begin() ); iter != m_servers.end(); ++iter)
    retval.push_back( iter->first );
  return retval;
}

void
MBTileSrvFactory::DumpServersID()
const
{
  for( auto & id : ServersID() )
    DUMP_INFO_CONSOLE(id.c_str());
}

void
MBTileSrvFactory::Start()
{
  DUMP_INFO(__PRETTY_FUNCTION__);
  Prepare();
  for( auto & srv_ref : m_servers )
  {
    if( not srv_ref.second->IsOpen() )
    {
      srv_ref.second->SetStatus(
            srv_ref.second->Start(true)
            ? HttpStatus::OK
            : HttpStatus::InternalServerError );
    }
  }
}

void
MBTileSrvFactory::Stop()
{
  DUMP_INFO( __PRETTY_FUNCTION__ );
  for( auto & srv_ref : m_servers )
  {
    while( not srv_ref.second->Stop() )
      std::this_thread::yield();
  }
}

MBTileSrvFactory &
MBTileSrvFactory::Instance( const std::string & config_path )
noexcept
{
  static MBTileSrvFactory srv( config_path );
  return std::ref( srv );
}

void
MBTileSrvFactory::OnTile
(
    const std::string                       & db_name,
    MBTileInfo                              & tile
)
{
  auto sh_ptr( std::dynamic_pointer_cast<MBTileDatabase>( MBTilesDbFactory::Instance().GetDb( db_name ) ) );
  if( sh_ptr )
  {
    sh_ptr->OnComplete(
    [ &tile ]( MBTileInfo & t)
    {
      tile = t;
    } );
  }
}

void
MBTileSrvFactory::Prepare()
noexcept
{
  DUMP_INFO(__PRETTY_FUNCTION__);
  BEGIN_LOCK_SECTION_SELF_;
  m_servers.clear();
  if( IsValidConfig() )
  {
    try
    {
      Json::Value root;
      std::ifstream ( GetConfigRef().string(), std::ifstream::binary) >> root;

      Json::Value val = root.get( root.getMemberNames().at(0), Json::nullValue );
      for( Json::ArrayIndex i(0); val.isArray() and ( i < val.size() ); i++ )
      {
        auto item = val.get(i,Json::arrayValue);
        if(item.isNull() )
          continue;

        std::string server_name;
        for( auto & str : item.getMemberNames() )
        {
          Json::Value v = item.get( str, Json::nullValue );
          if( v.isString() )
            server_name = v.asString();

          if( v.isArray() and ( not server_name.empty() ) )
          {
            std::map<std::string, std::string > params;
            for( Json::ArrayIndex j(0); v.isArray() and ( j < v.size() ); j++ )
            {
              auto sp = v.get(j,Json::objectValue);
              for( auto & srv_param : sp.getMemberNames() )
              {
                params[srv_param] = sp.get(srv_param, Json::nullValue ).asString();
              }
            }
            if( params.size() < 1 )
              throw Json::RuntimeError( MBTile_JsonFile_Corrupt );

            m_servers[server_name].reset(
                  new MBTileServer(
                    static_cast<uint16_t>(std::atoi( params.at(MBTile_Srv_Port_Key).c_str() ) )) );
            server_name.clear();
          }
        }
      }
      return;
    }
    catch( const Json::RuntimeError & je )
    {
      DUMP_EXCEPTION(je);
    }
    catch( const std::exception & e )
    {
      DUMP_EXCEPTION(e);
    }
    m_servers.clear();
  }
  END_LOCK_SECTION_;
}

}// namespace                     tiles
}// namespace                     htl
