/*
 * ctask.h
 *
 *  Created on: Jan 18, 2017
 *      Author: wujing
 */

#ifndef CTASK_H_
#define CTASK_H_
#include"string.h"
#include<iostream>

using namespace std;

/**
 * 执行任务的类，设置任务数据并执行
 */
class CTask
{
protected:
    string m_strTaskName;  /** 任务的名称 */
    void* m_ptrData;       /** 要执行的任务的具体数据 */
public:
    CTask(){}
    CTask(string taskName);
    CTask(void* m_ptrData);
    CTask(string taskName,void* m_ptrData);
    virtual int Run()= 0;
    void SetData(void* data);    /** 设置任务数据 */

public:
    virtual ~CTask(){}
};

#endif /* CTASK_H_ */
