#include <iostream>
#include <pthread.h>

#include "download_debug.h"
#include "download_file.h"
#include "download_conn.h"
#include "download_conn_http.h"
#include "download_thread.h"

using namespace std;

//�ļ��ܳ��ȣ��ֽڣ�
template<> int Download_file<Download_conn_http>::m_fileLength = 0;

//�Ѿ����ص��ֽ���
template<>
int Download_file<Download_conn_http>::m_downloaded = 0;

//���ذٷְ٣�0~100��
template<>
int Download_file<Download_conn_http>::m_percent = 0;

//�ļ�·��,ʵ����
template<>
string Download_file<Download_conn_http>::m_filePath = "";

//�ļ����
template<>
FILE* Download_file<Download_conn_http>::m_fp = NULL;

template<typename T>
size_t Download_file<T>::download_file_fileLength(T * pConn)
{
	Download_conn* pConnClass = (Download_conn*)pConn; //����ָ��ָ�����������

	return pConnClass->Download_content_length();
}

template<typename T>
int Download_file<T>::Init(const std::string& filePath, T* pConn)
{
	int ret = ERROR_TYPE_FAILED;
	size_t fileLength = 0;

	//get filelength
	fileLength = download_file_fileLength(pConn);
	if (0 >= fileLength)
	{
		cout << "The file is empty, fileLength: "<< fileLength << "!"<< endl;
		return ret;
	}

	//open file
	FILE* fp = fopen(filePath.c_str(), "wb");
	if (NULL == fp)
	{
		cout << "Open failed, filePath: " << filePath << "!" << endl;
		return ret;
	}

	m_filePath = filePath;
	m_fileLength = fileLength;
	m_fp = fp;

    return ERROR_TYPE_SUCCESS;
}

//called by conn callback
//template<typename T>
// size_t Download_file<T>::file_write(void* ptr, size_t size, size_t nmemb, void* pdata)
template<>
size_t Download_file<Download_conn_http>::file_write(void* ptr, size_t size, size_t nmemb, void* pdata)
{
	int ret = ERROR_TYPE_FAILED;
	pThreadNode* node = (pThreadNode*)pdata;
	FILE* fp = Download_file::m_fp;
	int written = 0;
	
	cout << "Start to write, size*nmemb: "
		<< size << "*" << nmemb
		<< " start: " << node->start
		<< " end: " << node->end
		<< endl;

	//AutoLock* p = &AutoLock(&AutoLock::m_lock);

	if (node->start > node->end)
	{
		//�Ѿ��������
	}
	else if (node->start + (int)(size * nmemb) <= node->end + 1)
	{
		fseek(fp, (long)node->start, SEEK_SET);		//��startΪindex��ʼд
		written = fwrite(ptr, size, nmemb, fp);	//��ʱ����ֵwritten����nmemb

		node->start += (int)(size* nmemb);
		written = (int)(size * nmemb);//�����ݴ���ûʲô����

		cout << "write type <1> success" << endl;
	}
	else
	{
		//���ճ���ʣ���ֶ�
		fseek(fp, (long)node->start, SEEK_SET);
		written = fwrite(ptr, 1, node->end - node->start + 1, fp);
		node->start = node->end;

		cout << "write type <2> success" << endl;
	}

	//���ؽ���
	m_downloaded = m_downloaded + written;
	m_percent = (int)(100 * ((1.0)*m_downloaded) / ((1.0) * m_fileLength));

	//��int��Ϊsize_t���޷���ӡ����������������
	cout<<"���ؽ���:"<<m_percent<< "%�� " << m_downloaded <<"�� "<< m_fileLength <<endl << endl;

	return written;
}

template<typename T>
int Download_file<T>::Fini()
{
	FILE* fp = Download_file::m_fp;

	if (NULL != fp)
	{
		fclose(fp);
	}

	return ERROR_TYPE_SUCCESS;
}



