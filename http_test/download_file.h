#ifndef _DOWNLOAD_FILE_
#define _DOWNLOAD_FILE_

//Download_file
template<typename T>
class Download_file {
public:
    static std::string m_filePath;
    static int m_fileLength;

public:
    static Download_file* GetInstance()
    {
        static Download_file instance;
        return &instance;
    }

    //外部函数
    int Init(const std::string& filePath, T* pConn);
    int Fini();

    //内部函数
    size_t download_file_fileLength(T* pConn);
    size_t file_write(void* ptr, size_t size, size_t nmemb, void* pdata);

private: 
    //文件句柄
    static FILE* m_fp;

    //下载进度
    static int m_downloaded;
    static int m_percent;

    Download_file()
    {
    };

    ~Download_file()
    {
    };
}; 

//fixme 
#include "download_conn.h"
#include "download_conn_http.h"

template class Download_file<Download_conn_http>;

#endif
