#include "DbFactory.h"

using namespace htl::db;

int main()
{
  SqliteDatabase db(":memory:");
  if( db.IsValid() )
  {

    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
