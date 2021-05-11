//#include <boost/asio.hpp>
//#include <boost/asio/spawn.hpp>
#include <iostream>

#include "AsioSocketSession.h"
#include "HTTPMBTileHeader.h"
#include <MBTileInfo.h>

#include <MBTilesDbFactory.h>
#include <Utils.h>
#include <sqlite_modern_cpp.h>

using boost::asio::ip::tcp;
using boost::system::error_code;
using boost::asio::streambuf;

//static sqlite::sqlite_config config;
//config.flags = sqlite::OpenFlags::READONLY;

static std::string Str500 =
    "HTTP/1.1 204 No Content\r\n"
    "Vary: Origin\r\n"
    "Access-Control-Allow-Origin: *\r\n"
    "\r\n"
    ;
static std::string Str200 =
    "HTTP/1.1 200 OK\r\n"
    "Vary: Origin\r\n"
    "Access-Control-Allow-Origin: *\r\n"
    "Content-Type: application/x-protobuf\r\n"
    "Content-Encoding: gzip\r\n"
    "Cache-Control: public\r\n"
    "\r\n"
    ;

namespace ba = boost::asio;
using ba::ip::tcp;
using ba::yield_context;

int main() {
    ba::io_service svc;

    tcp::acceptor a(svc);
    a.open(tcp::v4());
    a.set_option(tcp::acceptor::reuse_address(true));
    a.bind({{}, 8888}); // bind to port 6767 on localhost
    a.listen(/*5*/);


    spawn(svc, [&a](yield_context yield) {
        while(1)
        {
          auto cnt_ptr = a.get_executor().target<boost::asio::io_context>();
          assert( nullptr != cnt_ptr );
          using session = std::shared_ptr<tcp::socket>;
            session s = std::make_shared<session::element_type>( *cnt_ptr );

            a.async_accept( * s, yield);
//            std::async( std::launch::async,
//                [&]()
//            {
            spawn(yield, [s=std::move(s)](yield_context yield) mutable
            {
                // do a read
                auto buf = std::vector<char>(s->available() );
                boost::system::error_code ec;
                size_t bytes = s->async_read_some( ba::buffer(buf), yield[ ec ] );

                if( 0 < bytes )
                {
                  htl::tiles::HTTPMBTileHeader header( std::string(buf.data(), buf.size()) );
                  boost::asio::socket_base::keep_alive opt_keep_alive( header.IsKeepAlive() );
                  s->set_option(opt_keep_alive);

                  htl::tiles::MBTileInfo ti( header.GetTileInfo() );

                  std::string result_header = ti.IsEmpty() ? Str500 : Str200;
                  std::cout << result_header.c_str() << std::endl;
                  std::flush( std::cout );

                  std::vector<char> asio_buffer;
                  auto data = ti.GetData();
                  if( not ti.IsEmpty() )
                    asio_buffer.assign( data.begin(), data.end() );
                  else
                    asio_buffer.assign( BLANK_PNG.begin(), BLANK_PNG.end() );

                  asio_buffer.insert( asio_buffer.begin(), result_header.begin(), result_header.end() );

                  std::cout << "ANSWER = " << asio_buffer.data() << std::endl;
                  bytes = async_write(*s, ba::buffer(asio_buffer), yield);
                  std::cout << "bytes = " << bytes << std::endl;
                  std::flush( std::cout );
                }
            });
//        }).wait_for(std::chrono::milliseconds(0));
        }
    });

    svc.run(); // wait for shutdown (Ctrl-C or failure)
}

//int main()
//{
//  auto & svc = htl::asio::AsioService::Instance().ServiceRef();
//    std::shared_ptr<tcp::acceptor> a = std::make_shared<tcp::acceptor>(svc);
//    a->open(tcp::v4());
//    a->set_option(tcp::acceptor::reuse_address(true));
//    a->set_option(tcp::acceptor::keep_alive(true));
//    a->bind({{}, 8888}); // bind to port 6767 on localhost
//    a->listen(5);

////    std::shared_ptr<tcp::acceptor> an = std::make_shared<tcp::acceptor>(svc);
////    an->open(tcp::v4());
////    an->set_option(tcp::acceptor::reuse_address(true));
////    an->set_option(tcp::acceptor::keep_alive(true));
////    an->bind({{}, 9999}); // bind to port 6767 on localhost
////    an->listen(5);

////    using session = std::shared_ptr<tcp::socket>;
//    using session = tcp::socket;

//    std::function<void()>        do_accept;
////    std::function<void()>        do_accept_n;
//    std::function<void(session)> do_session;
////    std::function<void( htl::asio::SocketSessionShared<boost::asio::ip::tcp> & )> do_session;
//    db_map databases
//        {
//            std::make_pair("all"     , sqlite::database("/opt/mbtiles/all.mbtiles"    /*, config*/) ),
//            std::make_pair("russia"     , sqlite::database("/opt/mbtiles/russia.mbtiles"    /*, config*/) ),
//            std::make_pair("navdata"    , sqlite::database("/opt/mbtiles/navdata.mbtiles"   /*, config*/) ),
//            std::make_pair("obstacles"  , sqlite::database("/opt/mbtiles/obstacles.mbtiles" /*, config*/) ),
//        };

//    /*
//    do_session = [&](session && ses) {
//      session s;
//      s.swap(ses);
//        // do a read
//        auto read_size( s->available() );
//        if(1 > read_size )
//            return;

//        boost::asio::socket_base::keep_alive opt_keep_alive(true);
//        s->set_option(opt_keep_alive);
//        auto buf = std::make_shared<std::vector<char>>(read_size);

//        s->async_receive(boost::asio::buffer(*buf), [&,s,buf](error_code ec, size_t) {
//            if ( ec and ( ec != boost::asio::error::eof ))
//                std::cerr << "read failed: " << ec.message() << "\n";
//            else {
////                std::cout << "Echo to " << s->remote_endpoint(ec) << ": " << std::string(buf->begin(), buf->end());
//                if (ec)
//                    std::cerr << "endpoint failed: " << ec.message() << "\n";
//                else
//                {
//                  if( htl::utils::StrSplit( buf->data(), "\n").size() < 5 )
//                    return ;

//                  auto strings = htl::utils::StrSplit( htl::utils::StrSplit(htl::utils::StrSplit( buf->data(), "\n").at(0)," ").at(1), "/");
//                  auto y (std::atoi( htl::utils::StrSplit( strings.back(), ".").front().c_str()));
//                  auto x (std::atoi( strings.at( strings.size() - 2 ).c_str() ));
//                  auto z (std::atoi( strings.at( strings.size() - 3).c_str() ));
//                  y = (1 << std::uint64_t(z)) - 1 - y;

//                  std::cout << "z = " << z <<" x = " << x << " y = " << y << std::endl;
//                  std::flush( std::cout );
//                  std::string db_key ( strings.at( strings.size() - 5) );
//                  std::vector<char> pbf;
//                  databases.at( db_key )
//                      << std::string("select tile_data from tiles where zoom_level = ? and tile_column = ? and tile_row = ?")
//                      << z << x << y
//                      >>
//                  [&]( std::vector<char> p )
//                  {
//                      pbf.swap(p);
//                  };

//                      std::string result_header = pbf.empty() ? Str500 : Str200;
//                      std::cout << result_header.c_str() << std::endl;
//                      std::flush( std::cout );

//                    std::vector<boost::asio::const_buffer> asio_buffer;
//                    asio_buffer.push_back( boost::asio::const_buffer(result_header.data(), result_header.size()) );
//                    if( not pbf.empty() )
//                        asio_buffer.push_back( boost::asio::const_buffer(pbf.data(), pbf.size()) );
//                    else
//                      asio_buffer.push_back( boost::asio::const_buffer(BlankPNG.data(), BlankPNG.size()) );
//                    pbf.clear();

//                    s->async_send( asio_buffer,
//                                   [&,s ](error_code ec, size_t wr_size )
//                    {
//                      if (ec) std::cerr << "write failed: " << ec.message() << "\n";
//                      std::cout << "wr_size =" << wr_size << std::endl;
//                      std::flush( std::cout );
//                    });
//                  };
//                }
//                do_session(s); // full duplex, can read while writing, using a second buffer
////                std::async(std::launch::async,[&](){ do_session(s); } ).wait_for( std::chrono::milliseconds(0));
//            }
////        }
//);
//    };
//*/

////    do_session = [&](htl::asio::SocketSessionShared<boost::asio::ip::tcp> & ses)
//    do_session = [&]( session s)
//    {
//      auto read_size( s.available() );

//      if(1 > read_size )
//        return;

//      boost::asio::socket_base::keep_alive opt_keep_alive(true);
//      s.set_option(opt_keep_alive);
//      auto buf = std::make_shared<std::vector<char>>(read_size);

//      s.async_receive( boost::asio::buffer(*buf),
//                      [&,&s,buf](error_code ec, size_t)
//      {
//            if ( ec and ( ec != boost::asio::error::eof ))
//            {
//                std::cerr << "read failed: " << ec.message() << "\n";
//            }
//            else
//            {
////                std::cout << "Echo to " << s.remote_endpoint(ec) << ": " << std::string(buf->begin(), buf->end());
//                if (ec)
//                  std::cerr << "endpoint failed: " << ec.message() << "\n";
//                else
//                {
//                  if( htl::utils::StrSplit( buf->data(), "\n").size() < 5 )
//                    return ;

//                  auto strings = htl::utils::StrSplit( htl::utils::StrSplit(htl::utils::StrSplit( buf->data(), "\n").at(0)," ").at(1), "/");
//                  auto y (std::atoi( htl::utils::StrSplit( strings.back(), ".").front().c_str()));
//                  auto x (std::atoi( strings.at( strings.size() - 2 ).c_str() ));
//                  auto z (std::atoi( strings.at( strings.size() - 3).c_str() ));
//                  y = (1 << std::uint64_t(z)) - 1 - y;

//                  std::cout << "z = " << z <<" x = " << x << " y = " << y << std::endl;
//                  std::flush( std::cout );
//                  std::string db_key ( strings.at( strings.size() - 5) );
//                  std::vector<char> pbf;

//                  databases.at( db_key )
//                      << std::string("select tile_data from tiles where zoom_level = ? and tile_column = ? and tile_row = ?")
//                      << z << x << y
//                      >> [&]( std::vector<char> p ) { pbf.swap(p); };

//                  std::string result_header = pbf.empty() ? Str500 : Str200;
//                  std::cout << result_header.c_str() << std::endl;
//                  std::flush( std::cout );

//                  std::vector<boost::asio::const_buffer> asio_buffer;
//                  asio_buffer.push_back( boost::asio::const_buffer(result_header.data(), result_header.size()) );
//                  if( not pbf.empty() )
//                      asio_buffer.push_back( boost::asio::const_buffer(pbf.data(), pbf.size()) );
//                  else
//                    asio_buffer.push_back( boost::asio::const_buffer(BlankPNG.data(), BlankPNG.size()) );

//                  pbf.clear();

//                  s.async_send(
//                        asio_buffer,
//                        [ & ](error_code ec, size_t wr_size )
//                        {
//                          if (ec) std::cerr << "write failed: " << ec.message() << "\n";
//                          std::cout << "wr_size =" << wr_size << std::endl;
//                          std::flush( std::cout );
//                        } );
//                }
//            }
//      });
////      do_session(ses);
//    };

//    do_accept = [&] {
//        auto s =  session(svc);
////        auto s =
////                htl::asio::MakeSocketSession< boost::asio::ip::tcp >(
//////                  m_server_ref.ActionsRef(),
////                  htl::asio::TransferType::HalfDuplexIn,
////                  true,//m_keep_alive,
////                  10000 );


//        a->async_accept( s,
//                         [&](error_code ec)
//        {
//          if(ec)
//          {
//            std::cerr << "accept failed: " << ec.message() << "\n";
//          }
//          else {
//            a->set_option(tcp::acceptor::keep_alive(true));
//            std::async
//            (
//              std::launch::async,
//              [ & ]()
//              {
//                do_session( std::move(s) );
//              }
//            ).wait_for( std::chrono::milliseconds(0) );
////            std::async(
////                  std::launch::async,
////                  [&](){ do_session(s); }
////            ).wait_for( std::chrono::milliseconds(0));
//////              std::async(std::launch::async,[&](){ do_session(s->SocketRef()); } ).wait_for( std::chrono::milliseconds(0));
////////              std::async(std::launch::async,[&](){ do_accept(); } ).wait_for( std::chrono::milliseconds(0)); // accept the next
////////              do_session(s);
//            do_accept();
//          }
//        } );
//    };

////    do_accept_n = [&] {
////      //        auto s =  std::make_shared<session::element_type>(svc);
////              auto s =
////                      htl::asio::MakeSocketSession< boost::asio::ip::tcp >(
////      //                  m_server_ref.ActionsRef(),
////                        htl::asio::TransferType::HalfDuplexIn,
////                        true,//m_keep_alive,
////                        10000 );

////        an->async_accept( s->SocketRef(),
////                          [&](error_code ec)
////        {
////            if (ec)
////                std::cerr << "accept failed: " << ec.message() << "\n";
////            else {
////              an->set_option(tcp::acceptor::keep_alive(true));
////              std::async
////              (
////                std::launch::async,
////                [ & ]()
////                {
////                  do_session( s );
////                }
////              ).wait_for( std::chrono::milliseconds(0) );

//////              std::async(
//////                    std::launch::async,
//////                    [&](){ do_session( s ); }
//////              ).wait_for( std::chrono::milliseconds(0) );
////////              std::async(std::launch::async,[&](){ do_accept(); } ).wait_for( std::chrono::milliseconds(0)); // accept the next
////////              do_session(s);
////              do_accept_n();
////            }
////        });
////    };

////    std::async(std::launch::async,[&](){ do_accept(); } ).wait_for( std::chrono::milliseconds(0)); // accept the next
//    do_accept();
////    do_accept_n();
//    while( not htl::asio::AsioService::Instance().Start( true ) );
//    while( true );
////     svc.run();   // wait for shutdown (Ctrl-C or failure)
//}
