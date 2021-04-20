#ifndef _DOWNLOAD_CONN_HTTP_
#define _DOWNLOAD_CONN_HTTP_

//Download_conn_http
class Download_conn_http :public Download_conn {
public:
    static Download_conn_http* GetInstance(void)
    {
        static Download_conn_http instance;
        return &instance;
    }

    //Init
    virtual int Init(const std::string& downloadUrl);

    //Fini
    virtual void Fini();

    //外部调用(主线程调用)
    virtual size_t Download_content_length();
    virtual void* Download_new_conn(void* userdata);
    virtual  int Download_conn_perform(void* conn, const std::string& szRange);

    //内部函数
    static size_t downLoad_conn_writeFun(void* ptr, size_t size, size_t nmemb, void* pdata);
    static void download_global_init();

    //test
    int  download_trans_test(void);

private:
    Download_conn_http()
    {
    };

    ~Download_conn_http()
    {
    };
};

#endif