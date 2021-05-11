#include <AppSkeleton.h>
#include <MBTileServer.h>
#include <iostream>

using namespace std;
using namespace htl::tiles;
using namespace htl::app;

int main( int argc, char** argv )
{
  AppSkeleton app( argc, argv );
  app.SetExistsCallback (
  [ name = app.Name() ]()
  {
    std::cout << "Application " << name.c_str () << " exists." << std::endl;
    std::flush(std::cout);
  });

  MBTileServer server(8888);
  app.SetBlockMode( true );
  app.SetAction( []( MBTileServer & server_ref)
  {
    server_ref.Start( true );

    auto i(0);
    while( i++ < 200 )
    {
      std::this_thread::sleep_for( std::chrono::seconds(1) );
    }
    server_ref.Stop();
  }, std::ref( server ) );
  app.Exec();
  return app.GetCode ();
}
