#ifndef HTTPHEADER_H
#define HTTPHEADER_H

#include <Document.h>
#include <HTTPLiterals.h>

namespace                         htl   {
namespace                         http  {

//------------------------------------------------------------------------------
/**
 * @brief Класс HTTPHeader реализуат атрибуты и методы для работы с заголовками HTTP сообщений
 */
class COMMON_SHARED_EXPORT        HTTPHeader
: public                          htl::docs::Document< http_header_t >
{
public:
  using base_class_t            = htl::docs::Document< http_header_t >;
  /**/                            HTTPHeader  ();
  /**/                            HTTPHeader  ( const std::string & req );

  bool                            IsValid     () const noexcept override;
  bool                            IsCodeValid () const;
  bool                            IsKeepAlive ();

  virtual void                    SetRequest  ( const std::string & request );
  virtual std::string             Get( const std::string & key );
  virtual void                    Set( const HttpStatus status );
  virtual void                    Set( const std::string & key, const std::string & value );
  std::string                     GetHeader();

  void                            ClearContent    () override;

  std::string                     GetDestinition  () const;

  std::string                     Dump();

protected:
  virtual void                    Parce   ( const std::string & request );

private:
  std::string                     m_destinition;
  HttpStatus                      m_status      = HttpStatus::Unused;
};

}// namespace                     http
}// namespace                     htl

#endif // HTTPHEADER_H
