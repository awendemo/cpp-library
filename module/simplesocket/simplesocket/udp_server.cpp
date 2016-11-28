#include "udp_server.hpp"

namespace module {

    CUDPServer::CUDPServer() : __pUdpSocket(NULL), __bRun(true) {}

    CUDPServer::~CUDPServer() {
        __bRun = false;
    }

    int CUDPServer::CreateServer(const char *pip, uint16 uport) {
        if (__pUdpSocket == NULL) {
            __pUdpSocket = new CUDPSocket();
        }

        if (__pUdpSocket->Create(pip, uport, false) == -1) {
            return -1;
        }

        __bRun = true;

        return this->StartRunning();
    }

    void CUDPServer::CloseServer() {
        if (__pUdpSocket != NULL) {
            __pUdpSocket->Close();
        }

        __bRun = false;
    }

    int CUDPServer::Write(const void *pin, size_t szin, const char *pip, uint16 uport) {
        if (__pUdpSocket != NULL) {
            return __pUdpSocket->Write(pin, szin, pip, uport);
        }
        return -1;
    }

    int CUDPServer::Write(const void *pin, size_t szin, uint32 uip, uint16 uport) {
        if (__pUdpSocket != NULL) {
            return __pUdpSocket->Write(pin, szin, uip, uport);
        }
        return -1;
    }

    int CUDPServer::Write(const void *pin, size_t szin, SOCKADDR addr) {
        if (__pUdpSocket != NULL) {
            return __pUdpSocket->Write(pin, szin, addr);
        }
        return -1;
    }

    const SOCKET CUDPServer::GetConnectSocket() const {
        if (__pUdpSocket == NULL)
            return INVALID_SOCKET;

        return __pUdpSocket->GetConnectSocket();
    }

    int CUDPServer::DoHandling() {
        SOCKADDR addr = {0};
        int nret = 0;
        char buffer[65535] = {0};

        while (__bRun) {
            if ((nret = __pUdpSocket->Read(buffer, sizeof(buffer), addr)) > 0) {
                this->OnRead((void *) buffer, nret, addr);
            }
        }

        return 0;
    }

    CUDPSingleBind::CUDPSingleBind() : __pCreater(NULL) {}

    CUDPSingleBind::~CUDPSingleBind() {}

    int CUDPSingleBind::OnRead(void *pdata, size_t szlen, SOCKADDR addr) {
        if (__pCreater != NULL) {
            return __pCreater->OnRead(pdata, szlen, addr, this);
        }

        return 0;
    }

    int CUDPSingleBind::CreateServer(const char *pip, unsigned short uport, CUDPMultiBind *pCreater) {
        __pCreater = pCreater;
        return CUDPServer::CreateServer(pip, uport);
    }

    void CUDPSingleBind::CloseServer() {
        this->CloseServer();
    }

    CUDPMultiBind::CUDPMultiBind() : __pSingleBind(NULL) {}

    CUDPMultiBind::~CUDPMultiBind() {
        if (__pSingleBind != NULL)
            delete[] __pSingleBind;
    }

    int CUDPMultiBind::Write(void *pdata, size_t szlen, SOCKADDR addr, CUDPSingleBind *pTrans) {
        if (pTrans != NULL) {
            return pTrans->Write(pdata, szlen, addr);
        }

        return -1;
    }

    int CUDPMultiBind::CreateServer(ADDRESS_LIST address_list) {
        if (address_list.size() == 0)
            return 0;

        __bindSize = address_list.size();
        __pSingleBind = new CUDPSingleBind[__bindSize];

        for (size_t i = 0; i < address_list.size(); i++) {
            if (__pSingleBind[i].CreateServer(address_list[i].strip.c_str(), address_list[i].uport, this) == -1) {
                delete[] __pSingleBind;
                __pSingleBind = NULL;

                return -1;
            }
        }

        return 0;
    }

    void CUDPMultiBind::CloseServer() {
        for (int i = 0; i < __bindSize; ++i) {
            __pSingleBind[i].CloseServer();
        }

        delete[] __pSingleBind;
    }
}
