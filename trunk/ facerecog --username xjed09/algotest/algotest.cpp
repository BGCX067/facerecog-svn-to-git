// ��������������㷨���ԣ�Ŀǰ֧��ORL���FERET��
// �Ƚ���ֵ��ǣ���Matlab 2009b ���ƵĲ��Գ������У�Matlab������c��һЩ���²���Matlab��������Ͽ�
// FaceMngr��Ҫ�� FaceMngr.h �ж��� COMPILE_MNGR_FAST��FERET��������ܻ���Ҫ����һ��

#include "algotest.h"

ofstream os;
CString msg;

CFaceMngr fm;

CString	rootPath;
DWORD	trainFlag, enrFlag, testFlag;
CvSize	faceSz;

void out(LPCTSTR str)
{
	//TRACE(str);
	os<<str;
}

void main()
{
	os.open("..\\output.txt", ios::app); // ������Ϣ���ļ�

	CString msg;
	time_t ti = time(0);
	char tmp[64]; // ��¼����ʱ��
	strftime(tmp, sizeof(tmp), "%m/%d %X %a",localtime(&ti));
	msg.Format("\n%s\t", tmp);
	out(msg);

	out("feret\n");

	run_orl();

	os.close();
	//system("pause");
}
