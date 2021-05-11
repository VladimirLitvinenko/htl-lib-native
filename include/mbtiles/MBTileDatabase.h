#ifndef MBTILEDATABASE_H
#define MBTILEDATABASE_H

#include "MBTileInfo.h"
#include "MBTileLiterals.h"
#include "SqliteDatabase.h"

#include <memory>
#include <type_traits>
#include <experimental/optional>

namespace                         htl   {
namespace                         tiles {

//------------------------------------------------------------------------------
/**
 * @brief Класс MBTileDatabase производит чтение данных из тайловоф БД
 */
class COMMON_SHARED_EXPORT        MBTileDatabase
: public                          htl::db::SqliteDatabase
{
public:
  using base_class_t            = htl::db::SqliteDatabase;

  /**/                            MBTileDatabase() = delete;
  explicit                        MBTileDatabase( const std::string & db_name );
  /**/                          ~ MBTileDatabase() override {}

  void                            SetName       ( const std::string & db_name ) noexcept override;
  void                            ReadTile      ( MBTileInfo & tile_info ) ;
  std::experimental::optional<std::string>  ReadGeoJSON   () ;
  std::pair<int, int>             ReadZoomValues    () ;
  std::pair<int, int>             ReadMinMaxColumns () ;
  std::pair<int, int>             ReadMinMaxRows    () ;
  void                            Validation    () noexcept;

  void                            OnComplete    ( std::function<void(MBTileInfo&)>&& action )
  {
    m_on_complete = std::forward<std::function<void(MBTileInfo&)>>(action) ;
  }
private:
  std::function<void(MBTileInfo&)> m_on_complete;
};

//------------------------------------------------------------------------------

}// namespace                     tiles
}// namespace                     htl

#endif // MBTILEDATABASE_H
