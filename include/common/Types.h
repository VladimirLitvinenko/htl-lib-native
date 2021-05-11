
#ifndef HTLTYPES_H
#define HTLTYPES_H

#include "Global.h"
#include "ErrLiterals.h"

//#include <thread>
#include <iostream>
#include <memory>
#include <future>
#include <mutex>
#include <map>
#include <vector>

#include <boost/filesystem.hpp>

#ifdef QT_VERSION
# include <QDebug>
#endif

#ifndef UNUSED
#  define UNUSED(variable) (void)variable;
#endif

//#ifndef BOOST_COROUTINE_NO_DEPRECATION_WARNING
//#  define BOOST_COROUTINE_NO_DEPRECATION_WARNING
//#endif

#ifndef BEGIN_LOCK_SECTION_
#  define BEGIN_LOCK_SECTION_(mutex) \
  std::unique_lock< std::recursive_mutex > l( mutex )
#endif

#ifndef BEGIN_LOCK_SECTION_SELF_
#  define BEGIN_LOCK_SECTION_SELF_ BEGIN_LOCK_SECTION_( this->m_Mutex )
#endif

#ifndef BEGIN_UNLOCK_SECTION_
#  define BEGIN_UNLOCK_SECTION_(mutex) l.unlock();
#endif

#ifndef BEGIN_UNLOCK_SECTION_SELF_
#  define BEGIN_UNLOCK_SECTION_SELF_ BEGIN_UNLOCK_SECTION_( this->m_Mutex )
#endif

#ifndef END_LOCK_SECTION_
#  define END_LOCK_SECTION_ UNUSED(l);
#endif

//------------------------------------------------------------------------------

EXPORT_MODULE(htl.types); // reseve fo last c++ versions///

#ifndef DIR_SEPARATOR
#  ifdef Q_OS_WIN
#    define DIR_SEPARATOR QLatinChar('\\')
#  else
#    define DIR_SEPARATOR "/"
# endif
#endif


namespace htl {

using file_path_t            = boost::filesystem::path;
using file_pathes_t          = std::vector< file_path_t >;

using thread_t                = std::thread;
using threadptr_t             = std::shared_ptr< thread_t >;

template< typename Res_ >
using shared_future_t           = std::shared_ptr< std::future< Res_ > >;

/**
 * @brief Тип devprop_t представляет карту (ассоц. контейнер) свойств устройств
 *        операционной системы.
 */
using devprop_t                 = std::map< std::string, std::string >;

/**
 * @brief Тип socket_byffer_t определяет тип буфера обмена данных.
 */
template< typename                ByteT_ >
using socket_byffer_t           = std::vector< typename std::decay<ByteT_>::type >;

using strings_t = std::vector< std::string >;

//template< typename ResT_, typename ... Args_ >
//using simple_fnc_t              = std::function< ResT_ ( Args_... ) >;

//------------------------------------------------------------------------------
using vectors_data_t               = std::vector< std::vector< char > >;

//------------------------------------------------------------------------------
using errors_t                  = std::map< std::remove_reference< decltype(errno) >::type, std::string >;
const errors_t                    SocketErrorsInfo
{
  { 0               , "Ошибки отсутствуют." },
  { ENOENT          , "Отсутствует файл или каталог." },
  { EACCES          , "Нет прав на создание сокета указанного типа и/или протокола." },
  { EPERM           , "Пользователь  попытался  соединиться  с  широковещательным  адресом,  не  установив  широковещательный  флаг на сокете или же запрос на соединение завершился неудачно из-за правила локального межсетевого экрана." },
  { EAFNOSUPPORT    , "Реализация не поддерживает указанное семейства адресов или Адрес имеет некорректное семейство адресов в поле sa_family." },
  { EINVAL          , "Неверные флаги в type, или Неизвестный протокол, или недоступное семейство протоколов." },
  { EMFILE          , "Переполнение таблицы файлов процесса." },
  { ENFILE          , "Достигнуто максимальное количество открытых файлов в системе." },
  { ENOBUFS         , "Недостаточно памяти для создания сокета. Сокет не может быть создан, пока не будет освобождено достаточное количество ресурсов." },
  { ENOMEM          , "Недостаточно памяти для создания сокета. Сокет не может быть создан, пока не будет освобождено достаточное количество ресурсов." },
  { EPROTONOSUPPORT , "Тип протокола или указанный протокол не поддерживаются в этом домене." },
  { EADDRINUSE      , "Локальный адрес уже используется." },
  { EADDRNOTAVAIL   , "(доменные сокеты Интернета) Сокет, указанный sockfd, ранее не был привязан к адресу и при попытке привязать его к эфемеридному порту, было определено, что все номера в  диапазоне  эфемеридных портов уже используются." },
  { EAGAIN          , "Недостаточно элементов в кэше маршрутизации." },
  { EALREADY        , "Сокет является неблокирующим, а предыдущая попытка установить соединение ещё не завершилась." },
  { EBADF           , "Файловый дескриптор не является правильными индексом в таблице дескрипторов." },
  { ECONNREFUSED    , "Ничто не слушает удалённый адрес." },
  { EFAULT          , "Адрес структуры сокета находится за пределами пользовательского адресного пространства." },
  { EINPROGRESS     , "Сокет  является неблокирующим, а соединение не может быть установлено немедленно. Можно использовать select(2) или poll(2), чтобы закончить соединение, установив ожидание возможности записи в"
"сокет. После того, как select(2) сообщит о такой возможности, используйте getsockopt(2), чтобы прочитать флаг SO_ERROR на уровне SOL_SOCKET, чтобы определить, успешно ли завершился  connect()"
"(в этом случае SO_ERROR равен нулю) или неудачно (тогда SO_ERROR равен одному из обычных кодов ошибок, перечисленных здесь, и объясняет причину неудачи)." },
  { EINTR           , "Системный вызов был прерван пойманным сигналом; см. signal(7)." },
  { EISCONN         , "Соединение на сокете уже произошло." },
  { ENETUNREACH     , "Сеть недоступна." },
  { ENOTSOCK        , "Файловый дескриптор не связан с сокетом." },
  { EPROTOTYPE      , "Тип сокета не поддерживается запрошенным протоколом связи. Это ошибка может возникать при попытке подключить доменнный датаграммный сокет UNIX к потоковому сокету." },
  { ETIMEDOUT       , "Произошел  тайм-аут  во время ожидания соединения. Сервер, возможно, очень занят и не может принимать новые соединения. Заметьте, что для IP-сокетов тайм-аут может быть очень длинным, если на сервере разрешено использование syncookies." },
  { ELOOP           , "При определении addr превышено количество переходов по символьной ссылке." },
  { ENAMETOOLONG    , "Аргумент addr слишком большой." },
  { ENOTDIR         , "Компонент в префиксе пути не является каталогом." },
  { EROFS           , "Попытка создания inode сокета на файловой системе, доступной только для чтения." },
  { ENXIO           , "Нет такого устройства или адреса" },
};

//------------------------------------------------------------------------------
const std::string               PROC_DIRECTORY          = std::string(DIR_SEPARATOR)+"proc"+DIR_SEPARATOR;
const std::string               PROC_CMDLINE_FILE       = std::string(DIR_SEPARATOR)+"cmdline";
const std::string               ETC_HOSTS_FILE          = std::string(DIR_SEPARATOR)+"etc"+DIR_SEPARATOR+"hosts";

const std::string               USER_ROOT               = "root";

const std::string               TMP_PATH                = std::string(DIR_SEPARATOR)+"tmp";
const std::string               PID_PATH                = std::string(DIR_SEPARATOR)+"run";
const std::string               LOG_PATH                = std::string(DIR_SEPARATOR)+"var"+DIR_SEPARATOR+"log";

const std::string               DATETIME_STR_FORMAT     = "dd.MM.yyyy HH.mm.ss";
const std::string               NET_MAC_STR_FORMAT      = "%02x-%02x-%02x-%02x-%02x-%02x";

const std::string               WARNING_TITLE           = "Внимание!";
const std::string               WARNING_APP_TITLE       = "Вариант приложения \"%1\" запущен ранее.";
const std::string               WROMG_APP_TITLE         = "Вариант приложения не создан.";
const std::string               WARNING_BTN_TITLE       = "Закрыть";

//------------------------------------------------------------------------------
const std::string               MAIN_ROOT               = std::string(DIR_SEPARATOR)+"home"+DIR_SEPARATOR+"aurora"+DIR_SEPARATOR+"app";

const std::string               PATH_BIN                = "bin";
const std::string               PATH_CONF               = "conf";
const std::string               PATH_CACHE              = "cache";
const std::string               PATH_DB                 = "db";
const std::string               PATH_LOG                = "log";
const std::string               PATH_LIB                = "lib";
const std::string               PATH_STYLES             = "styles";
const std::string               PATH_UPLOAD             = "upload";
const std::string               PATH_NAVDATA            = "navdata";
const std::string               PATH_CHARTS             = "charts";
const std::string               PATH_GEO                = "geo";

const std::string               MAIN_BIN                = MAIN_ROOT + DIR_SEPARATOR + PATH_BIN;
const std::string               MAIN_CONF               = MAIN_ROOT + DIR_SEPARATOR + PATH_CONF;
const std::string               MAIN_CACHE              = MAIN_ROOT + DIR_SEPARATOR + PATH_CACHE;
const std::string               MAIN_DB                 = MAIN_ROOT + DIR_SEPARATOR + PATH_DB;
const std::string               MAIN_LOG                = MAIN_ROOT + DIR_SEPARATOR + PATH_LOG;
const std::string               MAIN_LIB                = MAIN_ROOT + DIR_SEPARATOR + PATH_LIB;
const std::string               MAIN_STYLES             = MAIN_ROOT + DIR_SEPARATOR + PATH_STYLES;
const std::string               MAIN_UPLOAD             = MAIN_ROOT + DIR_SEPARATOR + PATH_UPLOAD;
const std::string               NAVDATA_UPLOAD          = MAIN_UPLOAD + DIR_SEPARATOR + PATH_NAVDATA;
const std::string               CHARTS_UPLOAD           = MAIN_UPLOAD + DIR_SEPARATOR + PATH_CHARTS;
const std::string               GEO_UPLOAD              = MAIN_UPLOAD + DIR_SEPARATOR + PATH_GEO;

const std::string               MAIN_CONF_DB            = MAIN_CONF + DIR_SEPARATOR + "db.conf";
const std::string               ENVOY_CONF_DB           = MAIN_CONF + DIR_SEPARATOR + "envoy.db.conf";
const std::string               UI_CONF_DB              = MAIN_CONF + DIR_SEPARATOR + "ui.db.conf";
const std::string               MAIN_CONF_SRV           = MAIN_CONF + DIR_SEPARATOR + "servers.conf";
const std::string               GEOJSON_SRC_FILE        = "sources-envoy-ui.json";
const std::string               GEOJSON_LRS_FILE        = "layers-envoy-ui.json";

//------------------------------------------------------------------------------
const std::string               AIRHELI_IMG_FILE_EXT    = ".jpg";

}// namespace htl

#endif // HTLTYPES_H

