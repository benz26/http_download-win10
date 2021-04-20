#include <iostream>
#include <pthread.h>

#include "download_debug.h"
#include "download_thread.h"
#include "download_file.h"
#include "download_conn.h"
#include "download_conn_http.h"

using namespace std;

//默认10个线程同时下载
template<>
int Download_thread<Download_conn_http>::m_threadNum = 10;

//每个线程下载的数据块大小（最大值）
template<>
int Download_thread<Download_conn_http>::m_blockSize = 0;

//默认未整除
template<>
bool Download_thread<Download_conn_http>::m_remainFlag = false;

//线程执行信息
template<>
pThreadNode* Download_thread<Download_conn_http>::m_pThreadNodes = NULL;

pthread_mutex_t AutoLock::m_lock = PTHREAD_MUTEX_INITIALIZER;

template<typename T>
void Download_thread<T>::Init(const int threadNum)
{
	int fileLength = Download_file<Download_conn_http>::m_fileLength;

	m_threadNum = threadNum;

	if (0 == fileLength % threadNum)
	{
		//整除
		m_remainFlag = false;
		
		//每个线程下载的数据块大小（依据数据总长度% 线程数计算）
		m_blockSize = fileLength / threadNum;
	}
	else
	{
		//剩余, 未整除
		m_remainFlag = true;

		//每个线程下载的数据块大小（依据数据总长度% 线程数计算）
		m_blockSize =  (fileLength / threadNum) + 1;         //如,257字节 10线程，每线程下载26字节
	}

	//初始化线程信息
	m_pThreadNodes = new pThreadNode[threadNum];

	/*cout << "fileLength " << fileLength
		<< " m_threadNum " << m_threadNum
		<< " m_blockSize " << m_blockSize
		<< " m_remainFlag " << m_remainFlag<<endl;*/

	return;
}

//for pthread_create(), callback
template<>
void* Download_thread<Download_conn_http>::download_thread_work(void* Param)
{
	pThreadNode* pNode = (pThreadNode*)Param;	
	Download_conn* pConnClass = (Download_conn*)pNode->pConnClass;

	char tmp[32] = { 0 };
	sprintf(tmp, "%lld-%lld", (int)pNode->start, (int)pNode->end);
	string szRange = tmp;
	
	(void)pConnClass->Download_conn_perform(pNode->conn, szRange);

	//cout << " download_thread_work,tid "<< pNode->tIndex 
	//	<< " ,start " << pNode->start
	//	<< " ,end " << pNode->end
	//	<< " ,conn " << pNode->conn
	//	<< endl;

	pthread_exit(NULL);
	
	return NULL;
}

//called by main
template<typename T>
int Download_thread<T>::Download_thread_handle(T* pConnInst)
{
	int ret = ERROR_TYPE_FAILED;
	pthread_attr_t attr;
	pThreadNode* pThreadNodes = m_pThreadNodes;
	Download_conn* pConnClass = (Download_conn*)pConnInst;//http等派生类对象地址

	// 初始化并设置线程为可连接的（joinable）
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	//创建i个线程
	for (int i = 0; i < m_threadNum; i++)
	{
		//填充start\end
		pThreadNodes[i].start = i * m_blockSize;	

		if ((m_remainFlag)
			&& (i == m_threadNum - 1))
		{
			pThreadNodes[i].end = Download_file<Download_conn_http>::m_fileLength - 1;
			pThreadNodes[i].nBlock = Download_file<Download_conn_http>::m_fileLength - i * m_blockSize;
		}
		else
		{
			pThreadNodes[i].end = (i + 1) * m_blockSize - 1;// 257byte,10 threads;0~25,end是25，end可写入
			pThreadNodes[i].nBlock = m_blockSize;
		}

		//填充pThreadNode的conn、tIndex
		pThreadNodes[i].conn = (void*)(pConnClass->Download_new_conn((void*)&pThreadNodes[i])); //http或其他协议连接的句柄
		pThreadNodes[i].tIndex = i;
		pThreadNodes[i].pConnClass = pConnClass;
		
		//pthread_create
		ret = pthread_create(&(pThreadNodes[i].tid), &attr, /*Download_thread<Download_conn_http>::*/download_thread_work, &pThreadNodes[i]);
		if (0 != ret)
		{
			cout << "Pthread_create failed, ret: " << ret<<" ,m_threadNum: " << i << endl;
		}
	}

	// 删除属性，并等待其他线程
	pthread_attr_destroy(&attr);
	for (int i = 0; i < m_threadNum; i++)
	{
		void* status;
		ret = pthread_join(pThreadNodes[i].tid, &status);
		if (0 != ret)
		{
			cout << "Error join thread, ret: "<<ret<< " status:  " << status << endl;
			//exit(-1);
		}
	}
	
	return ret;
}

template<typename T>
void Download_thread<T>::Fini()
{
	if (NULL != m_pThreadNodes)
	{
		delete[]m_pThreadNodes;
	}

	return;
}

/*************库环境接口测试************/
template<>
void* Download_thread<Download_conn_http>::Function_t(void* Param)
{
	printf("我是线程！ ");

	pthread_t myid = pthread_self();
	//printf("线程ID=%d \n", myid);

	return NULL;
}

template<typename T>
int Download_thread<T>::Download_thread_test()
{
	pthread_t pid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&pid, &attr, Function_t, NULL);
	printf("========================================\n ");

	pthread_attr_destroy(&attr);

	return ERROR_TYPE_SUCCESS;
}


