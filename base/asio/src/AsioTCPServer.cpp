#include <AsioTCPServer.h>

namespace                         htl   {
namespace                         asio  {

/**
 * @brief Конструктор AsioTCPServer формирует экземпляр класса для
 *        прослушивания TCP-подключений по сети к серверу по назначенному
 *        порту.
 * @param type тип (режим) работы сервера. Тип @a htl::asio::TransferType;
 * @param port порт TCP-сервера;
 * @param serviceTimeoutMs время ожидания активации сервиса типа @a boost::asio::io_service.
 */
AsioTCPServer::AsioTCPServer( const port_t port, const int64_t& serviceTimeoutMs )
  : self_t::base_class_t    ( port, self_t::tcp_t::v4(), serviceTimeoutMs )
  , m_acceptor              ( std::ref( * this ) )
{
}

AsioTCPServer::~AsioTCPServer() = default;

bool
AsioTCPServer::IsOpen()
const noexcept
{
  return m_acceptor.IsOpen();
}

bool
AsioTCPServer::Start(bool asThread) noexcept
{
  bool retval( m_acceptor.Start() )  ;
  if( retval )
    retval = AsioTCPServer::base_class_t::Start( asThread );
  return retval;
}

bool
AsioTCPServer::Stop() noexcept
{
  m_acceptor.Stop();
  std::this_thread::sleep_for( std::chrono::milliseconds(100));
  return AsioTCPServer::base_class_t::Stop();
}

}// namespace                     asio
}// namespace                     htl
