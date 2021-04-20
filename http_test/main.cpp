#include <iostream>
#include <pthread.h>

#include "download_debug.h"
#include "download_file.h"
#include "download_conn.h"
#include "download_conn_http.h"
#include "download_md5.h"
#include "download_thread.h"

using namespace std;

/*
	main
*/
int main(void)
{
	int ret = ERROR_TYPE_FAILED;

	//ÊäÈë²ÎÊý
	int threadNum = 10;
	string downloadUrl = "http://gdown.baidu.com/data/wisegame/cf3a36db382df851/446ccf3a36db382df851e28a20c86241.apk";
	string filePath = downloadUrl.substr(downloadUrl.find_last_of("/") + 1,100); //446ccf3a36db382df851e28a20c86241.apk
	
	//init
	Download_conn_http::GetInstance()->Init(downloadUrl);
	Download_file<Download_conn_http>::GetInstance()->Init(filePath, Download_conn_http::GetInstance());
	Download_thread<Download_conn_http>::GetInstance()->Init(threadNum);

	//download
	cout << "Download start..." << endl;

	ret = Download_thread<Download_conn_http>::GetInstance()->Download_thread_handle(Download_conn_http::GetInstance());
	if (0 > ret)
	{
		cout << "Download handle, ret: " << ret << endl;
	}

	//Fini
	Download_thread<Download_conn_http>::GetInstance()->Fini();
	Download_conn::GetInstance()->Fini();
	Download_file<Download_conn_http>::GetInstance()->Fini();

	//md5
	ret = Download_md5::GetInstance()->Download_md5_test();
	if (0 <= ret)
	{
		cout << "Download succed! ret: " << ret << endl;
	}
	
	pthread_exit(0);
	
	return ERROR_TYPE_SUCCESS;
}