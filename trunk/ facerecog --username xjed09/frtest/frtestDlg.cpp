
// frtestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "frtest.h"
#include "frtestDlg.h"
#include "WndUtils.h"
#include "ProjDefs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFaceMngr	*g_faceMngr;
CWebCam		g_webcam;
bool		g_bHasWebcam;

CString		g_strPicPath, g_strFacePath;
CString		g_strDefMat, g_strDefModel;


bool CfrtestDlg::InitSys()
{
	RECT rc;
	GetWindowRect(&rc);
	SetWindowPos(NULL, rc.left, 100, 0,0, SWP_NOSIZE | SWP_NOZORDER);

	ReadIni();
	g_bHasWebcam = g_webcam.Init();

	if (g_bHasWebcam)
	{
		CWnd *pWnd = GetDlgItem(IDC_VIDEO);
		CvSize frameSz = g_webcam.GetFrameSize(), showSz = frameSz;
		if (showSz.height != 240) // (240*320)��Ԥ����ʾ��С�����frameSz��������е���
			showSz *= (240.0/showSz.height);
		if (showSz.width > 320)
			showSz *= (320.0/showSz.width);
		EmbedCvWindow(pWnd->m_hWnd, "video", showSz.width, showSz.height);
		pWnd->SetWindowPos(NULL, 0,0, showSz.width+10, showSz.height+10, SWP_NOZORDER | SWP_NOMOVE);

		m_frame = cvCreateImage(frameSz, IPL_DEPTH_8U, 3);
		m_bFrameStarted = true;
		m_thrdFrame = ::AfxBeginThread(ShowFrameProc, this);
	}

	m_dlgTrain.m_strTrainFd = g_strFacePath;
	::AfxBeginThread(InitMngrProc, this);
	return true;
}

void CfrtestDlg::ReadIni()
{
	CString fn = ".\\config.ini", sn = "folders"; // �ļ�������ӡ�.\\��
	::GetPrivateProfileString(sn, "default_pic_path", DEF_PIC_PATH, 
		g_strPicPath.GetBuffer(MAX_PATH), MAX_PATH, fn);
	g_strPicPath.ReleaseBuffer();
	::GetPrivateProfileString(sn, "default_frmat", "", 
		g_strDefMat.GetBuffer(MAX_PATH), MAX_PATH, fn);
	g_strDefMat.ReleaseBuffer();
	::GetPrivateProfileString(sn, "default_frmodel", "", 
		g_strDefModel.GetBuffer(MAX_PATH), MAX_PATH, fn);
	g_strDefModel.ReleaseBuffer();
	g_strFacePath = g_strPicPath;
	g_strFacePath.TrimRight('\\');
	g_strFacePath += '\\';
	g_strFacePath += FACE_REL_PATH;
}

UINT CfrtestDlg::ShowFrame()
{
	while(m_bFrameStarted)
	{
		g_webcam.GetFrame(m_frame);
		cvShowImage("video", m_frame);
		cvWaitKey(1);
		Sleep(100);
	}

	return 0;
}

void CfrtestDlg::OnBnClickedEnroll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_dlgEnroll.DoModal();
}

void CfrtestDlg::OnBnClickedTrain()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nRet = m_dlgTrain.DoModal();
	ShowWindow(SW_SHOWNORMAL);
	if (nRet == IDOK)
	{
		SetDlgItemText(IDC_STATE, "ѵ����...");
		tic();//double t = (double)cvGetTickCount();
		vector<SFInfo> paths;

		::GenFileList(m_dlgTrain.m_strTrainFd, paths);
		DWORD flag = FM_DO_NOT_NORM;
		if (m_dlgTrain.m_bSave2Model > 0) flag |= FM_TRAIN_SAVE2MODEL;
		if (m_dlgTrain.m_bHasNormed == 0) flag |= FM_DO_NORM;
		bool ret = g_faceMngr->Train(m_dlgTrain.m_strTrainFd, paths, flag);

		SetDlgItemText(IDC_STATE, "");
		if (ret)
		{
			double t1 = toc(); // ((double)cvGetTickCount() - t) / ((double)cvGetTickFrequency()*1e6);
			CString msg;
			msg.Format("����%d�˵�%d��ͼƬ�μ�ѵ�����õ�����%dά����ά��Ϊ%dά��ѵ����ɣ���ʱ%fs��",
				g_faceMngr->m_trainclsNum, g_faceMngr->m_trainNum, 
				g_faceMngr->m_featureSz, g_faceMngr->m_modelSz, t1);
			::AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
		}
		paths.clear();
	}
}

void CfrtestDlg::OnBnClickedRecog()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (! g_faceMngr->HasTrained())
	{
		::AfxMessageBox("����ѵ����");
		return;
	}
	if (g_faceMngr->GetModelCount() == 0)
	{
		::AfxMessageBox("����ע�ᡣ");
		return;
	}
	m_dlgRecog.DoModal();
	ShowWindow(SW_SHOWNORMAL);
}

void CfrtestDlg::OnBnClickedExport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool ret1 = false;
	setlocale(LC_ALL, "Chinese-simplified"); //�������Ļ���

	if (g_faceMngr->HasTrained())
	{
		ret1 = true;
		CFileDialog dlgFile(FALSE, "frmat", "test", 
			OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT , "����ʶ������ļ� (*.frmat)|*.frmat||");
		if (dlgFile.DoModal() == IDOK)
		{
			CString path = dlgFile.GetPathName();
			ofstream os(path, ios::binary);
			if (!os)
			{
				::AfxMessageBox(CString(path)+"���޷���ȡ��");
				return;
			}
			else
			{
				g_faceMngr->WriteMatToFile(os);
				os.close();
			}
		}
	}

	if (g_faceMngr->GetModelCount() > 0)
	{
		ret1 = true;
		CFileDialog dlgFile1(FALSE, "frmodel", "test", 
			OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT, "����ģ���ļ� (*.frmodel)|*.frmodel||");
		if (dlgFile1.DoModal() == IDOK)
		{
			CString path = dlgFile1.GetPathName();
			ofstream os(path, ios::binary);
			if (!os)
			{
				::AfxMessageBox(CString(path)+"���޷���ȡ��");
				return;
			}
			else
			{
				g_faceMngr->WriteModelToFile(os);
				os.close();
			}
		}
	}

	setlocale(LC_ALL, "C");// ��ԭ

	if ( !ret1 )
	{
		::AfxMessageBox("ûʲôҪ��ģ�");
		return;
	}

	::AfxMessageBox("������ɡ�\nÿ������ģ���ļ���Ӧ����Ψһ��Ӧ�ľ����ļ�������ʱ�벻ҪŪ�졣", MB_OK | MB_ICONINFORMATION);
}

void CfrtestDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//_CrtDumpMemoryLeaks(); // ��������ͷź���֮ǰ�Ͱ����пռ��ͷŵ�����������Memory leak!
	if (g_bHasWebcam)
	{
		m_bFrameStarted = false;
		if (m_thrdFrame) ::WaitForSingleObject(m_thrdFrame->m_hThread, INFINITE);
		cvReleaseImage(&m_frame);
		cvDestroyWindow("video");
	}
	delete g_faceMngr;

	OnCancel();
}

UINT ShowFrameProc( LPVOID pParam )
{
	CfrtestDlg *pDlg = (CfrtestDlg *)pParam;
	return pDlg->ShowFrame();
}

UINT InitMngrProc( LPVOID pParam )
{
	g_faceMngr = new CFaceMngr;

	CString msg;
	if (! g_faceMngr->Init()) // ��ʼ��ʧ��
	{
		CfrtestDlg *pdlg = (CfrtestDlg *)pParam;
		pdlg->GetDlgItem(IDC_ENROLL)->EnableWindow(FALSE);
		pdlg->GetDlgItem(IDC_TRAIN)->EnableWindow(FALSE);
		pdlg->GetDlgItem(IDC_RECOG)->EnableWindow(FALSE);
		pdlg->GetDlgItem(IDC_EXPORT)->EnableWindow(FALSE);
		return 1;
	}

	setlocale(LC_ALL, "Chinese-simplified"); //�������Ļ���
	if (! g_strDefMat.IsEmpty())
	{
		ifstream is(g_strDefMat, ios::binary);
		if (!is)
			::AfxMessageBox(g_strDefMat + "���޷���ȡ��");
		else
		{
			if (! g_faceMngr->ReadMatFromFile(is)) 
				::AfxMessageBox("Ĭ��frmat�ļ���ȡʧ�ܡ�");
			is.close();
		}
	}

	if (! g_strDefModel.IsEmpty())
	{
		ifstream is(g_strDefModel, ios::binary);
		if (!is)
			::AfxMessageBox(g_strDefModel + "���޷���ȡ��");
		else
		{		
			int readNum = g_faceMngr->ReadModelFromFile(is);
			CString msg;
			msg.Format("������%d��ģ�壬����%d��ģ�塣\n��ע��ģ����ӿռ併ά����Ķ�Ӧ��", 
				readNum, g_faceMngr->GetModelCount());
			::AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
			is.close();
		}
	}

	setlocale(LC_ALL, "C");// ��ԭ
	return 0;
}