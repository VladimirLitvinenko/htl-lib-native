#include "AsioAcceptor.h"
#include "common/AsyncWrapper.h"
#include "Logger.h"

#include <future>
#include <type_traits>

namespace                         htl   {
namespace                         asio  {

/**
 * @brief  Конструктор AsioAcceptor формирует экземпляр класса для
 *            прослушивания TCP-подключений по сети к серверу по назначенному
 *            порту.
 * @param service ссылка на экземпляр сервиса обслуживания типа @a boost::asio::io_service
 * @param port порт TCP-сервера
 * @param protocol протокол для передачи данных. По умолчанию -
 */
AsioAcceptor::AsioAcceptor( htl::asio::AsioServer<htl::asio::tcp_t> & serverRef )
  : m_server_ref  ( serverRef )
  , m_acceptor    ( serverRef.ServicePtrRef()->ServiceRef() )
{
  m_server_ref.ServicePtrRef()->SetCallback(
    IoServiceActionType::BeforeStart,
    [this]()
    {
      this->Start();
    } );
}

AsioAcceptor::~AsioAcceptor()// = default;
{

}

/**
 * @brief Метод ServiceRef возвращает ссылку на сервис ввода/вывода Boost.Asio
 * @return ссылка на сервис ввода/вывода типа @a boost::asio::io_service библиотеки Boost.Asio
 */
tcp_t::acceptor::executor_type
AsioAcceptor::ServiceRef()
{
  return m_acceptor.get_executor();
}

/**
 * @brief Метод IsOpen возвращает признак активности аксептора @a m_scceptor.
 * @return Булево значение:
 * @value true акцептор активен и принимает подключения к серверу;
 * @value false акцептор неактивен, сервис @a ServiceRef() неактивен.
 */
bool
AsioAcceptor::IsOpen()
const noexcept
{
  return m_acceptor.is_open();
}

bool
AsioAcceptor::IsActive()
const noexcept
{
  return m_server_ref.ServicePtrRef()->IsActive();
}

bool
AsioAcceptor::Start()
{
  bool retval( IsOpen() );
  if( not retval )
  {
    short try_count = 10;
    retval = TryOpen();
    while( ( not retval ) and ( try_count-- > 0) )
    {
      std::this_thread::yield();
      std::this_thread::sleep_for( std::chrono::milliseconds(100));
      retval = TryOpen();
    }
    if( retval )
    {
      std::async(std::launch::async, &AsioAcceptor::DoAccept, this ).wait_for(std::chrono::milliseconds(0));
//      threadptr_t( new threadptr_t::element_type( boost::bind( &AsioAcceptor::DoAccept, this ) ) )->detach();
    }
  }
  return retval;
}

/**
 * @brief Метод StopAcceptor производит попытки останова обслуживания подключений
 * @param tryStopCount количество попыток останова
 */
void
AsioAcceptor::Stop(short tryStopCount )
{
  while( IsOpen() and ( tryStopCount-- > 0 ) )
  {
    TryClose();
    if( not IsOpen() )
      break;
    std::this_thread::sleep_for( std::chrono::milliseconds(100) );
  }
}

/**
 * @brief Метод TryOpen производит попытки активизации обслуживания подключения
 * @return Булево значение:
 * @value true  подключение прошло удачно;
 * @value false подключение не состоялось
 */
bool
AsioAcceptor::TryOpen()
{
  try
  {
    {// создание и ассинхронный запуск задачи последовательной привязки прослушивания сокетов
      auto acceptor_future =
          std::async(
            std::launch::async,
            [ this ]()
      {
        try
        {
          boost::system::error_code ec;
          do
          {
            this->m_acceptor.open( this->m_server_ref.Protocol(), ec );
            if( IsNoErr(ec))
            {
              this->SetOptions( true );
              this->m_acceptor.bind( this->m_server_ref.Endpoint(), ec );
              if( IsNoErr(ec))
              {
                this->m_acceptor.listen( boost::asio::socket_base::max_connections, ec );
              }
            }
            DUMP_ASIO_ERROR( ec );
            std::this_thread::yield();
          }
          while( not IsNoErr(ec) );
        }
        catch( const std::exception & e )
        {
          DUMP_EXCEPTION( e );
        }
      } );

      // проверка готовности задачи выдать результат по истечении заданного
      // промежутка времени @a m_scceptorTimeoutMs
      return
          ( acceptor_future.wait_for( std::chrono::milliseconds( m_server_ref.TimeoutMs() ) )
            ==
            std::future_status::ready )
          and
          IsOpen();
    }
  }
  catch( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
  }
  return IsOpen();
}

/**
 * @brief Метод TryClose производит попытки завершения обслуживания подключения
 * @return Булево значение:
 * @value true  удачное завершение обслуживания подключений к сокетам;
 * @value false завершение не состоялось
 */
void
AsioAcceptor::TryClose()
{
  // создание и ассинхронный запуск задачи отключения
  auto acceptor_future =
      std::async
      (
        std::launch::async,
        [ this ]()
        {
          boost::system::error_code ec;
          while( this->IsOpen() and ( IsNoErr( ec ) ) )
          { // выполнять попытки, пока обслуживание активно и нет ошибок отключения
            try
            {
              this->m_acceptor.cancel( ec );
              this->m_acceptor.close( ec );
            }
            catch( std::exception & e )
            {
              DUMP_EXCEPTION( e );
              break;
            }
          }
        }
  );

  // проверка готовности задачи выдать результат по истечении заданного
  // промежутка времени @a m_scceptorTimeoutMs
  if( acceptor_future.wait_for( std::chrono::milliseconds( m_server_ref.TimeoutMs() ) )
      ==
      std::future_status::ready )
    acceptor_future.get();
}

/**
 * @brief Защищенный виртуальный метод SetOptions задаёт опции для сокете по умолчанию.
 */
void
AsioAcceptor::SetOptions( bool is_keep_alive )
{
  using namespace htl::asio;
  boost::system::error_code ec;

  m_acceptor.set_option(acceptor_t::keep_alive                ( is_keep_alive ) , ec);
  m_acceptor.set_option(acceptor_t::reuse_address             (true)            , ec);
  m_acceptor.set_option(acceptor_t::enable_connection_aborted (true)            , ec);
}

/**
 * @brief Метод AsioAcceptor::DoAccept
 */
void
AsioAcceptor::DoAccept()
{
  if( not IsOpen() )
    return;

//  auto strand = boost::asio::make_strand(m_acceptor.get_executor());
  auto session =
      htl::asio::MakeSocketSession< boost::asio::ip::tcp >(
#if BOOST_ASIO_VERSION >= 101401
//        *cnt_ptr,
        static_cast<boost::asio::io_context&>(strand.get_inner_executor().context()),
#else
#if BOOST_ASIO_VERSION >= 101201
        m_acceptor.get_io_context(),
#else
        m_acceptor.get_io_service(),
#endif
#endif
        true );

  if( not session )
  {
      DUMP_INFO( std::string("Asio session is NULL.") )
      return;
//    throw std::runtime_error(__PRETTY_FUNCTION__);
  }

  try
  {
    m_acceptor.async_accept(
          session->SocketRef(),
          [this, session ]( boost::system::error_code ec )
          {
            if( IsNoErr(ec) )
            {
//              std::make_shared<htl::AsyncWrapper<void, session_ptr, buffer_transformation_t>>(
              std::async(
                    std::launch::async,
                    //-----------
                    []
                    (
                      session_ptr && ses,
                      buffer_transformation_t t
                    )
                    {
                      session_ptr s = std::move( ses );
                      htl::asio::set_socket_options<boost::asio::ip::tcp::socket>()(
                            s->SocketRef(), s->IsKeepAlive() );
                      s->Send( std::move( std::__invoke( t, s->Receive() ) ) );
                      s.reset();
                    }
                    ,
                    //-----------
                    std::move( session ),
                    this->m_server_ref.Transformation()
              ).wait_for( std::chrono::milliseconds(0) );
//                )->Start( std::chrono::milliseconds(0),
//                       std::move( session ),
//                       this->m_server_ref.Transformation() );
            }
            if( this->IsOpen() and IsNoErr( ec ) )
            {
              std::async(std::launch::async, &AsioAcceptor::DoAccept, this ).wait_for(std::chrono::milliseconds(0));
//              this->DoAccept();
            }
          }
    );
  }
  catch( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
  }
}

//void
//AsioAcceptor::DoAcceptSpawn()
//{
//  if( not IsOpen() )
//    return;

//  boost::system::error_code ec;
//  boost::asio::spawn(
//        boost::asio::io_service::strand( ServiceRef() ),
//        [ this, &ec ]( boost::asio::yield_context yield )
//  {
//    while( this->IsOpen() )
//    {
//      auto session = std::move( htl::asio::MakeSocketSession< boost::asio::ip::tcp >( true ) );
//      if( session )
//      {
//        m_acceptor.async_accept( session->SocketRef(), yield[ ec ] );
//        if( IsNoErr( ec ) )
//        {
//          boost::asio::spawn(
//                boost::asio::io_service::strand( ServiceRef() ),
//                [ this, session ]( boost::asio::yield_context yield )
//                {
//                  auto m = [ this, &ec, yield ](
//                      const SocketSessionShared< boost::asio::ip::tcp > & ses,
//                      buffer_transformation_t t
//                  )
//                  {
//                    self_t::session_t s(
//                          std::move(
//                            * const_cast< session_ptr & >( ses ) ) );
//                    set_socket_options<boost::asio::ip::tcp::socket>()(
//                          s.SocketRef(),
//                          s.IsKeepAlive() );
//                  };
//                  boost::asio::spawn(
//                        boost::asio::io_service::strand( ServiceRef() ),
//                        [ &m, &session, this ]( boost::asio::yield_context yield )
//                        {
//                          std::__invoke( m, session, this->m_server_ref.Transformation(), yield );
//                        }
//                } );
//        }
//      }
//      else
//      {
//        break;
//      }
//    }
//  } );
//}

}// namespace                     asio
}// namespace                     htl
