#include <AsioTCPServer.h>
#include <AsioSocketSession.h>
#include <Types.h>

#include <iostream>

using namespace std;

int main()
{
  htl::asio::AsioTCPServer server1( 2222 );
  server1.SetSocketDeadline( 10000 );

  server1.SetTransformation( []( const htl::vectors_data_t &)
                            -> htl::vectors_data_t
                            {
                              return htl::vectors_data_t();
                            } );
  while( not server1.Start( true ) );
  while( true );

  return 0;
}
