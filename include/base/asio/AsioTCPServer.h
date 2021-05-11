#ifndef ASIOTCPSERVER_H
#define ASIOTCPSERVER_H

#include "AsioServer.h"
#include "AsioAcceptor.h"
#include "AsioCommon.h"

namespace                         htl   {
namespace                         asio  {

//------------------------------------------------------------------------------

/**
 * @brief Класс AsioTCPServer определяет атрибуты и методы работы TCP сервера по
 *        приему-передаче данных по сети.
 *
 * Класс реализует ассинхронные прием и/или передачу данных между TCP-сервером и
 * подключенными клиентами.
 *
 * Тип шаблона указывает на единицу информации буфера обмена типа @a spo::socket_byffer_t
 * По-умолчанию, назначается тип unsigned char (byte).
 *
 * Сервер реализует обновременную передачу данных от клиента к серверу и наоборот.
 * Т.о., сервер может не обрабатывать принимаемые данные и постоянно отправлять
 * некоторые пакеты к вновь подключенным клиентам: находится в активном
 * режиме передачи.
 * Сервер может и не передавать ничего по запросам клиентов, в то же время постоянно
 * принимать данные от них.
 *
 * Данные режимы реализуются алгоритмами в подключаемых обработчиках
 * буферов вода и вывода методом @a AsioTCPServer::SetBufferAction. Например,
 * если метод типа @a htl::asio::DataType::Input приема данных не назначен, а
 * метод типа @a htl::asio::DataType::Output активен, то
 * сервер будет принимать данные, но не включит их обработчик.
 * Если, например,
 * метод типа @a htl::asio::DataType::Output реализует непрерывную цепочку данных
 * к передаче, сервер будет постоянно формировать буфер отправки и рассылать
 * содержимое всем подключенным клиентам.
 *
 * Обработка данных из буферов обмена производится поочередно: ЧТЕНИЕ (Receive)
 * и ЗАПИСЬ (Send).
 * Выполнение чтения и записи по сокету идут в ассинхронном режиме.
 *
 * Манипулируя алгоритмами методов и их отключения или подключения можно добиться
 * перевода TCP-сервера в режимы:
 * @value SimplexOutput метод непрерывной или с задержками передачи данных к клиентам;
 * @value SimplexInput метод непрерывной или с задержками обработки данных от клиентов;
 * @value HalfDuplex метод последовательного прима и передачи данных между клиентами и сервером;
 * @value FullDuplex метод одновременных прима и передачи данных между клиентами и сервером;
 *
 * Методы класса используют технологию Boost.Coroutine реализации алгоритмов на
 * основе сопрограмм.
 *
 */
class COMMON_SHARED_EXPORT        AsioTCPServer :
public                            htl::asio::AsioServer< boost::asio::ip::tcp >
{
public:
  using self_t                  = htl::asio::AsioTCPServer;
  using tcp_t                   = boost::asio::ip::tcp;
  using base_class_t            = htl::asio::AsioServer< self_t::tcp_t >;
  using acceptor_type           = htl::asio::AsioAcceptor;

public:
  /**/                            AsioTCPServer
  (
      const htl::asio::port_t     port,
      const std::int64_t        & serviceTimeoutMs = ASIO_DEADLINE_MILLISEC_TYPE << 1
  );
                                ~ AsioTCPServer () override;

  bool                            IsOpen        () const noexcept;
  bool                            Start         ( bool asThread ) noexcept override;
  bool                            Stop          () noexcept override;

private:
  mutable acceptor_type           m_acceptor;
};

//------------------------------------------------------------------------------

}// namespace                     asio
}// namespace                     htl

#endif // ASIOTCPSERVER_H
