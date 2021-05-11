
#include "AppSkeleton.h"
#include "MBTilesDbFactory.h"
#include "Utils.h"

#include <iostream>
#include <ostream>

using namespace sqlite;
using namespace std;
using namespace htl::app;

void test_mbtile_db_factory()
{
  try
  {
    using namespace htl::tiles;
    htl::tiles::MBTilesDbFactory factory;
    std::cout << std::boolalpha << factory.IsValid() << std::endl;
  }
  catch( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
  }
  std::cout << "Test complete." << std::endl;
  std::flush(std::cout);
}

int main( int argc, char** argv )
{
  AppSkeleton app( argc, argv );
    app.SetExistsCallback (
    [ name = app.Name() ]()
    {
      std::cout << "Application " << name.c_str () << " exists." << std::endl;
      std::flush(std::cout);
    });

  app.SetAction( test_mbtile_db_factory );
  app.Start();
  return app.Exec();
}
