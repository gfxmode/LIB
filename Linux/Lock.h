/** Lock class for linux
 * author: gfxmode@live.com
 * date: 2016-01-19
 * version: 1.0.0.0
 */

#ifndef LOCK_H
#define LOCK_H

#include <pthread.h>


/**  MutexLock class */
typedef pthread_mutex_t Mutex_T;
typedef pthread_mutexattr_t Mattr_T;
typedef int Mtype_T;

class CMutexLock
{
public:
    CMutexLock();
    CMutexLock(Mtype_T mtype);
    ~CMutexLock();

public:
    int lock() { return pthread_mutex_lock(&m_mutex); }
    int unLock() { return pthread_mutex_unlock(&m_mutex); }
    int tryLock() { return pthread_mutex_trylock(&m_mutex); }
    Mutex_T& getMutex() { return m_mutex; }

private:
    Mutex_T m_mutex;
    Mattr_T m_attr;
};

/** ReadWriteLock class */
typedef pthread_rwlock_t RWLock_T;

class CRWLock
{
public:
    CRWLock();
    ~CRWLock();
public:
    int rLock() { return pthread_rwlock_rdlock(&m_rwlock); }
    int wLock() { return pthread_rwlock_wrlock(&m_rwlock); }
    int tryRLock() { return pthread_rwlock_tryrdlock(&m_rwlock); }
    int tryWLock() { return pthread_rwlock_trywrlock(&m_rwlock); }
    int unLock() { return pthread_rwlock_unlock(&m_rwlock); }

private:
    RWLock_T m_rwlock;
};

#endif // LOCK_H
