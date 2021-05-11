#include <QCoreApplication>
#include "AsioClient.h"
#include "../../Common.h"
#include "Logger.h"

#include <algorithm>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  using namespace htl::asio;

//  SetHalfDuplexMode( true );

  auto  connector ( std::make_shared< htl::asio::AsioClient< boost::asio::ip::tcp > >
                    ( "localhost", "8888", true, true, 1000 ) );


  connector->SetTimeoutMs( 10 );
  connector->SetTransformation( []( const htl::vectors_data_t & data )
                            -> htl::vectors_data_t
                            {
                                 for( auto & d : data )
                                 DUMP_CRITICAL_CONSOLE( d.data() );
                              return data;
                            } );

//  while( true  )
//  {
//    UNUSED( htl::asio::AsioService::Instance().Start() );
//  }
  while( not connector->Start( true ) );

  return a.exec();
}
