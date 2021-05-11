#include "base/application/AppSkeleton.h"
#include "common/AsyncWrapper.h"
#include "spdlog/Logger.h"

namespace                         htl   {
namespace                         app   {

bool
AppSkeleton::Execute()
noexcept
{
  BEGIN_LOCK_SECTION_SELF_;
  DUMP_INFO_CONSOLE( "Try Execute..." );
  m_app_state.m_status = app_state::status::Idle;
  bool retval( m_action_ptr and /*( not IsActive() ) and*/ ( not m_app_state.is_run() ) );
  if( retval )
  {
    try
    {
      SetStatus( app_state::status::Run );
      m_action_ptr->Run();
      SetStatus( app_state::status::Complete );
      DUMP_INFO( "Run method" );
    }
    catch( const std::exception & e )
    {
      SetStatus( app_state::status::Idle );
      DUMP_EXCEPTION( e );
      retval = false;
    }
//    m_app_state.m_status = app_state::status::Idle;

//    m_starter = std::make_shared< htl::threadptr_t::element_type >(
//                boost::bind( &app_state::starter, &(this->m_app_state), std::ref( *this ) ) );
//    m_executer = std::make_shared< htl::threadptr_t::element_type >(
//                boost::bind( &app_state::executor, &(this->m_app_state), std::ref( *this ) ) );

//    SetCode( m_starter->joinable()
//             and
//             m_executer->joinable()
//             ? EXIT_SUCCESS
//             : EXIT_FAILURE );
//    retval = IsCodeSuccess();

//    if( not retval )
//    {
//      m_starter.reset();
//      m_executer.reset();
//    }
  }
  END_LOCK_SECTION_;
  return retval;
}

void AppSkeleton::SetStatus(AppSkeleton::app_state::status s)
{
  m_app_state.m_status = s;
}

AppSkeleton::AppSkeleton(int argc, char** argv)
  : AppSkeleton::base_class_t ()
{
  MakeArguments ( argc, argv );
  htl::log::Logger::Instance( Name() + ".log" );
}

bool
AppSkeleton::IsActive()
const
{
  return m_action_ptr and ( not m_app_state.is_terminate() );
}

bool
AppSkeleton::IsComplete()
const
{
  return m_action_ptr and m_app_state.is_complete();
}

void
AppSkeleton::InitSignalsDefault()
{
  BEGIN_LOCK_SECTION_SELF_;

  for( auto & sN :
  {
       SIGTRAP,
       SIGCLD,
       SIGCHLD,
       SIGSTOP,
       SIGTSTP,
       SIGPROF,
       SIGWINCH,
} )
  {
    AddIgnore( static_cast< htl::sys::signals_t::value_type >( sN ) );
  }

  AddAction( SIGINT  , [this]( int sn ){ this->Quit   ( sn ); } );
  AddAction( SIGHUP  , [this]( int sn ){ this->Reload ( sn ); } );
  AddAction( SIGQUIT , [this]( int sn ){ this->Quit   ( sn ); } );
  AddAction( SIGTERM , [this]( int sn ){ this->Quit   ( sn ); } );
  AddAction( SIGABRT , [this]( int sn ){ this->Abort  ( sn ); } );
  END_LOCK_SECTION_;
}

bool
AppSkeleton::IsUnique()
const
{
  return m_unique_mode;
}

bool
AppSkeleton::BlockMode()
const
{
  return m_app_state.m_block_mode;
}

void AppSkeleton::SetBlockMode(bool mode)
{
  BEGIN_LOCK_SECTION_SELF_;
  m_app_state.m_block_mode = mode;
  m_app_state.m_condition.notify_all();
  END_LOCK_SECTION_;
}

void
AppSkeleton::SetUnique(bool uniqueMode)
{
  m_unique_mode = uniqueMode;
}

std::string
AppSkeleton::Name()
const
{
  BEGIN_LOCK_SECTION_SELF_;
  std::string retval;
  if( IsValid () )
  {
    auto app_name( m_propertyPtr->ApplicationName () );
    auto list( htl::utils::StrSplit ( app_name, DIR_SEPARATOR ) );
    if( list.size () > 0 )
      retval = list.back ();
  }
  END_LOCK_SECTION_;
  return retval;
}

/**
 * @brief AppSkeleton::Path
 * @return
 */
std::string
AppSkeleton::Path() const
{
  BEGIN_LOCK_SECTION_SELF_;
  return
      IsValid ()
      ? m_propertyPtr->ApplicationName ()
      : std::string();
  END_LOCK_SECTION_
}

void
AppSkeleton::Start()
{
  DUMP_INFO(__PRETTY_FUNCTION__ );

  if( not IsValid() )
    Abort( EXIT_FAILURE );

  InitSignalsDefault ();

  DUMP_INFO( std::string("IsUnique() : ") + std::to_string(IsUnique()) );
  DUMP_INFO( std::string("ExistsCallback().second : ") + std::to_string(ExistsCallback().second) );
  if( IsUnique() and ExistsCallback().second )
  {
    Abort( EXIT_FAILURE );
  }

  DUMP_INFO_CONSOLE( "Try Start..." );
  auto try_count(10);
  while( ( not Execute() ) and ( try_count-- > 0) )
  {
    std::this_thread::yield();
    std::this_thread::sleep_for( std::chrono::milliseconds(500));
  }
}

void AppSkeleton::Stop()
{
  SetBlockMode( false );
  m_app_state.m_status = app_state::status::Complete;
}

void
AppSkeleton::Restart(int code)
{
  DUMP_INFO(" Interrupt signal: RESTART.");
  SetCode(code);

  auto bm( BlockMode() );
  Stop();
  SetBlockMode( bm );
  Start();
}

void
AppSkeleton::Reload(int code)
{
  DUMP_INFO(std::string(" Interrupt signal: RELOAD."));
  SetCode(code);
  Restart( code );
}

/**
 * @brief AppSkeleton<Args_>::Quit
 * @param code
 */
void
AppSkeleton::Quit(int code)
{
  DUMP_INFO(std::string(" Interrupt signal: terminale as QUIT."));
  SetCode(code);
  SetStatus( app_state::status::Terminate );
//  Stop ();
  ::_exit(EXIT_SUCCESS);
}

/**
 * @brief AppSkeleton<Args_>::Abort
 */
void
AppSkeleton::Abort(int)
{
  DUMP_INFO(std::string(" Interrupt signal: terminale as ABORT."));
  SetCode(EXIT_FAILURE);
  SetStatus( app_state::status::Terminate );
  Stop ();
  ::_exit(EXIT_FAILURE);
}

/**
 * @brief AppSkeleton<Args_>::Delete
 */
void
AppSkeleton::Delete()
{
  Stop ();
//  std::async( std::launch::async,
  htl::AsyncWrapper<void>(
              [&]()
              {
                try
                {
                  delete this;
                }
                catch( const std::exception & e )
                {
                  DUMP_EXCEPTION ( e );
                }
              } ).
//    wait_for( std::chrono::milliseconds(0));
      Start( std::chrono::milliseconds(0));
}

int
AppSkeleton::Exec()
{
  while( IsActive() and BlockMode() )
  {
    if( m_app_state.is_terminate() )
        break;

    std::this_thread::yield();
    std::this_thread::sleep_for( std::chrono::milliseconds(300));
  }
  return GetCode();
}

void AppSkeleton::SetCode( const decltype (EXIT_SUCCESS) code )
{
  BEGIN_LOCK_SECTION_SELF_;
  m_code = code;
  END_LOCK_SECTION_;
}

/**
 * @brief AppSkeleton<Args_>::IsValid
 * @return
 */
bool
AppSkeleton::IsValid()
const
{
  BEGIN_LOCK_SECTION_SELF_;
  return
      IsCodeSuccess()
      and
      m_propertyPtr.operator bool ()
      ;
  END_LOCK_SECTION_
}

/**
 * @brief AppSkeleton<Args_>::GetProperty
 * @return
 */
appproperty_week_t
AppSkeleton::GetProperty()
const
{
  return appproperty_week_t( m_propertyPtr );
}

/**
 * @brief AppSkeleton<Args_>::SetExistsCallback
 * @param callback
 * @warning Не передавать функтор по ссылке!
 */
void
AppSkeleton::SetExistsCallback( appexists_action_t callback)
{
  BEGIN_LOCK_SECTION_SELF_;
  m_exists_callback = callback;
  END_LOCK_SECTION_;
}

/**
 * @brief AppSkeleton<Args_>::ExistsCallback
 * @return
 */
std::pair<pid_t, bool>
AppSkeleton::ExistsCallback()
{
  std::pair<pid_t, bool> retval{ -1, false };
  if( m_propertyPtr )
  {
    retval = ApplicationExists ( m_propertyPtr->ApplicationName () );
    DUMP_INFO( std::string("retval : ") + std::to_string(retval.second) );
    DUMP_INFO( std::string("m_exists_callback : ") + std::to_string(m_exists_callback.operator bool()) );
    DUMP_INFO( std::string("retval.second     : ") + std::to_string(retval.second) );
    if( m_exists_callback and retval.second )
    {
      std::__invoke( m_exists_callback );
    }
  }
  return retval;
}

AppSkeleton &
AppSkeleton::MakeArguments( int argc, char ** argv )
noexcept
{
  BEGIN_LOCK_SECTION_SELF_;
  m_propertyPtr = std::make_shared< appproperty_shared_t::element_type >( argc, argv );
  return std::ref( * this );
  END_LOCK_SECTION_
}

AppSkeleton&
AppSkeleton::operator=(appproperty_shared_t&& app_property)
{
  m_propertyPtr.swap( app_property );
  return std::ref(* this);
}

void
AppSkeleton::SetPid(pid_t pid)
{
  BEGIN_LOCK_SECTION_SELF_;
  m_pid = pid;
  END_LOCK_SECTION_;
}

void
AppSkeleton::app_state::starter(AppSkeleton& app)
{
  if( app.m_action_ptr )
  {
    {
      std::unique_lock<std::mutex> l( m_mutex );
      m_status = status::Run;
      m_condition.notify_all();
      UNUSED(l);
    }
    std::unique_lock<std::mutex> lock( m_mutex );
    m_condition.wait( lock, [ this ](){ return this->is_complete(); } );
    m_status = status::Idle;
    m_condition.notify_all();
    UNUSED(lock);
  }
}

void
AppSkeleton::app_state::executor(AppSkeleton& app)
{
  std::unique_lock<std::mutex> lock( m_mutex );
  m_condition.wait(lock, [this, &app](){ return app.m_action_ptr and is_run(); } );
  lock.unlock();
  lock.lock();
  app.m_action_ptr->Run();
  m_condition.wait(lock, [this](){ return not this->is_block(); } );
  m_status = app_state::status::Complete;
  m_condition.notify_all();
}

}// namespace                     app
}// namespace                     htl
