#include "Dispatch.h"

class Dispatch : public boost::enable_shared_from_this<Dispatch>,
               boost::noncopyable
{
 public:
  void setup()
  {
    client_.setConnectionCallback(
        boost::bind(&Dispatch::onClientConnection, shared_from_this(), _1));
    //client_.setMessageCallback(
      //  boost::bind(&Dispatch::onClientMessage, shared_from_this(), _1, _2, _3));
  }

  void teardown()
  {
    client_.setConnectionCallback(muduo::net::defaultConnectionCallback);
    client_.setMessageCallback(muduo::net::defaultMessageCallback);
    if (serverConn_)
    {
      serverConn_->setContext(boost::any());
      serverConn_->shutdown();
    }
  }

  void connect()
  {
    client_.connect();
  }

  void disconnect()
  {
    client_.disconnect();
    // serverConn_.reset();
  }

  void onClientConnection(const muduo::net::TcpConnectionPtr& conn)
  {
    LOG_DEBUG << (conn->connected() ? "UP" : "DOWN");
    if (conn->connected())
    {
      conn->setTcpNoDelay(true);
      conn->setHighWaterMarkCallback(
          boost::bind(&Dispatch::onHighWaterMarkWeak, boost::weak_ptr<Dispatch>(shared_from_this()), _1, _2),
          10*1024*1024);
      serverConn_->setContext(conn);
      if (serverConn_->inputBuffer()->readableBytes() > 0)
      {
        conn->send(serverConn_->inputBuffer());
      }
    }
    else
    {
      teardown();
    }
  }

  void onClientMessage(const muduo::net::TcpConnectionPtr& conn,
                       muduo::net::Buffer* buf,
                       muduo::Timestamp)
  {
    LOG_DEBUG << conn->name() << " " << buf->readableBytes();
    if (serverConn_)
    {
      serverConn_->send(buf);
    }
    else
    {
      buf->retrieveAll();
      abort();
    }
  }

  void onHighWaterMark(const muduo::net::TcpConnectionPtr& conn,
                       size_t bytesToSent)
  {
    LOG_INFO << "onHighWaterMark " << conn->name()
             << " bytes " << bytesToSent;
    disconnect();
  }

  static void onHighWaterMarkWeak(const boost::weak_ptr<Dispatch>& wkTunnel,
                                  const muduo::net::TcpConnectionPtr& conn,
                                  size_t bytesToSent)
  {
    boost::shared_ptr<Dispatch> tunnel = wkTunnel.lock();
    if (tunnel)
    {
      tunnel->onHighWaterMark(conn, bytesToSent);
    }
  }
};

#endif

