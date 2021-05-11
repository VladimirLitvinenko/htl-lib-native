#ifndef HTTPGEOHEADER_H
#define HTTPGEOHEADER_H

#include "HTTPHeader.h"

namespace                         htl   {
namespace                         tiles {

//------------------------------------------------------------------------------

class COMMON_SHARED_EXPORT        HTTPGeoHeader :
public                            htl::http::HTTPHeader
{
public:
  using base_class_t            = htl::http::HTTPHeader;

  /**/                            HTTPGeoHeader ();
  explicit                        HTTPGeoHeader ( const std::string & req );
  void                            SetRequest    ( const std::string & req ) override;
  std::string                     GetGeoJSON    () const;

protected:
  virtual void                    PrepareGeoJSON() noexcept;

private:
  std::string                     m_geo_json;
};

//------------------------------------------------------------------------------

}// namespace                     tiles
}// namespace                     htl

#endif // HTTPGEOHEADER_H
