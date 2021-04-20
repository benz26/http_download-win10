#ifndef _DOWNLOAD_THREAD_
#define _DOWNLOAD_THREAD_


//����ĳһ�߳�д�루�������ݣ�
struct pThreadNode
{
    size_t start;    //���̼߳���д��file��index
    size_t end;      //д��file��ֱ����end��index��
    int nBlock;  //�����С(���߳���Ҫд����ֽ���)
    //size_t nRecv;  //�����ѽ���
    pthread_t tid;
    void* conn;
    int tIndex;
    void* pConnClass; ////http������������ַ

};

class AutoLock {

public:
    pthread_mutex_t* _lock;
    static pthread_mutex_t m_lock;

public:
    AutoLock(pthread_mutex_t* lock) {
        _lock = lock;
        pthread_mutex_lock(_lock);
    }
    ~AutoLock() {
        pthread_mutex_unlock(_lock);
    }
};

//Download_thread
template<typename T>
class Download_thread {
public:
    static Download_thread* GetInstance()
    {
        static Download_thread instance;
        return &instance;
    }

    //�ⲿ����
    void Init(const int threadNum);
    void Fini();
    int Download_thread_handle(T* pConnInst);

    //�ڲ�����
    static void* download_thread_work(void* Param);

    //�⻷���ӿڲ���
    static void* Function_t(void* Param);
    int  Download_thread_test();

private:
    static int m_threadNum;      //�߳���
    static int m_blockSize;   //ÿ�߳����ص�����ַ���
    static bool m_remainFlag;   //�����ļ�����/�߳���Ŀ��true:���������޷�����,false������
    static pThreadNode* m_pThreadNodes; //�߳���Ϣ����
    //static int m_runThreadCnt;

    Download_thread()
    {
    };

    ~Download_thread()
    {
    };
};


//fixme 
#include "download_conn.h"
#include "download_conn_http.h"

//fixme 
//��ʾʵ������Download_conn_http��Download_thread��(����main�Ҳ�����Ա��������)
template class Download_thread<Download_conn_http>;

#endif
