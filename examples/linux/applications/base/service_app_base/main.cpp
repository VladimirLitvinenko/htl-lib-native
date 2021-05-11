#include <AppService.h>
#include <AppSkeleton.h>

using namespace htl::app;

int main( int argc, char ** argv )
{
  // >ps xa | grep service_app_base
  // kill -s TERM <first PID>
  // kill -s TERM <second PID>

  AppService< AppSkeleton > service( argc, argv );

  service.SetAction([]( const std::string & name )
  {
    std::cout << " Execute " << name.c_str () << " application." << std::endl;
    std::flush(std::cout);
  },
  std::string( argv[0] ) );

  service.SetExistsCallback (
  [ name = service.Name () ]()
  {
    std::cout << "Service  " << name.c_str () << "   exists. Terminate." << std::endl;
    std::flush(std::cout);
  });
  service.SetUnique(true);
  service.RunService();
  return service.GetCode();
}
