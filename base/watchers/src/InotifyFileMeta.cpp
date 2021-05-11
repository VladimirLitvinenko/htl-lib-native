#include <InotifyFileMeta.h>
#include <type_traits>
#include <boost/bind/bind.hpp>

namespace                         htl   {
namespace                         watch {


InotifyFileMeta::~InotifyFileMeta() = default;

bool
InotifyFileMeta::IsValid()
const
{
  return
//      base_class_t::IsValid()
//      and
      m_executer_ptr.operator bool();
}

}// namespace                     watch
}// namespace                     htl
