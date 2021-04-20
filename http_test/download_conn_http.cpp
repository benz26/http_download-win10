#include <iostream>
#include <curl/curl.h>    
#include <curl/easy.h>

#include "download_debug.h"
#include "download_conn.h"
#include "download_file.h"
#include "download_conn_http.h"

using namespace std;


void Download_conn_http::download_global_init(void)
{
	(void)curl_global_init(CURL_GLOBAL_ALL);
}

int Download_conn_http::Init(const std::string& downloadUrl)
{
	Download_conn::m_downloadUrl = downloadUrl;
	download_global_init();

	return ERROR_TYPE_SUCCESS;
}

void Download_conn_http::Fini()
{
	(void)curl_global_cleanup();

	return;
}

size_t Download_conn_http::Download_content_length()
{
	size_t fileLenth = -1;
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

	return fileLenth;
}

//for new conn, CURLOPT_WRITEFUNCTION
size_t Download_conn_http::downLoad_conn_writeFun(void* ptr, size_t size, size_t nmemb, void* pdata)
{
	size_t written = 0;

	//cout << "downLoad_conn_writeFun" << endl;
	written = Download_file<Download_conn_http>::GetInstance()->file_write(ptr, size, nmemb, pdata);

	return written;
}

//new conn
void* Download_conn_http::Download_new_conn(void* userdata)
{
	CURL* curl = curl_easy_init();

	//curl set
	curl_easy_setopt(curl, CURLOPT_URL, m_downloadUrl.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, downLoad_conn_writeFun);//fix me
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, userdata);

	//DNS���������ó�ʱʱ��
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

#if DEBUG_ON
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif

	//�������ٶ�< 1 �ֽ�/�� ���� 5 ��ʱ,�����ӻ���ֹ.
	curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1L);
	curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 10L);

	return curl;
}

//for thread work
int Download_conn_http::Download_conn_perform(void* conn, const std::string& szRange)
{
	CURL* curl = (CURL*)conn;
	CURLcode res;

	//����range��ȡ�µ����ݶ�
	res = curl_easy_setopt(curl, CURLOPT_RANGE, szRange.c_str());
	res = curl_easy_perform(curl);
	if (CURLE_OK != res)
	{
		cout << "curl_easy_perform() failed: " << res << "," << curl_easy_strerror(res) << endl;
		if ((23 == res) || (28 == res))
		{
			//23��дʧ�ܣ�28�ǳ�ʱ
		}
	}

	curl_easy_cleanup(curl);

	return ERROR_TYPE_SUCCESS;
}


//�⻷���ӿڲ���
int Download_conn_http::download_trans_test(void)
{
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

	return ERROR_TYPE_SUCCESS;
}
