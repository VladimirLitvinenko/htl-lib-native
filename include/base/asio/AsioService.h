#ifndef ASIO_SERVICE_H
#define ASIO_SERVICE_H

#include "common/Utils.h"
#include "base/asio/AsioError.h"
#include "common/ErrLiterals.h"

#include <experimental/optional>
#include <map>

namespace                         htl   {
namespace                         asio  {

//------------------------------------------------------------------------------
/**
 * @brief The IoServiceActionType enum
 */
enum                              IoServiceActionType
{
  BeforeStart                   = 0,
  BeforeStop                    = 1,
  AfterSart                     = 2,
  AfterStop                     = 3,
};

/**
 * @typedef
 */
using ios_callbacks_map         = std::map< IoServiceActionType, htl::utils::ordered_actions_t >;

//------------------------------------------------------------------------------
/**
 * @brief Класс-перечисление AsioState типа @a short объявляет значения
 *        возможных состояний сервера
 */
enum class                        AsioState : short
{
  Unknown                       = -1, ///< значение не определено (неопределённое состояние)
  Ok                            = 0,  ///< корректное состояние

  Exception                     = 1,

  ErrPortCount                  = 10,  ///< переполнение количества портов
  ErrChannelId                  = 11,  ///< ошибка идентификации канала данных

  ErrSocketCount                = 20, ///< переполнение допустимого количества
                                      ///< одновременно открытых сокетов

  ErrConnection                 = 30  ///< ошибка подключения сокета
};

//------------------------------------------------------------------------------
/**
 * @brief Класс AsioService
 */
class COMMON_SHARED_EXPORT        AsioService
{
public:
  /**/                            AsioService         ( std::int64_t timeoutMs = 10000 );
  /**/                            AsioService         ( const AsioService &             ) = delete;
  /**/                            AsioService         ( AsioService &&                  ) = delete;
  /**/                            AsioService         ( const htl::asio::io_service_t & ) = delete;
  /**/                            AsioService         ( htl::asio::io_service_t &&      ) = delete;
  virtual                       ~ AsioService         ();

  virtual AsioService &           operator=           ( const AsioService&              ) = delete;
  virtual AsioService &           operator=           ( AsioService &&                  ) = delete;
  virtual AsioService &           operator=           ( const htl::asio::io_service_t & ) = delete;
  virtual AsioService &           operator=           ( htl::asio::io_service_t &&      ) = delete;

  bool                            InThread            () const noexcept;
  bool                            IsStateValid        () const noexcept;
  void                            ResetState          () noexcept;
  virtual bool                    IsValid             () const;
  virtual bool                    IsActive            () const noexcept;
  std::int64_t                    CloseTimeout        () const;
  void                            SetCloseTimeout     ( const std::int64_t & timeoutMs = 10000 ) noexcept;

  boost::system::error_code       ErrorCode           () const;
  boost::system::errc::errc_t     ErrorValue          () const;
  void                            SetError            ( const AsioError & error ) noexcept;
  void                            SetError            ( AsioError && error ) noexcept;
  void                            SetError            ( const boost::system::error_code & error ) noexcept;
  void                            SetError            ( boost::system::errc::errc_t ec ) noexcept;
  bool                            IsError             ( const boost::system::error_code & error ) noexcept;
  void                            SetErrorCallbacks   ( asio_errcallbacks_t callbacks ) noexcept;
  void                            SetErrorCallback    ( const asio_errcallbacks_t::key_type key, asio_errcallbacks_t::mapped_type action ) noexcept;

  bool                            Start               ( bool asThread );
//  void                            Poll                ();
  void                            Stop                () noexcept;

  io_service_t                  & ServiceRef          ();
  void                            SetState            ( const AsioState state ) noexcept;

  template< typename Action_, typename ... ArgsT_ >
  void                            SetCallback
  (
      IoServiceActionType         type,
      Action_                  && action,
      ArgsT_...                   params
  ) noexcept;
  std::experimental::optional< utils::ordered_actions_t >
                                  GetCallback( IoServiceActionType type ) noexcept;
  std::experimental::optional< utils::ordered_actions_t::mapped_type >
                                  GetCallback( IoServiceActionType type, utils::ordered_actions_t::key_type ) noexcept;
  void                            RunCallback( IoServiceActionType type ) noexcept;
  void                            RunCallback( IoServiceActionType type, utils::ordered_actions_t::key_type ) noexcept;

private:
  io_service_t                    m_Service;
  asio_workptr_t                  m_WorkPtr;
  AsioError                       m_Error;
  std::atomic_bool                m_Active            { false };
  std::atomic_bool                m_as_thread         { false };
  std::int64_t                    m_TimeoutMs         { 10000 };
  AsioState                       m_State             { AsioState::Unknown };
  ios_callbacks_map               m_callbacks;

  void                            SetDefaultErrorCallbacks () noexcept;
  void                            RunService          () noexcept;
  void                            RunThreadService    () noexcept;
};

//------------------------------------------------------------------------------

template< typename Action_, typename ... ArgsT_ >
void
AsioService::SetCallback( IoServiceActionType type, Action_&& action, ArgsT_... params ) noexcept
{
  auto iter(m_callbacks.find( type ));
  utils::ordered_actions_t::key_type key(0);

  if( iter != m_callbacks.end() )
      key = utils::OrderedActionNext( iter->second );

  m_callbacks[ type ][ key ] =
    std::make_shared<utils::action_p_t<Action_, ArgsT_... >>
    ( std::forward<Action_&&>(action), std::forward< ArgsT_ >(params)... );
}

using AsioServiceShared         = std::shared_ptr< AsioService >;

//------------------------------------------------------------------------------

}// namespace                   asio
}// namespace                   htl

#endif // ASIO_SERVICE_H
