/*
 * main.cpp
 *
 *  Created on: Jan 18, 2017
 *      Author: wujing
 */
#include"mytask.h"
#include"stdio.h"

int main()
{
    CThreadPool threadPool(10);

    sleep(5);

    CMyTask taskObj;
    char szTmp[] = "this is the first thread running";
    taskObj.SetData((void*)szTmp);
    taskObj.SetName("one");

    for(int i = 0; i < 3; i++)
    {
        threadPool.AddTask(&taskObj);
    }

    CMyTask taskObj2;
    char szTmp2[] = "this is the second thread running";
    taskObj2.SetData((void*)szTmp2);
    for(int i = 0; i < 3; i++)
    {
        threadPool.AddTask(&taskObj2);
    }

    CMyTask taskObj3=CMyTask();
    threadPool.AddTask(&taskObj3);

    while(1)
    {
    	   printf("there are still %d tasks need to handle\n", threadPool.getTaskSize());
    	        if (threadPool.getTaskSize() == 0)
    	        {
    	        	cout <<"No task"<<endl;
    	            if (threadPool.StopAllThreads())
    	            {
    	                printf("Now I will exit from main\n");
    	                exit(0);
    	            }
    	        }
    	        sleep(2);
    	    }

    	    return 0;
    	}




