#include "stdafx.h"
#include "curlFile.h"

//curl
#include "curl/include/curl.h" 
#pragma comment(lib, "curl/lib/libcurl_release.lib")

curlFile::curlFile()
{
}


curlFile::~curlFile()
{
}

CString cstringlie(CString Line, CString lie, int Count)//得到指定列  
{

	int i = 0;
	int i_pos = 0;
	int pos = 0;
here_pos:
	if (i < Count)
	{
		if (i != 0) { i_pos++; }
		pos = i_pos;
		i_pos = Line.Find(lie, i_pos);
		if (i_pos < 0) { i_pos = Line.GetLength(); }
		i++;
		goto here_pos;
	}
	if (i == 1) { return Line.Mid(pos, i_pos); }
	// cout<<"pos"<<pos<<"i_pos"<<i_pos<<endl;  
	return Line.Mid(pos, i_pos - pos);
}


size_t File_save(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t sizes = size * nmemb;
	DWORD dwHasWritten;
	WriteFile((HANDLE)stream, ptr, sizes, &dwHasWritten, NULL);
	return sizes;
}


int GetFile(CString URL, HANDLE hFile)
{

	CString  strURL;
	if ( (URL.Find(L"http://") < 0) && (URL.Find(L"https://") < 0))
		strURL = L"http://" + URL;
	else
		strURL = URL;


	CString strURL_host = cstringlie(strURL, L"/", 3);//自定义函数得到第三列  
	strURL_host = L"Host:" + strURL_host;


	const size_t strsize_host = (strURL_host.GetLength() + 1) * 2; // 宽字符的长度;  
	char * pstr_host = new char[strsize_host]; //分配空间;  
	size_t sz_host = 0;
	wcstombs_s(&sz_host, pstr_host, strsize_host, strURL, _TRUNCATE);


	char *pURL_host = pstr_host;

	const size_t strsize = (strURL.GetLength() + 1) * 2; // 宽字符的长度;  
	char * pstr = new char[strsize]; //分配空间;  
	size_t sz = 0;
	wcstombs_s(&sz, pstr, strsize, strURL, _TRUNCATE);


	char *pURL = pstr;


	char *pURL_Referer = pstr;


	CURL *curl = curl_easy_init();
	int responseCode = -1;
	struct curl_slist *headers = NULL;
	CURLcode returnCode = CURLE_OK;
	if (curl) {


		headers = curl_slist_append(headers, pURL_host);
		headers = curl_slist_append(headers, "Connection:keep-alive");
		headers = curl_slist_append(headers, "Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
		headers = curl_slist_append(headers, "User-Agent:Mozilla/4.0 (compatible; MSIE 9.0; Windows NT 6.1; 2Pac; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022)");
		headers = curl_slist_append(headers, "Content-type:application/x-www-form-urlencoded");
		headers = curl_slist_append(headers, pURL_Referer);
		//headers = curl_slist_append(headers, "Accept-Encoding: none");//不接受gzip加密  
		headers = curl_slist_append(headers, "Accept-Encoding: gzip,deflate");
		headers = curl_slist_append(headers, "Accept-Language:zh-CN,zh;q=0.8");


		//跳过服务器SSL验证，不使用CA证书    
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		//验证服务器端发送的证书，默认是 2(高)，1（中），0（禁用）    
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);


		//设置301、302跳转跟随location    
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);


		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10); //设置连接超时时间    


															//发送http请求头  
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);


		//设置CURLOPT_ACCEPT_ENCODING （7.21.8之前为CURLOPT_ENCODING ）  
		curl_easy_setopt(curl, CURLOPT_ENCODING, L"gzip,deflate");




		//发送cookie值给服务器    
		//curl_easy_setopt(curl, CURLOPT_COOKIE, "name1=var1; name2=var2;");     
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, L"./cookies.txt");//设置访问前得到的COOKIES 存放路径 导入  
		curl_easy_setopt(curl, CURLOPT_COOKIEJAR, L"./cookies.txt"); //设置访问后得到的COOKIES 存放路径 导出  


		curl_easy_setopt(curl, CURLOPT_URL, pURL);              //设置请求链接    


		curl_easy_setopt(curl, CURLOPT_WRITEDATA, hFile);      //设置CURLOPT_WRITEFUNCTION的最后一个参数值    




		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, File_save); //设置接收到数据之后的回调函数    


		curl_easy_perform(curl);                                    //开始传输    
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode); //获取返回信息    
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
		headers = NULL;
	}


	delete pstr_host;//释放内存  
	delete pstr;//释放内存  
	return responseCode;
}



bool curlFile::CURL_GetFile(CString URL, CString FilePath) //下载指定URL文件  
{
	HANDLE hFile = CreateFileW(FilePath, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ /* | FILE_SHARE_DELETE */, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		GetFile(URL, hFile);
	}
	else return 0;
	CloseHandle(hFile);
	hFile = NULL;
	return 1;
}
