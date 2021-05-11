#include <AsioSignals.h>

namespace                       htl   {
namespace                       asio  {

AsioSignals::AsioSignals        ( const htl::asio::AsioService & service )
  : m_ServiceRef                ( const_cast< htl::asio::AsioService & >( service ) )
//  , m_Signals                   ( const_cast< boost::asio::io_service & >( const_cast< htl::asio::AsioService & >( service ).ServiceRef() ) )
  , m_Signals                   ( m_ServiceRef.ServiceRef() )
{

}

AsioSignals::~AsioSignals() = default;

boost::system::error_code AsioSignals::AddSignal(int signalNumber)
{
  boost::system::error_code err;
  err = this->m_Signals.add( signalNumber, err );
  return err;
}

boost::system::error_code
AsioSignals::RemoveSignal( int signalNumber )
{
  boost::system::error_code err;
  auto iter( m_SignalActions.find( signalNumber) );
  if( iter != m_SignalActions.end() )
  {
    m_SignalActions.erase( iter );
  }
  err = this->m_Signals.remove( signalNumber, err );
  return err;
}

boost::system::error_code
AsioSignals::AddSignalHandler
(
    int                           signalNumber,
    const asio_signal_action_t  & method
)
{
  auto err = AddSignal( signalNumber );
  if( IsNoErr( err ) )
  {
    m_SignalActions[ signalNumber ] = method;
  }
  return err;
}

boost::system::error_code
AsioSignals::AddSignalHandler
(
    int                     signalNumber,
    asio_signal_action_t && method
)
{
  auto err = AddSignal( signalNumber );
  if( IsNoErr( err ) )
  {
    m_SignalActions[ signalNumber ] = std::move( method );
  }
  return err;
}

void AsioSignals::SetSignalsDefault()
{
  auto on_start = [this]( int , boost::system::error_code& e)
  {
    if( not this->m_ServiceRef.IsActive() )
      this->m_ServiceRef.Start( this->m_ServiceRef.InThread() );
    return e;
  };
  auto on_stop = [this]( int , boost::system::error_code& e)
  {
    this->m_ServiceRef.Stop();
    return e;
  };
  auto on_exit = [this]( int , boost::system::error_code& e )
  {
    this->m_ServiceRef.Stop();
    ::exit( IsNoErr(e) ? 0 : 1 );
    return e;
  };
  auto on_restart = [this]( int , boost::system::error_code& e )
  {
    this->m_ServiceRef.Stop();
    this->m_ServiceRef.Start( this->m_ServiceRef.InThread() );
    ::exit( IsNoErr(e) ? 0 : 1 );
    return e;
  };

//  AddSignalHandler( SIGHUP  , on_restart );
  AddSignalHandler( SIGINT  , on_stop );
  AddSignalHandler( SIGQUIT , on_stop );
  AddSignalHandler( SIGTRAP , on_exit );
  AddSignalHandler( SIGABRT , on_exit );
  AddSignalHandler( SIGBUS  , on_restart );
  AddSignal( SIGFPE ); // ignore
  AddSignalHandler( SIGSEGV , on_exit );
  AddSignalHandler( SIGPIPE , on_restart );
  AddSignalHandler( SIGTERM , on_stop );

  AddSignalHandler( SIGSTOP , on_stop );
  AddSignalHandler( SIGCONT , on_start );
  AddSignal( SIGTSTP );
  AddSignal( SIGTTIN ); // ignore
  AddSignal( SIGTTOU ); // ignore
  AddSignal( SIGURG );
  AddSignalHandler( SIGXCPU , on_restart );
  AddSignal( SIGXFSZ );

  AddSignal( SIGVTALRM );
  AddSignal( SIGPROF );

  AddSignal( SIGWINCH );
  AddSignal( SIGPOLL );
  AddSignal( SIGIO );
  AddSignalHandler( SIGPWR  , on_stop );
  AddSignalHandler( SIGSYS  , on_stop );

  m_Signals.async_wait( boost::bind( &AsioSignals::SignalHandler, this,
                                     boost::placeholders::_1,
                                     boost::placeholders::_2 ) );
}

void
AsioSignals::SignalHandler(const boost::system::error_code& error, int signal_number)
{
  if( IsNoErr( error) )
  {
    auto iter( m_SignalActions.find( signal_number ) );
    if( iter != m_SignalActions.end() )
    {
      boost::system::error_code e( error );
      e = iter->second( signal_number, e );
    }
  }
  m_Signals.async_wait( boost::bind( &AsioSignals::SignalHandler, this,
                                     boost::placeholders::_1,
                                     boost::placeholders::_2 ) );
}

}// namespace                   asio
}// namespace                   htl
