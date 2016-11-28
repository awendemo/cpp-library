/*
 * Copyright(C) 2013,Company yawen. All rights reserved.
 *
 * @file    socket_run.hpp
 * @brief   套接字线程回调支持
 * @version 1.0
 * @author  zhangyawen
 * @data    2013-3-5
 * */

#ifndef __SOCKET_RUN_HPP
#define __SOCKET_RUN_HPP

namespace module{

#ifdef OS_WINDOWS
#	define WIN32_LEAN_AND_MEAN 
#	include <windows.h>
#	include <process.h>
#	include <intrin.h>
#	define 	HTHREAD	   HANDLE
#	define	RET_THREAD DWORD WINAPI 
	typedef unsigned (WINAPI *lpfnWorkThread)(LPVOID);
#else
#   include	<unistd.h>
#	include <pthread.h>
#	include <semaphore.h>
#	define 	HTHREAD	   pthread_t*
#	define	RET_THREAD void*
#endif	// OS_WINDOWS

	class CSocketRunable {
	public:
		CSocketRunable() : __hRunning(NULL) {}

		~CSocketRunable() {
#ifdef OS_WINDOWS
            if (__hRunning) {
                WaitForSingleObject(__hRunning,INFINITE);

                CloseHandle(__hRunning);
            }
#else
			delete __hRunning;
#endif
		}

	public:

		virtual int DoHandling() = 0;

		int StartRunning() {
#ifdef OS_WINDOWS
            __hRunning = (HANDLE)_beginthreadex(NULL,0,(lpfnWorkThread)__SocketWorkThread, this,0,NULL);

            if (__hRunning != 0) {
                return 0;
            } else {
                return GetLastError();
            }
#else
			pthread_attr_t attr;

			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

			__hRunning = new pthread_t;

			pthread_create(__hRunning, &attr, __SocketWorkThread, this);
			pthread_attr_destroy(&attr);
#endif
			return 0;
		}

	private:

		static RET_THREAD __SocketWorkThread(void *arg) {
			CSocketRunable *pThis = (CSocketRunable *) arg;
#ifdef OS_WINDOWS
			return pThis->DoHandling();
#else
			pThis->DoHandling();

			return (RET_THREAD)0;
#endif
		}

	private:

		HTHREAD __hRunning;
	};
}

#endif //__SOCKET_RUN_HPP
