/*
 * threadpool.cpp
 *
 *  Created on: Jan 18, 2017
 *      Author: wujing
 */
#include"threadpool.h"

vector<CTask*> CThreadPool::m_vecTaskList;         //任务列表
bool CThreadPool::shutdown = false;
int CThreadPool::m_maxThreadNum =0;
int CThreadPool::m_busyThreadNum = 0;
int CThreadPool::m_idleThreadNum = 0;
vector<pthread_t> CThreadPool::m_vecIdleThread;
vector<pthread_t> CThreadPool::m_vecBusyThread;
pthread_mutex_t CThreadPool::m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CThreadPool::m_pthreadCond = PTHREAD_COND_INITIALIZER;

/**
 * 线程池管理类构造函数
 */
CThreadPool::CThreadPool(int threadNum)
{
	CThreadPool::m_maxThreadNum = threadNum;
	CThreadPool::m_busyThreadNum = 0;
    cout << "I will create " << threadNum << " threads in thread pool--------" << endl;
 //   m_pthreadID= vector<pthread_t>(threadNum);

    Create();
    cout <<"I have created " <<m_idleThreadNum<<" threads in thread pool------"<<endl;
}

/**
 * 线程回调函数
 */
void* CThreadPool::ThreadFunc(void* threadData)
{
	//int num=(int)threadData;
	//free(threadData);
    pthread_t tid = pthread_self();
    while (1)
    {
        pthread_mutex_lock(&m_pthreadMutex);
        while (m_vecTaskList.size() == 0 && !shutdown)
        {
        	cout << "there is no task in task vector, waiting for task....."<<endl;
            pthread_cond_wait(&m_pthreadCond, &m_pthreadMutex);
        }

        if (shutdown)
        {
            pthread_mutex_unlock(&m_pthreadMutex);
            cout <<"thread "<<pthread_self()<<" will exit\n";
            pthread_exit(NULL);
        }

        vector<CTask*>::iterator iter = m_vecTaskList.begin();

        /**
        * 取出一个任务并处理之
        */

        if (iter != m_vecTaskList.end())
        	MoveToBusy(tid);
        CTask* task = *iter;
        m_vecTaskList.erase(iter);

        cout << "Now: idle threads "<< m_idleThreadNum<< endl;
        cout <<"     busy threads "<< m_busyThreadNum<<endl;
        pthread_mutex_unlock(&m_pthreadMutex);
        cout <<"thread "<<tid<<" is running.....\n";
        task->Run(); /** 执行任务 */
        cout<<"thread: "<<tid <<"is idle, waiting for task.....\n";
        MoveToIdle(tid);
    }
    return (void*)0;
}

int CThreadPool::MoveToIdle(pthread_t tid)
{
	vector<pthread_t>::iterator busyiter=m_vecBusyThread.begin();
	while(busyiter!=m_vecBusyThread.end())
	{
		if(tid == *busyiter)
			break;
		busyiter++;
	}
	m_vecBusyThread.erase(busyiter);
	m_vecIdleThread.push_back(tid);
	m_idleThreadNum=m_vecIdleThread.size();
	m_busyThreadNum=m_vecBusyThread.size();
	return 0;
}

int CThreadPool::MoveToBusy(pthread_t tid)
{
	vector<pthread_t>::iterator idleiter=m_vecIdleThread.begin();
	while(idleiter!=m_vecIdleThread.end())
	{
		if(tid == *idleiter)
			break;
		idleiter++;
	}
	m_vecIdleThread.erase(idleiter);
	m_vecBusyThread.push_back(tid);
	m_idleThreadNum=m_vecIdleThread.size();
	m_busyThreadNum=m_vecBusyThread.size();
	return 0;
}
/**
 * 往任务队列里边添加任务并发出线程同步信号
 */
int CThreadPool::AddTask(CTask *task)
{
    pthread_mutex_lock(&m_pthreadMutex);
    this->m_vecTaskList.push_back(task);
    pthread_mutex_unlock(&m_pthreadMutex);
    pthread_cond_signal(&m_pthreadCond);
    return 0;
}

/**
 * 创建线程
 */
int CThreadPool::Create()
{
   // pthread_id = (pthread_t*)malloc(sizeof(pthread_t) * m_iThreadNum);

	/*vector<pthread_t>::iterator iter=m_pthreadID.begin();
    while(iter<m_pthreadID.end())
    {
        pthread_create(&(*iter), NULL, ThreadFunc, NULL);
        m_vecIdleThread.push_back(*iter);
        iter++;
    }*/
	pthread_t tid=0;
	//int *p=(int*)malloc(sizeof(int));
	for(int i=0;i<m_maxThreadNum;i++)
	{
		//*p=i;
		pthread_create(&tid, NULL, ThreadFunc,NULL);
		m_vecIdleThread.push_back(tid);
	}
    m_idleThreadNum=m_vecIdleThread.size();  //当前空闲的线程集合
    return 0;
}

/**
 * 停止所有线程
 */
int CThreadPool::StopAllThreads()
{
    /** 避免重复调用 */
    if (shutdown)
    {
        return -1;
    }

    shutdown = true;
    /** 唤醒所有等待线程，线程池要销毁了 */
    pthread_cond_broadcast(&m_pthreadCond);

	cout <<"Now I will destroy all threads!!" <<endl;
	vector<pthread_t>::iterator iter=m_vecIdleThread.begin();
	while(iter<m_vecIdleThread.end())
	{
		cout<<"destroy idle threads"<<endl;
		//pthread_cancel(*iter);
		pthread_join(*iter,NULL);
		iter++;
	}
	vector<pthread_t>::iterator it=m_vecBusyThread.begin();
	while(it<m_vecBusyThread.end())//(iter!=m_vecBusyThread.end())
	{
		cout<<"destroy busy threads"<<endl;
		//pthread_cancel(*iter);
		pthread_join(*it,NULL);
		it++;
	}
	m_maxThreadNum=0;
	m_busyThreadNum=0;
	m_idleThreadNum=0;


    /** 阻塞等待线程退出，否则就成僵尸了 */
  /*  vector<pthread_t>::iterator iter=m_pthreadID.begin();
    while(iter<m_pthreadID.end())
    {
        pthread_join(*iter, NULL);
        iter++;
    }*/

    //delete [] m_pthreadID;
   // pthread_id = NULL;

    /** 销毁条件变量和互斥体 */
	pthread_mutex_destroy(&m_pthreadMutex);
	pthread_cond_destroy(&m_pthreadCond);

    return 0;
}

bool CThreadPool::StartTask()
{

	return true;
}

bool CThreadPool::DestroyThreadPool()
{
	StopAllThreads();
	pthread_mutex_destroy(&m_pthreadMutex);
	pthread_cond_destroy(&m_pthreadCond);
	return true;
}

bool CThreadPool::ADDThreads(int threadNum)
{
	this->m_maxThreadNum += threadNum;
	cout << "I will create " << threadNum << " threads in thread pool--------" << endl;
	 //   m_pthreadID= vector<pthread_t>(threadNum);
	Create();
	cout <<"I have created " <<this->m_idleThreadNum<<" threads in thread pool------"<<endl;
	return true;
}
/**
 * 获取当前队列中任务数
 */
int CThreadPool::getTaskSize()
{
    return m_vecTaskList.size();
}

/**
 * 判断线程池中任务量是否已满
 */
bool CThreadPool::Isfull()
{
	if(m_busyThreadNum==m_maxThreadNum)
		return true;
	else
		return false;
}


