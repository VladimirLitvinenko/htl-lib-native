#include "JSONFileInfo.h"

namespace htl     {
namespace qt      {

//------------------------------------------------------------------------------
JSONFileInfo::JSONFileInfo    ( QObject *parent )
  : JSONFileInfo::base_class_t( parent )
{
  Init();
}

bool
JSONFileInfo::SetConnections()
{
  auto & i_ref = JSONFilesFactory::Instance();
  return( QObject::connect( &i_ref, &JSONFilesFactory::sgnUpdate,
                            this  , &JSONFileInfo::OnUpdateFile ) );
}

void
JSONFileInfo::Init()
noexcept
{
  auto & i_ref = JSONFilesFactory::Instance();
  i_ref.Start();
}

//------------------------------------------------------------------------------

}// namespace qt
}// namespace htl
