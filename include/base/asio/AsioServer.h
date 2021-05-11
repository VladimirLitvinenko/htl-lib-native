#ifndef ASIOSERVER_H
#define ASIOSERVER_H

#include "base/asio/ClientServerBase.h"
#include "base/asio/AsioSocketSession.h"
#include "common/ErrLiterals.h"

#include <type_traits>

namespace                         htl   {
namespace                         asio  {

//------------------------------------------------------------------------------

template< typename                ProtocolT_ >
class COMMON_SHARED_EXPORT        AsioServer
: public                          htl::asio::ClientServerBase< ProtocolT_ >
{
  static_assert ( std::is_same< typename std::decay<ProtocolT_>::type,
                                boost::asio::ip::tcp >::value
                  or
                  std::is_same< typename std::decay<ProtocolT_>::type,
                                boost::asio::ip::udp >::value
                  or
                  std::is_same< typename std::decay<ProtocolT_>::type,
                                boost::asio::ip::icmp >::value
                  , ST_ASSERT_TEMPLATE_VALIDATOR );
public:
  using protocol_t              = typename std::decay<ProtocolT_>::type;
  using self_t                  = htl::asio::AsioServer< protocol_t >;
  using base_class_t            = htl::asio::ClientServerBase< protocol_t >;

public:
  /**/                            AsioServer
  (
      const htl::asio::port_t     port,
      const protocol_t            protocol_version,
      const std::int64_t        & serviceTimeoutMs = 10000
  );
  virtual                       ~ AsioServer();

  protocol_t                      Protocol() const;
  endpoint_t< protocol_t >        Endpoint() const;
  htl::asio::port_t               Port    () const;

private:
  endpoint_t< protocol_t >        m_Endpoint;
};

//------------------------------------------------------------------------------

template<typename ProtocolT_>
/**
 * @brief Конструктор AsioServer формирует экземпляр класса для
 *        прослушивания подключений по сети по назначенному
 *        порту и типу протокола.
 * @param ports порты сервера
 * @param protocol протокол для передачи данных.
 */
AsioServer<ProtocolT_>::AsioServer
(
    const port_t                                port,
    const AsioServer<ProtocolT_>::protocol_t    protocol_version,
    const int64_t                             & serviceTimeoutMs
)
  : base_class_t  ( serviceTimeoutMs )
  , m_Endpoint    ( htl::asio::endpoint_t< protocol_t >(
                      const_cast< protocol_t & >( protocol_version ),
                      port ) )
{
   base_class_t::ServicePtrRef()->
      SetState( Port() > 0
                ? AsioState::Ok
                : AsioState::ErrPortCount );
}

template<typename ProtocolT_>
AsioServer<ProtocolT_>::~AsioServer() = default;

template<typename ProtocolT_>
/**
 * @brief Метод Protocol возвращает тип (версию) подключения:
 * @return Значение типа (версии) подключения
 */
typename AsioServer<ProtocolT_>::protocol_t
AsioServer<ProtocolT_>::Protocol() const
{
  return m_Endpoint.protocol();
}

template<typename ProtocolT_>
/**
 * @brief Метод Endpoint возвращает экземпляр класcа точки подключения к серверу.
 * @return Значение точки подключения к серверу.
 */
endpoint_t< typename AsioServer<ProtocolT_>::protocol_t >
AsioServer<ProtocolT_>::Endpoint() const
{
  return m_Endpoint;
}

template<typename ProtocolT_>
/**
 * @brief метод Port сообщает значение порта, открытого сервером для приема подключений.
 * @return значение порта, открытого сервером для приема подключений.
 */
port_t
AsioServer<ProtocolT_>::Port() const
{
  return static_cast< htl::asio::port_t >( m_Endpoint.port() );
}

//------------------------------------------------------------------------------

}// namespace                     asio
}// namespace                     htl


#endif // ASIOSERVER_H
