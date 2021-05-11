#include <AppSkeleton.h>
#include <MBTileDatabase.h>
#include <Utils.h>

#include <iostream>
#include <ostream>

using namespace sqlite;
using namespace std;
using namespace htl::app;

void test_mbtile( const std::string & db_name )
{
  try
  {
    using namespace htl::tiles;
    MBTileDatabase db( db_name );
    std::cout << db.StateMessage().c_str() << std::endl;
  }
  catch( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
    exit(EXIT_FAILURE);
  }
  std::cout << "Test \"" << db_name.c_str () << "\" complete." << std::endl;
  std::flush(std::cout);
};


int main( int argc, char** argv )
{
  AppSkeleton app( argc, argv );
    app.SetExistsCallback(
    [ name = app.Name() ]()
    {
      std::cout << "Application " << name.c_str () << " exists." << std::endl;
      std::flush(std::cout);
    });

  app.SetAction(test_mbtile, "/home/nemo/navdata/russia.mbtiles");
  app.Start();
  return app.Exec();
}
