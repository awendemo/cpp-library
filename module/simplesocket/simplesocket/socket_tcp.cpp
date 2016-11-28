#include "socket_tcp.hpp"

namespace module {

    CTCPSocket::CTCPSocket() : __sListen(INVALID_SOCKET), __sConnect(INVALID_SOCKET) {}

    CTCPSocket::~CTCPSocket() {
        CBaseSocket::Close(__sListen);
        CBaseSocket::Close(__sConnect);
    }

    int CTCPSocket::Create(const char *pip, uint16 uport, bool bclient/* = true*/) {
        if (bclient) {
            if ((__sConnect = CBaseSocket::Create(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
                perror("CreateServer tcp socket failed!");
                return -1;
            }

            SOCKADDRV4 sin;
            sin.sin_family = AF_INET;
            sin.sin_port = htons(uport);
            sin.sin_addr.s_addr = inet_addr(pip);

            if (CBaseSocket::Connect(__sConnect, (SOCKADDR *) &sin, sizeof(sin)) == SOCKET_ERROR) {
                perror("Connect server failed!");
                return -1;
            }

            return 0;
        }

        if ((__sListen = CBaseSocket::Create(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
            perror("CreateServer tcp socket failed!");
            return -1;
        }

#ifndef OS_WINDOWS
        int on = 1;
        // set port reuse
        if (CBaseSocket::SetSocketOption(__sListen, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(int)) == -1) {
            perror("Set port reuse failed!");
            return -1;
        }
#endif

        SOCKADDRV4 sin;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(uport);
        sin.sin_addr.s_addr = inet_addr(pip);

        if (CBaseSocket::Bind(__sListen, (SOCKADDR *) &sin, sizeof(sin)) == SOCKET_ERROR) {
            perror("Bind failed!");
            return -1;
        }

        if (CBaseSocket::Listen(__sListen, LISTEN_BLOCKS) == SOCKET_ERROR) {
            perror("Listen failed!");
            return -1;
        }

        return 0;
    }

    int CTCPSocket::Read(void *pout, size_t szout) {
        return CBaseSocket::Recv(__sConnect, pout, szout, 0);
    }

    int CTCPSocket::Write(const void *pin, size_t szin) {
        return CBaseSocket::Send(__sConnect, pin, szin, 0);
    }

    int CTCPSocket::Write(SOCKET fd, const void *pin, size_t szin) {
        return CBaseSocket::Send(fd, pin, szin, 0);
    }

    SOCKET CTCPSocket::Accept(SOCKADDR &addr, SOCK_LEN &addrlen) {
        return CBaseSocket::Accept(__sListen, addr, addrlen);
    }

    const SOCKET CTCPSocket::GetListenSocket() const {
        return __sListen;
    }

    const SOCKET CTCPSocket::GetConnectSocket() const {
        return __sConnect;
    }

    void CTCPSocket::Close() {
        if (__sListen != INVALID_SOCKET) {
            CBaseSocket::Close(__sListen);
            __sListen = INVALID_SOCKET;
        }

        if (__sConnect != INVALID_SOCKET) {
            CBaseSocket::Close(__sConnect);
            __sConnect = INVALID_SOCKET;
        }
    }

}
