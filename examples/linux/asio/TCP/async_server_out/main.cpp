#include <QCoreApplication>
#include <AsioTCPServer.h>
#include "../../Common.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  htl::asio::AsioTCPServer server( htl::asio::TransferType::SimplexOut, 33444 );
  server.SetSocketDeadline( 2000 );

  server.SetBufferAction(
        htl::asio::DataType::Output,
        boost::bind( DumpSend, _1 ) );

  while( not htl::asio::AsioService::Instance().Start() );

  return a.exec();
}
