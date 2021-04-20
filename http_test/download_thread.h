#ifndef _DOWNLOAD_THREAD_
#define _DOWNLOAD_THREAD_


//控制某一线程写入（运行数据）
struct pThreadNode
{
    size_t start;    //该线程即将写入file的index
    size_t end;      //写入file，直至的end（index）
    int nBlock;  //本块大小(本线程需要写入的字节数)
    //size_t nRecv;  //本块已接收
    pthread_t tid;
    void* conn;
    int tIndex;
    void* pConnClass; ////http等派生类对象地址

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

    //外部调用
    void Init(const int threadNum);
    void Fini();
    int Download_thread_handle(T* pConnInst);

    //内部函数
    static void* download_thread_work(void* Param);

    //库环境接口测试
    static void* Function_t(void* Param);
    int  Download_thread_test();

private:
    static int m_threadNum;      //线程数
    static int m_blockSize;   //每线程下载的最大字符数
    static bool m_remainFlag;   //计算文件长度/线程数目，true:有余数，无法整除,false：整除
    static pThreadNode* m_pThreadNodes; //线程信息管理
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
//显示实例化了Download_conn_http的Download_thread类(否则main找不到成员函数定义)
template class Download_thread<Download_conn_http>;

#endif
