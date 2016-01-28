#include "ThreadWrapper.h"

CThreadWrapper::CThreadWrapper()
{
    m_pthread = 0;
    memset(m_thread_name, 0, sizeof(m_thread_name));
    m_thread_priority = 100;
    m_stack_size = 1000;
    m_isStarted = false;
    m_erro_num = 0;
}

CThreadWrapper::~CThreadWrapper()
{
    // when destroy, call close() again, make sure thread has quit
    close();
}

int CThreadWrapper::getThreadPolicy(pthread_attr_t& attr)
{
    int policy;

    if( pthread_attr_getschedpolicy(&attr, &policy) != 0)
    {
    	printf("ThreadWrapper: pthread_attr_getschedpolicy failed\n");
    	return -1;
    }

    switch(policy)
    {
    case SCHED_FIFO:
            printf("policy = SCHED_FIFO\n");
            break;

    case SCHED_RR:
            printf("policy = SCHED_RR\n");
            break;

    case SCHED_OTHER:
            printf("policy = SCHED_OTHER\n");
            break;

    default:
            printf("policy = UNKNOWN\n");
            break;
    }

    return policy;
}

bool CThreadWrapper::setThreadPolicy(pthread_attr_t &attr, int policy)
{
    if(pthread_attr_setschedpolicy(&attr, policy) != 0)
    {
    	printf("ThreadWrapper: setThreadPolicy to %d failed\n", policy);
    	return false;
    }
    int newPolicy = getThreadPolicy(attr);

    return (newPolicy == policy);
}

int CThreadWrapper::getThreadPriorityMax(int policy)
{
    int priority = sched_get_priority_max(policy);

    return priority;
}

int CThreadWrapper::getThreadPriorityMin(int policy)
{
    int priority = sched_get_priority_min(policy);

    return priority;
}

int CThreadWrapper::getThreadPriority(pthread_attr_t &attr)
{
    struct sched_param param;

    if(pthread_attr_getschedparam( &attr, &param ) != 0)
    {
    	printf("ThreadWrapper: getThreadPriority failed\n");
    	return -1;
    }
    printf("priority = %d\n", param.__sched_priority);

    return param.__sched_priority;
}

bool CThreadWrapper::start(ThreadFunc func, char *pName, int priority, int stack_size, void *param)
{
    if(m_isStarted)
    {
        m_erro_num = THD_OBJ_CREATED;
    	printf("ThreadWrapper: the thread already started!");
        return false;
    }
    if(((pName == NULL) || (strcmp(pName, "") == 0)))
    {
    	m_erro_num = THD_NAME_ERROR;
    	printf("ThreadWrapp: thread name error");
    	return false;
    }
    memset(m_thread_name, 0, sizeof(m_thread_name));
    sprintf(m_thread_name, "%s", pName);

    // pthread attribute declare
    pthread_attr_t attr;                
    struct sched_param sched;
    int priorityMax, priorityMin;

    // init thread attr
    if(pthread_attr_init(&attr) != 0)
    {
    	printf("ThreadWrapper: pthread_attr_init failed, threadName=%s\n", pName);
    	return false;
    }
    pthread_attr_getschedparam(&attr, &sched);

    // set ThreadPolicy to SCHED_RR, thus adjust thread priority
    if(! setThreadPolicy(attr, SCHED_RR))
    {
    	printf("ThreadWrapper: setThreadPolicy to SCHED_RR failed, threadName=%s\n", pName);
    	return false;
    }
    priorityMax = getThreadPriorityMax(SCHED_RR);	// get threadPriority Max\Min according to threadPolicy
    priorityMin = getThreadPriorityMin(SCHED_RR);
    if((priority < priorityMin) || (priority > priorityMax))
    {
    	printf("ThreadWrapper: threadPriority=%d, is out of range[%d, %d], set to default Max\n", priority, priorityMin, priorityMax);
    	priority = priorityMax;						// priority range is [Min, Max], if out of range, set default Max
    }
    sched.sched_priority = priority;
    if(pthread_attr_setschedparam(&attr, &sched) != 0)
    {
    	printf("ThreadWrapper: setThread priority failed, threadName=%s\n", pName);
    	return false;
    }

    // set Thread stack_size
    if(pthread_attr_setstacksize(&attr, stack_size) != 0)
    {
    	printf("ThreadWrapper: setThread stackSize failed, stackSize=%d, threadName=%s\n", stack_size, pName);
    	return false;
    }
    size_t threadStackSize;
    pthread_attr_getstacksize(&attr, &threadStackSize);

    // create thread
    int ret = pthread_create(&m_pthread, &attr, (void*(*)(void*))func, param);
    if(ret != 0)
    {
    	printf("ThreadWrapper: pthread_create failed, threadName=%s\n", pName);
    	m_erro_num = THD_CREATE_FAILED;
    	return false;
    }

    // destroy thread attr, will not affect thread created
    if(pthread_attr_destroy(&attr) != 0)
    {
    	printf("ThreadWrapper: pthread_attr_destroy failed\n");
    }

    printf("ThreadWrapper: pthread_create succeed, threadName=%s, threadPriority=%u, threadStackSize=%lu\n" , pName, sched.sched_priority, threadStackSize);
    m_isStarted = true;

    return true;
}

/** close Thread
 * param:
 *       waitTime: wait milli-seconds before close thread
 *       isForceClose: when time out, is force thread quit
 */
bool CThreadWrapper::close(int waitTime, bool isForceClose)
{
    void* status = NULL;
    timespec timeSpec;
    timeval timeNowVal;

    if(m_isStarted)
    {
    	bool bQuitFlag = false;
    	printf("ThreadWrapper: threadName=%s, wait %dms to quit\n", m_thread_name, waitTime);
    	if(waitTime == WAIT_FOREVER)
    	{
    		if(pthread_join(m_pthread, &status) == 0)
    		{
    			bQuitFlag = true;
    		}
    	}
    	else
    	{
    		memset(&timeNowVal, 0, sizeof(timeNowVal));
    		gettimeofday(&timeNowVal, NULL);
    		timeSpec.tv_nsec = timeNowVal.tv_usec * 1000 + (waitTime%1000) * 1000000;

    		timeSpec.tv_sec = timeSpec.tv_nsec / 1000000000 + timeNowVal.tv_sec + waitTime / 1000;
    		timeSpec.tv_nsec = timeSpec.tv_nsec % 1000000000;

    		if(pthread_timedjoin_np(m_pthread, &status, &timeSpec) == 0)
    		{
    			bQuitFlag = true;
    		}
    	}

    	if(! bQuitFlag)
    	{
    		printf("ThreadWrapper: %s thread quit timeout\n", m_thread_name);
    		if(isForceClose)
    		{
    			pthread_join(m_pthread, NULL);
    		}
    	}
    }

    printf("ThreadWrapper: %s thread quit finished. waitTime(%d), m_isStarted(%d).\n", m_thread_name, waitTime, m_isStarted);
    m_isStarted = false;

    return true;
}

int CThreadWrapper::get_error()
{
    return m_erro_num;
}
