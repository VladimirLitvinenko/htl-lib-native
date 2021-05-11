#ifndef INOTIFYFILEMETA_H
#define INOTIFYFILEMETA_H

#include <InotifyManager.h>
#include <Logger.h>
#include <Utils.h>

namespace                         htl   {
namespace                         watch {

//------------------------------------------------------------------------------
//TODO: Зарезервировано к разработке

/**
 * @brief The InotifyFileMeta class
 */
class COMMON_SHARED_EXPORT        InotifyFileMeta
: public                          htl::watch::InotifyManager
{
public:
  using base_class_t            = htl::watch::InotifyManager;

  /**/                            InotifyFileMeta     () = default;
  /**/                          ~ InotifyFileMeta     () override;

  bool                            IsValid             () const;// override;

  template< typename Function_, typename ... Args_ >
  void                            SetExecuter         (  Function_ executer, Args_... params ) noexcept;

  template< typename Function_, typename ... Args_ >
  void                            Run
  (
      const htl::strings_t::value_type & path,
      htl::watch::NotifyFlags     flags,
      Function_                   executer,
      Args_                   ... params
  )
  {
    SetFlags    ( flags );
    SetExecuter ( std::forward<Function_>(executer), std::forward<Args_>(params)... );
    base_class_t::Start( htl::strings_t({ path }) );
  }

private:
  htl::utils::action_ptr  m_executer_ptr;
};

template<typename Function_, typename ... Args_>
void
InotifyFileMeta::SetExecuter(  Function_ executer, Args_ ... params)
noexcept
{
  m_executer_ptr =
      std::make_shared<htl::utils::action_p_t< Function_, Args_... > >
      ( std::forward< Function_ >( executer ),
        std::forward< Args_ >(params)... );
}

//------------------------------------------------------------------------------

}// namespace                     watch
}// namespace                     htl

#endif // INOTIFYFILEMETA_H
