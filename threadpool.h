/*
 * threadpool.h
 *
 *  Created on: Jan 18, 2017
 *      Author: wujing
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_
#include<vector>
#include<iostream>
#include"pthread.h"
#include"stdlib.h"
#include"ctask.h"

using namespace std;
/**
 * 线程池管理类的实现
 */
class CThreadPool
{
private:
    static  vector<CTask*> m_vecTaskList;     /** 任务列表 */
    static  bool shutdown;                    /** 线程退出标志 */
    int     m_iThreadNum;                     /** 线程池中启动的线程数 */
    //pthread_t   *pthread_id;
    vector<pthread_t> m_pthreadID;    /**线程队列*/

    static pthread_mutex_t m_pthreadMutex;    /** 线程同步锁 */
    static pthread_cond_t m_pthreadCond;      /** 线程同步的条件变量 */

protected:
    static void* ThreadFunc(void * threadData); /** 新线程的线程回调函数 */
    static int MoveToIdle(pthread_t tid);       /** 线程执行结束后，把自己放入到空闲线程中 */
    static int MoveToBusy(pthread_t tid);       /** 移入到忙碌线程中去 */

    int Create();          /** 创建线程池中的线程 */

public:
    CThreadPool(int threadNum = 10);
    int AddTask(CTask *task);      /** 把任务添加到任务队列中 */
    int StopAll();                 /** 使线程池中的线程退出 */
    int getTaskSize();             /** 获取当前任务队列中的任务数 */
};

vector<CTask*> CThreadPool::m_vecTaskList;         //任务列表
bool CThreadPool::shutdown = false;

pthread_mutex_t CThreadPool::m_pthreadMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CThreadPool::m_pthreadCond = PTHREAD_COND_INITIALIZER;

/**
 * 线程池管理类构造函数
 */
CThreadPool::CThreadPool(int threadNum)
{
    this->m_iThreadNum = threadNum;
    cout << "I will create " << threadNum << " threads in thread vector--------" << endl;
    m_pthreadID= vector<pthread_t>(threadNum);
    Create();
}

/**
 * 线程回调函数
 */
void* CThreadPool::ThreadFunc(void* threadData)
{
    pthread_t tid = pthread_self();
    while (1)
    {
        pthread_mutex_lock(&m_pthreadMutex);
        while (m_vecTaskList.size() == 0 && !shutdown)
        {
            pthread_cond_wait(&m_pthreadCond, &m_pthreadMutex);
        }

        if (shutdown)
        {
            pthread_mutex_unlock(&m_pthreadMutex);
            cout <<"thread "<<pthread_self()<<" will exit\n";
            pthread_exit(NULL);
        }

        cout <<"tid "<<tid<<" run\n";
        vector<CTask*>::iterator iter = m_vecTaskList.begin();

        /**
        * 取出一个任务并处理之
        */
        CTask* task = *iter;
        if (iter != m_vecTaskList.end())
        {
            task = *iter;
            m_vecTaskList.erase(iter);
        }

        pthread_mutex_unlock(&m_pthreadMutex);

        task->Run(); /** 执行任务 */
        cout<<"tid: "<<tid <<" idle\n";
    }
    return (void*)0;
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

	vector<pthread_t>::iterator iter=m_pthreadID.begin();
    while(iter<m_pthreadID.end())
    {
        pthread_create(&(*iter), NULL, ThreadFunc, NULL);
        iter++;
    }
    return 0;
}

/**
 * 停止所有线程
 */
int CThreadPool::StopAll()
{
    /** 避免重复调用 */
    if (shutdown)
    {
        return -1;
    }
    cout <<"Now I will end all threads!!" <<endl;
    /** 唤醒所有等待线程，线程池要销毁了 */
    shutdown = true;
    pthread_cond_broadcast(&m_pthreadCond);

    /** 阻塞等待线程退出，否则就成僵尸了 */
    for (int i = 0; i < m_iThreadNum; i++)
    {
        pthread_join(m_pthreadID[i], NULL);
    }

    //delete [] m_pthreadID;
   // pthread_id = NULL;

    /** 销毁条件变量和互斥体 */
    pthread_mutex_destroy(&m_pthreadMutex);
    pthread_cond_destroy(&m_pthreadCond);

    return 0;
}

/**
 * 获取当前队列中任务数
 */
int CThreadPool::getTaskSize()
{
    return m_vecTaskList.size();
}


#endif /* THREADPOOL_H_ */
