/**
 * @file SysSygnalsController.cpp
 *
 * @brief Файл реализации описания класса SysSignalsController реализации данных
 * и методов для работы с сигналами ОC *NIX
 *
 * @author Владимир Н. Литвиненко.
 * @copyright Все права защищены.
 */

#include "HtlGlobal"
#include "SysSignalsController.h"
#include "AppWithSignals.h"
#include "Logger.h"
#include "Utils.h"
#include <type_traits>

namespace                       htl   {
namespace                       sys   {

/**
 * @brief Аргумент m_ActionsMap ассоциативный контейнер-карту соответствия
 *        обработчиков конкретным сигналам.
 * @value std::map::key_type    - значение сигнал;
 * @value std::map::mapped_type - обработчик сигнала
 *
 * Если сигналу можно назначить обработчик, и обработчик присутствует в данном
 * контейнере, то он вызывается при получении наследником класса сигнала со
 * значением ключа контейнера типа \a std::map::key_type.
 */
static signals_map_t  m_actions_map;
static signal_info    m_si;

//------------------------------------------------------------------------------
/**
 * @brief Метод SignalValid проверяет на наличие сигнала в контейнере
 *        @a UNIX_SIGNALS допустимых к работе сигналов.
 * @param sN значение зигнала.
 * @return Логическое значение:
 * @value true сигнал прошел проверку успешно;
 * @value false сигнал проверку не прошел: недопустим к применению обрабтки.
 */
bool
SignalValid( const std::sig_atomic_t sN )
{
  return
      std::find( UNIX_SIGNALS.cbegin(),
                 UNIX_SIGNALS.cend(),
                 sN )
      !=
      UNIX_SIGNALS.cend();
}

SysSignalsController::SysSignalsController()
{
}

SysSignalsController::~SysSignalsController()
{

}

bool
SysSignalsController::AddIgnore( std::sig_atomic_t signalNumber )
{
  BEGIN_LOCK_SECTION_SELF_;
  try
  {
    return m_si.set_signal_ignore( signalNumber );
  }
  catch( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
  }
  return false;
  END_LOCK_SECTION_;
}

bool
SysSignalsController::AddDefault( std::sig_atomic_t signalNumber )
{
  BEGIN_LOCK_SECTION_SELF_;
  try
  {
    return m_si.set_signal_default( signalNumber );
  }
  catch( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
  }
  return false;
  END_LOCK_SECTION_;
}

bool
SysSignalsController::AddAction( std::sig_atomic_t signalNumber, signal_action_t action )
{
  BEGIN_LOCK_SECTION_SELF_;
  try
  {
    m_si.set_signal( signalNumber,action );
  }
  catch( const std::exception & e )
  {
    DUMP_EXCEPTION( e );
  }
  return false;
  END_LOCK_SECTION_;
}

signals_t::value_type
SysSignalsController::SendSignal( htl::sys::signals_t::value_type sn )
{
  return std::raise( static_cast< int >( sn ) );
}

//-----------------------------------------------------------------------------
void handler(int s)
{
  auto iter( m_actions_map.find(s) );
  if( iter != m_actions_map.end() )
  {
    sigset_t set = m_si.m_sa.sa_mask;
    if( 0 == sigemptyset(&set) )
    {
      if( 0 == sigaddset(&set, s) )
      {
        if( 0 == sigprocmask(SIG_BLOCK, &set, nullptr))
        {
          std::__invoke( iter->second, s );
        }
        sigprocmask(SIG_UNBLOCK, &set, nullptr);
        m_si.m_sa.sa_mask = set;
        m_si.set_signal(s, iter->second );
      }
    }
  }
}

signal_info::signal_info()
{
  memset(&m_sa, 0, sizeof(m_sa));
  m_sa.sa_handler = handler;
  sigemptyset(&m_sa.sa_mask);
  m_sa.sa_flags = SA_RESTART;
}

signal_info::signal_info(sig_atomic_t sn, signal_action_t sh)
  : signal_info()
{
  set_signal( sn, sh );
}

bool
signal_info::set_signal(sig_atomic_t sn, signal_action_t sh)
{
  bool retval( SignalValid(sn) );
  if( retval )
  {
    retval = sigaction(sn, &m_sa, nullptr) == 0;
    if( retval )
      m_actions_map[ sn ] = sh;
  }
  return retval;
}

bool signal_info::set_signal_ignore(sig_atomic_t sn)
{
  return set_signal( sn , SIG_IGN );
}

bool signal_info::set_signal_default(sig_atomic_t sn)
{
  return set_signal( sn , SIG_DFL );
}

}// namespace                 sys
}// namespace                 htl
