#include "MBTileDatabase.h"
#include "DbFactory.h"
#include "MBTileInfo.h"
#include "Logger.h"
#include "ErrLiterals.h"
#include "Utils.h"

#include <experimental/tuple>

namespace                         htl   {
namespace                         tiles {

MBTileDatabase::MBTileDatabase(const std::string& db_name)
  : MBTileDatabase::base_class_t()
{
  MBTileDatabase::SetName( db_name );
}

void
MBTileDatabase::SetName(const std::string& db_name)
noexcept
{
  MBTileDatabase::base_class_t::SetName( db_name );
  Validation();
}

void
MBTileDatabase::Validation()
noexcept
{
  if( IsValid()
      and
      htl::db::DBTableValidation(* this, MBTileView, htl::utils::StrReplace(MBTile_Validator_Message, "%1", MBTileView ) )
      and
      htl::db::DBTableValidation(* this, MBMetaData, htl::utils::StrReplace(MBTile_Validator_Message, "%1", MBMetaData ) )
  )
  {
    MBTileInfo ti;
    * DbRef()
        << MBTile_Validator_Data
        >> [&]( std::vector<char> d )
    {
      ti.SetData(d);
    };
    if( ti.IsEmpty() )
      SetState( htl::db::DBState::MissingTiles, MBTile_Validator_Data_Message );
  }
}

void
MBTileDatabase::ReadTile( MBTileInfo& tile_info )
{
  SetState( htl::db::DBState::Ok );
  tile_info.ClearData();
  if( IsValid() )
  {
    try
    {
      auto select_expr = htl::db::PrepareDBSelect(
          MBTileView,
          htl::strings_t({ "tile_data" }),
          std::string( "(zoom_level = ?) AND (tile_column = ?) AND (tile_row = ?)" ) );
      if( select_expr )
      {
        * DbRef()
            << select_expr.value()
//            << "select get_tile( ?,?,?) from tiles"
            << tile_info.GetZ()
            << tile_info.GetX()
            << tile_info.GetY()
            >> [&]( const std::vector<char> & d )
               {
                 tile_info.SetData( d );
               };
        if( m_on_complete )
        {
          m_on_complete(tile_info);
        }
      }
    }
    catch( const std::runtime_error & e )
    {
      SetState( htl::db::DBState::SelectError );
      DUMP_EXCEPTION( e );
    }
    catch( const std::exception & e )
    {
      SetState( htl::db::DBState::NoDatabase );
      DUMP_EXCEPTION( e );
    }
  }
}

std::experimental::optional<std::string>
MBTileDatabase::ReadGeoJSON()
{
  if( IsValid() )
  {
    try
    {
      std::string geo_json;
      * DbRef()
          << MBTile_GeoJSON
          >> [&geo_json]( const std::vector<char> & d )
             {
               geo_json.resize( d.size() );
               std::copy( d.begin(), d.end(), geo_json.begin() );
             };
      return geo_json;
    }
    catch( const std::runtime_error & e )
    {
      SetState( htl::db::DBState::SelectError );
      DUMP_EXCEPTION( e );
    }
    catch( const std::exception & e )
    {
      SetState( htl::db::DBState::NoDatabase );
      DUMP_EXCEPTION( e );
    }
  }
  return nullptr;
}

std::pair<int, int>
MBTileDatabase::ReadZoomValues()
{
  if( IsValid() )
  {
    try
    {
      int min_z{-1}, max_z{-1};
      * DbRef()
          << MBTile_MetaMinMaxZoom
          >> [&min_z, &max_z]( const int mi, const int ma )
             {
               min_z = mi, max_z = ma;
             };
      return std::make_pair( min_z, max_z );
    }
    catch( const std::runtime_error & e )
    {
      SetState( htl::db::DBState::SelectError );
      DUMP_EXCEPTION( e );
    }
    catch( const std::exception & e )
    {
      SetState( htl::db::DBState::NoDatabase );
      DUMP_EXCEPTION( e );
    }
  }
  return std::make_pair(-1, -1);
}

std::pair<int, int> MBTileDatabase::ReadMinMaxColumns()
{
  int min_c{-1}, max_c{-1};
  if( IsValid() )
  {
    try
    {
      * DbRef()
          << MBTile_MinColumn
          >> min_c;
      * DbRef()
          << MBTile_MaxColumn
          >> max_c;
      return std::make_pair(min_c, max_c);
    }
    catch( const std::runtime_error & e )
    {
      SetState( htl::db::DBState::SelectError );
      DUMP_EXCEPTION( e );
    }
    catch( const std::exception & e )
    {
      SetState( htl::db::DBState::NoDatabase );
      DUMP_EXCEPTION( e );
    }
  }
  return std::make_pair(-1, -1);
}

std::pair<int, int> MBTileDatabase::ReadMinMaxRows()
{
  int min_r{-1}, max_r{-1};
  if( IsValid() )
  {
    try
    {
      * DbRef()
          << MBTile_MinRow
          >> min_r;
      * DbRef()
          << MBTile_MaxRow
          >> max_r;
      return std::make_pair(min_r, max_r);
    }
    catch( const std::runtime_error & e )
    {
      SetState( htl::db::DBState::SelectError );
      DUMP_EXCEPTION( e );
    }
    catch( const std::exception & e )
    {
      SetState( htl::db::DBState::NoDatabase );
      DUMP_EXCEPTION( e );
    }
  }
  return std::make_pair(-1, -1);
}

}// namespace                     watch
}// namespace                     htl
