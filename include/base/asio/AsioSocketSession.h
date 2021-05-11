#ifndef ASIOSOCKETSESSION_H
#define ASIOSOCKETSESSION_H

#include "AsioService.h"
#include "AsioError.h"
#include "SteadyTimer.h"
#include "Logger.h"
#include "ErrLiterals.h"

#include <memory>
#include <functional>
#include <type_traits>

#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/lexical_cast.hpp>

namespace                         htl   {
namespace                         asio  {

//------------------------------------------------------------------------------
template< typename SocketT_ >
/**
 * @brief The set_socket_options struct
 */
struct set_socket_options
{
  static_assert ( std::is_same< typename std::decay<SocketT_>::type, boost::asio::ip::tcp::socket >::value
                  or
                  std::is_same< typename std::decay<SocketT_>::type, boost::asio::ip::udp::socket >::value
                  , ST_ASSERT_TEMPLATE_VALIDATOR );
    bool operator()( SocketT_ &, bool = false )
    {
      assert (false);
      return false;
    }
};

template<>
/**
 * @brief The set_socket_options<boost::asio::ip::tcp::socket> struct
 */
struct set_socket_options< boost::asio::ip::tcp::socket >
{
  bool operator()( boost::asio::ip::tcp::socket & socket, bool keepAlive = true )
  {
//    boost::asio::ip::tcp::socket::non_blocking_io no_blk  ( true );
////    boost::asio::socket_base::bytes_readable      readable( true );

      boost::system::error_code ec;
      socket.non_blocking( true, ec );

    bool retval( socket.is_open() );
    if( retval )
    {
      boost::system::error_code ec;
//      socket.io_control( no_blk, ec );
      if( IsNoErr( ec ) )
      {
          socket.set_option( std::move( boost::asio::ip::tcp::no_delay( true )                      ) );
          socket.set_option( std::move( boost::asio::socket_base::keep_alive( keepAlive )           ) );
          socket.set_option( std::move( boost::asio::socket_base::enable_connection_aborted( true ) ) ) ;
          socket.set_option( std::move( boost::asio::socket_base::reuse_address(true)               ) ) ;
      }
    }
    return retval;
  }
};

template<>
struct set_socket_options< boost::asio::ip::udp::socket >
{
    bool operator()( boost::asio::ip::udp::socket & socket, bool = false )
    {
      boost::system::error_code ec;
      socket.non_blocking( true, ec );
      bool retval( IsNoErr(ec) and socket.is_open() );
      if( retval )
      {
//        socket.io_control( no_blk );

        socket.set_option( std::move( boost::asio::ip::udp::socket::broadcast( true ) ) );
        socket.set_option( std::move( boost::asio::socket_base::reuse_address(true)   ) ) ;
        socket.set_option( std::move( boost::asio::socket_base::keep_alive(true)      ) ) ;
      }
      return retval;
    }
};

//------------------------------------------------------------------------------
/**
 * @brief Шаблонная структура async_reader определяет специализацию
 *        опрератора operator() для получения данных из сокета определенного
 *        типа протокола.
 * @param Prot_ тип протокола
 */
template
<
    typename SocketSession,
    typename Prot_
>
struct async_reader
{
  static_assert ( std::is_same< typename std::decay<Prot_>::type, boost::asio::ip::tcp >::value
                  or
                  std::is_same< typename std::decay<Prot_>::type, boost::asio::ip::udp >::value
            //      or
            //      std::is_same< typename std::decay<Prot_>::type, boost::asio::ip::icmp >::value
                  ,
                  ST_ASSERT_TEMPLATE_VALIDATOR );
//    virtual std::size_t operator()
//    (
//        SocketSession               &,
//        vectors_data_t::value_type  &,
//        boost::asio::yield_context
//    ) const
//    {
//      assert( false );
//      return 0;
//    }
    virtual void operator()
    (
        SocketSession               &,
        vectors_data_t::value_type  &,
        boost::system::error_code   &,
        std::function< void( boost::system::error_code, std::size_t ) >
    ) const
    {
      assert( false );
    }
  virtual void operator()
  (
      SocketSession               &,
      std::vector< char >         &
  ) const
  {
    assert( false );
  }
};

/**
 * @brief Шаблонная структура async_reader определяет реализацию
 *        опрератора operator() для получения данных из сокета TCP-протокола.
 */
template < typename SocketSession >
struct async_reader< SocketSession, boost::asio::ip::tcp >
{
//  virtual std::size_t operator()
//  (
//      SocketSession               & session,
//      vectors_data_t::value_type  & bufs,
//      boost::asio::yield_context    yield
//  ) const
//  {
//    return
//        session.IsOpen() and (not bufs.empty() )
//        ? boost::asio::async_read( session.SocketRef(),
//                                   boost::asio::buffer(bufs),
//                                   yield )
//        : 0;
//  }

  virtual void operator()
  (
      SocketSession               & session,
      vectors_data_t::value_type  & bufs,
      boost::system::error_code   & ec,
      std::function< void( boost::system::error_code &, std::size_t ) > fnc
  ) const
  {
    if( session.IsOpen() and (not bufs.empty() ) )
    {
      session.SocketRef().async_read_some(
                               boost::asio::buffer(bufs),
                               boost::bind( fnc, ec, boost::placeholders::_2 ) );
    }
  }

  virtual void operator()
  (
      SocketSession               & session,
      vectors_data_t::value_type  & bufs
  ) const
  {
    if( session.IsOpen() and (not bufs.empty() ) )
    {
      session.SocketRef().async_read_some( boost::asio::buffer(bufs),
                                           [&bufs]( boost::system::error_code ec, std::size_t )
      {
        if( not IsNoErr(ec) )
          bufs.clear();
      } );
    }
  }
};

/**
 * @brief Шаблонная структура async_reader определяет реализацию
 *        опрератора operator() для получения данных из сокета UDP-протокола.
 */
template < typename SocketSession >
struct async_reader< SocketSession, boost::asio::ip::udp >
{
//  virtual std::size_t operator()
//  (
//      SocketSession               & session,
//      vectors_data_t::value_type  & bufs,
//      boost::asio::yield_context    yield
//  ) const
//  {
//    return
//        session.SocketRef().is_open() and ( not bufs.empty() )
//        ? session.SocketRef().async_receive_from(  boost::asio::buffer(bufs), session.EndpointRef(), yield )
//        : 0;
//  }
  virtual void operator()
  (
      SocketSession               & session,
      vectors_data_t::value_type  & bufs,
      boost::system::error_code   & ec,
      const std::function< void( boost::system::error_code & , std::size_t ) > & fnc
  ) const
  {
    if( session.SocketRef().is_open() and (not bufs.empty() ) )
    {
      session.SocketRef().async_receive_from(  boost::asio::buffer(bufs),
                                               session.EndpointRef(),
                                               boost::bind( fnc, ec, boost::placeholders::_2 ) );
    }
  }
  virtual void operator()
  (
      SocketSession               & session,
      vectors_data_t::value_type  & bufs
  ) const
  {
    if( session.SocketRef().is_open() and (not bufs.empty() ) )
    {
      session.SocketRef().async_receive_from(  boost::asio::buffer(bufs),
                                               session.EndpointRef(),
                                               [&bufs]( boost::system::error_code ec, std::size_t )
      {
        if( not IsNoErr(ec) )
          bufs.clear();
      } );
    }
  }
};

//------------------------------------------------------------------------------
/**
 * @brief Шаблонная структура async_writer определяет специализацию
 *        опрератора operator() для отправки данных в сокет определенного
 *        типа протокола.
 * @param Prot_ тип протокола
 */
template
<
    typename SocketSession,
    typename Prot_
>
struct async_writer
{
  static_assert ( std::is_same< typename std::decay<Prot_>::type, boost::asio::ip::tcp >::value
                  or
                  std::is_same<  typename std::decay<Prot_>::type, boost::asio::ip::udp >::value
                  ,
                  ST_ASSERT_TEMPLATE_VALIDATOR );
//    virtual std::size_t operator()
//    (
//      SocketSession               &,
//      const vectors_data_t        &,
//      boost::system::error_code   &,
//      boost::asio::yield_context
//    )
//    {
//      assert( false );
//      return 0;
//    }
    virtual void operator()
    (
      SocketSession               &,
      const vectors_data_t        &,
      std::function< void( boost::system::error_code, std::size_t ) >
    )
    {
      assert( false );
    }
  virtual void operator()
  (
    SocketSession               &,
    const vectors_data_t        &
  )
  {
    assert( false );
  }
};

/**
 * @brief Шаблонная структура async_writer определяет реализацию
 *        опрератора operator() для отправки данных в сокет TCP-протокола.
 */
template < typename SocketSession >
struct async_writer< SocketSession, boost::asio::ip::tcp >
{
//  virtual std::size_t  operator()
//  (
//      SocketSession               & session,
//      const vectors_data_t        & bufs,
//      boost::system::error_code   & ec,
//      boost::asio::yield_context    yield
//  )
//  {
//    return
//        0 < bufs.size()
//        ? 0
//        : session.IsOpen() and  session.SocketRef().is_open() and ( not bufs.empty() )
//          ? boost::asio::async_write( session.SocketRef(),
//                                      boost::asio::buffer(bufs),
//                                      yield[ ec ] )
//          : 0;
//  }
  virtual void  operator()
  (
      SocketSession               & session,
      const vectors_data_t        & bufs,
      std::function< void( boost::system::error_code, std::size_t ) > fnc
  )
  {
    if( session.IsOpen() and ( not bufs.empty() ) )
    {
        std::vector< boost::asio::const_buffer > asio_buffer;
        asio_buffer.reserve( bufs.size() );
        for( auto & val : bufs )
        {
          asio_buffer.push_back( boost::asio::buffer(val) );
        }
        session.SocketRef().async_write_some(
                                boost::asio::buffer(bufs),
                                boost::bind( fnc, boost::placeholders::_1, boost::placeholders::_2 ) );
    }
  }
  virtual void  operator()
  (
      SocketSession               & session,
      const vectors_data_t        & bufs
  )
  {
      if( session.IsOpen() and  session.SocketRef().is_open() and ( not bufs.empty() ) )
      {
        std::vector< boost::asio::const_buffer > asio_buffer;
        asio_buffer.reserve( bufs.size() );
        for( auto & val : bufs )
        {
          asio_buffer.push_back( boost::asio::buffer(val) );
        }

        session.SocketRef().async_write_some(
                                  asio_buffer,
                                  [ &bufs ]( boost::system::error_code, std::size_t )
                                  {} );
      }
  }
};

/**
 * @brief Шаблонная структура async_writer определяет реализацию
 *        опрератора operator() для отправки данных в сокет UDP-протокола.
 */
template < typename SocketSession >
struct async_writer< SocketSession, boost::asio::ip::udp >
{
//  virtual std::size_t  operator()
//  (
//      SocketSession               & session,
//      const vectors_data_t        & bufs,
//      boost::system::error_code   & ec,
//      boost::asio::yield_context    yield
//  )
//  {
//    return
//        0 < bufs.size()
//        ? 0
//        : session.IsOpen() and  session.SocketRef().is_open() and ( not bufs.empty() )
//          ? session.SocketRef().async_send_to( boost::asio::buffer(bufs), session.EndpointRef(), yield[ ec ] )
//          : 0;
//  }
  virtual void  operator()
  (
      SocketSession               & session,
      const vectors_data_t        & bufs,
      const std::function< void( boost::system::error_code, std::size_t ) > & fnc
  )
  {
    if( 0 < bufs.size() )
      if( session.IsOpen() and  session.SocketRef().is_open() and ( not bufs.empty() ) )
          session.SocketRef().async_send_to( boost::asio::buffer(bufs),
                                             session.EndpointRef(),
                                             boost::bind( fnc, boost::placeholders::_1, boost::placeholders::_2 ) );
  }
  virtual void  operator()
  (
      SocketSession               & session,
      const vectors_data_t        & bufs
  )
  {
    if( 0 < bufs.size() )
      if( session.IsOpen() and  session.SocketRef().is_open() and ( not bufs.empty() ) )
          session.SocketRef().async_send_to( boost::asio::buffer(bufs),
                                             session.EndpointRef(),
                                             []( boost::system::error_code, std::size_t ){} );
  }
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 * @brief Шаблонный класс AsioSocketSession определяет атрибуты и методы работы
 *        с IP-сокетом передачи сетевых данных.
 *
 * Параметры шаблона:
 * @value ProtocolT_ тип протокола для обмена данными по сети.
 * @value ByteT_ тип единицы информации для каналов обмена данными.
 *
 * Объект сессии запускается как std::shared_ptr для корректного завершения работы
 * с сокетом при отключении от нее ведущих обьектов приема / передачи данных таких
 * как @a boost::asio::ip::tcp::acceptor и @a boost::asio::connect
 *
 * @par Пример использования:
 * @code language="cpp"
 * . . .
 *  m_Acceptor.async_accept( socket, yield[ ec ] );
 *
 *  if( IsNoErr( ec ) )
 *  {
 *    auto session_ptr( std::make_shared< AsioSocketSession<boost::asio::ip::tcp, char> >(
 *                        TransferType::SimplexIn,
 *                        std::move( socket ),
 *                        1000 ) );
 *    if( session_ptr )
 *    {
 *      . . .
 *      session_ptr->Start();
 *    }
 *    . . .
 *  }
 *
 * @endcode
 */
template< typename                ProtocolT_ >
class COMMON_SHARED_EXPORT        AsioSocketSession :
public                            std::enable_shared_from_this< htl::asio::AsioSocketSession< ProtocolT_ > >
{
  static_assert ( std::is_same< typename std::decay<ProtocolT_>::type, boost::asio::ip::tcp >::value
                  or
                  std::is_same<  typename std::decay<ProtocolT_>::type, boost::asio::ip::udp >::value
//                  or
//                  std::is_same<  typename std::decay<ProtocolT_>::type, boost::asio::ip::icmp >::value
                  , ST_ASSERT_TEMPLATE_VALIDATOR );
public:
  using self_t                  = htl::asio::AsioSocketSession< ProtocolT_ >;
  using protocol_t              = typename std::decay<ProtocolT_>::type;
  using socket_t                = typename protocol_t::socket;
  using socket_ptr              = std::shared_ptr< socket_t >;
  using shared_t                = std::enable_shared_from_this< self_t >;
  using timer_ptr               = std::shared_ptr< SteadyTimer >;

private:
  socket_t                        m_socket;
  bool                            m_keep_alive    = false;
  timer_ptr                       m_timer_ptr;

public:
  /* конструктор */               AsioSocketSession
  (
      io_service_t              & service,
      bool                        keep_alive,
      boost::int64_t              deadLine  = boost::int64_t( ASIO_DEADLINE_DEFAULT * 10 )
  );
  /* конструктор */               AsioSocketSession( AsioSocketSession && s );
  virtual                       ~ AsioSocketSession();

  AsioSocketSession             & operator =       ( AsioSocketSession && s );

  void                            StopTimer   ();
  void                            StartTimer   ();
  timer_ptr                     & TimerRef      ();
  bool                            IsTimerActive () const;

  bool                            IsKeepAlive () const;
  bool                            IsOpen ();

  io_service_t                  & ServiceRef ();
  socket_t                      & SocketRef ();

  void                            Stop    ();

//  virtual vectors_data_t          Receive ( boost::asio::yield_context yield );
  virtual vectors_data_t          Receive ();
  virtual void                    Send( const vectors_data_t & buffer_out );
//  virtual void                    Send( const vectors_data_t & buffer_out, boost::asio::yield_context yield );

};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

template< typename                ProtocolT_ >
using SocketSessionShared       = std::shared_ptr< htl::asio::AsioSocketSession< ProtocolT_ > >;

template< typename ProtocolT_, typename ... Args >
SocketSessionShared< ProtocolT_ > MakeSocketSession( io_service_t & service, Args ... params )
{
  static_assert ( std::is_same< typename std::decay<ProtocolT_>::type, boost::asio::ip::tcp >::value
                  or
                  std::is_same<  typename std::decay<ProtocolT_>::type, boost::asio::ip::udp >::value
                  or
                  std::is_same<  typename std::decay<ProtocolT_>::type, boost::asio::ip::icmp >::value,
                  ST_ASSERT_TEMPLATE_VALIDATOR );
  SocketSessionShared< ProtocolT_ > retval;
  retval.reset( new AsioSocketSession< ProtocolT_ >( service, std::forward< Args >( params ) ... ) );
  return retval;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

template<typename ProtocolT_>
/**
 * @brief Конструктор AsioSocketSession принимает ссылку на сервис boost::asio::io_service
 *        и определяет режимы и время ожидания ответа приема и передачи данных.
 * @param type тип передачи данных
 * @param socket r-value ссылка на сокет
 * @param deadLine время ожидания завершкения приема/передачи данныхчерез сетевой
 *        сокет @a m_Socket;
 *
 * Конструктор закрепляет за сессией владение сокетом ( @a std::move )
 *
 * @see htl::asio::TransferType
 */
AsioSocketSession<ProtocolT_>::AsioSocketSession
(
    io_service_t  & service,
    bool            keep_alive,
    int64_t         deadLine
)
  : m_socket( service )
  , m_keep_alive{ keep_alive }
{
  m_timer_ptr = std::make_shared< AsioSocketSession<ProtocolT_>::timer_ptr::element_type>(
                  service,
                  deadLine );
  assert( m_timer_ptr );
}

template<typename ProtocolT_>
AsioSocketSession<ProtocolT_>::AsioSocketSession( AsioSocketSession&& s)
{
  *this = std::move( s );
}

template<typename ProtocolT_>
AsioSocketSession<ProtocolT_>::~AsioSocketSession()
{
//  Stop();
}

template<typename ProtocolT_>
AsioSocketSession<ProtocolT_> &
AsioSocketSession<ProtocolT_>::operator = ( AsioSocketSession && s )
{
  m_keep_alive  = s.IsKeepAlive();
  m_socket      = std::move( s.m_socket );
  m_timer_ptr   . swap( s.m_timer_ptr );

  return std::ref( * this );
}

template<typename ProtocolT_>
/**
 * @brief Метод StopTimer останавливает таймер @a m_TimerPtr задержки приема
 *        или передачи данных через сокет.
 */
void
AsioSocketSession<ProtocolT_>::StopTimer()
{
  if( m_timer_ptr )
    m_timer_ptr->Stop();
}

template<typename ProtocolT_>
/**
 * @brief Метод StartTimer pfgrcrftn таймер @a m_TimerPtr задержки приема
 *        или передачи данных через сокет.
 * По истечению времени задержки прием или передача прекращаются.
 */
void
AsioSocketSession<ProtocolT_>::StartTimer()
{
  if( m_timer_ptr )
    m_timer_ptr->Start();
}

template<typename ProtocolT_>
/**
 * @brief Метод TimerRef возвращает ссылку на общий указатель, содержащий
 *        переменную со значением времени ожидания приема или передачи данных.
 * @return ссылку на общий указатель.
 */
typename AsioSocketSession<ProtocolT_>::timer_ptr &
AsioSocketSession<ProtocolT_>::TimerRef()
{
  return std::ref( m_timer_ptr );
}

template<typename ProtocolT_>
/**
 * @brief Метод IsTimerActive возвращает пнризнак раброты таймера (запущен
 * ли таймер ожидания приема или передпачи информации).
 * @return Признак:
 * @value true  таймер запущен;
 * @value false таймер остановлен (не запущен)
 */
bool
AsioSocketSession<ProtocolT_>::IsTimerActive() const
{
  return
      m_timer_ptr
      ? m_timer_ptr->IsActive()
      : false;
}

template<typename ProtocolT_>
bool
AsioSocketSession<ProtocolT_>::IsKeepAlive() const
{
  return m_keep_alive;
}

template<typename ProtocolT_>
/**
 * @brief Метод IsOpen возвращает признак открытого сокета.
 * @return булево значение:
 * @value true  сокет открыт;
 * @value false сокет закрыт.
 */
bool
AsioSocketSession<ProtocolT_>::IsOpen()
{
  return SocketRef().is_open();
}

template<typename ProtocolT_>
/**
 * @brief Метод ServiceRef возвращает ссылку на сервис воода/вывода
 *        типа @a boost::asio::io_service для сокета @a m_Socket.
 * @return ссылку на сервис воода/вывода
 */
io_service_t &
AsioSocketSession<ProtocolT_>::ServiceRef()
{
  return SocketRef().get_io_service();
}

template<typename ProtocolT_>
/**
 * @brief Метод Stop выполняет останов сессии работы с сокетом.
 *
 * Таймеры всех каналов приема и отправки данных прекращают отсчет ожидания,
 * буферы с данными очищаются, сокет закрывается.
 */
void
AsioSocketSession<ProtocolT_>::Stop()
{
//  DUMP_INFO(__PRETTY_FUNCTION__);
  boost::system::error_code ec;
  try
  {
    StopTimer();
    if( IsOpen() )
    {
      m_socket.shutdown( boost::asio::socket_base::shutdown_both, ec );
    }
  }
  catch( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
  }
  m_socket.close( ec );
}

//template<typename ProtocolT_>
//vectors_data_t
//AsioSocketSession<ProtocolT_>::Receive( boost::asio::yield_context yield )
//{
//  vectors_data_t buffer_in;
//  if( IsOpen() )
//  {
//    vectors_data_t::value_type b;
//    boost::system::error_code ec;
//    auto avail = static_cast< std::size_t >(SocketRef().available( ec ));
//    if( avail > 0 /*and IsNoErr( ec ) */)
//    {
//      buffer_in.resize(avail);
//      async_reader< self_t, ProtocolT_ >()( std::ref( * this ), b, yield[ ec ] );
//      if( IsNoErr(ec) )
//        buffer_in.push_back( b );
//    }
//  }
//  return buffer_in;
//}

template<typename ProtocolT_>
vectors_data_t
AsioSocketSession<ProtocolT_>::Receive()
{
  vectors_data_t buffer_in;
  if( IsOpen() )
  {
    boost::system::error_code ec;
    auto avail = static_cast< std::size_t >(SocketRef().available( ec ));
    while( avail > 0 and IsNoErr( ec ) )
    {
      typename vectors_data_t::value_type buffer( avail, 0 );
      htl::asio::async_reader< self_t, ProtocolT_ >()(
            std::ref( * this ),
            buffer,
            ec,
            [ &buffer_in ]( boost::system::error_code & ec, std::size_t )
            {
//              DUMP_ASIO_ERROR( ec );
              if( not IsNoErr( ec ) )
              {
                buffer_in.clear();
              }
            } );
      if( not IsNoErr( ec ) )
        break;

      if( not buffer.empty() and IsNoErr( ec ) )
        buffer_in.push_back( buffer );

      avail = static_cast< std::size_t >(SocketRef().available( ec ) );
    }
  }
  return buffer_in;
}

template<typename ProtocolT_>
void
AsioSocketSession<ProtocolT_>::Send(const vectors_data_t & buffer_out )
{
  if( IsOpen() and ( not buffer_out.empty() ) )
  {
    vectors_data_t vd;
    vd.assign( buffer_out.begin(), buffer_out.end() );
    async_writer< self_t, ProtocolT_ >()(
          std::ref( * this ), vd/*,
              []( boost::system::error_code e, std::size_t s)
              {
                DUMP_INFO( std::string("sended = ") + std::to_string(s) );
                DUMP_ASIO_ERROR( e );
      //          DUMP_INFO_STL( std::string(b.data()) );
              }*/ );
  }
}

//template<typename ProtocolT_>
//void
//AsioSocketSession<ProtocolT_>::Send
//(
//    const vectors_data_t        & buffer_out,
//    boost::asio::yield_context    yield
//)
//{
//  if( IsOpen() )
//  {
//    vectors_data_t vd;
//    vd.assign( buffer_out.begin(), buffer_out.end() );
//    boost::system::error_code ec;
//    async_writer< self_t, ProtocolT_ >()( std::ref( * this ), vd, ec, yield );
//  }
//}

template<typename ProtocolT_>
/**
 * @brief Метод SocketRef возвращает ссылку на сокет.
 * @return Ссылка на сокет.
 */
typename AsioSocketSession<ProtocolT_>::socket_t &
AsioSocketSession<ProtocolT_>::SocketRef()
{
  return std::ref( m_socket );
}

//------------------------------------------------------------------------------

}// namespace                   asio
}// namespace                   htl

#endif // ASIOSOCKETSESSION_H
