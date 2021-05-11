#include <QCoreApplication>
#include <AsioServerDuplex.h>
#include "../../Common.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  htl::asio::AsioServerDuplex< htl::asio::AsioTCPServer >
      server( 33333, 33444, 1000 );

  server.SetSocketDeadline( htl::asio::DataType::Input  , 1000 );
  server.SetSocketDeadline( htl::asio::DataType::Output , 2000 );

  server.SetBufferAction(
        htl::asio::DataType::Input,
        boost::bind( DumpReceive, _1 ) );

  server.SetBufferAction(
        htl::asio::DataType::Output,
        boost::bind( DumpSend, _1 ) );

  while( not htl::asio::AsioService::Instance().Start() );
  while(true);
//  server.Start();
  return a.exec();
}
