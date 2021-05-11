#include "db/SqliteDatabase.h"
#include "base/application/AppSkeleton.h"
#include "base/application/AppProperty.h"
#include <iostream>
#include "mbgl/util/compression.hpp"
#include "mbtiles/vector_tile_compression.h"
#include "mbtiles/vector_tile_config.h"
#include "mbtiles/MBTileLiterals.h"

void usage( const std::string& app_name )
{
  std::cout << std::endl << "Usage: " << app_name << " z x y <database path> [<output path>]" << std::endl << std::endl;
}

int main( int argc, char** argv )
{
  htl::app::AppSkeleton app( argc, argv );
  app.SetBlockMode( true );
  app.SetAction(
        [ &app ]()
  {
    auto w_ptr = app.GetProperty();
    auto arguments = w_ptr.lock()->Content();
    if( arguments.size() < 5 )
    {
      usage( app.Name() );
      app.Abort( EXIT_FAILURE );
    }
    int z{ std::atoi( arguments.at(1).c_str()) };
    int x{ std::atoi( arguments.at(2).c_str()) };
    int y{ std::atoi( arguments.at(3).c_str()) };
    auto db_name = arguments.at(4);
    auto output_path = arguments.size() > 5 ? arguments.at(5) : app.Path();

    auto db_ptr = htl::db::MakeDb<htl::db::SqliteDatabase>( db_name );
    (* db_ptr->DbRef() )
        << "select tile_data from tiles where zoom_level = ? and tile_column = ? and tile_row = ?"
        << z << x << y
        >> [ &app, &output_path, z, x, y ](const std::vector<char> & data )
        {
          if( not data.empty() )
          {
            std::string str;
            std::string str_out;
            str.assign( data.begin(),data.end() );
            if( htl::tiles::is_gzip( str.data(), 3 ) or htl::tiles::is_zlib( str.data(), 3 ) )
            {
              htl::tiles::zlib_decompress( str.data(), str.size(), str_out );
            }
            else
            {
              str_out.swap( str );
            }
            std::string out_file = output_path + "/" + std::to_string(z) + "-" + std::to_string(x) + "-" + std::to_string(y) + MBTile_Proto_Suffix;
            std::ofstream ofs( out_file, std::ios_base::binary );
            if( ofs.is_open() )
            {
//              ofs.write( data.data(), data.size() );
              ofs.write( str_out.data(), str_out.size() );
              ofs.close();
              std::cout << std::endl << "File \"" << out_file << "\" complete." << std::endl;
              app.Quit(EXIT_SUCCESS);
            }
          }
        };
    app.Abort( EXIT_FAILURE );
  }
        );
  app.Start();
  return app.Exec();
}
