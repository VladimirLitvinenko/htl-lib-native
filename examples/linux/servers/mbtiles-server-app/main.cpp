#include "server/MBTilesApplication.h"

using namespace htl::tiles;

int main( int argc, char** argv )
{
  MBTilesApplication app( argc, argv );
  app.Start();
  return app.Exec();
}
