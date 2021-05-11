//#include <BytesDocument.h>
#include <string>
#include <vector>
#include <iostream>
#include <Utils.h>
#include <boost/asio.hpp>

bool DumpReceive ( std::vector< char > & data )
{
  bool retval( not data.empty() );
  if( retval )
  {
    std::cout << "-------RECEIVE----------" << std::endl;
    std::cout << "SIZE: " << data.size() << std::endl;
    std::cout << "vector =" << data.data() << std::endl;;
    std::flush( std::cout );
  }
  return retval;
}

bool DumpSend ( std::vector< char > & data )
{
//  auto tm = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//  std::string tstr( std::ctime(& tm ) );

////  data.FromByteArray(tstr);
//  data.assign(tstr.begin(), tstr.end());
//  std::cout << "\n\t-------------------- Send --------------------" << std::endl;
////  auto dumpstr( data.ToByteArray() );
//  std::string dumpstr( data.data() );
//  std::cout << "SIZE: " << dumpstr.size() << std::endl;
////  std::cout << std::string( dumpstr.c_str(), 10 ) << "..." << (dumpstr.size() - 20) << "..." << std::string( dumpstr.c_str() + dumpstr.size() - 10, 10 ) << std::endl;
//  std::cout << dumpstr.c_str() << std::endl;
//  std::flush( std::cout );

  std::string str =
    "HTTP/1.1 200 OK\r\n"
    "Access-Control-Allow-Origin: *\r\n"
    "Content-Type: application/x-protobuf\r\n"
    //    "Content-Type: application/octet-stream\r\n"
    "Content-Encoding: gzip\r\n"
    //    "Transfer-Encoding: chunked\r\n"
    //    "Cache-Control: public\r\n"
    "\r\n"

//    "GET /services/russia/tiles/4/12/14.pbf HTTP/1.1\r\n"
//    "Host: localhost:8888\r\n"
//    "User-Agent: MapboxGL/47fb3565 (Qt 5.11.0)\r\n"
//    "Connection: Keep-Alive\r\n"
//    "Accept-Encoding: gzip, deflate\r\n"
//    "Accept-Language: ru-RU,en,*\r\n\r\n"
    ;

  data.assign( str.begin(), str.end() );
  std::cout << "\n\t-------------------- Send --------------------" << std::endl;
  std::cout << "SIZE: " << data.size() << std::endl;
  std::flush( std::cout );
  return not data.empty();
}
