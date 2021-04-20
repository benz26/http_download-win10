#include <iostream>

#include "download_debug.h"
#include "download_conn.h"

#define DEBUG_BASE_CLASS 0

#if DEBUG_BASE_CLASS 
#include <curl/curl.h>    
#include <curl/easy.h>

#include "download_file.h"
#endif

using namespace std;

string Download_conn::m_downloadUrl = "";

int Download_conn::Init(const std::string& downloadUrl)
{
	Download_conn::m_downloadUrl = downloadUrl;

	return ERROR_TYPE_SUCCESS;
}

void Download_conn::Fini()
{
#if DEBUG_BASE_CLASS
	curl_global_cleanup();
#endif
	return;
}

size_t Download_conn::Download_content_length()
{
	size_t fileLenth = -1;

#if DEBUG_BASE_CLASS

    CURL* curl = NULL;
        
    curl = curl_easy_init();
	if (NULL == curl)
	{
		return fileLenth;
	}
	/*cout << "curl " << curl << endl;*/
	cout << "url " << m_downloadUrl << endl;

    //ֻ��Ҫheaderͷ,����Ҫbody
    curl_easy_setopt(curl, CURLOPT_URL, m_downloadUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_HEADER, 1);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1);

	CURLcode res = curl_easy_perform(curl);
    if (CURLE_OK == res)
    {
        curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &fileLenth);
		/*cout << "fileLenth" << fileLenth << endl;*/
    }
	/*cout << "res" << res << endl;*/
	
    curl_easy_cleanup(curl);
#endif
    return fileLenth;
}

//new conn
void* Download_conn::Download_new_conn(void* userdata)
{
	void* conn = NULL;
#if DEBUG_BASE_CLASS
	CURL* conn = curl_easy_init();

	//curl set
	curl_easy_setopt(conn, CURLOPT_URL, m_downloadUrl.c_str());

	curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, downLoad_conn_writeFun);//fix me
	curl_easy_setopt(conn, CURLOPT_WRITEDATA, userdata);

	//DNS���������ó�ʱʱ��
	//curl_easy_setopt(conn, CURLOPT_NOSIGNAL, 1L);

	curl_easy_setopt(conn, CURLOPT_NOSIGNAL, 1L);

	curl_easy_setopt (conn, CURLOPT_VERBOSE, 1L);

	//�������ٶ�< 1 �ֽ�/�� ���� 5 ��ʱ,�����ӻ���ֹ.
	curl_easy_setopt(conn, CURLOPT_LOW_SPEED_LIMIT, 1L);
	curl_easy_setopt(conn, CURLOPT_LOW_SPEED_TIME, 10L);

#endif
	return conn;
}

//for thread work
int Download_conn::Download_conn_perform(void* conn, const std::string& szRange)
{
#if DEBUG_BASE_CLASS
	CURL* curl = (CURL *)conn;
	CURLcode res;

	//����range��ȡ�µ����ݶ�
	res = curl_easy_setopt(curl, CURLOPT_RANGE, szRange.c_str());
	res = curl_easy_perform(curl);
	if (CURLE_OK != res)
	{
		cout << "curl_easy_perform() failed: " <<res<<","<< curl_easy_strerror(res)<< endl;
		if ((23 == res) || (28 == res))
		{
			//23��дʧ�ܣ�28�ǳ�ʱ
		}
	}

	curl_easy_cleanup(curl);
#endif
	return ERROR_TYPE_SUCCESS;
}


//�⻷���ӿڲ���
int Download_conn::download_trans_test(void)
{
#if DEBUG_BASE_CLASS
	CURL* curl = 0;
	CURLcode res;

	curl = curl_easy_init();
	if (curl != 0)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "https://www.baidu.com");

		//����Ϊ������ת
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		//��������
		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			//������������룬��Ϊû����UTF-8
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}

		//������
		curl_easy_cleanup(curl);
	}

	//	printf("hello!");
	//	system("pause");
#endif
	return ERROR_TYPE_SUCCESS;
}
