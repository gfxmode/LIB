#include "Lock.h"

CMutexLock::CMutexLock()
{
    pthread_mutexattr_init(&m_attr);
    pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_NORMAL);
    pthread_mutex_init(&m_mutex, &m_attr);

}

CMutexLock::CMutexLock(Mtype_T mType)
{
    pthread_mutexattr_init(&m_attr);
    pthread_mutexattr_settype(&m_attr, mType);
    pthread_mutex_init(&m_mutex, &m_attr);
}

CMutexLock::~CMutexLock()
{
    pthread_mutex_destroy(&m_mutex);
    pthread_mutexattr_destroy(&m_attr);
}

CRWLock::CRWLock()
{
    pthread_rwlock_init(&m_rwlock, NULL);
}

CRWLock::~CRWLock()
{
    pthread_rwlock_destroy(&m_rwlock);
}
