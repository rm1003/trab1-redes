#ifndef RAWSOCKET_HPP_
#define RAWSOCKET_HPP_

#include <arpa/inet.h>
extern "C" {
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <stdlib.h>
}

namespace CustomSocket {

  class RawSocket {
    private:
      int socketFd;

      int CreateSocket(const char *netIntName);
      void SetRecvTimeout(long timeout);
    public:
      RawSocket(const char *netIntName, long timeout);
      ~RawSocket();
      int Send(void *ptr, size_t len);
      int Recv(void *ptr, size_t len);
  };

}

#endif