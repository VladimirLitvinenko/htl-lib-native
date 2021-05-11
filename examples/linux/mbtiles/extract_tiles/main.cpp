#include "base/application/AppSkeleton.h"
#include "base/application/AppProperty.h"
#include <iostream>
#include "mbtiles/MBTilesDbFactory.h"
#include "common/Utils.h"
#include <mapbox/vector_tile.hpp>

std::string open_tile(std::string const& path) {
  std::ifstream stream(path.c_str(),std::ios_base::in|std::ios_base::binary);
  if (!stream.is_open())
  {
    throw std::runtime_error("could not open: '" + path + "'");
  }
  std::string message(std::istreambuf_iterator<char>(stream.rdbuf()),(std::istreambuf_iterator<char>()));
  stream.close();
  return message;
}

class print_value {

public:
  std::string operator()(const mapbox::feature::value & val )
  {
    using namespace mapbox::feature;
    if( val.is<null_value_t>() )
    {
      return "nullptr";
    }
    if( val.is<bool>() )
    {
      return val.get<bool>() ? "true" : "false";
    }
    if( val.is<uint64_t>() )
    {
      return std::to_string(val.get<uint64_t>());
    }
    if( val.is<int64_t>() )
    {
      return std::to_string(val.get<int64_t>());
    }
    if( val.is<double>() )
    {
      return std::to_string(val.get<double>());
    }
    if( val.is<std::string>() )
    {
      return val.get<std::string>();
    }
    if( val.is<std::vector<value>>() )
    {
      return "vector";
    }
    if( val.is<std::unordered_map<std::string, value>>() )
    {
      return "unordered_map";
    }
    return "undefined";
  }
};

void usage( const std::string& app_name )
{
  std::cout << std::endl << "Usage: " << app_name << " <tiles path>" << std::endl << std::endl;
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
    if( arguments.size() < 2 )
    {
      usage( app.Name() );
      app.Abort( EXIT_FAILURE );
    }

//    std::string out_path = htl::utils::StrChop( arguments.at(1), 1 );
    std::string out_path = arguments.at(1);
//    for( std::string db_path : { /*"navdata", */ "russia" /*, "obstacles"*/} )
    for( std::string db_path : { "navdata" } )
    {
      auto db_name = db_path + ".mbtiles";
      DUMP_INFO( std::string("Parce ") + db_name + "...")
      htl::tiles::MBTileDatabase db( out_path + DIR_SEPARATOR + db_name );
      if( not db.IsValid() )
      {
        DUMP_CRITICAL( std::string("Database not valid."));
        app.Abort( EXIT_FAILURE );
      }
      auto z_data = db.ReadZoomValues();
      if( (z_data.first == -1) or (z_data.second == -1) )
      {
        DUMP_CRITICAL( std::string("Error Zoom values."));
        app.Abort( EXIT_FAILURE );
      }
      auto x_data = db.ReadMinMaxRows();
      if( (x_data.first == -1) or (x_data.second == -1) )
      {
        DUMP_CRITICAL( std::string("Error Rows values."));
        app.Abort( EXIT_FAILURE );
      }
      auto y_data = db.ReadMinMaxColumns();
      if( (y_data.first == -1) or (y_data.second == -1) )
      {
        DUMP_CRITICAL( std::string("Error Columns values."));
        app.Abort( EXIT_FAILURE );
      }

      db_path = out_path + DIR_SEPARATOR + db_path;
      htl::tiles::MBTileInfo ti;
      for( auto z(z_data.first); z <= z_data.second; z++)
      {
        for( auto y(y_data.first); y <= y_data.second; y++)
        {
          for( auto x(x_data.first); x <= x_data.second; x++)
          {
            ti.SetCoordinates( z, x, y, false );
            db.ReadTile( ti );
            ti.SetCoordinates( z, x, y );
            auto file_info = ti.ToPath( db_path, true );
            if( file_info.first )
            {
              try {
                std::string tile_path(file_info.second.c_str());
                std::string buffer = open_tile(tile_path);
                mapbox::vector_tile::buffer tile(buffer);
                DUMP_INFO( std::string("Decoding tile: ")+ tile_path);
                for (auto const& name : tile.layerNames())
                {
                  DUMP_INFO( std::string("Layer ") + name );
                  const mapbox::vector_tile::layer layer = tile.getLayer(name);
                  std::size_t feature_count = layer.featureCount();
                  if (feature_count == 0) {
                    DUMP_INFO( std::string("Layer ") + name + " (empty)");
                    continue;
                  }
                  std::cout << "  Features:\n";
                  for (std::size_t i=0;i<feature_count;++i) {
                    auto const feature = mapbox::vector_tile::feature(layer.getFeature(i),layer);
                    auto const& feature_id = feature.getID();
                    if (feature_id.is<uint64_t>()) {
                      DUMP_INFO( std::string("    id: ") + std::to_string(feature_id.get<uint64_t>()));
                    } else {
                      DUMP_INFO( "    id: (no id set)");
                    }
                    DUMP_INFO( std::string("    type: ") + std::to_string( int(feature.getType()) ));
                    auto props = feature.getProperties();
                    DUMP_INFO( "    Properties:");
                    for (auto const& prop : props) {
                      print_value printvisitor;
                      std::string value = mapbox::util::apply_visitor(printvisitor, prop.second);
                      DUMP_INFO( std::string("      ") + prop.first  + ": " + value);
                    }
//                    DUMP_INFO( "    Vertices:");
//                    mapbox::vector_tile::points_arrays_type geom = feature.getGeometries<mapbox::vector_tile::points_arrays_type>(1.0);
//                    for (auto const& point_array : geom) {
//                      for (auto const& point : point_array) {
//                        DUMP_INFO( std::to_string(point.x) + "," + std::to_string( point.y ));
//                      }
//                    }
                  }
                }
              } catch (std::exception const& ex) {
                DUMP_INFO( std::string("ERROR: ") + ex.what());
              }
              DUMP_INFO( file_info.second + " complete.")
            }
          }
        }
      }
    }
  }
        );
  app.Start();
  return app.Exec();
}
