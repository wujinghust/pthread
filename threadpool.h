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
    static  int     m_busyThreadNum;            /** 线程池中启动的线程数 */
    static  int     m_idleThreadNum;           /**空闲线程个数*/
    static  int     m_maxThreadNum;            /**最大线程个数*/
    static vector<pthread_t> m_vecIdleThread;   //当前空闲的线程集合
    static vector<pthread_t> m_vecBusyThread;   //当前正在执行的线程集合
    //pthread_t   *pthread_id;
  //  vector<pthread_t> m_pthreadID;            /**线程队列*/

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
    int StopAllThreads();                 /** 使线程池中的线程退出 */
    int getTaskSize();             /** 获取当前任务队列中的任务数 */
    bool DestroyThreadPool();
    bool ADDThreads(int threadNum);
    bool StartTask();

    bool Isfull();
};

#endif /* THREADPOOL_H_ */
