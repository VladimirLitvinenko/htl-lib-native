#include <iostream>
#include <AppSkeleton.h>
#include <Utils.h>
#include <MBTileDatabase.h>
#include <ostream>
#include <istream>

#include <mapbox/vector_tile.hpp>
//#include "mbtiles/vector_tile.pb.h"

using namespace sqlite;
using namespace std;
using namespace htl::app;
using namespace htl::tiles;
using namespace htl::db;

void test_geojsom_select( htl::tiles::MBTileDatabase * db_ptr  )
{
  try
  {
    auto gj = db_ptr->ReadGeoJSON();
    if( gj )
    {
      std::cout << "\tGeoGSON: " << gj.value().c_str() << std::endl;
    }
  }
  catch( const std::exception & e )
  {
    std::cout << db_ptr->StateMessage().c_str() << std::endl;
    DUMP_EXCEPTION( e );
  }
//  std::cout << "Test \"" << db_ptr->GetDbName().c_str () << "\" complete." << std::endl;
  std::flush(std::cout);
};

void test_mbtile_select( htl::tiles::MBTileDatabase * db_ptr, short z, short x, short y  )
{
  try
  {
//    std::ifstream is( "/tmp/7-74-90.txt", std::ios::binary );
//    if( is.is_open() )
//    {
//      std::string str;
//      is >> str;
//      const std::string b_str{ str };
////      str = mbgl::util::decompress( str );
//      is.close();
//      mapbox::vector_tile::buffer b( b_str );
//      auto layers = b.getLayers();
//      std::cout << "Layer Names: " << std::endl;
//      auto layer_names = b.layerNames();
//      for( auto & l : layer_names )
//      {
//        std::cout << l << std::endl;
//      }
//      std::flush(std::cout);
//    }
//    return;

    MBTileInfo ti;
    ti.SetCoordinates(z,x,y);

    db_ptr->ReadTile( ti );
    if( not ti.IsEmpty() )
    {
      std::cout << "Z: " << ti.GetZ() << ", X: " << ti.GetX() << ", Y: " << ti.GetY() << std::endl;
      std::cout << "\tРазмер тайла: " << ti.GetData().size() << std::endl;
      std::cout << "\tФормат тайла: " << ti.GetFormatStr().c_str() << std::endl;
      std::cout << db_ptr->StateMessage().c_str() << std::endl;

//      vector_tile::Tile tile;
//      if( tile.ParseFromString( ti.DataToString() ) )
//      {

//      }
      std::flush(std::cout);
    }
  }
  catch( const std::exception & e )
  {
    std::cout << db_ptr->StateMessage().c_str() << std::endl;
    DUMP_EXCEPTION( e );
  }
//  std::cout << "Test \"" << db_ptr->GetDbName().c_str () << "\" complete." << std::endl;
  std::flush(std::cout);
};


int main( int argc, char** argv )
{
  AppSkeleton app( argc, argv );
    app.SetExistsCallback (
    [ name = app.Name() ]()
    {
      std::cout << "Application " << name.c_str () << " exists." << std::endl;
      std::flush(std::cout);
    });

  app.SetAction( []()
  {
    auto db_shared = std::static_pointer_cast<htl::tiles::MBTileDatabase>( MakeDb<htl::tiles::MBTileDatabase>( "/home/nemo/navdata/navdata.mbtiles" ) );
    test_geojsom_select( db_shared.get() );

    for( auto z(0); z < 7; z++ )
    {
      for( auto x(0); x < 80; x++ )
      {
        for( auto y(0); y < 10; y++ )
        {
          test_mbtile_select( db_shared.get(), z, x, y );
        }
      }
    }
  } );
  app.Start();
  return app.Exec();
}
