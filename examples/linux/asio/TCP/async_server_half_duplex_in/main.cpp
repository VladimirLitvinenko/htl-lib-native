#include <QCoreApplication>
#include <AsioServerDuplex.h>
#include "../../Common.h"

#undef ACCEPT_IN_WHILE
//#define ACCEPT_IN_WHILE

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  htl::asio::AsioTCPServer server( htl::asio::TransferType::HalfDuplexIn, /*33333*/8888 );

  server.SetSocketDeadline( 10000 );
  server.SetBufferAction(
        htl::asio::DataType::Input,
        boost::bind( DumpReceive, _1 ) );

  server.SetBufferAction(
        htl::asio::DataType::Output,
        boost::bind( DumpSend, _1 ) );


  return a.exec();
}
