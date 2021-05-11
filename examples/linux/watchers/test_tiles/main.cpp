#include <MBTilesWatcher.h>
#include <Logger.h>
#include <Utils.h>
#include <iostream>
#include <boost/filesystem.hpp>

using namespace std;

int main()
{
  const std::string mtiles_path("/opt/mbtiles/"); // lasy... :)
//  int count = 100;

  using namespace htl::tiles;
  using namespace htl::watch;

  MBTilesWatcher fm(mtiles_path);
  fm.SetErrorHandler( []( notify_error_t err, const std::string & str )
  {
    DUMP_INFO( std::string( "SYSTEM ERROR:" ) + std::string( strerror( err ) ) );
    DUMP_INFO( std::string( "DETAIL: " ) + str );
  },
  notify_error_t(), std::string() );


//  boost::filesystem::directory_iterator it{ boost::filesystem::path( mtiles_path ) };
//  htl::watch::notify_actions_t actions;
//  actions.push_back (
//        std::make_pair(
//          mtiles_path,
//          [ &fm, &count ]( const std::string & file, htl::watch::NotifyFlags flag )
//          {
//            std::cout << "file " << file.c_str () << " action: ";
//            fm.DumpFlags ( flag );
//            if( count-- < 1 )
//              fm.Stop ();

//          } ) );
//  while (it != boost::filesystem::directory_iterator{})
//  {
//    actions.push_back (
//          std::make_pair(
//            (*it++).path ().string (),
//            [ &fm, &count ]( const std::string & file, htl::watch::NotifyFlags flag )
//            {
//              std::cout << "file " << file.c_str () << " action: ";
//              fm.DumpFlags ( flag );
//              if( count-- < 1 )
//                fm.Stop ();

//            } ) );
//  }
//  fm.SetFlags ( {
//        NotifyFlags::Open,
//        NotifyFlags::Close,
//        NotifyFlags::IsDir ,
//        NotifyFlags::Create ,
//        NotifyFlags::Delete ,
//        NotifyFlags::DeleteSelf ,
//        NotifyFlags::Modify ,
//        NotifyFlags::MovedTo ,
//        NotifyFlags::MoveSelf ,
//        NotifyFlags::MovedFrom, } );

  fm.Start ();

  while( fm.IsActive () );

  std::cout << "Watcher stoped..." << '\n';
  std::flush( std::cout );
  return 0;
}
