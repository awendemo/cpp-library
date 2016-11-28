/*
 * Copyright(C) 2013,Company yawen. All rights reserved.
 *
 * @file    tcp_server.hpp
 * @brief   TCP长连接服务器类
 * @version 1.0
 * @author  zhangyawen
 * @data    2013-3-7
 * */

#ifndef __TCP_SERVER_HPP
#define __TCP_SERVER_HPP

#include "socket_tcp.hpp"
#include "socket_run.hpp"

namespace module {

    class CTCPServer :
            private CSocketRunable {
    public:

        CTCPServer();

        ~CTCPServer();

    public:

        virtual int OnRead(void *pdata, size_t szlen, SOCKET fd_use) = 0;

        virtual int OnAccept(SOCKET fd_use, SOCKADDR addr) = 0;

        virtual int OnClose(SOCKET fd_use) = 0;

    public:

        int CreateServer(const char *pip, uint16 uport);

        void CloseServer();

        const SOCKET GetConnectSocket() const;

    private:

        /*
         * @brief  DoHandling
         * @param
         * @return int
         * @note   重载CSocketRunable的DoHandling函数，内部会回调OnRead等函数
         * @data   2013-3-7 By zhangyawen
         * */
        int DoHandling();

    private:

        CTCPSocket *__pTcpSocket;

    private:

        volatile bool __bRun;
    };


    class CTCPSingleBind;

    class CTCPMultiBind;

    /*
     * 绑定单个IP
     * */
    class CTCPSingleBind :
            public CTCPServer {
    public:

        CTCPSingleBind();

        ~CTCPSingleBind();

    public:

        virtual int OnRead(void *pdata, size_t szlen, SOCKET fd_use);

        virtual int OnAccept(SOCKET fd_use, SOCKADDR addr);

        virtual int OnClose(SOCKET fd_use);

    public:

        int CreateServer(const char *pip, unsigned short uport, CTCPMultiBind *pCreater = NULL);

        void CloseServer();

    private:

        CTCPMultiBind *__pCreater;

    };

    /*
     * 绑定多个IP
     * */
    class CTCPMultiBind :
            private CBaseSocket {
    public:

        CTCPMultiBind();

        ~CTCPMultiBind();

    public:

        virtual int OnRead(void *pdata, size_t szlen, SOCKET fd_use) = 0;

        virtual int OnAccept(SOCKET fd_use, SOCKADDR addr) = 0;

        virtual int OnClose(SOCKET fd_use) = 0;

    public:

        int CreateServer(ADDRESS_LIST address_list);

        void CloseServer();

        int Write(SOCKET fd_use, void *pdata, size_t szlen);

    private:

        CTCPSingleBind *__pSingleBind;

        int __bindSize;
    };

}

#endif	//__TCP_SERVER_HPP
