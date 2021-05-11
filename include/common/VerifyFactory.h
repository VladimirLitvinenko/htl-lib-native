#ifndef VERIFYFACTORY_H
#define VERIFYFACTORY_H

#include "common/Verificator.h"
#include "base/documents/Document.h"
#include <unordered_map>

namespace                         htl   {
namespace                         utils {

//------------------------------------------------------------------------------
using verify_map                = std::unordered_map< std::size_t/*hash code*/, htl::utils::Verificator >;

//------------------------------------------------------------------------------
/**
 * @brief Класс VerifyFactory реализует фабрику классов методов проверки.
 *
 * Основан на апинципе "Документ".
 * Содержимое - хэш-таблица классов проверок
 */
class COMMON_SHARED_EXPORT        VerifyFactory
: public                          htl::docs::Document< htl::utils::verify_map >
{
public:
  using base_class_t            = htl::docs::Document< htl::utils::verify_map >;

  /**/                            VerifyFactory     ();
  /**/                            VerifyFactory     ( const VerifyFactory & ) = delete;
  /**/                            VerifyFactory     ( VerifyFactory && );

  virtual VerifyFactory &         operator=         ( const VerifyFactory & ) = delete;
  virtual VerifyFactory &         operator=         ( VerifyFactory && ) noexcept;

  /**
   * @brief Метод Instance возвращает ссылку на статический экземпляр фабрики
   * @return
   */
  static VerifyFactory &          Instance          ();

  template< typename Function_, typename ... Args_ >
  verify_method_map::iterator   & SetAction
  (
      std::size_t                 factory_key,
      std::size_t                 verify_key,
      Function_                && action,
      Args_                   ... params
  )
  {
    BEGIN_LOCK_SECTION_SELF_;
    return
        ContentRef()[ factory_key ].
        SetAction( verify_key,
                   std::forward<Function_&&>(action),
                   std::forward<Args_>      (params)...);
    END_LOCK_SECTION_
  }
  template< typename Function_, typename ... Args_ >
  verify_method_map::iterator   & SetAction
  (
      std::size_t                 factory_key,
      const std::string         & verify_name,
      Function_                && action,
      Args_                   ... params
  )
  {
    BEGIN_LOCK_SECTION_SELF_;
    return
        ContentRef()[ factory_key ].
        SetAction( verify_name,
                   std::forward<Function_&&>(action),
                   std::forward<Args_>      (params)...);
    END_LOCK_SECTION_
  }
  template< typename Function_, typename ... Args_ >
  verify_method_map::iterator   & SetAction
  (
      const std::string         & factory_name,
      std::size_t                 verify_key,
      Function_                && action,
      Args_                   ... params
  )
  {
    BEGIN_LOCK_SECTION_SELF_;
    return
        ContentRef()[ std::hash<std::string>()(factory_name) ].
        SetAction( verify_key,
                   std::forward<Function_&&>(action),
                   std::forward<Args_>      (params)...);
    END_LOCK_SECTION_
  }
  template< typename Function_, typename ... Args_ >
  verify_method_map::iterator   & SetAction
  (
      const std::string         & factory_name,
      const std::string         & verify_name,
      Function_                && action,
      Args_                   ... params
  )
  {
    BEGIN_LOCK_SECTION_SELF_;
    return
        ContentRef()[ std::hash<std::string>()(factory_name) ].
        SetAction( verify_name,
                   std::forward<Function_&&>(action),
                   std::forward<Args_>      (params)...);
    END_LOCK_SECTION_
  }
  virtual void                    Verify      ( std::size_t factory_key );
  virtual void                    Verify      ( const std::string & name )
  {
    Verify( std::hash<std::string>()(name) );
  }
  virtual void                    Verify      ( std::size_t factory_key, std::size_t verify_key );
  virtual void                    Verify      ( std::size_t factory_key, const std::string & verify_name );
  virtual void                    Verify      ( const std::string & factory_name, std::size_t verify_key )
  {
    Verify( std::hash<std::string>()(factory_name),verify_key);
  }
  virtual void                    Verify      ( const std::string & factory_name, const std::string & verify_name  )
  {
    Verify( std::hash<std::string>()(factory_name),verify_name);
  }
  virtual void                    VerifyOnly  ( std::size_t verify_key );
  virtual void                    VerifyOnly  ( const std::string & verify_name )
  {
    VerifyOnly( std::hash<std::string>()( verify_name) );
  }
  virtual void                    Verify      ( bool clear_after_verification = true );

  virtual void                    Clear       ();
  virtual void                    Clear       ( std::size_t factory_key );
  virtual void                    Clear       ( const std::string & factory_name )
  {
    Clear( std::hash<std::string>()(factory_name) );
  }
  virtual void                    Clear       ( std::size_t factory_key, std::size_t verify_key );
  virtual void                    Clear       ( std::size_t factory_key, const std::string & verify_name )
  {
    Clear( factory_key, std::hash<std::string>()(verify_name) );
  }
  virtual void                    Clear       ( const std::string & factory_name, std::size_t verify_key )
  {
    Clear( std::hash<std::string>()(factory_name), verify_key );
  }
  virtual void                    Clear       ( const std::string & factory_name, const std::string & verify_name )
  {
    Clear( std::hash<std::string>()(factory_name), std::hash<std::string>()(verify_name) );
  }

protected:
  void                            ClearContent      () override
  {
    BEGIN_LOCK_SECTION_SELF_;
    ContentRef().clear();
    END_LOCK_SECTION_
  }
};

//------------------------------------------------------------------------------

}// namespace                     utils
}// namespace                     htl

#endif // VERIFYFACTORY_H
