#include "common/VerifyFactory.h"
#include "base/documents/DocumentLiterals.h"

namespace                         htl   {
namespace                         utils {

VerifyFactory::VerifyFactory()
  : VerifyFactory::base_class_t     ( htl::docs::DOC_VERIFY_FACTORY_ID )
{

}

VerifyFactory::VerifyFactory(VerifyFactory&& v)
  : VerifyFactory::VerifyFactory()
{
  *this = std::move(v);
}

VerifyFactory &
VerifyFactory::operator=(VerifyFactory&& v)
noexcept
{
  BEGIN_LOCK_SECTION_SELF_;
  m_content = std::move( v.m_content );
  return std::ref( *this );
  END_LOCK_SECTION_
}

VerifyFactory & VerifyFactory::Instance()
{
  static VerifyFactory retval;
  return std::ref( retval );
}

void VerifyFactory::Clear()
{
  BEGIN_LOCK_SECTION_SELF_;
  ContentRef().clear();
  END_LOCK_SECTION_
}

void
VerifyFactory::Clear(std::size_t factory_key)
{
  BEGIN_LOCK_SECTION_SELF_;
  auto iter( ContentRef().find(factory_key) );
  if( iter != ContentRef().end() )
    iter->second.Clear();
  END_LOCK_SECTION_
}

void
VerifyFactory::Clear(std::size_t factory_key, std::size_t verify_key)
{
  BEGIN_LOCK_SECTION_SELF_;
  auto iter( ContentRef().find(factory_key) );
  if( iter != ContentRef().end() )
    iter->second.Clear( verify_key );
  END_LOCK_SECTION_;
}

void VerifyFactory::Verify(std::size_t factory_key)
{
  BEGIN_LOCK_SECTION_SELF_;
  auto iter( ContentRef().find(factory_key) );
  if( iter != ContentRef().end() )
    iter->second.Verify();
  END_LOCK_SECTION_
}

void VerifyFactory::Verify(std::size_t factory_key, std::size_t verify_key)
{
  BEGIN_LOCK_SECTION_SELF_;
  auto iter( ContentRef().find(factory_key) );
  if( iter != ContentRef().end() )
    iter->second.Verify(verify_key);
  END_LOCK_SECTION_;
}

void VerifyFactory::Verify(std::size_t factory_key, const std::string& verify_name)
{
  BEGIN_LOCK_SECTION_SELF_;
  auto iter( ContentRef().find(factory_key) );
  if( iter != ContentRef().end() )
    iter->second.Verify(verify_name);
  END_LOCK_SECTION_
}

void VerifyFactory::VerifyOnly(std::size_t verify_key)
{
  BEGIN_LOCK_SECTION_SELF_;
  for( auto iter(ContentRef().begin()); iter != ContentRef().end(); ++iter)
    iter->second.Verify(verify_key);
  END_LOCK_SECTION_
}

void VerifyFactory::Verify(bool clear_after_verification )
{
  BEGIN_LOCK_SECTION_SELF_;
  for( auto iter(ContentRef().begin()); iter != ContentRef().end(); ++iter)
    iter->second.Verify();
  if( clear_after_verification )
    Clear();
  END_LOCK_SECTION_
}

}// namespace                     utils
}// namespace                     htl
