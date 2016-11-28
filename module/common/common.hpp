/*
* Copyright(C) 2013,Company ghca. All rights reserved.
* 
* @file    common.hpp
* @brief   ����API�Ŀ�ƽ̨��װ
* @version 1.0
* @author  zhangyawen
* @data    2013-3-8
*/

#ifndef __COMMON_HPP
#define __COMMON_HPP

#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#	include <windows.h>
#	include <process.h>
#	pragma comment(lib,"WS2_32.lib")
#else

#	include <unistd.h>
#	include <sys/time.h>
#	include <unistd.h>
#	include <pthread.h>
#	include <semaphore.h>
#	include <arpa/inet.h>

#endif // WIN32

#ifdef WIN32
#	define ADD_REFCOUNT(x)   (InterlockedIncrement(&(x)))
#	define SUB_REFCOUNT(x)   (InterlockedDecrement(&(x)))
#	define SET_REFCOUNT(x,y) (InterlockedExchange(&(x),(y)))
#else
#	define ADD_REFCOUNT(x)      (__sync_add_and_fetch (&(x),1))
#	define SUB_REFCOUNT(x)      (__sync_sub_and_fetch (&(x),1))
#	define SET_REFCOUNT(x, y)   (__sync_lock_test_and_set(&(x),(y)))
#endif

#ifndef DEL_P
#define DEL_P(p) \
    if (p != NULL) {\
        delete[] p;\
        p = NULL;\
    }
#endif

namespace module {

    void Sleep(unsigned long long usec);

    unsigned long long GetTickTime();

    struct tm *GetLocalTime();

    char *strupr(char *pIn);

    int stricmp(char const *pIn1, char const *pIn2);

    int strnicmp(char const *pIn1, char const *pIn2, unsigned int cnt);

    unsigned long long hl64ton(unsigned long long host);

    unsigned long long ntohl64(unsigned long long host);

    bool checkLittleCPU();

    bool checkBigCPU();
}

#endif	//__COMMON_HPP
