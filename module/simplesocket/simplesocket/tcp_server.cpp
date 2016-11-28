#include "tcp_server.hpp"

namespace module {

    CTCPServer::CTCPServer() : __pTcpSocket(NULL), __bRun(true) {}

    CTCPServer::~CTCPServer() {
        __bRun = false;
    }

    int CTCPServer::CreateServer(const char *pip, uint16 uport) {
        if (__pTcpSocket == NULL) {
            __pTcpSocket = new CTCPSocket();
        }

        if (__pTcpSocket->Create(pip, uport, false) == -1) {
            return -1;
        }

        __bRun = true;

        return this->StartRunning();
    }

    void CTCPServer::CloseServer() {
        if (__pTcpSocket != NULL) {
            __pTcpSocket->Close();
        }

        __bRun = false;
    }

    inline const SOCKET CTCPServer::GetConnectSocket() const {
        if (__pTcpSocket == NULL)
            return INVALID_SOCKET;

        return __pTcpSocket->GetConnectSocket();
    }

    int CTCPServer::DoHandling() {
        SOCKET fd_Listen = __pTcpSocket->GetListenSocket();
        SOCKET fd_Con[FD_SIZE] = {INVALID_SOCKET};
        SOCKET fd_Use = INVALID_SOCKET;

        SOCKADDR addr = {0};
        char buffer[65535] = {0};
        int nconn = 0, nlastconn = 0, nret = 0;

        while (__bRun) {
            nlastconn = nconn;

            if ((nret = __pTcpSocket->CBaseSocket::SelectServer(fd_Listen, fd_Con, nconn, fd_Use, addr, (void *) buffer, sizeof(buffer))) > 0) {
                this->OnRead((void *) buffer, nret, fd_Use);
            } else {
                if (nconn > nlastconn) {
                    this->OnAccept(fd_Use, addr);
                } else if (nconn < nlastconn) {
                    this->OnClose(fd_Use);
                }
            }
        }

        if (__pTcpSocket != NULL) {
            delete __pTcpSocket;
        }

        return 0;
    }

    CTCPSingleBind::CTCPSingleBind() : __pCreater(NULL) {}

    CTCPSingleBind::~CTCPSingleBind() {}

    int CTCPSingleBind::OnRead(void *pdata, size_t szlen, SOCKET fd_use) {
        if (__pCreater != NULL)
            __pCreater->OnRead(pdata, szlen, fd_use);

        return 0;
    }

    int CTCPSingleBind::OnAccept(SOCKET fd_use, SOCKADDR addr) {
        if (__pCreater != NULL)
            __pCreater->OnAccept(fd_use, addr);

        return 0;
    }

    int CTCPSingleBind::OnClose(SOCKET fd_use) {
        if (__pCreater != NULL)
            __pCreater->OnClose(fd_use);

        return 0;
    }

    int CTCPSingleBind::CreateServer(const char *pip, unsigned short uport, CTCPMultiBind *pCreater/* = NULL*/) {
        __pCreater = pCreater;
        return CTCPServer::CreateServer(pip, uport);
    }

    void CTCPSingleBind::CloseServer() {
        this->CloseServer();
    }

    CTCPMultiBind::CTCPMultiBind() : __pSingleBind(NULL) {}

    CTCPMultiBind::~CTCPMultiBind() {}

    int CTCPMultiBind::CreateServer(ADDRESS_LIST address_list) {
        if (address_list.size() == 0)
            return 0;

        __bindSize = address_list.size();
        __pSingleBind = new CTCPSingleBind[__bindSize];

        for (size_t i = 0; i < address_list.size(); i++) {
            if (__pSingleBind[i].CreateServer(address_list[i].strip.c_str(), address_list[i].uport, this) == -1) {
                delete[] __pSingleBind;
                __pSingleBind = NULL;
                return -1;
            }
        }

        return 0;
    }

    void CTCPMultiBind::CloseServer() {
        for (int i = 0; i < __bindSize; ++i) {
            __pSingleBind[i].CloseServer();
        }

        delete[] __pSingleBind;
    }

    int CTCPMultiBind::Write(SOCKET fd_use, void *pdata, size_t szlen) {
        return CBaseSocket::Send(fd_use, pdata, szlen, 0);
    }
}
