#ifndef ASIOUDPSERVER_H
#define ASIOUDPSERVER_H

#include "AsioServer.h"
#include "AsioSocketSession.h"

namespace                         htl   {
namespace                         asio  {

//------------------------------------------------------------------------------

//class COMMON_SHARED_EXPORT        AsioUDPServer :
//public                            htl::asio::AsioServer< boost::asio::ip::udp >
//{
//public:
//  using self_t                  = htl::asio::AsioUDPServer;
//  using udp_t                   = boost::asio::ip::udp;
//  using base_class_t            = htl::asio::AsioServer< self_t::udp_t >;

//  boost::system::error_code TryOpen( boost::asio::ip::udp::socket & socket )
//  {
//    boost::system::error_code ec =
//        boost::system::errc::make_error_code( boost::system::errc::success );
//    try
//    {
//      if( not socket.is_open() )
//      {
//        socket.open( base_class_t::Protocol(), ec );
//        htl::asio::set_socket_options< boost::asio::ip::udp::socket >()( socket );

//        AsioService::Instance().SetError( ec );
//        if( IsNoErr( ec ) )
//        {
//          socket.bind( base_class_t::Endpoint(), ec );
//          AsioService::Instance().SetError( ec );
//        }
//      }
//    }
//    catch( const std::exception & e )
//    {
//      AsioService::Instance().SetError( boost::system::errc::owner_dead );
//      DUMP_EXCEPTION( e );
//    }

//    if( AsioService::Instance().IsError( ec ) )
//    {
//      socket.shutdown( boost::asio::socket_base::shutdown_both, ec );
//      socket.close( ec );
//    }
//    return AsioService::Instance().ErrorCode();
//  }

//  /**
//   * @brief Конструктор AsioUDPServer формирует экземпляр класса для
//   *        прослушивания UDP-подключений по сети к серверу по назначенному
//   *        порту.
//   * @param port порт UDP-сервера
//   * @param protocol протокол для передачи данных. По умолчанию -
//   */
//  /**/                            AsioUDPServer
//  (
//      const htl::asio::TransferType   type,
//      const htl::asio::port_t         port,
//      const std::int64_t            & serviceTimeoutMs = ASIO_DEADLINE_MILLISEC_TYPE << 1
//  )
//    : self_t::base_class_t( type, port, self_t::udp_t::v4(), serviceTimeoutMs )
//  {
//    Init();
//  }

//private:

//  /**
//   * @brief Метод Listen реализует алгоритм обработки UDP подключений
//   * @param yield контекстный метод обработки передачи управления сопрограмме
//   */
//  void Listen ( /*boost::asio::ip::udp::socket && socket, */boost::asio::yield_context yield )
//  {
//    boost::system::error_code ec;
//    if( not base_class_t::SocketsValid() )
//    {
//      AsioService::Instance().SetState( AsioState::ErrSocketCount );
//      yield[ ec ];
//      return;
//    }

//    auto session_ptr( MakeSocketSession< boost::asio::ip::udp >(
//                                  base_class_t::ActionsRef(),
//                                  base_class_t::TransferType(),
//                                  base_class_t::Endpoint(),
//                                  base_class_t::SocketDeadline() ) );

//    // назначение сокета и проверка подключения
////    boost::asio::ip::udp::socket socket( AsioService::Instance().ServiceRef() );
//    ec = TryOpen( session_ptr->SocketRef() );

//    if( IsNoErr( ec ) )
//    {
//      base_class_t::IncSocketsCount();

//      // создание shared-сессии работы с UDP-сокетом
//        struct socket_udp
//        {
//          boost::asio::ip::udp::socket & s;
//          htl::asio::AsioUDPServer * ptr = nullptr;
//          socket_udp( boost::asio::ip::udp::socket& sk, htl::asio::AsioUDPServer * p )
//            : s{ sk }
//            , ptr( p )
//          {}
//        };
//        socket_udp obj( session_ptr->SocketRef(), this );

//        session_ptr->SetAfterStop(
//              [ this, yield ]( void * ptr )
//              {
//                auto s_ptr( reinterpret_cast< socket_udp * >(ptr) );
//                if( nullptr != s_ptr )
//                {
//                  s_ptr->ptr->DecSocketsCount();
//                  //перезапуск UDP-приема после останова работы сессии
////                  if( AsioService::Instance().IsActive() and s_ptr->ptr->SocketsValid() )
////                  {
////                    try
////                    {
////                      std::async( std::launch::async,
////                                  [ this, yield, s_ptr]
////                      {
////                      boost::asio::spawn(
////                            io_strand_t( AsioService::Instance().ServiceRef() ),
//////                            boost::bind( & self_t::Listen, s_ptr, yield ) );
////                            boost::bind( & self_t::Listen,
////                                         s_ptr->ptr,
////                                         s_ptr->s,
////                                         yield ) );
////                      } ).wait_for( std::chrono::milliseconds( 0 ) );
////                    }
////                    catch( const std::exception & e )
////                    {
////                      DUMP_EXCEPTION( e )
////                    }
////                  }
//                }
//              }, &obj );
//        AsioService::Instance().ServiceRef().post(
//          boost::bind(
//            & htl::asio::AsioSocketSession< boost::asio::ip::udp >::Start,
//            session_ptr ) );
//      }
//      else
//      {
//        ec = boost::system::errc::make_error_code( boost::system::errc::invalid_argument );
//      }
//  }

//  /**
//   * @brief Метод Init устанавливает алгорим прослушивания UDP-подклучений после
//   *        запуска сервиса обработкм ввода/вывода типа @a boost::asio::io_service
//   *
//   * @see AsioService::Instance
//   */
//  void Init () noexcept
//  {
//    AsioService::Instance().AddBeforeStartCallback
//        ( {
//            [ this ](void * ptr)
//            {
//              auto self( reinterpret_cast< self_t * >(ptr) );
//              if( nullptr != self )
//              {
//                try
//                {
//                  boost::asio::spawn(
//                        io_strand_t( AsioService::Instance().ServiceRef() ),
//                        boost::bind(& self_t::Listen,
//                                      self,
////                                      std::move( boost::asio::ip::udp::socket( AsioService::Instance().ServiceRef() ) ),
//                                      _1 ) );
//                }
//                catch ( const std::exception & e)
//                {
//                  DUMP_EXCEPTION( e );
//                }
//              }
//            },
//            this
//          } );
//  }
//};

//------------------------------------------------------------------------------

}// namespace                     asio
}// namespace                     htl


#endif // ASIOUDPSERVER_H
