#ifndef HTL_UTILS_H
#define HTL_UTILS_H

#include "Types.h"
IMPORT_MODULE(htl.types);

#include <vector>
//#include <iostream>
#include <memory>
#include <map>
#include <utility>
#include <experimental/tuple>
#include <type_traits>

EXPORT_MODULE(htl.utils);

namespace htl   {
namespace utils {

//------------------------------------------------------------------------------
using active_proc_t           = std::map< std::string, pid_t >;
using active_pid_t            = std::map< pid_t, std::string >;
using active_processes_t      = std::pair< active_pid_t, active_proc_t >;

std::pair< pid_t, bool >        IsProcessDir      ( const std::string & dir );
active_processes_t              ActiveProcessMap  ();
std::pair< pid_t, bool >        ProcessIsActive   ( const std::string & procName );
std::pair<std::string, bool>    ProcessAsPid      ( const pid_t pid );
std::pair< std::string, bool >  HostName();
//bool                            SetSysyemWatches  ( int param, std::size_t value );
/**
 * @brief Метод UserName определяет имя текущего пользователя ОС.
 * @return имя текущего пользователя ОС как значение типа @a std::string.
 */
inline std::string              UserName() noexcept
{
  return ::getenv("LOGNAME");
}

/**
 * @brief Метод IsSuperuser возвращает признак текущего пользователя программы
 *        как суперпользователя (root).
 * @return Булево значение:
 * @value true текущий пользователь - root (например, программа выполняется как sudo <программа>);
 * @value false текущий пользователь не обладает правами суперпользователя root.
 */
inline bool                     IsSuperuser() noexcept
{
  return UserName() == htl::USER_ROOT;
}

//------------------------------------------------------------------------------
std::pair< std::string, bool > MakePath( const std::string & path );
std::pair< std::string, bool > MakeFile( const std::string & fileName );

//------------------------------------------------------------------------------
std::string           UpperFirst( const std::string & string );
std::string           Upper( const std::string & string );
std::string           Lower( const std::string & string );
htl::strings_t        NormalisationStrings( const htl::strings_t & strings );
htl::strings_t        StrSplit( const std::string & str, const std::string & splitter );
inline htl::strings_t StrSplit( const std::string & str, char splitter )
{
  return StrSplit( str, std::string( 1, splitter ) );
}
std::string           StrJoin ( const htl::strings_t & strings, const std::string & splitter );
inline std::string    StrJoin ( const htl::strings_t & strings, char splitter )
{
  return StrJoin( strings, std::string( 1, splitter ) );
}
std::string     StrToHex   ( const std::string & str );
std::string     VectorToHex   ( const std::vector<char> & v );
std::string     StrFromHex ( const std::string & str );
std::string     StrChop    ( const std::string & str, std::size_t endCount );
std::string     StrReplace ( const std::string & str, const std::string & str_from, const std::string & str_to );
inline bool     StrContains( const htl::strings_t & strings, const std::string & str )
{
  return
      std::find_if( const_cast<strings_t &>(strings).begin(),
                    const_cast<strings_t &>(strings).end(),
                    [ & str ]( const typename htl::strings_t::value_type & s )
                    {
                      return str == s;
                    } )
      !=
      const_cast<strings_t &>(strings).end();
}

inline bool     StrContains( const std::string & str, const std::string & part )
{
  return str.find( part.c_str() ) != std::string::npos;
}

//------------------------------------------------------------------------------

/**
 * @brief LineRowTransform
 * @param z
 * @param y
 * @return
 */
constexpr std::uint64_t TileRowTransform( const std::uint32_t z, const std::uint64_t y )
{
  return ( std::uint64_t{1} << std::uint64_t( z ) ) - std::uint64_t{1} - y;
}

//------------------------------------------------------------------------------
/**
 * @brief Абстрактная структура i_action предназначена для приведения типа при
 *        динамическом созданиии объектов типа @a action_t с различными методами
 *        и параметрами.
 */
struct i_action
{
  i_action() = default;
  virtual ~i_action() {}

  virtual void Run() = 0;

  template< typename Function_ >
  void SetAction( Function_&& );

  template< typename ... Args_ >
  i_action & operator ()( Args_ ... );

  template< typename ... Args_ >
  void Exec( Args_ ... );
};

//------------------------------------------------------------------------------
template< typename Function_ >
  /**
 * @brief Структура action_t содержит метод и кортеж его параметров.
 */
struct  action_t
  : public i_action
  , public std::enable_shared_from_this< htl::utils::action_t<Function_ > >
{
  Function_             m_action; ///< метод типа @a std::function

  action_t( Function_ && action )
    : i_action()
      , m_action( std::forward< Function_&& >(action) )
  {}

  void Run() override
  {
    std::__invoke( m_action );
  }
};

//------------------------------------------------------------------------------
template
<
    typename Function_,
    typename ... Args_
>
/**
 * @brief Структура action_t содержит метод и кортеж его параметров.
 */
struct  action_p_t
: public i_action
, public std::enable_shared_from_this< htl::utils::action_p_t<Function_, Args_... > >
{
  Function_             m_action; ///< метод типа @a std::function
  std::tuple<Args_...>  m_params; ///< кортеж параметров метода @a action_t::m_action

  action_p_t( Function_&& action, Args_&&... params )
    : i_action()
    , m_action( std::forward< Function_ >(action) )
    , m_params( std::make_tuple<Args_...>( std::forward<Args_>(params)... ) )
  {}

  void Run() override
  {
    std::experimental::apply( m_action, m_params );
  }
};

//------------------------------------------------------------------------------
template< typename ... Args_ >
i_action & i_action::operator ()( Args_ ... p )
{
  auto ptr( reinterpret_cast< action_p_t<void(*)(Args_...), Args_...> *>( this ) );
//  ptr->m_params = std::forward_as_tuple( std::forward< Args_ >(p)... );
  ptr->m_params = std::make_tuple( std::forward< Args_ >(p)... );
  return std::ref( * ptr );
}

template< typename Function_ >
void i_action::SetAction( Function_&& action )
{
  reinterpret_cast< action_t<Function_> *>( this )->m_action =
    std::forward< Function_&& >(action) ;
}

template< typename ... Args_ >
void i_action::Exec( Args_ ... p )
{
  if( std::tuple_size< std::tuple<Args_...> >::value > 0 )
  {
    auto ptr(reinterpret_cast< action_p_t<void(*)(Args_...), Args_...> *>( this ) );
    std::experimental::apply( ptr->m_action, std::make_tuple( std::forward<Args_>(p)... ) );
  }
  else
  {
    auto ptr(reinterpret_cast< action_t<void(*)()> *>( this ) );
    std::__invoke( ptr->m_action );
  }
}

std::string DoubleToCoordinateStr
(
    const double        coord,
    const std::string & sign_grad = "°",
    const std::string & sign_min  = "'",
    const std::string & sign_sec  = "''"
);

double CoordinateStrToDouble
(
    const std::string & coord,
    const std::string & sign_grad = "°",
    const std::string & sign_min  = "'",
    const std::string & sign_sec  = "''"
);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//namespace arch {

//std::string   Compress(const std::string& raw);
//std::string   Decompress(const std::string& raw);

//} // namespace arch

/**
 * @type action_ptr
 * @brief Тип определяет структуру метода и кортежа с параметрами для метода.
 */
using action_ptr  = std::shared_ptr<i_action >;

/**
 * @typedef ordered_actions_t
 * @brief Тип методов с приоритетами.
 * @value ordered_actions_t::key_type приоритет. Высший - меньшее значение;
 * @value ordered_actions_t::mapped_type метод (функция).
 */
using ordered_actions_t = std::map< std::size_t, action_ptr >;

/**
 * @brief OrderedActionMin
 * @param a
 * @return
 */
std::size_t OrderedActionMin( const ordered_actions_t & a ) noexcept;
/**
 * @brief OrderedActionMax
 * @param a
 * @return
 */
std::size_t OrderedActionMax( const ordered_actions_t & a ) noexcept;
/**
 * @brief OrderedActionNext
 * @param a
 * @return
 */
std::size_t OrderedActionNext( const ordered_actions_t & a ) noexcept;
/**
 * @brief OrderedActionPrev
 * @param a
 * @return
 */
std::size_t OrderedActionPrev( const ordered_actions_t & a ) noexcept;

template <typename _Key, typename _Tp>
std::vector< _Key > map_keys( const std::map<_Key, _Tp > & m )
{
  std::vector< _Key > retval;
  retval.reserve( m.size() );
  for( auto & val : m )
    retval.push_back( val.first );
  return  retval;
}

}// namespace utils
}// namespace htl

#endif // HTL_UTILS_H

