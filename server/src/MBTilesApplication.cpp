#include "server/MBTilesApplication.h"

namespace                         htl   {
namespace                         tiles {

MBTilesApplication::MBTilesApplication( int argc, char ** argv )
  : MBTilesApplication::base_class_t  ( argc, argv)
{
  MBTilesApplication::base_class_t::SetExistsCallback(
  [ name = Name() ]()
  {
    std::cout << "Application " << name.c_str () << " exists." << std::endl;
    std::flush(std::cout);
  });
  SetBlockMode( true );
}

MBTilesApplication::~MBTilesApplication() = default;

void
MBTilesApplication::Reload( int code )
{
  Restart(code);
}

void MBTilesApplication::Start()
{
  SetAction( boost::bind( &factory_t::Start, &this->m_factory ) );
  MBTilesApplication::base_class_t::Start();
}

void
MBTilesApplication::Stop()
{
  MBTilesApplication::base_class_t::Stop();

  while( m_factory.IsActive() )
    m_factory.Stop();
  SetAction( [](){} );
}

}// namespace                     tiles
}// namespace                     htl
