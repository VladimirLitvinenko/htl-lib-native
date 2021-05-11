#include "AsioService.h"
#include "common/AsyncWrapper.h"
#include "Logger.h"
#include <boost/system/error_code.hpp>
#include <functional>
#include <type_traits>

namespace                       htl   {
namespace                       asio  {

//------------------------------------------------------------------------------

AsioService::AsioService        ( std::int64_t timeoutMs )
  : m_TimeoutMs                 ( timeoutMs )
{
  SetDefaultErrorCallbacks();
}

AsioService::~ AsioService      () {}

bool
AsioService::InThread()
const noexcept
{
  return m_as_thread.load();
}

/**
 * @brief Метод IsStateValid проверяет состояние сервера
 * @return Булево значение:
 * @value true  сервер в исправном состоянии;
 * @value false сервер неисправен.
 */
bool
AsioService::IsStateValid()
const noexcept
{
  return m_State == AsioState::Ok;
}

/**
 * @brief Метод ResetState сбрасывает сервер в неопределенное (начальное) состояние
 */
void
AsioService::ResetState()
noexcept
{
  m_State = AsioState::Unknown;
}

bool
AsioService::IsValid()
const
{
  return
      IsStateValid()
      and
      ( m_Error.Value() == boost::system::errc::success );
}

bool
AsioService::IsActive()
const noexcept
{
  return m_Active;
}

int64_t
AsioService::CloseTimeout()
const
{
  return m_TimeoutMs;
}

void
AsioService::SetCloseTimeout(const int64_t& timeoutMs)
noexcept
{
  m_TimeoutMs = timeoutMs < 0 ? 0 : timeoutMs;
}

boost::system::error_code
AsioService::ErrorCode()
const
{
  return m_Error.Code();
}

boost::system::errc::errc_t
AsioService::ErrorValue()
const
{
  return m_Error.Value();
}

void
AsioService::SetError(const AsioError& error)
noexcept
{ m_Error.SetErrorCode( error.Code() ); }

void
AsioService::SetError( AsioError && error)
noexcept
{
  m_Error.SetErrorCode( std::move( error ).Code() );
}

void AsioService::SetError(const boost::system::error_code& error)
noexcept
{
  m_Error.SetErrorCode( error );
}

void
AsioService::SetError(boost::system::errc::errc_t ec)
noexcept
{
  m_Error.SetErrorCode( ec );
}

bool
AsioService::IsError(const boost::system::error_code& error)
noexcept
{
  SetError( error );
  return not IsNoErr( ErrorCode() );
}

void
AsioService::SetErrorCallbacks( asio_errcallbacks_t callbacks )
noexcept
{
  m_Error.SetErrorCallbacks( callbacks );
}

void
AsioService::SetErrorCallback( const asio_errcallbacks_t::key_type key, asio_errcallbacks_t::mapped_type action )
noexcept
{
  m_Error.SetErrorCallback( key, action );
}

bool
AsioService::Start(bool asThread)
{
  m_as_thread = asThread;
  if( not IsActive() )
  {
    try
    {
      InThread()
          ? RunThreadService()
          : RunService      ();
    }
    catch( const std::exception & e )
    {
      DUMP_EXCEPTION( e );
      m_Active = false;
      SetError( boost::system::errc::errc_t::connection_aborted );
    }
  }
  return IsActive();
}

//void
//AsioService::Poll()
//{
//  if( IsActive() )
//    return;

//  try
//  {
//    RunServiceCallbacks( IoServiceActionType::BeforeStart );
//    m_Service.poll();
//    RunServiceCallbacks( IoServiceActionType::AfterStop );
//  }
//  catch( const std::exception & e )
//  {
//    DUMP_EXCEPTION( e );
//    SetError( boost::system::errc::errc_t::owner_dead );
//  }
//}

void
AsioService::Stop()
noexcept
{
  try
  {
    if( not IsActive() )
      return;

    RunCallback( IoServiceActionType::BeforeStop );
    m_WorkPtr.reset();

    htl::AsyncWrapper<void> aw
    (
      [ this ]()
      {
        this->m_Service.stop();
        this->m_Service.reset();
        this->m_Active = false;
      }
    );
    aw.Start( std::chrono::milliseconds( m_TimeoutMs ) );
  }
  catch( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
    this->m_Active = false;
  }
}

io_service_t &
AsioService::ServiceRef()
{
  return std::ref( m_Service );
}

/**
 * @brief Метод SetState назначает текущее состояние серверу
 * @param state значение назначаемого состояния
 */
void
AsioService::SetState(const AsioState state)
noexcept
{
  m_State = state;
}

std::experimental::optional< utils::ordered_actions_t >
AsioService::GetCallback( IoServiceActionType type ) noexcept
{
  std::experimental::optional< utils::ordered_actions_t > retval{ std::experimental::nullopt };
  auto iter( m_callbacks.find( type ) );
  if( iter != m_callbacks.end() )
  {
    retval = iter->second;
  }
  return retval;
}

std::experimental::optional< utils::ordered_actions_t::mapped_type >
AsioService::GetCallback
(
    IoServiceActionType                 type,
    utils::ordered_actions_t::key_type  key
)
noexcept
{
  std::experimental::optional< utils::ordered_actions_t::mapped_type > retval
  {
    std::experimental::nullopt
  };

  auto iter( m_callbacks.find( type ) );
  if( iter != m_callbacks.end() )
  {
    auto r_iter( iter->second.find( key ) );
    if( r_iter != iter->second.end() )
      retval = r_iter->second;
  }
  return retval;
}

void
AsioService::RunCallback( IoServiceActionType type )
noexcept
{
  if( m_callbacks.empty() )
    return;

  auto iter( m_callbacks.find( type ) );
  if( iter != m_callbacks.end() )
  {
    for( auto & r : iter->second )
      r.second->Run();
  }
}

void
AsioService::RunCallback(IoServiceActionType type, utils::ordered_actions_t::key_type key)
noexcept
{
  auto opt{ GetCallback( type, key ) };
  if( opt )
    opt.value()->Run();
}

void
AsioService::SetDefaultErrorCallbacks()
noexcept
{
  SetErrorCallbacks( {
        {
           // обработка исключения exception
            boost::system::errc::errc_t::owner_dead,
            boost::bind( &AsioService::Stop, this )
        }, } );
}

void
AsioService::RunService()
noexcept
{
  RunCallback( IoServiceActionType::BeforeStart );
  if( not IsActive() )
  {
    try
    {
      m_Active = true;
      m_WorkPtr = std::make_shared< asio_workuptr_t::element_type >( ServiceRef() );
      boost::system::error_code ec;
      m_Service.run( ec );
      SetError( ec );
    }
    catch( const std::exception & e )
    {
      DUMP_EXCEPTION( e );
    }

    m_WorkPtr.reset();
    m_Active = false;
    RunCallback( IoServiceActionType::AfterStop );
  }
}

void
AsioService::RunThreadService()
noexcept
{
  try
  {
    if( not IsActive() )
    {
      std::make_shared< htl::thread_t >(
            boost::bind( & AsioService::RunService, this  ) )->detach();
    }
  }
  catch ( const std::exception & e )
  {
    DUMP_EXCEPTION( e )  ;
  }
}



}// namespace                   asio
}// namespace                   htl
