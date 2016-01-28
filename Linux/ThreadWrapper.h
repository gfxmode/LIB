/** CThreadWrapper class for Linux
 * author: gfxmode@live.com
 * date: 2015-12-27
 * version: 1.0.0.0
 *
 */
#ifndef THREADWRAPPER_H
#define THREADWRAPPER_H

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "GlobalDefine.h"

#define __stdcall
const int THDMBASEERR = 600;
const int THD_OBJ_CREATED = THDMBASEERR + 1;
const int THD_NAME_ERROR = THDMBASEERR + 2;
const int THD_CREATE_FAILED = THDMBASEERR + 3;
const int THD_PRI_FAILED = THDMBASEERR + 4;

typedef void (__stdcall *ThreadFunc)(void *);

class CThreadWrapper
{
public:
    CThreadWrapper();
    virtual ~CThreadWrapper();

    static int getThreadPolicy(pthread_attr_t &attr);
    static bool setThreadPolicy(pthread_attr_t &attr, int policy);
    static int getThreadPriorityMax(int policy);
    static int getThreadPriorityMin(int policy);
    static int getThreadPriority(pthread_attr_t &attr);
    static int setThreadPriority(pthread_attr_t &attr, int policy);

protected:
    pthread_t m_pthread;
    char m_thread_name[100];
    int m_thread_priority;
    int m_stack_size;
    bool m_isStarted;
    int m_erro_num;
    

public:
    /**
     * @brief start the *func* as a thread
     * @param func: pointer to func
     * @param pName: thread name
     * @param priority: thread priority
     * @param statck_size: thread stack size
     * @param param: thread start with params
     * @return: is start successful. true: successfule; false: failed
     */
    bool start(ThreadFunc func, char* pName, int priority, int statck_size, void* param);
    /**
     * @brief close the *func* thread
     * @param wait_time: close the thread waittime, if timeout and is_force_close=true, will force thread exit
     * @param is_force_close: whether force close thread or not if timeout
     * @return: is close successfule. true: successfule; false: failed
     */
    bool close(int wait_time = 1000, bool is_force_close = false);
    /**
     * @brief get_error: get error code if failed
     * @return error code
     */
    int get_error();
};

#endif // THREADWRAPPER_H
