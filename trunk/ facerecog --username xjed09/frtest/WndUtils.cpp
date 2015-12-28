#include "stdafx.h"
#include "WndUtils.h"

void EmbedCvWindow( HWND pWnd, CString strWndName, int w, int h )
{
	cvNamedWindow(strWndName, 0);
	HWND hWnd = (HWND) cvGetWindowHandle(strWndName);
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, pWnd); // Ƕ�뵽pWnd����
	::ShowWindow(hParent, SW_HIDE);
	::SetWindowPos(pWnd, NULL, 0,0, w,h, SWP_NOMOVE | SWP_NOZORDER);
	cvResizeWindow(strWndName, w,h);
}

int CALLBACK BrowserCallbackProc
(
 HWND   hWnd,  
 UINT   uMsg,  
 LPARAM   lParam,  
 LPARAM   lpData  
 )
{
	switch(uMsg)  
	{  
	case BFFM_INITIALIZED:  
		::SendMessage ( hWnd, BFFM_SETSELECTION, 1, lpData );  
		break;  
	default:  
		break;  
	}  
	return 0;  
}  

bool SelDirectory ( HWND hWnd, LPCTSTR strTitle, CString &strDir )  
{  
	BROWSEINFO bi;  
	char szDisplayName[MAX_PATH] = {0};  

	bi.hwndOwner = hWnd;  
	bi.pidlRoot = NULL;  
	bi.pszDisplayName = szDisplayName;  
	bi.lpszTitle = strTitle;  
	bi.ulFlags = 0;  
	bi.lpfn = BrowserCallbackProc;  
	bi.lParam = (LPARAM)(LPCTSTR)strDir;  
	bi.iImage = NULL;  

	// ��ʾĿ¼ѡ��Ի����������ȡ������ôpiidΪNULL  
	ITEMIDLIST* piid = ::SHBrowseForFolder ( &bi );  

	if ( piid == NULL )  return false;  

	// �õ�ѡ���Ŀ¼  
	BOOL bValidPath = ::SHGetPathFromIDList ( piid, szDisplayName );  
	if ( ! bValidPath ) return false;  

	LPMALLOC lpMalloc;  
	HRESULT hr = ::SHGetMalloc ( &lpMalloc );
	assert(hr == NOERROR);  
	lpMalloc->Free ( piid );  
	lpMalloc->Release ();  

	if ( szDisplayName[0] == '\0' ) return false; //�û�ѡ��Ŀ��ܵ������ļ�ϵͳ  

	strDir = szDisplayName;  

	return true;
}

void GenFileList( CString folder, vector<SFInfo> &list )
{
	folder.TrimRight('\\');
	folder += '\\';

	CFileFind finder;
	BOOL bWork = finder.FindFile(folder+"*.*");
	vector<CString> namelist;
	while(bWork)
	{
		bWork = finder.FindNextFile(); // bWork is zero if the file found is the last one
		if (finder.IsDirectory()) continue;
		CString fn = finder.GetFileName();
		if (fn[0] == '!') continue; // ��һ����������ʶ��Щ��������ѵ����ͼƬ
		CString name = ::FindName(fn);

		vector<CString>::iterator iter = namelist.begin();
		int cnt = 0;
		for (; iter != namelist.end(); iter++)
			if (name == *iter) break;
			else cnt++;
		if (cnt == namelist.size()) namelist.push_back(name);
		list.push_back(SFInfo(cnt, fn));
	}
}

CString FindName( CString fn )
{
	int p = fn.Find('_'), q = fn.ReverseFind('.');
	return p > 0 ? fn.Left(p) : (q > 0 ? fn.Left(q) : fn);
}
