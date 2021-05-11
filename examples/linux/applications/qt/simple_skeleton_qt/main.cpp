#include <AppUtils.h>
#include <application/QtAppSkeleton.h>

#include <QCoreApplication>

#include <iostream>

using namespace htl::app::qt;

int main( int argc, char ** argv )
{
//  if( htl::app::Create< htl::app::qt::QtAppSkeleton< QCoreApplication > >
//      ( argc, argv,
//        []( htl::app::qt::QtAppSkeleton< QCoreApplication > & a )
//        {
//          std::cout << "Run validator for Create." << std::endl;
//          // например:
//          return a.IsValid ();
//        },
//        false, false ) )
  {
    QtAppSkeleton<QCoreApplication> app( argc, argv );
    app.SetEexistsCallback (
    [ name = app.Name () ]()
    {
      std::cout << "Application \"" << name.c_str () << "\" exists. Terminate." << std::endl;
    });

    app.Exec(
          true,
          []( std::string name )
          {
            std::cout << "Execute "
                      << name.c_str ()
                      << " application."
                      << std::endl;
          },
          app.Name ()
    );
    return app.GetCode ();
  }
  return  EXIT_FAILURE;
}
