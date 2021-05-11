#include <QCoreApplication>
#include <AsioClient.h>
#include "../../Common.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  using namespace htl::asio;

  auto  connector ( std::make_shared< htl::asio::AsioClient< boost::asio::ip::tcp > >
                    ( htl::asio::TransferType::HalfDuplexOut,
                      "127.0.0.1",
                      "8888",
//                      "33444",
////                      "33333",
                      1000 ) );


  htl::asio::AsioService::Instance().AddAfterStopCallback
      ( {
          []( void * )
          {
            std::cout << "SERVICE STOPED...." << std::endl;
          }
          ,
          nullptr
        } );

  connector->SetTimeoutMs( 20000 );
  connector->SetBufferAction(
        htl::asio::DataType::Input,
        boost::bind( DumpReceive, _1 ) );

  connector->SetBufferAction(
        htl::asio::DataType::Output,
        boost::bind( DumpSend, _1 ) );

  while( not htl::asio::AsioService::Instance().Start() );

  return a.exec();
}
