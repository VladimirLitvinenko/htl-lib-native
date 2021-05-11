#ifndef GEOJSONSOURCES_H
#define GEOJSONSOURCES_H

#include "JSONFileInfo.h"

namespace htl     {
namespace qt      {

//------------------------------------------------------------------------------
class COMMON_SHARED_EXPORT        GeoJsonSources
: public                          htl::qt::JSONFileInfo
{
  Q_OBJECT

public:
  using base_class_t            = htl::qt::JSONFileInfo;

  explicit                        GeoJsonSources( QObject * parent = Q_NULLPTR );
  virtual                       ~ GeoJsonSources()  {}

  Q_SLOT void                     OnUpdateFile  () override;
};

//------------------------------------------------------------------------------

}// namespace qt
}// namespace htl

#endif // GEOJSONSOURCES_H
