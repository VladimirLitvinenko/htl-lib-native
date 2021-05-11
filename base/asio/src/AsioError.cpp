#include "AsioError.h"

namespace                         htl   {
namespace                         asio  {

AsioError &
AsioError::operator=( AsioError && err)
{
  m_Callbacks.swap( err.m_Callbacks );
  SetErrorCode( err.m_ErrorCode );
  err.m_ErrorCode.clear();
  return std::ref( * this );
}

AsioError &
AsioError::operator=( const AsioError& err )
{
  m_Callbacks   = err.m_Callbacks;
  SetErrorCode( err.m_ErrorCode );
  return std::ref( * this );
}

void
AsioError::Execute(const asio_errcallbacks_t::key_type& errorCode, bool async)
{
  if( m_Callbacks.empty() )
    return;

  auto iter( m_Callbacks.find( errorCode ) );
  if( iter != m_Callbacks.end() )
  {
    auto ec( boost::system::errc::make_error_code ( errorCode ) );
    if( async )
    {
      std::async( std::launch::async,
                  iter->second,
                  ec
                  ).wait_for( std::chrono::milliseconds(0));
      return;
    }
    iter->second( ec );
  }
}

void
AsioError::SetErrorCode(boost::system::errc::errc_t ec)
{
  m_ErrorCode = boost::system::errc::make_error_code( ec );
  if( m_ErrorCode != boost::system::errc::errc_t::success )
  {
    DUMP_ASIO_ERROR( m_ErrorCode );
  }
  Execute( ec );
}

void
AsioError::SetErrorCode(const boost::system::error_code& ec)
{
  SetErrorCode( static_cast< boost::system::errc::errc_t >( ec.value() ) );
}

void
AsioError::Erase(const asio_errcallbacks_t::key_type& errorCode)
noexcept
{
  if( m_Callbacks.empty() )
    return;

  auto iter( m_Callbacks.find( errorCode ) );
  if( iter != m_Callbacks.end() )
  {
    m_Callbacks.erase( iter );
  }
}

}// namespace                   asio
}// namespace                   htl
