#include <AppSkeleton.h>
#include <iostream>

using namespace htl::app;

void method( const std::string & name )
{
  std::cout << " Execute " << name.c_str () << " application." << std::endl;
  std::flush(std::cout);
};

int main( int argc, char ** argv )
{
  AppSkeleton app( argc, argv );
  app.SetExistsCallback(
  [ name = app.Name() ]()
  {
    std::cout << "Application " << name.c_str () << " exists." << std::endl;
    std::flush(std::cout);
  });
  app.SetBlockMode( true );
  app.SetUnique( true );
  app.SetAction( method, argv[0] );
  app.Start();
  return app.Exec();
}
