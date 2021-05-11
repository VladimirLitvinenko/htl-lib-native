#ifndef QTAPPSKELETON_H
#define QTAPPSKELETON_H

#include <AppSkeleton.h>
#include <QApplication>
#include <QGuiApplication>
#include <QPointer>
#include <Logger.h>
#include <memory>
#include <experimental/optional>
//#include <stdlib.h>

namespace                         htl   {
namespace                         app   {
namespace                         qt    {

//------------------------------------------------------------------------------
/**
 * @brief Класс AppSkeleton
 */
template< typename                ApplicationT_ >
class COMMON_SHARED_EXPORT        QtAppSkeleton
: public                          htl::app::AppSkeleton
{
public:
  using type                    = typename std::decay<ApplicationT_ >::type;
  using base_class_t            = htl::app::AppSkeleton;

  static_assert ( std::is_base_of< QCoreApplication, type >::value,
                  ST_ASSERT_BASE_CLASS_VALIDATOR );

private:
  mutable QPointer<ApplicationT_> m_application;
  void                            SetBlockMode    ( bool = true ) override final {}
  void                            Init
  (
      bool                        unique,
      int                         argc,
      char                     ** argv
  ) noexcept;

public:
  template< typename          ... Args_>
  /**/                            QtAppSkeleton
  (
      bool                        unique,
      int                         argc,
      char                     ** argv
  ) : QtAppSkeleton::base_class_t( argc, argv )
  {
    Init(unique,argc,argv);
  }
  /**/                            QtAppSkeleton   ( const QtAppSkeleton & ) = delete;
  /**/                            QtAppSkeleton   ( QtAppSkeleton && app ) = delete;
                                ~ QtAppSkeleton   () override { Delete (); }

  bool                            IsValid         () const override;
  bool                            BlockMode       () const override final { return true; }

  void                            Start           ()            override;
  void                            Delete          ()            override;
  void                            Quit            ( int )       override;
  void                            Abort           ( int code )  override;
  int                             Exec            ()            override;

  ApplicationT_                 * AppPtr          () const
  {
    m_application = dynamic_cast<ApplicationT_*>( QCoreApplication::instance() );
    return m_application.data();
  }
};

//------------------------------------------------------------------------------
template<typename ApplicationT_>
bool
QtAppSkeleton< ApplicationT_ >::IsValid() const
{
  return
      ( nullptr != AppPtr() )
      and
      QtAppSkeleton< ApplicationT_ >::base_class_t::IsValid ()
      ;
}

template<typename ApplicationT_>
void
QtAppSkeleton<ApplicationT_>::Start()
{
  DUMP_INFO(__PRETTY_FUNCTION__ );
  Q_ASSERT( IsValid() );
  if( IsUnique() and ExistsCallback().second )
  {
    Quit( EXIT_FAILURE );
    return;
  }

  DUMP_INFO_CONSOLE( "Try Start..." );
  AppSkeleton::SetStatus( app_state::status::Run );
  m_action_ptr->Run();
}

template< typename ApplicationT_ >
void
QtAppSkeleton< ApplicationT_ >::Delete()
{
  Stop();
  if( IsValid () )
  {
    AppPtr()->deleteLater ();
  }
}

template< typename ApplicationT_ >
void
QtAppSkeleton< ApplicationT_ >::Quit(int)
{
  DUMP_INFO(std::string(" Interrupt signal: terminale as QUIT."));
  IsValid()
      ? AppPtr()->quit ()
      : ::_exit ( EXIT_FAILURE );
  ::_exit ( EXIT_FAILURE );
}

template< typename ApplicationT_ >
void
QtAppSkeleton< ApplicationT_ >::Abort(int code)
{
  DUMP_INFO(std::string(" Interrupt signal: terminale as ABORT."));
  IsValid()
      ? AppPtr()->exit ( code )
      : ::_exit ( EXIT_FAILURE );
  ::_exit ( EXIT_FAILURE );
}

template<typename ApplicationT_>
int
QtAppSkeleton< ApplicationT_ >::Exec()
{
  return
      IsValid()
        ? AppPtr()->exec()
        : GetCode();
}

template<typename ApplicationT_>
void QtAppSkeleton<ApplicationT_>::Init
(
    bool     unique,
    int      argc,
    char  ** argv
) noexcept
{
  DUMP_INFO(__PRETTY_FUNCTION__);
  SetUnique(unique);
  m_application = std::forward<ApplicationT_*>(new ApplicationT_( argc, argv ));
  if( IsUnique() and ExistsCallback().second )
  {
    Abort( EXIT_FAILURE );
  }
  InitSignalsDefault ();
}

//------------------------------------------------------------------------------

}// namespace                     qt
}// namespace                     app
}// namespace                     htl

#endif // QTAPPSKELETON_H
