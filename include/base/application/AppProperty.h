#ifndef APPPROPERTY_H
#define APPPROPERTY_H

#include "Document.h"

#include <mutex>
#include <memory>

namespace                         htl   {
namespace                         app   {

//------------------------------------------------------------------------------
/**
 * @brief Класс AppProperty описывает свойства и методы работы с параметрами приложения/
 *
 * Представляет собой экземпляр типа "Документ" с набором комнейнера строк как атрибутов приложения
 */
class COMMON_SHARED_EXPORT        AppProperty
: public                          htl::docs::Document< std::vector< std::string > >
, public                          std::enable_shared_from_this< AppProperty >
{
public:
  using base_class_t            = htl::docs::Document< std::vector< std::string > >;

  /**/                            AppProperty   () = default;
  /**/                            AppProperty   ( int argc, char ** argv );
                                ~ AppProperty   () override;

  virtual AppProperty           & operator=     ( const std::pair< int, char** > & args );
  virtual AppProperty           & operator=     ( const std::tuple< int, char** > & p );

  std::pair< int, std::shared_ptr<char**> > Arguments     ();
  void                            ClearContent () override  { m_content.clear (); }

  std::string                     ApplicationName () const;
};

using appproperty_shared_t      = std::shared_ptr< htl::app::AppProperty >;
using appproperty_week_t        = std::weak_ptr<appproperty_shared_t::element_type>;

}// namespace                   app
}// namespace                   htl

#endif // APPPROPERTY_H
