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

    //�ⲿ����(���̵߳���)
    virtual size_t Download_content_length();
    virtual void* Download_new_conn(void* userdata);

    //static int Download_conn_perform(void* conn,const std::string& szRange);
    virtual int Download_conn_perform(void* conn,const std::string& szRange);

    //�ڲ�����
    virtual int download_trans_test(void);

protected:
    static std::string m_downloadUrl; //���ӹ��ã������Բ���
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