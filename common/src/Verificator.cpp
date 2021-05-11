#include "common/Verificator.h"
#include "base/documents/DocumentLiterals.h"

namespace                         htl   {
namespace                         utils {

Verificator::Verificator          ()
  : Verificator::base_class_t     ( htl::docs::DOC_VERIFY_ID )
{

}

Verificator::Verificator(const Verificator& v)
  : Verificator::Verificator      ()
{
  *this = v;
}

Verificator::Verificator          ( Verificator&& v )
  : Verificator::Verificator      ()
{
  *this = std::move(v);
}

Verificator& Verificator::operator=(const Verificator& v)
{
  BEGIN_LOCK_SECTION_SELF_;
  auto key(
      v.m_position != v.m_content.end()
                      ? v.m_position->first
                      : 0 );
  SetContent( v.m_content );
  if( key > 0 )
    Positioning( key );
  return std::ref( * this );
  END_LOCK_SECTION_
}

Verificator &
Verificator::operator=(Verificator&& v )
noexcept
{
  BEGIN_LOCK_SECTION_SELF_;
  auto key(
      v.m_position != v.ContentRef().end()
                      ? v.m_position->first
                      : 0 );
  m_content = std::move( v.m_content );
  if( key > 0 )
    Positioning( key );
  return std::ref( * this );
  END_LOCK_SECTION_
}

void
Verificator::Verify(const std::string& name)
{
  Verify( std::hash< std::string>()(name) );
}

verify_method_map::iterator &
Verificator::Positioning(std::size_t key)
{
  BEGIN_LOCK_SECTION_SELF_;
  m_position = ContentRef().find( key );
  return std::ref( m_position );
  END_LOCK_SECTION_
}

void
Verificator::Verify( std::size_t key)
{
  BEGIN_LOCK_SECTION_SELF_;
  UNUSED( Positioning( key ) );
  if( m_position != ContentRef().end() and m_position->second )
    m_position->second->Run();
  END_LOCK_SECTION_
}

void
Verificator::Verify()
{
  BEGIN_LOCK_SECTION_SELF_;
  for( auto & val : ContentRef() )
    if( val.second )
      val.second->Run();
  END_LOCK_SECTION_
}

void Verificator::Clear(std::size_t key)
{
  BEGIN_LOCK_SECTION_SELF_;
  UNUSED( Positioning( key ) );
  if( m_position != ContentRef().end() )
    ContentRef().erase(key);
  END_LOCK_SECTION_
}

}// namespace                     utils
}// namespace                     htl
