// PHZ
// 2018-5-15

#ifndef XOP_TCP_SOCKET_H
#define XOP_TCP_SOCKET_H

#include <cstdint>
#include <string>
#include "Socket.h"

namespace xop
{
    
class TcpSocket
{
public:
    TcpSocket(SOCKET sockfd=-1);
    virtual ~TcpSocket();

    SOCKET create();
    bool bind(const std::string& ip, uint16_t port) const;
    bool listen(int backlog) const;
    SOCKET accept() const;
    bool connect(const std::string& ip, uint16_t port, int timeout=0) const;
    void close();
    void shutdownWrite();

    SOCKET fd() const { return _sockfd; }
    
private:
    SOCKET _sockfd = -1;
};

}

#endif



