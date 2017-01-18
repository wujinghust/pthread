/*
 * mytask.h
 *
 *  Created on: Jan 18, 2017
 *      Author: wujing
 */

#ifndef MYTASK_H_
#define MYTASK_H_
#include"threadpool.h"
#include"unistd.h"
#include "ctask.h"

class CMyTask: public CTask
{
public:
    CMyTask(){}

    inline int Run()
    {
       cout <<(char*)this->m_ptrData <<endl;
        sleep(10);
        return 0;
    }
};




#endif /* MYTASK_H_ */
