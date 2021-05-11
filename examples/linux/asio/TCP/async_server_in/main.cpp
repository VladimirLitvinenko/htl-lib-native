#include <QCoreApplication>
#include <AsioTCPServer.h>
#include "../../Common.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  htl::asio::AsioTCPServer server( htl::asio::TransferType::SimplexIn, /*33333*/8888 );
  server.SetSocketDeadline( 2000 );
  server.SetBufferAction(
        htl::asio::DataType::Input,
        boost::bind( DumpReceive, _1 ) );

  while( not htl::asio::AsioService::Instance().Start() );

  return a.exec();
}
