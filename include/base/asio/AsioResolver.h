#ifndef ASIORESOLVER_H
#define ASIORESOLVER_H

#include "AsioService.h"
#include "ErrLiterals.h"

namespace                         htl   {
namespace                         asio  {

//------------------------------------------------------------------------------
template< typename                ProtocolT_ >
/**
 * @brief Класс AsioResolver формирует правила и атрибуты для подключения к
 * серверу по проеделенному типу протокола и правилами подключения.
 *
 * Параметры шаблона:
 * @value ProtocolT_ тип протокола для обмена данными по сети.
 * @value ByteT_ тип единицы информации для каналов обмена данными.
 */
class COMMON_SHARED_EXPORT        AsioResolver
{
  static_assert ( std::is_same< typename std::decay<ProtocolT_>::type, boost::asio::ip::tcp >::value
                  or
                  std::is_same<  typename std::decay<ProtocolT_>::type, boost::asio::ip::udp >::value
                  /*or
                  std::is_same<  typename std::decay<ProtocolT_>::type, boost::asio::ip::icmp >::value*/,
                  ST_ASSERT_TEMPLATE_VALIDATOR );
public:
  using protocol_t              = typename std::decay<ProtocolT_>::type;
  using self_t                  = htl::asio::AsioResolver< protocol_t >;
  using resolver_t              = typename protocol_t::resolver;
  using query_t                 = typename resolver_t::query;
  using iterator_t              = typename resolver_t::iterator;
  using endpoint_t              = typename protocol_t::endpoint;
  using endpoints_t             = std::vector< typename self_t::endpoint_t >;

  explicit                        AsioResolver
  (
      htl::asio::AsioService & asio_service,
      const std::string   & remoute_service,
      const std::int64_t    serviceTimeoutMs,
      boost::asio::ip::resolver_query_base::flags resolve_flags =
        boost::asio::ip::resolver_query_base::flags::passive
        |
        boost::asio::ip::resolver_query_base::flags::address_configured
  );

  explicit                        AsioResolver
  (
      htl::asio::AsioService & asio_service,
      const std::string   & host,
      const std::string   & remoute_service,
      const std::int64_t    serviceTimeoutMs,
      boost::asio::ip::resolver_query_base::flags resolve_flags =
        boost::asio::ip::resolver_query_base::flags::passive
        |
        boost::asio::ip::resolver_query_base::flags::address_configured
  );

  virtual                       ~ AsioResolver() {}

  boost::asio::detail::addrinfo_type & Hints      () const;
  std::string                     HosName         () const;
  std::string                     RemouteService  () const;
  typename self_t::resolver_t   & ResolverRef     ();
  typename self_t::query_t      & QueryRef        ();
  bool                            IsValid         ( bool rescan = false );
  io_service_t                  & ServiceRef      ();
  void                            Scan            () noexcept;
  typename self_t::endpoints_t    Endpoints       ( bool rescan = false );

private:
  /**
   * @brief Атрибут m_resolver отвечает за возможность разрешения доступа к удаленному серверу.
   * @see boost::asio::ip::basic_resolver
   */
  mutable resolver_t m_resolver;

  /**
   * @brief Атрибут m_query содержит запрос к @a m_resolver для формирования
   *        конечных точек доступа
   * @see boost::asio::ip::basic_resolver::query
   */
  query_t m_query;

  /**
   * @brief Атрибут m_Endpoints содержит список конечных точек доступа к
   * удаленным сервисам (серверам)
   */
    mutable typename self_t::endpoints_t m_Endpoints;
};

template<typename ProtocolT_>
AsioResolver<ProtocolT_>::AsioResolver
(
    AsioService                                 & asio_service,
    const std::string                           & remoute_service,
    const int64_t                                 serviceTimeoutMs,
    boost::asio::ip::resolver_query_base::flags   resolve_flags
)
  : m_resolver( asio_service.ServiceRef() )
  , m_query   ( remoute_service, resolve_flags )
{
  asio_service.SetCloseTimeout(serviceTimeoutMs);
}

template<typename ProtocolT_>
AsioResolver<ProtocolT_>::AsioResolver
(
    AsioService                                 & asio_service,
    const std::string                           & host,
    const std::string                           & remoute_service,
    const int64_t                                 serviceTimeoutMs,
    boost::asio::ip::resolver_query_base::flags   resolve_flags
)
  : m_resolver( asio_service.ServiceRef() )
  , m_query   ( host, remoute_service, resolve_flags  )
{
  asio_service.SetCloseTimeout(serviceTimeoutMs);
}

template<typename ProtocolT_>
/**
 * @brief Метод Hints вовращает структуру, содержащую информацию об адресе
 *        сервиса провайдера.
 * @return информация об адресе сервиса провайдера.
 */
boost::asio::detail::addrinfo_type &
AsioResolver<ProtocolT_>::Hints()
const
{
  return m_query.hints();
}

template<typename ProtocolT_>
/**
 * @brief Метод HosName возвращает имя хоста сервера к подключению
 * @return имя хоста сервера типа @a std::string
 */
std::string
AsioResolver<ProtocolT_>::HosName()
const
{
  return m_query.host_name();
}

template<typename ProtocolT_>
/**
 * @brief   Метод RemouteService возвращает имя сервиса ассоциированного с
 *          запросом.
 * @return  имя сервиса
 */
std::string
AsioResolver<ProtocolT_>::RemouteService()
const
{
  return m_query.service_name();
}

template<typename ProtocolT_>
/**
 * @brief   Метод ResolverRef возвращает ссылку на атрибут ответчика.
 * @return  ссылка на атрибут @a m_resolver ответчика.
 */
typename AsioResolver<ProtocolT_>::self_t::resolver_t &
AsioResolver<ProtocolT_>::ResolverRef()
{
  return std::ref( m_resolver );
}

template<typename ProtocolT_>
/**
 * @brief   Метод QueryRef возвращает ссылку на атрибут очереди параметров
 *          подключения к серверу.
 * @return  ссылку на атрибут @a m_query очереди.
 */
typename AsioResolver<ProtocolT_>::self_t::query_t &
AsioResolver<ProtocolT_>::QueryRef ()
{
  return std::ref( m_query );
}

template<typename ProtocolT_>
/**
 * @brief   Метод IsValid возвращает признак контроля к работе.
 * @return  Признак контроля к работе:
 * @value true  экземпляр класса готов к работе;
 * @value false экземпляр класса НЕ готов к работе;
 */
bool
AsioResolver<ProtocolT_>::IsValid(bool rescan)
{
  if( rescan )
    Scan();
  return not m_Endpoints.empty();
}

template<typename ProtocolT_>
/**
 * @brief   Метод ServiceRef возвращает ссылку на сервис Boost.Asio обслуживания
 *          ввода/вывода.
 * @return  сервис Boost.Asio обслуживания ввода/вывода.
 */
io_service_t &
AsioResolver<ProtocolT_>::ServiceRef()
{
  return m_resolver.get_io_service();
}

template<typename ProtocolT_>
/**
 * @brief   Метод Scan выполняет проверку доступности удаленных провайдеров.
 *
 * Метод формирует новый список конечных точек доступа согласно параметрам
 * очереди запрсов на подключение.
 *
 * @see IsValid, Endpoints
 */
void
AsioResolver<ProtocolT_>::Scan()
noexcept
{
  m_Endpoints.clear();
  boost::system::error_code ec;
  auto iter( m_resolver.resolve( m_query, ec ) );
  if( IsNoErr( ec ) )
  {
    typename resolver_t::iterator end;
    while( iter != end )
    {
      m_Endpoints.push_back( *iter );
      iter++;
    }
    return;
  }
  m_Endpoints.clear();
}

template<typename ProtocolT_>
/**
 * @brief Метод Endpoints возвращает список конечных точек доступа к сервисам
 *        провайдера (сервера).
 * @param rescan  значение указывает на необходимость (true) сканирования очереди
 *                запросов перед формирование списка конечных точек доступа.
 * @return копия списка конечных точек доступа как значений атрибута @a m_Endpoints
 */
typename AsioResolver<ProtocolT_>::self_t::endpoints_t
AsioResolver<ProtocolT_>::Endpoints( bool rescan )
{
  if( rescan )
    Scan();

  return m_Endpoints;
}

//------------------------------------------------------------------------------

}// namespace                     asio
}// namespace                     htl

#endif // ASIORESOLVER_H
