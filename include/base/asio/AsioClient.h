#ifndef ASIOCLIENT_H
#define ASIOCLIENT_H

#include "AsioResolver.h"
#include "ClientServerBase.h"
#include "AsioSocketSession.h"
#include "Utils.h"
#include "ErrLiterals.h"

namespace                         htl   {
namespace                         asio  {

//------------------------------------------------------------------------------
/**
 * @brief Класс AsioClient
 */
template< typename                ProtocolT_ >
class COMMON_SHARED_EXPORT        AsioClient :
public                            htl::asio::ClientServerBase< ProtocolT_ >,
public                            htl::asio::AsioResolver< ProtocolT_ >
{
  static_assert ( std::is_same< typename std::decay<ProtocolT_>::type, boost::asio::ip::tcp >::value
                  or
                  std::is_same<  typename std::decay<ProtocolT_>::type, boost::asio::ip::udp >::value
                  or
                  std::is_same<  typename std::decay<ProtocolT_>::type, boost::asio::ip::icmp >::value,
                  ST_ASSERT_TEMPLATE_VALIDATOR );
public:
  using protocol_t              = typename std::decay<ProtocolT_>::type;
  using self_t                  = htl::asio::AsioClient< protocol_t >;
  using base_class_t            = htl::asio::ClientServerBase< protocol_t >;
  using resolver_t              = htl::asio::AsioResolver< protocol_t >;
  using session_t               = htl::asio::AsioSocketSession< protocol_t >;
  using session_shr_t           = htl::asio::SocketSessionShared<ProtocolT_>;

public:
  /**
   * @brief AsioClient
   * @param type
   * @param host
   * @param remoute_service
   */
  /**/                            AsioClient
  (
      const std::string         & host,
      const std::string         & remoute_service,
      bool                        keepAlive         = false,
      bool                        try_reconnect     = false,
      std::int64_t                serviceTimeoutMs  = 1000 //< msec ( 1 sec )
  );
  virtual                         ~ AsioClient(){}

  bool                            InSession             () const;
  bool                            IsKeepAlive           () const;
  void                            SetActive             ( const bool value );
  void                            SetKeepAlive          ( const bool value );
//  bool                            TryConnect            ();
  virtual void                    Request               ( const vectors_data_t  & rq );
  virtual void                    Request               ( const std::string     & rq );
  virtual void                    Request               ( const htl::strings_t  & rq );
  void                            ClearRequest          ();
  bool                            Start                 ( bool asThread ) noexcept override;
  bool                            Stop                  () noexcept override;

protected:
    void                          ProbeSession          ();
    void                          TransferData          ();

private:
    std::atomic_bool              m_KeepAlive     {false};
    std::atomic_bool              m_reconnect     {false};
    vectors_data_t                m_request_data;
    session_shr_t                 m_session_ptr;
};

//------------------------------------------------------------------------------

template<typename ProtocolT_>
AsioClient<ProtocolT_>::AsioClient(const std::string &host,
                                   const std::string &remoute_service,
                                   bool keepAlive,
                                   bool try_reconnect,
                                   int64_t serviceTimeoutMs )
  : AsioClient<ProtocolT_>::base_class_t( serviceTimeoutMs )
  , AsioClient<ProtocolT_>::resolver_t  ( *base_class_t::ServicePtrRef().get(),
                                          host,
                                          remoute_service,
                                          serviceTimeoutMs )
    , m_KeepAlive                         { keepAlive }
    , m_reconnect                         { try_reconnect }
{
//  SetBeforeServiceStart();
  base_class_t::ServicePtrRef()->SetState(
    (not host.empty()) and (not remoute_service.empty()) ? AsioState::Ok : AsioState::ErrPortCount);
}

template<typename ProtocolT_>
bool AsioClient<ProtocolT_>::InSession() const
{
  return m_session_ptr.operator bool();
}

template<typename ProtocolT_>
bool AsioClient<ProtocolT_>::IsKeepAlive() const
{
  return m_KeepAlive.load();
}

template<typename ProtocolT_>
void
AsioClient<ProtocolT_>::SetActive( const bool value)
{
  if( ( not value ) and InSession() )
  {
    m_session_ptr->Stop();
    m_session_ptr.reset();
  }
}

template<typename ProtocolT_>
void AsioClient<ProtocolT_>::SetKeepAlive(const bool value)
{
  m_KeepAlive.store(value);
}



template<typename ProtocolT_>
void AsioClient<ProtocolT_>::Request(const vectors_data_t &rq)
{
  m_request_data.assign(rq.begin(), rq.end());
}

template<typename ProtocolT_>
void AsioClient<ProtocolT_>::Request(const std::string &rq)
{
  vectors_data_t::value_type data;
  data.assign( rq.begin(), rq.end() );
  Request( vectors_data_t(1, data ) );
}

template<typename ProtocolT_>
void AsioClient<ProtocolT_>::Request(const htl::strings_t &rq)
{
  vectors_data_t data;
  data.reserve( rq.size() );

  for( auto & str : rq )
  {
    vectors_data_t::value_type val;
    val.assign( str.begin(), str.end() );
    data.push_back( std::move( val ) );
  }
  Request( data );
}

template<typename ProtocolT_>
void
AsioClient<ProtocolT_>::ClearRequest()
{
  m_request_data.clear();
}

template<typename ProtocolT_>
bool
AsioClient<ProtocolT_>::Start( bool asThread )
noexcept
{
  bool retval{ false };
  do
  {
    if( not AsioClient<ProtocolT_>::base_class_t::IsActive() )
      retval = AsioClient<ProtocolT_>::base_class_t::Start(asThread);
    if( retval )
    {
      bool is_active{ InSession() };
      ProbeSession();
      if( InSession() and ( not is_active ) )
      {
          std::make_shared<htl::threadptr_t::element_type>(
          boost::bind( &AsioClient<ProtocolT_>::TransferData, this ) )->detach();
      }
    }
  }
  while( m_reconnect );
  return retval;
}

template<typename ProtocolT_>
bool
AsioClient<ProtocolT_>::Stop ()
noexcept
{
  bool reconnect = m_reconnect;
  m_reconnect = false;
  self_t::SetActive( false );
  bool retval = AsioClient<ProtocolT_>::base_class_t::Stop();
  m_reconnect = reconnect;
  return retval;
}

template<typename ProtocolT_>
void
AsioClient<ProtocolT_>::ProbeSession()
{
  if( /*( not InSession() ) and */resolver_t::IsValid( true ) )
  {
    for (auto & ep : resolver_t::Endpoints())
    {
      m_session_ptr =
          htl::asio::MakeSocketSession<ProtocolT_>(
            base_class_t::ServicePtrRef()->ServiceRef(),
            IsKeepAlive(),
            base_class_t::SocketDeadline() );
      boost::system::error_code err_code;
      m_session_ptr->SocketRef().async_connect(
        ep,
        [ this, &err_code ]( boost::system::error_code ec )
        {
          err_code = ec;
        } );
      std::this_thread::yield();
      std::this_thread::sleep_for( std::chrono::milliseconds(1000));

      this->SetActive( IsNoErr( err_code ) );
      if( this->InSession() )
      {
        return;
      }
    }
  }
//  SetActive( false );
}

template<typename ProtocolT_>
/**
 * @brief Метод TransferData реализует функционал подключения к серверу.
 * @param type тип (режим) работы сервера
 * @param yield контекст передачи управления очередной сопрограмме
 */
void
AsioClient<ProtocolT_>::TransferData()
{
  try
  {
    while( this->IsKeepAlive() and this->InSession() )
    {
      set_socket_options<typename ProtocolT_::socket>()(
        m_session_ptr->SocketRef(),
        m_session_ptr->IsKeepAlive());
      m_session_ptr->Send(m_request_data );
      std::__invoke(base_class_t::Transformation(), m_session_ptr->Receive());
    }
  }
  catch( std::exception & e )
  {
    DUMP_EXCEPTION( e );
  }
  SetActive(false);
}

//------------------------------------------------------------------------------

}// namespace                     asio
}// namespace                     htl

#endif // ASIOCLIENT_H
