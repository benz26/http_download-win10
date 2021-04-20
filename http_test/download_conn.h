#ifndef _DOWNLOAD_CONN_
#define _DOWNLOAD_CONN_

//Download_conn
class Download_conn {
public:
    static Download_conn* GetInstance(void)
    {
        static Download_conn instance;
        return &instance;
    }

    //Init
    virtual int Init(const std::string& downloadUrl);

    //Fini
    virtual void Fini();

    //外部调用(主线程调用)
    virtual size_t Download_content_length();
    virtual void* Download_new_conn(void* userdata);

    //static int Download_conn_perform(void* conn,const std::string& szRange);
    virtual int Download_conn_perform(void* conn,const std::string& szRange);

    //内部函数
    virtual int download_trans_test(void);

protected:
    static std::string m_downloadUrl; //父子公用，都可以操作
    Download_conn()
    {
    };

    ~Download_conn()
    {
    };

private:
    //Download_conn()
    //{
    //};

    //~Download_conn()
    //{
    //};
};

#endif