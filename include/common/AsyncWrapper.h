#ifndef ASYNCWRAPPER_H
#define ASYNCWRAPPER_H

#include "common/Optional.h"
#include "common/Types.h"
//#include "spdlog/Logger.h"
#include <future>

namespace                         htl   {

enum class AsyncState : ushort
{
  None        = 0x0000,
  Ready       = 0x0001,
  Starting    = 0x0002,
  Started     = 0x0004,
  Stoping     = 0x0008,

  Error       = 0x8000,
  ErrFncInit  = 0x8001,
  ErrStarting = 0x8002,
};

constexpr bool IsAsyncStateError( const AsyncState state )
{
    return
        static_cast<AsyncState>( static_cast<ushort>( state ) | static_cast<ushort>( AsyncState::Error ))
        ==
        AsyncState::Error;
}

/**
 * @brief The AsyncWrapper class
 */
template
<
    typename      ResultT_,
    typename  ... Args_
>
class COMMON_SHARED_EXPORT        AsyncWrapper
{
    std::shared_future< ResultT_ >              m_future;
    std::function< ResultT_( Args_ ... ) >      m_functor;
    AsyncState                                  m_state     { AsyncState::None };

public:
    explicit AsyncWrapper( std::function< ResultT_( Args_ ... ) > && functor )
        : m_functor{ std::forward<std::function< ResultT_( Args_ ... ) >>(functor) }
        , m_state  { m_functor ? AsyncState::Ready : AsyncState::ErrFncInit }
    {}
    explicit AsyncWrapper( ResultT_(*functor)( Args_ ... ) )
        : m_functor{ functor }
        , m_state  { m_functor ? AsyncState::Ready : AsyncState::ErrFncInit }
    {}

    AsyncState  GetState    () const
    {
        return m_state;
    }
    bool    IsValid () const
    {
        return
        ( GetState() != AsyncState::None )
        and
        ( not IsError() )
        and
        m_functor;
    }
    bool    IsReady () const
    {
        return ( GetState() != AsyncState::Ready ) and ( not IsError() );
    }
    bool    IsBusy () const
    {
      return ( GetState() == AsyncState::Started ) or ( GetState() == AsyncState::Starting );
    }
    bool IsError () const
    {
        return IsAsyncStateError(m_state);
    }
    void SetFunctor( std::function< ResultT_( Args_ ... ) > && functor )
    {
      if( not IsBusy() )
        m_functor = std::forward<std::function< ResultT_( Args_ ... ) >>(functor);
      SetState( m_functor ? AsyncState::Ready : AsyncState::ErrFncInit );
    }
    void SetFunctor( ResultT_(*functor)( Args_ ... ) )
    {
      SetFunctor( std::function< ResultT_( Args_ ... ) >( functor ) );
    }

    /**
     * @brief Start
     * @param probe_tileout
     * @param params
     * @return
     */
    auto Start   ( std::chrono::milliseconds probe_timeout, Args_ ... params )
    {
      if( IsReady() )
      {
        SetState(AsyncState::Starting);
        try
        {
          m_future = std::async(std::launch::async, m_functor, std::forward<Args_>(params)...);
          if (m_future.wait_for(probe_timeout) != std::future_status::ready)
          {
            SetState(AsyncState::ErrStarting);
          }
          else
          {
            SetState(AsyncState::Started );
            return m_future.get();
          }
        } catch ( const std::exception & e )
        {
          SetState(AsyncState::ErrStarting);
        }
      }
      return ResultT_();
    }

    void Terminate()
    {
      if( IsValid() )
      {
        SetState(AsyncState::Stoping);
        std::set_terminate( m_functor );
        SetState(AsyncState::Ready );
      }
    }

protected:
    virtual void SetState( const AsyncState s )
    {
      m_state = s;
    }
    virtual void StateReset()
    {
      if( not IsBusy() and IsValid() )
        SetState( AsyncState::Ready );
    }
};

}// namespace                     htl

#endif // ASYNCWRAPPER_H
