#include <QCoreApplication>
#include <AsioClient.h>
#include "../../Common.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  using namespace htl::asio;

  UNUSED( htl::asio::AsioService::Instance( 2000 ) );

//  SetHalfDuplexMode( true );

  auto  connector ( std::make_shared< AsioClient< boost::asio::ip::tcp > >
                    ( htl::asio::TransferType::SimplexOut,
                      "localhost",
////                      "192.168.111.140",
//                      "33333",
                      "8888",
                      3000 ) );


  htl::asio::AsioService::Instance().AddAfterStopCallback
      ( {
          []( void * )
          {
            std::cout << "SERVICE STOPED...." << std::endl;
          }
          ,
          nullptr
        } );

  connector->SetTimeoutMs( 200 );

  connector->SetBufferAction(
        htl::asio::DataType::Output,
        boost::bind( DumpSend, _1 ) );

//  while( true  )
//  {
//    UNUSED( htl::asio::AsioService::Instance().Start() );
//  }
  while( not htl::asio::AsioService::Instance().Start() );

  return a.exec();
}
