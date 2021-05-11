#ifndef APPWITHSIGNALS_H
#define APPWITHSIGNALS_H

//#include <Application.h>
//#include <SysSignalsController.h>
//#include <Utils.h>

//#include <csignal>
//#include <unistd.h>
//#include <fcntl.h>
//#include <functional>
////#include <boost/noncopyable.hpp>
//#include <memory>

//namespace                         htl   {
//namespace                         app   {

////------------------------------------------------------------------------------
///**
// * @brief The SignalStatus enum
// */
//enum class                        SignalStatus : short
//{
//  Attached                      = 0,
//  Ignored,
//  Default,
//  Processed,
//  Sended,
//};

//using signal_status_t           = std::function< void( int ) >;
//using signal_statuses_t         = std::map< short/*SignalStatus*/, htl::app::signal_status_t >;

////------------------------------------------------------------------------------

//template< typename                ApplicationT_ >
//class COMMON_SHARED_EXPORT        AppWithSignals
//: public                          ApplicationT_
//, public                          htl::sys::SysSignalsController
//{
//private:
//  signal_statuses_t               m_Statuses
//  {
//    { static_cast< short >( SignalStatus::Attached  ), nullptr },
//    { static_cast< short >( SignalStatus::Ignored   ), nullptr },
//    { static_cast< short >( SignalStatus::Default   ), nullptr },
//    { static_cast< short >( SignalStatus::Processed ), nullptr },
//    { static_cast< short >( SignalStatus::Sended    ), nullptr },
//  };

//public:
//  using base_class_t            = ApplicationT_;
//  using sygnal_class_t          = htl::sys::SysSignalsController;
//  using app_factory_t           = htl::app::AppFactory<ApplicationT_>;

//  /**/                            AppWithSignals        ( int argc, char ** argv )
//    : htl::app::AppWithSignals< ApplicationT_>::base_class_t ( argc, argv )
//    , htl::app::AppWithSignals< ApplicationT_ >::sygnal_class_t()
//  {
//    app_factory_t::SetState( AppState::Ok );
//  }
//  virtual                       ~ AppWithSignals        ()  {}

//  inline void                     SetStatusHandler
//  (
//      htl::app::SignalStatus            status,
//      const htl::app::signal_status_t & handler
//  )
//  {
//    m_Statuses[ static_cast< short >(status) ] = boost::bind( handler, _1 );
//  }
//  inline void  SetStatusHandlers ( const htl::app::signal_statuses_t & handlers )
//  {
//    m_Statuses = handlers;
//  }
//  virtual void                    InitSignalsDefault    ()
//  {
//    for( auto & sN :
//    {
//         SIGQUIT,
//         SIGTRAP,
//         SIGABRT,
//         SIGCLD,
//         SIGCHLD,
//         SIGSTOP,
//         SIGTSTP,
//         SIGPROF,
//         SIGWINCH,
//    } )
//    {
//      AddIgnore( static_cast< htl::sys::signals_t::value_type >( sN ) );
//    }
//  }

//  htl::sys::signals_t::value_type AddIgnore  ( htl::sys::signals_t::value_type ) override;
//  htl::sys::signals_t::value_type AddDefault ( htl::sys::signals_t::value_type ) override;
//  bool                            AddAction
//  (
//      htl::sys::signals_t::value_type sn,
//      __sighandler_t                  fn
//  ) override;
//  htl::sys::signals_t::value_type SendSignal  ( htl::sys::signals_t::value_type ) override;

//  static AppWithSignals< ApplicationT_ > * Instance ()
//  {
//    return reinterpret_cast< AppWithSignals< ApplicationT_ > * >( AppFactory< ApplicationT_ >::Application() );
//  }

//  static QStringList              GetArguments          ();

//  void                            DisableStdout         () noexcept;
//  void                            SetStateHandler
//  (
//      const htl::app::AppState            & state,
//      const htl::app::app_state_handler_t & handler
//  )
//  ;
//};

////------------------------------------------------------------------------------

//template< typename ApplicationT_ >
//htl::sys::signals_t::value_type
//AppWithSignals< ApplicationT_ >::AddIgnore( htl::sys::signals_t::value_type sn )
//{
//  auto retval( AppWithSignals< ApplicationT_ >::sygnal_class_t::AddIgnore( sn ) );
//  if( ( 0 < retval ) and m_Statuses.at( static_cast< short >( SignalStatus::Ignored) ) )
//  {
//    m_Statuses.at( static_cast< short >( SignalStatus::Ignored) )( sn );
//  }
//  return retval;
//}

//template< typename ApplicationT_ >
//htl::sys::signals_t::value_type
//AppWithSignals< ApplicationT_ >::AddDefault( htl::sys::signals_t::value_type sn )
//{
//  auto retval( AppWithSignals< ApplicationT_ >::sygnal_class_t::AddDefault( sn ) );
//  if( ( 0 < retval ) and ( m_Statuses.at( static_cast< short >( SignalStatus::Default) ) ) )
//  {
//    m_Statuses.at( static_cast< short >( SignalStatus::Default ) )( sn );
//  }
//  return retval;
//}

//template< typename ApplicationT_ >
//bool
//AppWithSignals< ApplicationT_ >::AddAction
//(
//    htl::sys::signals_t::value_type sn,
//    __sighandler_t                  fn
//)
//{
//  auto retval( AppWithSignals< ApplicationT_ >::sygnal_class_t::AddAction( sn,fn ) );
//  if( retval and m_Statuses.at( static_cast< short >( SignalStatus::Attached) ) )
//  {
//    m_Statuses.at( static_cast< short >( SignalStatus::Attached) )( sn );
//  }
//  return retval;
//}

//template< typename ApplicationT_ >
//htl::sys::signals_t::value_type
//AppWithSignals< ApplicationT_ >::SendSignal( htl::sys::signals_t::value_type sn )
//{
//  auto retval( AppWithSignals< ApplicationT_ >::sygnal_class_t::SendSignal( sn ) );
//  if( ( 0 == retval ) and m_Statuses.at( static_cast< short >( SignalStatus::Sended ) ) )
//  {
//    m_Statuses.at( static_cast< short >( SignalStatus::Sended) )( sn );
//  }
//  return retval;
//}

//template< typename ApplicationT_ >
//inline
//QStringList
//AppWithSignals< ApplicationT_ >::GetArguments()
//{
//  return ApplicationT_::arguments();
//}

//template< typename ApplicationT_ >
//void
//AppWithSignals< ApplicationT_ >::DisableStdout() noexcept
//{
//  for( auto output : { ::stdin, ::stdout, ::stderr } )
//  {
//    if( -1 == ::dup( output->_fileno ) )
//    {
//      app_factory_t::SetState( AppState::ErrCloseDescriptor );
//    }
//  }
//}

//template< typename ApplicationT_ >
//inline
//void
//AppWithSignals< ApplicationT_ >::SetStateHandler
//(
//    const htl::app::AppState            & state,
//    const htl::app::app_state_handler_t & handler
//)
//{
//  app_factory_t::SetStateHandler( state, handler );
//  if( m_Statuses.at( static_cast< short >( SignalStatus::Processed ) ) )
//  {
//    std::async( std::launch::async,
//                [this]( int state )
//                {
//                  this->m_Statuses.at( static_cast< short >( SignalStatus::Processed ) )( state );
//                },
//                static_cast< int >( AppFactory<ApplicationT_>::GetState() ) ).
//        wait_for( std::chrono::milliseconds(10));
//  }
//}

//}// namespace                   spo
//}// namespace                   app

#endif // APPWITHSIGNALS_H
