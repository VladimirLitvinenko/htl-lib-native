#include "mapbox/vector_tile.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

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

int main(int argc, char** argv)
{
    try {
        if (argc < 2) {
            std::clog << "please provide path to uncompressed Mapbox Vector tile (.mvt)\n";
            return -1;
        }
        std::string tile_path(argv[1]);
        std::string buffer = open_tile(tile_path);
        mapbox::vector_tile::buffer tile(buffer);
        std::cout << "Decoding tile: " << tile_path << "\n";
        for (auto const& name : tile.layerNames())
        {
            const mapbox::vector_tile::layer layer = tile.getLayer(name);
            std::size_t feature_count = layer.featureCount();
            if (feature_count == 0) {
                std::cout << "Layer '" << name << "' (empty)\n";
                continue;
            }
            std::cout << "Layer '" << name << "'\n";
            std::cout << "  Features:\n";
            for (std::size_t i=0;i<feature_count;++i) {
                auto const feature = mapbox::vector_tile::feature(layer.getFeature(i),layer);
                auto const& feature_id = feature.getID();
                if (feature_id.is<uint64_t>()) {
                    std::cout << "    id: " << feature_id.get<uint64_t>() << "\n";
                } else {
                    std::cout << "    id: (no id set)\n";
                }
                std::cout << "    type: " << int(feature.getType()) << "\n";
                auto props = feature.getProperties();
                std::cout << "    Properties:\n";
                for (auto const& prop : props) {
                    print_value printvisitor;
                    std::string value = mapbox::util::apply_visitor(printvisitor, prop.second);
                    std::cout << "      " << prop.first  << ": " << value << "\n";
                }
                std::cout << "    Vertices:\n";
                mapbox::vector_tile::points_arrays_type geom = feature.getGeometries<mapbox::vector_tile::points_arrays_type>(1.0);
                for (auto const& point_array : geom) {
                    for (auto const& point : point_array) {
                        std::clog << point.x << "," << point.y;
                    }
                }
                std::clog << "\n";
            }
        }
    } catch (std::exception const& ex) {
        std::clog << "ERROR: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
