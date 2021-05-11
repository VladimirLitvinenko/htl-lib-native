#include "base/application/AppService.h"
#include "server/MBTilesApplication.h"

using namespace htl::tiles;

int main( int argc, char** argv )
{
  htl::app::AppService< htl::tiles::MBTilesApplication > app( argc, argv );
//  app.SetUnique( true );
  app.RunService();
  return app.GetCode();
}
