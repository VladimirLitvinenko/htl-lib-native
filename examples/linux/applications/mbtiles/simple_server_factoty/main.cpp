#include <MBTileSrvFactory.h>
#include <iostream>

using namespace std;
using namespace htl::tiles;

int main()
{
  MBTileSrvFactory factory;

  cout << factory.GetConfig() << endl;
  factory.DumpServersID();
  return 0;
}
