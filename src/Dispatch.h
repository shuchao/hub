#ifndef MUDUO_EXAMPLES_SOCKS4A_TUNNEL_H
#define MUDUO_EXAMPLES_SOCKS4A_TUNNEL_H

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpClient.h>
#include <muduo/net/TcpServer.h>

#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

class Dispatch : public boost::enable_shared_from_this<Dispatch>,
               boost::noncopyable
{
 public:
  Dispatch(muduo::net::EventLoop* loop,
         const muduo::net::InetAddress& serverAddr,
         const muduo::net::TcpConnectionPtr& serverConn)
    : client_(loop, serverAddr, serverConn->name()),
  {
    LOG_INFO << "Dispatch " << serverConn->peerAddress().toIpPort()
             << " <-> " << serverAddr.toIpPort();
  }

  ~Dispatch()
  {
    LOG_INFO << "~Dispatch";
  }

  void setup(void);
  
  void teardown(void);

  void connect(void);
  void disconnect();
  void onClientConnection(const muduo::net::TcpConnectionPtr& conn);
  

  void onClientMessage(const muduo::net::TcpConnectionPtr& conn,
                       muduo::net::Buffer* buf,
                       muduo::Timestamp);
  

  void onHighWaterMark(const muduo::net::TcpConnectionPtr& conn,
                       size_t bytesToSent);
  

  static void onHighWaterMarkWeak(const boost::weak_ptr<Dispatch>& wkTunnel,
                                  const muduo::net::TcpConnectionPtr& conn,
                                  size_t bytesToSent);
  

 private:
  muduo::net::TcpClient client_;
};
typedef boost::shared_ptr<Dispatch> TunnelPtr;

#endif


