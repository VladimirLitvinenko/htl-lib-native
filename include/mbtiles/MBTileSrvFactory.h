#ifndef MBTILESRVFACTORY_H
#define MBTILESRVFACTORY_H

#include "MBTileServer.h"
#include "MBTileLiterals.h"
#include "IfaceFactory.h"

namespace                         htl   {
namespace                         tiles {

using mbtile_srv_ptr            = std::shared_ptr< htl::tiles::MBTileServer >;
using mbtile_servers_t          = std::map< std::string/* server id*/, mbtile_srv_ptr >;

//------------------------------------------------------------------------------
/**
 * @brief Класс MBTileSrvFactory реализует фабрику тайловых серверов Mapbox
 */
class COMMON_SHARED_EXPORT        MBTileSrvFactory
: public                          htl::db::IfaceFactory
{
public:
  using base_class_t            = htl::db::IfaceFactory;

  /**/                            MBTileSrvFactory();
  explicit                        MBTileSrvFactory( const std::string & config_path );

  bool                            IsValid         () const;
  bool                            IsActive        () const;
  htl::strings_t                  ServersID       () const;
  void                            DumpServersID   () const;
  void                            Start           ();
  void                            Stop            ();

  /**
   * @brief Метод Instance формирует статический экземпляр класса
   * @param config_path файл конфигурации серверов
   * @return ссылка на единственный (статический) экземпляр класса
   */
  static MBTileSrvFactory       & Instance        ( const std::string & config_path = "" ) noexcept;

  void                            OnTile
  (
      const std::string         & db_name,
      MBTileInfo &                tile
  );

protected:
  void                            Prepare () noexcept;

private:
  mbtile_servers_t                m_servers;
};

//------------------------------------------------------------------------------

}// namespace                     tiles
}// namespace                     htl

#endif // MBTILESRVFACTORY_H
