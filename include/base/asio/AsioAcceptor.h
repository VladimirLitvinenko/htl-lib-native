/**
 * @file   AsioAcceptor.h
 * @brief  Файл содержит описание и реализацию шаблонного класса AsioAcceptor
 *         обслуживания подключений к серверу по IP-протоколу для обмена данных
 *         в сети.
 *
 * @author Vladimir N. Litvinenko
 *
 */

#ifndef ASIO_ACCEPTOR_H
#define ASIO_ACCEPTOR_H

#include <memory>
#include <type_traits>

#include "base/asio/AsioServer.h"
#include "base/asio/AsioSocketSession.h"
#include "base/asio/AsioCommon.h"

namespace                         htl   {
namespace                         asio  {

//------------------------------------------------------------------------------

/**
 * @brief Класс AsioAcceptor определяет атрибуты и методы работы по обслуживанию
 *        подключения клиентов к конечной точке.
 *
 * Параметры шаблона:
 * @value ProtocolT_ тип протокола обмена информации по IP-адресу сервера.
 * @value ByteT_ тип единицы информации для каналов обмена данными.
 *
 * Методы класса используют технологию Boost.Coroutine реализации алгоритмов на
 * основе сопрограмм.
 *
 */
class COMMON_SHARED_EXPORT        AsioAcceptor
: public                          std::enable_shared_from_this< htl::asio::AsioAcceptor >
{
public:
  using self_t                  = htl::asio::AsioAcceptor;
  using service_t               = htl::asio::AsioService;
  using shared_t                = std::enable_shared_from_this< htl::asio::AsioAcceptor >;
  using session_t               = htl::asio::AsioSocketSession< htl::asio::tcp_t >;
  using session_ptr             = htl::asio::SocketSessionShared< htl::asio::tcp_t >;
  using server_t                = htl::asio::AsioServer< htl::asio::tcp_t >;

  /**/                            AsioAcceptor( htl::asio::AsioServer< htl::asio::tcp_t > & serverRef );
  virtual                       ~ AsioAcceptor();

  tcp_t::acceptor::executor_type  ServiceRef    ();
  bool                            TryOpen ();
  bool                            IsOpen        () const noexcept;
  bool                            IsActive      () const noexcept;

  bool                            Start         ();
  void                            Stop          ( short tryStopCount = 10 );
  void                            Restart       ()
  {
    Stop();
    Start();
  }

protected:
  virtual void                    SetOptions    ( bool keep_alive = false );
  void                            DoAccept      ();
//  void                            DoAcceptSpawn ();

private:
  server_t                      & m_server_ref;
  typename tcp_t::acceptor        m_acceptor;

  void                            TryClose      ();
};

}// namespace                     asio
}// namespace                     htl

#endif // ASIO_ACCEPTOR_H
