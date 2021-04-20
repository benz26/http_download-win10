#ifndef _DOWNLOAD_MD5_
#define _DOWNLOAD_MD5_


//Download_md5
class Download_md5 {
public:
    static Download_md5* GetInstance()
    {
        static Download_md5 instance;
        return &instance;
    }

    //外部
    int Download_md5_test(void);

    //内部
    int get_file_md5(const std::string& file_name, std::string& md5_value);
    

private:
    Download_md5()
    {
    };

    ~Download_md5()
    {
    };
};

#endif