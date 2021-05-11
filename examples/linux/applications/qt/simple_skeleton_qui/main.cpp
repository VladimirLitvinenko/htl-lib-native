#include <application/QtAppSkeleton.h>
#include <QApplication>
#include <QMessageBox>

using namespace htl::app::qt;

void show_message( QtAppSkeleton<QApplication> & )
{
  if( QApplication::instance () )
  QMessageBox::information (nullptr,"GUI Simple example.", "enter kill -s 3 <app pid>.");
}

int main( int argc, char ** argv )
{
  QtAppSkeleton<QApplication> app( argc, argv );

  app.SetEexistsCallback (
  [ & ]()
  {
    QMessageBox::information (nullptr,"GUI Simple WARNING.",
                              QString(" GUI application \"%1\"  exists.").
                              arg (app.Name ().c_str ()));
  });

  app.Exec( true, show_message, app );

  return app.GetCode ();
}
