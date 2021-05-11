#ifndef CLIENTSERVERBASE_H
#define CLIENTSERVERBASE_H

#include "base/asio/AsioService.h"
#include "base/asio/AsioSocketSession.h"
#include "base/asio/AsioSignals.h"
#include "common/ErrLiterals.h"

#include <mutex>

namespace                         htl   {
namespace                         asio  {

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
template< typename                ProtocolT_ >
/**
 * @brief The ClientServerBase class
 */
class COMMON_SHARED_EXPORT        ClientServerBase
//: public                          htl::asio::AsioSignals
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
  using self_t                  = htl::asio::ClientServerBase< ProtocolT_ >;
//  using base_class_t            = htl::asio::AsioSignals;
  using service_t               = htl::asio::AsioService;
  using service_shr_t           = htl::asio::AsioServiceShared;
  using protocol_t              = ProtocolT_;

  explicit                        ClientServerBase  ( const std::int64_t & serviceTimeoutMs = 10000 );
  virtual                       ~ ClientServerBase  () { Stop(); }

  void                            SetTimeoutMs      ( const std::int64_t & timeoutMs );
  std::int64_t                    TimeoutMs         () const;

  void                            SetSocketDeadline ( const boost::int64_t & socketDeadline );
  boost::int64_t                  SocketDeadline    ();

  virtual bool                    IsValid           () const;
  virtual bool                    IsActive          () const;

  void                            SetTransformation ( const buffer_transformation_t & transformation ) noexcept;
  buffer_transformation_t         Transformation    () const noexcept;
  buffer_transformation_t       & TransformationRef () noexcept;

  virtual bool                    Start             ( bool asThread ) noexcept;
  virtual bool                    Stop              () noexcept;

  service_shr_t                 & ServicePtrRef     ();

private:
  /**
   * @brief Атрибут m_SocketDeadline содержит значение задержки ожидания
   *        приема/передачи данных сокетом.
   */
  std::atomic< std::int64_t >     m_SocketDeadline    { 10000 };
  service_shr_t                   m_service_ptr;
  buffer_transformation_t         m_transformation;
};

//------------------------------------------------------------------------------

template<typename ProtocolT_>
/**
 * @brief Конструктор ClientServerBase
 * @param type
 * @param serviceTimeoutMs
 */
ClientServerBase<ProtocolT_>::ClientServerBase(const int64_t& serviceTimeoutMs)
//  : ClientServerBase<ProtocolT_>::base_class_t( htl::asio::AsioService::Instance( serviceTimeoutMs ) )
  : m_service_ptr{ std::make_shared< service_shr_t::element_type>(serviceTimeoutMs)}
{
  assert( m_service_ptr );
}

template<typename ProtocolT_>
/**
 * @brief Метод SetTimeoutMs назначает величину ожидания попытки привязки
 *        приемника к сокету или ожидание подключения сокета клиента к серверу.
 * @param timeoutMs величина типа @a std::int64_t ожидания попытки привязки приемника к сокету.
 */
void
ClientServerBase<ProtocolT_>::SetTimeoutMs(const int64_t& timeoutMs)
{
  m_service_ptr->SetCloseTimeout( timeoutMs > -1 ? timeoutMs : 0 );
}

template<typename ProtocolT_>
/**
 * @brief Метод TimeoutMs возвращает время ожидания подключения в миллисекундах
 * @return значение времени ожидания подключения в миллисекундах.
 */
int64_t
ClientServerBase<ProtocolT_>::TimeoutMs()
const
{
  return m_service_ptr->CloseTimeout();
}

template<typename ProtocolT_>
/**
 * @brief Метод SocketDeadline вщзвращает значение времени ожидания активации
 *        аксептора при запуске сервера.
 *
 * Если по истечении данного времени ( в миллисекундах) акцептор не готов к
 * работе, сервер выполняет попытку поереполключения или завершает работу.
 *
 * @return значение времени ожидания активации аксептора при запуске сервера.
 */
int64_t
ClientServerBase<ProtocolT_>::SocketDeadline()
{
  return m_SocketDeadline;
}

template<typename ProtocolT_>
/**
 * @brief Метод SetSocketDeadline устанавливает значение времени ожидания активации
 *        аксептора при запуске сервера.
 *
 * Если по истечении данного времени ( в миллисекундах) акцептор не готов к
 * работе, сервер выполняет попытку поереполключения или завершает работу.
 *
 * @param socketDeadline значение времени ожидания активации
 *        аксептора при запуске сервера
 */
void
ClientServerBase<ProtocolT_>::SetSocketDeadline(const int64_t& socketDeadline)
{
  m_SocketDeadline.store(
        socketDeadline > 0 ?
          socketDeadline :
          0 );
}

template<typename ProtocolT_>
/**
 * @brief Метод IsValid готовность к приему подключения
 * @return Булево значение:
 * @value true  сервер готов к обслуживанию клиентов;
 * @value false сервер не готов к обслуживанию клиентов.
 */
bool
ClientServerBase<ProtocolT_>::IsValid()
const
{
  return m_service_ptr and m_service_ptr->IsStateValid();
}

template<typename ProtocolT_>
/**
 * @brief Метод IsActive сообщает об активном состоянии сервиса
 *        типа @a boost::asio::io_service обработки действий над сокетами
 * (объектами типа @a boost::asio::ip::tcp::socket).
 * @return Булево значение:
 * @value true сообщает об активном состоянии сервиса;
 * @value false сообщает об НЕактивном состоянии сервиса.
 */
bool
ClientServerBase<ProtocolT_>::IsActive() const
{
  return m_service_ptr and m_service_ptr->IsActive();
}

template<typename ProtocolT_>
void
ClientServerBase<ProtocolT_>::SetTransformation( const buffer_transformation_t & transformation )
  noexcept
{
  m_transformation = static_cast< buffer_transformation_t >( transformation );
}

template<typename ProtocolT_>
buffer_transformation_t
ClientServerBase< ProtocolT_ >::Transformation()
  const noexcept
{
  return m_transformation;
}

template<typename ProtocolT_>
buffer_transformation_t &
ClientServerBase< ProtocolT_ >::TransformationRef()
  noexcept
{
  return std::ref( m_transformation );
}

template<typename ProtocolT_>
bool
ClientServerBase<ProtocolT_>::Start(bool asThread)
noexcept
{
  if( IsValid() )
    m_service_ptr->Start( asThread );
  return IsValid();//IsActive();
}

template<typename ProtocolT_>
bool
ClientServerBase<ProtocolT_>::Stop()
noexcept
{
  if( m_service_ptr.operator bool() )
    m_service_ptr->Stop();
  return not IsActive();
}

template<typename ProtocolT_>
typename ClientServerBase<ProtocolT_>::service_shr_t &
ClientServerBase<ProtocolT_>::ServicePtrRef()
{
  return std::ref( m_service_ptr );
}

//------------------------------------------------------------------------------

}// namespace                     asio
}// namespace                     htl


#endif // CLIENTSERVERBASE_H
