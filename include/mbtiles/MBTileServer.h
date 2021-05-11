#ifndef MBTILESERVER_H
#define MBTILESERVER_H

#include "HTTPMBTileHeader.h"
#include "MBTileLiterals.h"
#include "AsioTCPServer.h"
#include "AsioService.h"

namespace                         htl   {
namespace                         tiles {

enum class                        TileGequestType
{
  Tile,
  Geo,
};

//------------------------------------------------------------------------------
/**
 * @brief Класс MBTileServer реализует тайловый сервер Mapbox
 *
 * Создан на основе TCP-сервера
 *
 * @see htl::asio::AsioTCPServer
 */
class COMMON_SHARED_EXPORT        MBTileServer
: public                          htl::asio::AsioTCPServer
{
public:
  using base_class_t            = htl::asio::AsioTCPServer;

  /**/                            MBTileServer  () = delete;
  /**/                            MBTileServer
  (
      std::uint16_t               port,
      std::int64_t                timeoutMs = 10000
  );

  bool                            IsValid       () const noexcept override;

  void                            SetStatus     ( const HttpStatus status );
  HttpStatus                      GetStatus     () const;
  bool                            StatusOk      () const;
  std::string                     GetStatusStr  () const;
  bool                            Start         ( bool asThread ) noexcept override;

protected:
  mutable std::recursive_mutex    m_Mutex;
  /**
   * @brief Метод Tramsformation преобразует входной набор данных (запрос)
   * в правила выполнения
   * @param in поток входных данных
   * @return поток выходных данных
   */
  vectors_data_t                  Tramsformation( const vectors_data_t & in );

private:
  mutable HttpStatus              m_status      { HttpStatus::Unused };
};

//------------------------------------------------------------------------------

}// namespace                     tiles
}// namespace                     htl

#endif // MBTILESERVER_H
