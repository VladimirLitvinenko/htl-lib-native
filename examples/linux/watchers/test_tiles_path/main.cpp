#include <QCoreApplication>
#include <InotifyFileMeta.h>
#include <Utils.h>
#include <iostream>
#include <boost/filesystem.hpp>

using namespace std;

int main( int argc, char* argv[] )
{
  QCoreApplication a( argc, argv );
  const std::string mtiles_path("/opt/mbtiles/");

  using namespace htl::watch;

  InotifyFileMeta fm;
  fm.SetErrorHandler( []()
  {
    DUMP_INFO( std::string( "SYSTEM ERROR:" ) + std::string( strerror( errno ) ) );
  });

  fm.SetEventHandler( []( const event_t & ev )
                      {
                        if( ( not ev.m_name.empty() ) and (ev.m_name.at(0) != '#') )
                        {
//                          InotifyFileMeta::DumpFlags( ev.m_flags );
                          if(
                             flag_exists( ev.m_flags, NotifyFlags::MovedTo )
                             or
                             flag_exists( ev.m_flags, NotifyFlags::MovedFrom )
                             or
                             flag_exists( ev.m_flags, NotifyFlags::CloseAfterWrite )
                             or
                             flag_exists( ev.m_flags, NotifyFlags::Delete )
                             or
                             flag_exists( ev.m_flags, NotifyFlags::DeleteSelf )
                          )
                          {
                            DUMP_INFO_CONSOLE( std::string("path: ") + ev.m_path + ", name: " + ev.m_name );
                            InotifyFileMeta::DumpFlags( ev.m_flags );
                          }
                        }
                      },
                      event_t() );

  fm.Run(
        mtiles_path,
        NotifyFlags::All,
        []( const std::string & path )
         {
           using namespace boost::filesystem;
           boost::filesystem::path p = path;
           directory_iterator it{p};
           while (it != directory_iterator{})
             std::cout << "\t" << *it++ << '\n';
           std::flush( std::cout );
         },
        mtiles_path );


  boost::filesystem::path t_dir(mtiles_path);
  std::cout << t_dir.root_name() << '\n';
  std::cout << t_dir.root_directory() << '\n';
  std::cout << t_dir.root_path() << '\n';
  std::cout << t_dir.relative_path() << '\n';
  std::cout << t_dir.parent_path() << '\n';
  std::cout << t_dir.filename() << '\n';
  std::flush( std::cout );

  return a.exec();
}
