#include <Utils.h>
#include <iostream>
#include <sqlite_modern_cpp.h>
#include <boost/network/protocol/http/server.hpp>
#include <boost/network/protocol/http/message/header.hpp>

using db_map = std::map< std::string, sqlite::database >;

//static sqlite::sqlite_config config;
//config.flags = sqlite::OpenFlags::READONLY;
static db_map databases
{
  std::make_pair("russia"     , sqlite::database("/opt/mbtiles/russia.mbtiles"    /*, config*/) ),
  std::make_pair("navdata"    , sqlite::database("/opt/mbtiles/navdata.mbtiles"   /*, config*/) ),
  std::make_pair("obstacles"  , sqlite::database("/opt/mbtiles/obstacles.mbtiles" /*, config*/) ),
};

namespace http = boost::network::http;

//static std::vector< std::map<std::wstring, std::wstring> > sendStr ({
////    "HTTP/1.1 200 OK\r\n"
//    {{"Access-Control-Allow-Origin", "*"},
//     {"Content-Type", "application/x-protobuf"},
////    "Content-Type: application/octet-stream"},
//    {"Content-Encoding","gzip"},
//    {"Transfer-Encoding","chunked"},
//    {"Cache-Control","public"},},
//    });
static std::string sendStr =
    "Access-Control-Allow-Origin: *\r\n"
    "Content-Type: application/x-protobuf\r\n"
//    "Content-Type: application/octet-stream\r\n"
    "Content-Encoding: gzip\r\n"
    "Transfer-Encoding: chunked\r\n"
    "Cache-Control: public\r\n"
    "\r\n"
    ;


struct tile_server;
typedef http::server<tile_server> server;

struct tile_server {
    void operator()(server::request const &request, server::connection_ptr connection) {
        server::string_type ip = source(request);
//        unsigned int port = request.source_port;
//        std::cout << "-------------" << std::endl;
//        std::cout << "Source: " << request.source.c_str() << std::endl;
//        std::cout << "Method: " << request.method.c_str() << std::endl;
//        for( auto & h : request.headers )
//        {
//          std::cout << h.name.c_str() << ": " << h.value.c_str() << std::endl;
//        }

//        std::cout << "Dest: " << request.destination.c_str() << std::endl;
        auto dest( htl::utils::StrSplit(request.destination,".") );
        if( dest.size() > 1 )
        {
          auto strings( htl::utils::StrSplit( dest.front(), "/") );

          auto y (std::atoi( strings.at( strings.size() - 1 ).c_str() ));
          auto x (std::atoi( strings.at( strings.size() - 2 ).c_str() ));
          auto z (std::atoi( strings.at( strings.size() - 3).c_str() ));

          std::string db_key ( strings.at( strings.size() - 5) );
          databases.at( db_key )
              << std::string("select tile_data from tiles where zoom_level = ? and tile_column = ? and tile_row = ?")
              << z << x << y
              >> [&]( std::vector<char> pbf )
                 {
                   if(pbf.empty() )
                     return;

                   std::cout << "z = " << z <<" x = " << x << " y = " << y << std::endl;
                   std::cout << "pbf size = " << pbf.size() << std::endl;
                   std::flush( std::cout );
                   boost::filesystem::path pbf_file(std::string("/tmp/") + std::to_string(z) + "_" + std::to_string(x) + "_" + std::to_string(y) +".pbf");
                   std::fstream out(pbf_file.string(), out.binary | out.trunc | out.out);
                   out.write( pbf.data(), pbf.size());
                   out.close();

//                   http::response_header<std::string> resp_header;
////                   resp_header.operator=( << send
//                   connection->set_headers( resp_header );
                   connection->set_status(server::connection::ok);
                   connection->write( pbf );
                   std::this_thread::sleep_for( std::chrono::microseconds(1000));
                 };
        }
    }
};

int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " address port" << std::endl;
        return 1;
    }

    try {
        tile_server handler;
        server::options options(handler);
        server server_(options.address(argv[1]).port(argv[2]));
        server_.run();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
