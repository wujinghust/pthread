/*
 * threadpool.cpp
 *
 *  Created on: Jan 18, 2017
 *      Author: wujing
 */
#include"threadpool.h"

CTask::CTask(string taskName)
 {
	m_strTaskName = taskName;
	m_ptrData = NULL;
 }

CTask::CTask(void* m_ptrData)
{
	 m_strTaskName = "no task name";
     this->m_ptrData = m_ptrData;
}

CTask::CTask(string taskName,void* m_ptrData)
{
	m_strTaskName = taskName;
	this->m_ptrData = m_ptrData;
}

void CTask::SetData(void * data)
{
    m_ptrData = data;
}
