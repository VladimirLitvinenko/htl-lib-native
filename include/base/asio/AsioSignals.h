#ifndef ASIOSIGNALS_H
#define ASIOSIGNALS_H

#include <AsioService.h>

namespace                       htl   {
namespace                       asio  {

//------------------------------------------------------------------------------
class COMMON_SHARED_EXPORT      AsioSignals
{
public:
  explicit                      AsioSignals( const htl::asio::AsioService & );
  virtual                     ~ AsioSignals();

  boost::system::error_code                    AddSignal           ( int signalNumber );
  boost::system::error_code                    RemoveSignal        ( int signalNumber );
  virtual boost::system::error_code            AddSignalHandler
  (
      int                         signalNumber,
      const asio_signal_action_t& method
  );
  virtual boost::system::error_code            AddSignalHandler
  (
      int                         signalNumber,
      asio_signal_action_t     && method
  );
  void                            SetSignalsDefault   ();

private:
  htl::asio::AsioService        & m_ServiceRef;
  asio_signal_t                   m_Signals;
  asio_signal_actions_t           m_SignalActions;

  void                            SignalHandler
  (
      const boost::system::error_code & error,        // Result of operation.
      int                               signal_number // Indicates which signal occurred.
  );
};

//------------------------------------------------------------------------------

}// namespace                   asio
}// namespace                   htl

#endif // ASIOSIGNALS_H
