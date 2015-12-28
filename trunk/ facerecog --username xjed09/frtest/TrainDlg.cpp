// TrainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "frtest.h"
#include "TrainDlg.h"
#include "ProjDefs.h"
#include "WndUtils.h"

// CTrainDlg �Ի���

IMPLEMENT_DYNAMIC(CTrainDlg, CDialog)

CTrainDlg::CTrainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrainDlg::IDD, pParent)
	, m_bHasNormed(TRUE)
	, m_bSave2Model(TRUE)
{
}

CTrainDlg::~CTrainDlg()
{
}

void CTrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_NORMED, m_bHasNormed);
	DDX_Check(pDX, IDC_SAVE2MODEL, m_bSave2Model);
	DDX_Text(pDX, IDC_TRAINFD, m_strTrainFd);
}


BEGIN_MESSAGE_MAP(CTrainDlg, CDialog)
	ON_BN_CLICKED(IDC_BROWSE_TRAINFD, &CTrainDlg::OnBnClickedBrowseTrainfd)
	ON_BN_CLICKED(IDC_IMPORT_MAT, &CTrainDlg::OnBnClickedImportMat)
END_MESSAGE_MAP()

BOOL CTrainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}

// CTrainDlg ��Ϣ�������

void CTrainDlg::OnBnClickedBrowseTrainfd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//char str[501];
	//::GetCurrentDirectory(500, str);
	//m_strTrainFd = m_strTrainFd;
	bool ret = SelDirectory(NULL, "ѡ��ѵ��ͼƬ�����ļ���", m_strTrainFd);
	if (!ret) return;
	UpdateData(FALSE);
}

void CTrainDlg::OnBnClickedImportMat()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlgFile(TRUE, "frmat", "test", 
		OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
		"����ʶ������ļ� (*.frmat)|*.frmat||");
	if (dlgFile.DoModal() == IDOK)
	{
		CString path = dlgFile.GetPathName();
		setlocale(LC_ALL, "Chinese-simplified"); //�������Ļ���
		ifstream is(path, ios::binary);
		if (!is)
		{
			::AfxMessageBox(CString(path)+"���޷���ȡ��");
			return;
		}

		if (g_faceMngr->ReadMatFromFile(is))
			::AfxMessageBox("������ɡ�\n��ע��ģ����ӿռ併ά����Ķ�Ӧ��", MB_OK | MB_ICONINFORMATION);

		is.close();
		setlocale(LC_ALL, "C");// ��ԭ
	}
	OnCancel();
}
