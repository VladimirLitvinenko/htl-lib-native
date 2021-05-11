#ifndef MBTILESAPPLICATION_H
#define MBTILESAPPLICATION_H

#include "base/application/AppSkeleton.h"
#include "mbtiles/MBTileSrvFactory.h"

namespace                         htl   {
namespace                         tiles {

//------------------------------------------------------------------------------
/**
 * @brief Класс MBTilesApplication
 */
class COMMON_SHARED_EXPORT        MBTilesApplication
: public                          htl::app::AppSkeleton
{
public:
  using base_class_t            = htl::app::AppSkeleton;
  using factory_t               = htl::tiles::MBTileSrvFactory;

  /**/                            MBTilesApplication( int argc, char ** argv );
  virtual                       ~ MBTilesApplication();

  void                            Start             () override;
  void                            Stop              () override;
  void                            Reload            ( int code ) override;

private:
  factory_t                       m_factory;
};

//------------------------------------------------------------------------------

}// namespace                     tiles
}// namespace                     htl

#endif // MBTILESAPPLICATION_H
