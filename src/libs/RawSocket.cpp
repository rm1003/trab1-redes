#include "RawSocket.hpp"
#include "Logging.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>

extern "C" {
#include <sys/socket.h>
#include <unistd.h>
}

// Given in miliseconds
long TIMEOUT_LEN;

CustomSocket::RawSocket::RawSocket(const char *ethInterfaceName, bool setTimeout) {
  if (this->CreateSocket(ethInterfaceName)) {
    exit(1);
  }
  if (setTimeout) {
    TIMEOUT_LEN = 100;
  } else {
    TIMEOUT_LEN = 0;
  }
  this->SetRecvTimeout();
}

CustomSocket::RawSocket::~RawSocket() {
  close(this->socketFd);
}

int CustomSocket::RawSocket::Send(void *ptr, size_t len) {
  int ret;
  ret = send(this->socketFd, ptr, len, 0);
  return ret;
}

int CustomSocket::RawSocket::Recv(void *ptr, size_t len) {
  int ret;
  ret = recv(this->socketFd, ptr, len, 0);
  return ret;
}

int CustomSocket::RawSocket::CreateSocket(const char *nome_interface_rede) {
  int ret;
  // Cria arquivo para o socket sem qualquer protocolo
  this->socketFd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (this->socketFd == -1) {
    ERROR_PRINT("Erro ao criar socket: Verifique se você é root!\n")
    return 1;
  }

  int ifindex = if_nametoindex(nome_interface_rede);

  struct sockaddr_ll endereco = {0};
  endereco.sll_family = AF_PACKET;
  endereco.sll_protocol = htons(ETH_P_ALL);
  endereco.sll_ifindex = ifindex;
  // Inicializa socket
  ret = bind(this->socketFd, (struct sockaddr*) &endereco, sizeof(endereco));
  if (ret == -1) {
      fprintf(stderr, "Erro ao fazer bind no socket\n");
      return 1;
  }

  struct packet_mreq mr = {0};
  mr.mr_ifindex = ifindex;
  mr.mr_type = PACKET_MR_PROMISC;
  // Não joga fora o que identifica como lixo: Modo promíscuo
  ret = setsockopt(this->socketFd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mr,
                  sizeof(mr));
  if (ret == -1) {
    fprintf(stderr, "Erro ao fazer setsockopt: "
        "Verifique se a interface de rede foi especificada corretamente.\n");
    return 1;
  }

  return 0;
}

void CustomSocket::RawSocket::SetRecvTimeout() {
  struct timeval timeout;
  timeout = {.tv_sec = TIMEOUT_LEN/1000, .tv_usec = (TIMEOUT_LEN%1000)*1000};
  setsockopt(this->socketFd, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout,
            sizeof(timeout));
}
