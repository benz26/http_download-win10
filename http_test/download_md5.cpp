#include <iostream>
#include <fstream>
#include <openssl/md5.h>
#include <string>

#include "download_debug.h"
#include "download_md5.h"
#include "download_file.h"
#include "download_conn.h"
#include "download_conn_http.h"

using namespace std;


int Download_md5::get_file_md5(const string& file_name, string& md5_value)
{
    md5_value.clear();

    std::ifstream file(file_name.c_str(), std::ifstream::binary);
    if (!file)
    {
        return ERROR_TYPE_FAILED;
    }

    MD5_CTX md5Context;
    MD5_Init(&md5Context);

    char buf[1024 * 16];
    while (file.good()) {
        file.read(buf, sizeof(buf));
        MD5_Update(&md5Context, buf, file.gcount());
    }

    unsigned char result[MD5_DIGEST_LENGTH];
    MD5_Final(result, &md5Context);

    char hex[35];
    memset(hex, 0, sizeof(hex));
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
    {
        sprintf(hex + i * 2, "%02x", result[i]);
    }
    hex[32] = '\0';
    md5_value = string(hex);

    return ERROR_TYPE_SUCCESS;
}

int Download_md5::Download_md5_test(void)
{
    string file_name = Download_file<Download_conn_http>::GetInstance()->m_filePath;  //fixme œ»’‚√¥–¥
    string md5value;

    int ret = get_file_md5(file_name, md5value);
    if (ret < 0)
    {
        printf("get file md5 failed. file=%s\n", file_name.c_str());
        return ERROR_TYPE_FAILED;
    }

    printf("the md5 value       =: %s\n", md5value.c_str());
    cout << "the corret MD5 value=: 446CCF3A36DB382DF851E28A20C86241" << endl;

    return ERROR_TYPE_SUCCESS;
}