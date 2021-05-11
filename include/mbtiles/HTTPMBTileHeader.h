#ifndef HTTPMBTILEHEADER_H
#define HTTPMBTILEHEADER_H

#include "HTTPHeader.h"
#include "MBTileInfo.h"

#include <functional>

namespace                         htl   {
namespace                         tiles {

//------------------------------------------------------------------------------
using db_function_t             = std::function< void( const std::string &/*db name */ ) >;
using mbtile_function_t         = std::function< void( const MBTileInfo & ) >;

//------------------------------------------------------------------------------
/**
 * @brief Класс HTTPMBTileHeader реализует работу с HTTP заголовками тайлов
 */
class COMMON_SHARED_EXPORT        HTTPMBTileHeader
: public                          htl::http::HTTPHeader
{
public:
  using base_class_t            = htl::http::HTTPHeader;

  /**/                            HTTPMBTileHeader();
  explicit                        HTTPMBTileHeader( const std::string & req );

  void                            SetRequest    ( const std::string & req ) override;
  MBTileInfo                      GetTileInfo   () const;
  bool                            IsGZip        ();
  bool                            IsDefalte     ();

protected:
  virtual void                    PrepareTile () noexcept;
  MBTileInfo                    & TileInfoRef ();

private:
  MBTileInfo                      m_tile_info;
};

//------------------------------------------------------------------------------

}// namespace                     tiles
}// namespace                     htl

#endif // HTTPMBTILEHEADER_H
