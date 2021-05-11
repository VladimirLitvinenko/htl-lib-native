/**
  * @file InotifyManager.h
  *
  * @brief Файл назначения типов и классов для работы с сообщениями об изменении
  *        каталогов и файлов Linux на уровне ядпа.
  *
  * @warning  Необходимо установить достаточное значение обработчика сообщений
  *           уровня ядра Linux.
  *           Например, при наличии разработчика в группе astra-admin OС Astra Linux 1.5 SE,
  *           можно в файле проекта qmake добавить установку признака
  *           INOTIFY_COMPLETE=$$system('sudo sysctl -w fs.inotify.max_user_watches=65536')
  *
  * @warning  Необходимо установить библиотеку inotifytools. Например, для
  *           OС Astra Linux 1.5 SE выполнить:
  *           sudo apt-get install -f inotifytools
  *           В файле проекта qmake необходимо добавить установку переменной
  *           LIBS qmake:
  *           LIBS += -linotifytools
  */
#ifndef INOTIFYMANAGER_H
#define INOTIFYMANAGER_H

#include "HtlGlobal"
#include "Utils.h"

#include <inotifytools/inotifytools.h>
#include <inotifytools/inotify.h>

#include <boost/bind/bind.hpp>

#include <set>

namespace                         htl   {
namespace                         watch {

//------------------------------------------------------------------------------
enum class                        UploadNotify : std::uint8_t
{
  FileAdd     = 0,
  FileRemove  = 1,
};

//------------------------------------------------------------------------------
/**
 * @brief Класс перечислений NotifyFlag определяет флаги сообщений при изменении
 *        состояния файлов и каталогов.
 *
 *  \note В аргументе inotify_add_watch(2) mask и поле mask структуры inotify_event,
 *  возвращаемых при чтении файлового дескриптора inotify, содержатся битовые маски,
 *  определяющие  события inotify.
 *  Следующие биты могут быть заданы в mask при вызове inotify_add_watch(2) и
 *  возвращены в поле mask, возвращаемом read(2):
 *  \value IN_ACCESS (+) Был произведён доступ к файлу (например, read(2), execve(2)).
 *  \value IN_ATTRIB (*) Изменились  метаданные  —  например,  права доступа
 *  (например, chmod(2)), отметки времени (например, utimensat(2)),
 *  расширенные атрибуты (setxattr(2)), счётчик ссылок (начиная с Linux 2.6.25;
 *  например, для аргумента назначения link(2) и unlink(2)) и идентификатор
 *  пользователя/группы (например, chown(2)).
 *  \value IN_CLOSE_WRITE (+) Файл, открытый для записи, был закрыт.
 *  \value IN_CLOSE_NOWRITE (*) Файл или каталог, не открытый для записи, был закрыт.
 *  \value IN_CREATE (+) В отслеживаемом каталоге был создан файл/каталог
 *  (например, open(2) O_CREAT, mkdir(2), link(2), symlink(2), bind(2) для
 *  доменного сокета UNIX).
 *  \value IN_DELETE (+) В отслеживаемом каталоге был удалён файл/каталог.
 *  \value IN_DELETE_SELF Отслеживаемый файл/каталог был удалён (это событие
 *  также возникает, если объект перемещён в другую файловую систему, так как mv(1),
 *  фактически, копирует файл в другую файловую систему и удаляет его из исходной).
 *  Также затем будет создано событие IN_IGNORED для дескриптора сторожка.
 *  \value IN_MODIFY (+) Файл был изменён (например, write(2), truncate(2)).
 *  \value IN_MOVE_SELF Отслеживаемый файл/каталог был перемещён.
 *  \value IN_MOVED_FROM (+) При переименовании генерируется для каталога,
 *  содержащего старое имя файла.
 *  \value IN_MOVED_TO (+) При переименовании генерируется для каталога,
 *  содержащего новое имя файла.
 *  \value IN_OPEN (*) Файл или каталог был открыт.
 *
 *  При наблюдении за каталогом:
 *  *  события, помеченные звёздочкой (*), могут возникать как для самого
 *  каталога, так и для файлов в каталоге; и
 *  *  события, помеченные знаком плюс (+), могут возникать только для объектов
 *  внутри каталога (но не самого каталога).
 *
 *  Когда события генерируются для объектов внутри отслеживаемого каталога,
 *  поле name, возвращаемое в структуре inotify_event, хранит имя файла внутри
 *  этого каталога.
 *
 *  Макрос IN_ALL_EVENTS определён как битовая маска всех перечисленных выше
 *  событий. Данный макрос можно использовать в качестве аргумента mask в
 *  вызове inotify_add_watch(2).
 *
 *  Дополнительно, два удобных макроса:
 *  \value IN_MOVE То же, что и IN_MOVED_FROM | IN_MOVED_TO.
 *  \value IN_CLOSE То же, что и IN_CLOSE_WRITE | IN_CLOSE_NOWRITE.
 *
 *  Также, при вызове inotify_add_watch(2) в mask могут быть указаны следующие биты:
 *  \value IN_DONT_FOLLOW (начиная с Linux 2.6.15) Не разыменовывать pathname,
 *  если это символическая ссылка.
 *  \value IN_EXCL_UNLINK (начиная с Linux 2.6.36) По  умолчанию,  при  слежении
 *  за  событиями  для  потомков  каталога, события генерируются для потомков
 *  даже после того, как они будут удалены из каталога. Это может привести к
 *  большому количеству ненужных для приложения событий (например, если следить
 *  за /tmp, в котором многие приложения создают и сразу удаляют временные  файлы).
 *  Указание IN_EXCL_UNLINK изменяет поведение по умолчанию, и такие события
 *  не генерируются для потомков после удаления из отслеживаемого каталога.
 *  \value IN_MASK_ADD Если  экземпляр  слежения  уже существует для объекта
 *  файловой системы соответствующего pathname, то выполнять добавление (OR)
 *  событий в mask к маске слежения (вместо замены маски).
 *  \value IN_ONESHOT Отслеживать объект файловой системы, соответствующий
 *  pathname до одного события, затем удалить объект из списка слежения.
 *  \value IN_ONLYDIR (начиная с Linux 2.6.15) Следить за pathname, только если
 *  это каталог. Этот флаг предоставляет приложению бессостязательный способ
 *  убедиться, что отслеживаемый объект — каталог.
 *  \value IN_IGNORED Слежение было снято явно (inotify_rm_watch(2)) или
 *  автоматически (файл был удалён или размонтирована файловая система).
 *  Также смотрите ДЕФЕКТЫ.
 *  \value IN_ISDIR Объект этого события — каталог.
 *  \value IN_Q_OVERFLOW Переполнена очередь событий (для этого события значение
 *  wd равно -1).
 *  \value IN_UNMOUNT Файловая система, содержащая отслеживаемый объект, была
 *  размонтирована. Также, будет сгенерировано событие IN_IGNORED для дескриптора
 *  сторожка.
 *
 *  \note   Интерфейсы в /proc
 *  Для ограничения потребления inotify памяти ядра, можно использовать
 *  следующие интерфейсы:
 *  \value /proc/sys/fs/inotify/max_queued_events  Значение  в  этом  файле
 *  используется  когда  приложение  вызывает  inotify_init(2) для установки
 *  верхнего порога количества событий, которые могут поместиться в очередь
 *  соответствующего экземпляра inotify. События, превысившие это ограничение,
 *  отбрасываются, но событие IN_Q_OVERFLOW генерируется всегда.
 *  \value /proc/sys/fs/inotify/max_user_instances В этом файле задаётся
 *  ограничение на количество экземпляров inotify, которые могут быть созданы
 *  для одного реального идентификатора пользователя.
 *  \value /proc/sys/fs/inotify/max_user_watches В этом файле задаётся ограничение
 *  на количество сторожков, которые могут быть созданы для одного реального
 *  идентификатора пользователя.
 *
 * @see inotifytools/inotify.h
 */
enum class                        NotifyFlags
{
  Access                        = IN_ACCESS,
  Modify                        = IN_MODIFY,
  Attributes                    = IN_ATTRIB,
  CloseAfterWrite               = IN_CLOSE_WRITE,
  CloseAfterNoWrite             = IN_CLOSE_NOWRITE,
  Open                          = IN_OPEN,
  Close                         = IN_CLOSE,
  MovedFrom                     = IN_MOVED_FROM,
  MovedTo                       = IN_MOVED_TO,
  Create                        = IN_CREATE,
  Delete                        = IN_DELETE,
  DeleteSelf                    = IN_DELETE_SELF,
  MoveSelf                      = IN_MOVE_SELF,

  FileWasIgnored                = IN_IGNORED,
  IsDir                         = IN_ISDIR,
  Unmount                       = IN_UNMOUNT,
  Overflow                      = IN_Q_OVERFLOW,

  NonBlock                      = IN_NONBLOCK,
  All                           = IN_ALL_EVENTS,
};

constexpr bool flag_exists( const NotifyFlags flags, const NotifyFlags flag )
{
  return ( static_cast<int>(flags) & static_cast<int>(flag) ) != int(0);
}
//------------------------------------------------------------------------------

using notify_error_t            = std::remove_reference< decltype(errno) >::type;

//------------------------------------------------------------------------------
/**
 *
 */
using notify_handlers_t =
struct notify_handlers
{
  htl::utils::action_ptr m_OnError;
  htl::utils::action_ptr m_Run;
  htl::utils::action_ptr m_OnStop;
  htl::utils::action_ptr m_OnEvent;
};

//using event_action_map          = std::map< std::string, htl::utils::action_ptr >;
/**
 * @brief Структура event_t
 */
struct event_t
{
  int         m_wd    {0};
  NotifyFlags m_flags {};
  std::string m_path;
  std::string m_name;

  event_t () = default;
  event_t ( inotify_event * ev )  { *this = ev; }
  event_t ( const event_t & e )   { *this = e;  }
  event_t ( event_t && e )        { *this = std::move( e ); }
  void clear()
  {
    m_wd    = 0;
    m_flags = NotifyFlags();
    m_path  . clear();
    m_name  . clear();
  }
  std::string absolute_path() const
  {
    return  m_path + m_name;
  }
  virtual event_t & operator=( inotify_event * ev )
  {
    clear();
    if( ( nullptr != ev ) and ( ev->wd > 0 ) )
    {
      m_wd = ev->wd;
      m_flags = static_cast<NotifyFlags>(ev->mask);
      auto ch_path( inotifytools_filename_from_wd( m_wd ) );
      if( nullptr != ch_path )
      {
        m_path = std::string( ch_path );
        if( ev->len > 0 )
          m_name = std::string( ev->name );
      }
    }
    return std::ref( * this );
  }
  virtual event_t & operator=( const event_t & e )
  {
    m_wd    = e.m_wd;
    m_flags = e.m_flags;
    m_path  = e.m_path;
    m_name  = e.m_name;
    return std::ref( * this );
  }
  virtual event_t & operator=( event_t && e )
  {
    m_wd    = e.m_wd;
    m_flags = e.m_flags;
    m_path  . swap( e.m_path );
    m_name  . swap( e.m_name);
    e       . clear();
    return std::ref( * this );
  }
};


class COMMON_SHARED_EXPORT        InotifyManager;
using file_handler_t            = std::function< void(InotifyManager &, const std::string & ) >;

//------------------------------------------------------------------------------
/**
 * @brief Класс InotifyManager
 *
 * @see man(7) inotify
 *
 * @note: notify - наблюдает за событиями файловой системы.
 *  Программный  интерфейс inotify предоставляет механизм для слежения за событиями
 *  в файловой системе. Его можно использовать для слежения за отдельными файлами
 *  или каталогами. При слежении за каталогами inotify возвращает события как
 *  для самого каталога, так и для файлов внутри каталога.
 *
 *  В программный интерфейс входят следующие системные вызовы:
 *
 *  *  Вызов inotify_init(2) создаёт экземпляр inotify и возвращает файловый
 *  дескриптор, ссылающийся на экземпляр inotify. Более новый inotify_init1(2)
 *  подобен  inotify_init(2),  но имеет аргумент flags, который предоставляет
 *  доступ к некоторым дополнительным возможностям.
 *
 *  *  Вызов  inotify_add_watch(2)  изменяет  «список  наблюдения»,  связанный
 *  с  экземпляром inotify. Каждый элемент (сторожок (watch)) в списке задаёт
 *  путь к файлу или каталогу и некоторый набор событий, которые ядро должно
 *  отслеживать для файла, на который указывает этот путь.
 *  Вызов  inotify_add_watch(2)  или  создаёт  новый  сторожок,  или  изменяет
 *  существующий. Каждый сторожок имеет уникальный «дескриптор сторожка» — целое
 *  число, возвращаемое inotify_add_watch(2) при создании сторожка.
 *
 *  *  При  возникновении  событий  с  отслеживаемыми  файлами  и  каталогами,
 *  эти события становятся доступны приложению в виде структурированных данных,
 *  которые можно прочитать из файлового дескриптора inotify с помощью read(2)
 *  (смотрите ниже).
 *
 *  *  Вызов inotify_rm_watch(2) удаляет элемент из списка наблюдения inotify.
 *
 *  *  При закрытии (с помощью close(2)) файловых дескрипторов, ссылающихся на
 *  экземпляр inotify, этот объект и его ресурсы освобождаются для  повторного
 *  использования  ядром;  все связанные сторожки освобождаются автоматически.
 *
 *  При  корректном  программировании,  приложение  может  эффективно использовать
 *  inotify для слежения и кэширования состояния набора объектов файловой системы.
 *  Однако, в тщательно проработанных приложениях нужно предполагать наличие
 *  ошибок в логике слежения или состязательности,  описанных  далее,  которые
 *  могут  приводить  к  рассогласованности  кэша  с состоянием файловой системы.
 *  Вероятно, лучше сделать некоторую проверку и перестроить кэш при обнаружении
 *  рассогласованности.
 */
class COMMON_SHARED_EXPORT        InotifyManager
{
public:

  /**/                            InotifyManager        () = default;
  virtual                       ~ InotifyManager        ();

  bool                            IsActive              () const;
  bool                            IsRecurcive           () const;

  void                            SetRecurcive          ( bool recursive );
  NotifyFlags                     Flags                 () const noexcept;
  void                            SetFlags              ( NotifyFlags flags ) noexcept;
  void                            AddFlag               ( NotifyFlags flag ) noexcept;
  void                            AddPath               ( const std::string & p ) noexcept;
  htl::strings_t                  GetPathes             () const;
  std::size_t                     PathesSize            () const;
  bool                            PathExists            ( const std::string & path ) const;

  void                            SetExcluded           ( const htl::strings_t & excluded = {} ) noexcept;
  void                            SetIgnore             ( const std::string & ignore_by_regex = "" ) noexcept;

  template< typename Function_, typename ... Args_ >
  void                            SetErrorHandler       ( Function_&& method, Args_... params ) noexcept;
  template< typename Function_, typename ... Args_ >
  void                            SetStopHandler        ( Function_&& method, Args_... params ) noexcept;
  template< typename Function_, typename ... Args_ >
  void                            SetEventHandler
  (
      Function_                && method,
      Args_                   ... params
  ) noexcept;

  /**
   * @brief Method Do
   * @param actions
   * @param flags
   * @param excluded
   * @param ignore_by_regex
   */
  virtual void                    Start                 ( const htl::strings_t & pathes = {} );
  virtual void                    Stop                  () noexcept;
  void                            Restart               ( const htl::strings_t & pathes = {} ) noexcept
  {
    Stop();
    Start(pathes);
  }

  static std::string              DumpStdString         ( NotifyFlags );
  static void                     DumpFlags             ( NotifyFlags );

protected:
  htl::strings_t                & GetPathesRef          ();
  void                            SetError              ( int error );
  bool                            Init                  ( bool recursive ) noexcept;

private:
  std::atomic_int                 m_inotify_descriptor  { 0 };
  NotifyFlags                     m_Flags               { NotifyFlags::All };
  htl::strings_t                  m_pathes;
  htl::strings_t                  m_excluded;
  std::string                     m_ignore_by_regex;
  std::atomic_bool                m_recurcive           { false };
  notify_handlers_t               m_Handlers;
  htl::threadptr_t                m_thread_runner;

  template< typename Function_, typename ... Args_ >
  void                            SetRunHandler         ( Function_&& method, Args_... params ) noexcept;
  template< typename Function_, typename ... Args_ >
  void                            SetHandler
  (
      htl::utils::action_ptr    & action,
      Function_                && method,
      Args_                   ... params
  )
  noexcept;
};

//------------------------------------------------------------------------------

template< typename Function_, typename ... Args_ >
void
InotifyManager::SetHandler
(
    htl::utils::action_ptr  & action,
    Function_              && method,
    Args_...                  params
)
noexcept
{
  action =
      std::make_shared<utils::action_p_t<Function_, Args_... > >
      ( std::forward< Function_&& >( method ),
        std::forward< Args_ >(params)... );
}

template< typename Function_, typename ... Args_ >
void
InotifyManager::SetRunHandler( Function_&& method, Args_... params ) noexcept
{
  SetHandler<Function_,Args_...>( std::ref(m_Handlers.m_Run),
                                  std::forward<Function_&&>(method),
                                  std::forward<Args_>(params)... );
  m_thread_runner =
      std::make_shared<htl::threadptr_t::element_type>(
      []( InotifyManager & inm )
      {
        if( inm.m_Handlers.m_Run )
        {
          inm.m_Handlers.m_Run->Run();
        }
      }, std::ref( * this ) );
}

template< typename Function_, typename ... Args_ >
void
InotifyManager::SetErrorHandler( Function_&& method, Args_... params ) noexcept
{
  SetHandler<Function_,Args_...>( std::ref(m_Handlers.m_OnError),
                                  std::forward<Function_&&>(method),
                                  std::forward<Args_>(params)... );
}
template< typename Function_, typename ... Args_ >
void
InotifyManager::SetStopHandler( Function_&& method, Args_... params ) noexcept
{
  SetHandler<Function_,Args_...>( std::ref(m_Handlers.m_OnStop),
                                  std::forward<Function_&&>(method),
                                  std::forward<Args_>(params)... );
}

template<typename Function_, typename ... Args_>
void
InotifyManager::SetEventHandler
(
    Function_&&         method,
    Args_           ... params
)
noexcept
{
  SetHandler<Function_,Args_...>( std::ref(m_Handlers.m_OnEvent),
                                  std::forward<Function_&&>(method),
                                  std::forward<Args_>(params)... );
}

}// namespace                     watch
}// namespace                     htl

Q_DECLARE_METATYPE(htl::watch::UploadNotify)

#endif // INOTIFYMANAGER_H
