/*
 * threadpool.cpp
 *
 *  Created on: Jan 18, 2017
 *      Author: wujing
 */
#include"threadpool.h"

/*CTask::CTask()
{
	m_strTaskName = "null";
	m_ptrData = NULL;
}
CTask::CTask(string taskName)
 {
	m_strTaskName = taskName;
	m_ptrData = NULL;
 }

CTask::CTask(void* m_ptrData)
{
	 m_strTaskName = "null";
     this->m_ptrData = m_ptrData;
}

CTask::CTask(string taskName,void* m_ptrData)
{
	m_strTaskName = taskName;
	this->m_ptrData = m_ptrData;
}
*/
void CTask::SetData(void * data)
{
    m_ptrData = data;
}

void CTask::SetName(string taskname)
{
	m_strTaskName  = taskname;
}
