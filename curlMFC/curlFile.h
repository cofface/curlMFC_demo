#pragma once
class curlFile
{
public:
	curlFile();
	~curlFile();

	bool CURL_GetFile(CString URL, CString FilePath);
};

