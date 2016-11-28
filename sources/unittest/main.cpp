#include <module/simplesocket/simplesocket.hpp>
#include <module/common/common.hpp>

using namespace module;

class CUDPServerTest :
        public CUDPServer {
public:
    CUDPServerTest() {

    }

    ~CUDPServerTest() {

    }

private:
    int OnRead(void *pdata, size_t szlen, SOCKADDR addr) {
        printf("udp: %lld %s\r\n", GetTickTime(), (char *) pdata);

        return 0;
    }
};

class CTCPServerTest :
        public CTCPServer {
public:
    CTCPServerTest() {

    }

    ~CTCPServerTest() {

    }

public:
    int OnRead(void *pdata, size_t szlen, SOCKET fd_use) {
        printf("tcp: %lld %s\r\n", GetTickTime(), (char *) pdata);

        return 0;
    }

    int OnAccept(SOCKET fd_use, SOCKADDR addr) {
        return 0;
    }

    int OnClose(SOCKET fd_use) {
        return 0;
    }
};

int main(int argc, char const *argv[]) {
    // UDP
    CUDPServerTest udpServerTest;
    udpServerTest.CreateServer("0.0.0.0", 4567);

    CUDPSocket udpSocket;
    udpSocket.Create();

    const char *udp = "udp";
    for (int i = 0; i < 10; ++i) {
        udpSocket.Write(udp, sizeof(udp), "127.0.0.1", 4567);
        Sleep(1000);
    }

    udpServerTest.CloseServer();

    // TCP
    CTCPServerTest tcpServerTest;
    tcpServerTest.CreateServer("0.0.0.0", 4567);

    CTCPSocket tcpSocket;
    tcpSocket.Create("127.0.0.1", 4567);

    const char *tcp = "tcp";
    for (int i = 0; i < 10; ++i) {
        tcpSocket.Write(tcp, sizeof(tcp));
        Sleep(1000);
    }

    tcpServerTest.CloseServer();

    return -1;
}
