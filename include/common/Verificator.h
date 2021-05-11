#ifndef VERIFICATOR_H
#define VERIFICATOR_H

#include "common/Utils.h"
#include "base/documents/Document.h"
#include <unordered_map>

namespace                         htl   {
namespace                         utils {

//------------------------------------------------------------------------------
using verify_method_map         = std::unordered_map<std::size_t , htl::utils::action_ptr >;

//------------------------------------------------------------------------------
/**
 * @brief Класс Verificator реализует набор методов проверки чего либо.
 *
 * Добавление метода произходит через SetAction
 * Выполнение проверок осуществляется методами Vetify
 */
class COMMON_SHARED_EXPORT        Verificator
: public                          htl::docs::Document< htl::utils::verify_method_map >
{
public:
  using base_class_t            = htl::docs::Document< htl::utils::verify_method_map >;

  /**/                            Verificator();
  /**/                            Verificator( const Verificator & );
  /**/                            Verificator( Verificator && );

  virtual Verificator &           operator=  ( const Verificator & );
  virtual Verificator &           operator=  ( Verificator && ) noexcept;

  virtual verify_method_map::iterator & Positioning ( std::size_t key );
  virtual verify_method_map::iterator & Positioning ( const std::string & name )
  {
    return Positioning( std::hash<std::string>()(name) );
  }

  template< typename Function_, typename ... Args_ >
  verify_method_map::iterator   & SetAction   ( std::size_t key, Function_&& action, Args_ ... params );

  template< typename Function_, typename ... Args_ >
  verify_method_map::iterator   & SetAction   ( const std::string & name, Function_&& action, Args_ ... params );

  template< typename Function_ >
  verify_method_map::iterator   & SetAction   ( std::size_t key, Function_&& action );

  template< typename ... Args_  >
  verify_method_map::iterator   & SetParams   ( std::size_t key, Args_ ... params );

  template< typename ... Args_  >
  verify_method_map::iterator   & SetParams   ( const std::string & name, Args_ ... params );

  template< typename Function_ >
  verify_method_map::iterator   & operator << ( Function_&& action )
  {
    return SetAction<Function_>( m_position, std::forward<Function_>(action) );
  }
  template< typename ... Args_  >
  verify_method_map::iterator   & operator << ( Args_ ... params )
  {
    BEGIN_LOCK_SECTION_SELF_;
    m_position = SetParams<Args_...>( m_position, std::forward<Args_>(params)... );
    return std::ref( m_position );
    END_LOCK_SECTION_
  }

  virtual void                    Verify      ( std::size_t key );
  virtual void                    Verify      ( const std::string & name );
  virtual void                    Verify      ();

  virtual void                    Clear       ()
  {
    ClearContent();
  }
  virtual void                    Clear       ( std::size_t key );
  virtual void                    Clear       ( const std::string & name )
  {
    Clear( std::hash<std::string>()(name) );
  }

protected:
  void                            ClearContent() override
  {
    BEGIN_LOCK_SECTION_SELF_;
    ContentRef().clear();
    END_LOCK_SECTION_
  }

  template< typename Function_ >
  verify_method_map::iterator   & SetAction   ( verify_method_map::iterator & iter, Function_&& action );

  template< typename ... Args_  >
  verify_method_map::iterator   & SetParams   ( verify_method_map::iterator & iter, Args_ ... params );

private:
  verify_method_map::iterator     m_position  { m_content.end() };
};

//------------------------------------------------------------------------------

template< typename ... Args_>
verify_method_map::iterator &
Verificator::SetParams( std::size_t key, Args_ ... params)
{
  BEGIN_LOCK_SECTION_SELF_;
  m_position = SetParams< Args_...>( Positioning( key ),
                                     std::forward< Args_ >(params)... );
  return std::ref( m_position );
  END_LOCK_SECTION_
}

template<typename ...Args_>
verify_method_map::iterator &
Verificator::SetParams(const std::string& name, Args_ ... params)
{
  BEGIN_LOCK_SECTION_SELF_;
  m_position = SetParams< Args_...>( Positioning(name),
                                     std::forward< Args_ >(params)... );
  return std::ref( m_position );
  END_LOCK_SECTION_
}

template<typename Function_>
verify_method_map::iterator &
Verificator::SetAction( std::size_t key, Function_&& action)
{
  BEGIN_LOCK_SECTION_SELF_;
  m_position = SetAction( Positioning( key ),
                          std::forward<Function_&&>(action));
  return std::ref( m_position );
  END_LOCK_SECTION_
}

template<typename Function_>
verify_method_map::iterator &
Verificator::SetAction( verify_method_map::iterator & iter, Function_&& action)
{
  BEGIN_LOCK_SECTION_SELF_;
  if( ( iter != ContentRef().end() ) and iter->second )
    iter->second->SetAction( std::forward< Function_&& >(action) );
  return iter;
  END_LOCK_SECTION_
}

template< typename Function_, typename ... Args_ >
verify_method_map::iterator &
Verificator::SetAction( std::size_t key, Function_&& action, Args_ ... params )
{
  BEGIN_LOCK_SECTION_SELF_;
  auto sh_act = [ & ]()
  {
    return std::make_shared< htl::utils::action_p_t< Function_, Args_... > >
        ( std::forward< Function_&& >( action ),
          std::forward< Args_ >(params)... );
  };
  UNUSED( Positioning( key ) );
  if( m_position == ContentRef().end() )
  {
    auto result = ContentRef().emplace( key, sh_act() );
    if( result.second )
      m_position = result.first;
  }
  else
  {
    m_position->second = sh_act();
  }
  return std::ref( m_position );
  END_LOCK_SECTION_
}

template<typename Function_, typename ...Args_>
verify_method_map::iterator &
Verificator::SetAction(const std::string & name, Function_&& action, Args_ ... params)
{
  return SetAction( std::hash< std::string>()(name),
             std::forward< Function_&& >( action ),
             std::forward< Args_ >(params)... );
}

template<typename ... Args_>
verify_method_map::iterator &
Verificator::SetParams( verify_method_map::iterator & iter, Args_ ... params)
{
  BEGIN_LOCK_SECTION_SELF_;
  if( ( iter != ContentRef().end() ) and iter->second )
    (* ( iter->second ) )( std::forward< Args_ >(params)... );
  return iter;
  END_LOCK_SECTION_
}

//------------------------------------------------------------------------------

}// namespace                     utils
}// namespace                     htl

#endif // VERIFICATOR_H
