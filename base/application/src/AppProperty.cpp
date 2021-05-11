#include "AppProperty.h"
#include "Logger.h"
#include "DocumentLiterals.h"

namespace                         htl   {
namespace                         app   {

AppProperty::~AppProperty() = default;

AppProperty::AppProperty(int argc, char** argv)
  : AppProperty::base_class_t ( htl::docs::DOC_APP_PROP_ID )
{
  * this = std::make_pair  ( argc, argv );
}

AppProperty &
AppProperty::operator=( const std::pair< int, char** > & args)
{
  BEGIN_LOCK_SECTION_SELF_;
  if( ( nullptr == args.second ) or ( args.first < 1 ) )
  {
    ClearContent ();
  }
  else
  {
    ContentRef ().resize( static_cast< std::size_t >( args.first ) );
    for( auto idx(0); idx < args.first; idx++ )
    {
      try
      {
        if( nullptr == args.second[ idx ] )
          break;
        ContentRef ()[ std::size_t(idx) ] = args.second[ idx ];
      }
      catch( const std::exception & e )
      {
        DUMP_EXCEPTION( e );
        break;
      }
    }
  }
  return std::ref( * this );
  END_LOCK_SECTION_
}

AppProperty&
AppProperty::operator=( const std::tuple< int, char** > & p)
{
  return operator =( std::make_pair( std::get<0>(p), std::get<1>(p) ) );
}

std::pair< int, std::shared_ptr<char**> >
AppProperty::Arguments()
{
  std::pair< int, std::shared_ptr<char**> >  retval
  {
    static_cast<int>( ContentRef ().size() ),
    std::make_shared<char**>( new char*[ContentRef ().size()] )
  };
  for( auto i(0); i < retval.first; i++ )
  {
    (*retval.second.get ())[i] = const_cast<char*>( ContentRef ().at (static_cast<std::size_t>(i)).c_str () );
  }
  return  retval;
}

std::string
AppProperty::ApplicationName()
const
{
  return
      m_content.size() > 0
      ? m_content.at(0)
      : std::string();
}

}// namespace                   app
}// namespace                   htl
