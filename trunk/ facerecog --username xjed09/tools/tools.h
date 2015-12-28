/* 
	һЩ�Ƚ�ͨ�õ�û���õ�MFC�ĺ��������㷨ģ����á�
	ʵ����cvTools.cpp��FreqTools.cpp
*/

#pragma once

#include "../opencv/PublicHeaders.h"

// �ж�IplImage�Ƿ��ǻҶ�ͼ����CvMat�Ƿ�ֻ��һ��channel
#define CV_IS_GRAY(arr)	(	(CV_IS_IMAGE_HDR(arr) && ((IplImage*)(arr))->nChannels == 1)	||	\
							(CV_IS_MAT_HDR(arr) && CV_MAT_CN(((CvMat*)(arr))->type) == 1)	)

#define VAR_NAME(x)		(#x)


#ifdef __cplusplus
extern "C"{
#endif


// Wrapper of MessageBox
DLLEXP int MessageBox1(LPCTSTR lpText, UINT uType = MB_OK | MB_ICONINFORMATION, 
				LPCTSTR lpCaption = "message", HWND hWnd = NULL);


// ��img�л�����Ȧ��
DLLEXP void MarkOut(IplImage *img, CvRect *rc, CvRect *parentRc, double scale, char shape, 
			 CvScalar color = CV_RGB(255, 0, 0), int thickness = 1);


// ����MATLAB��fftshift����Ƶ��ͼ����ĸ����޵�������ʹ��ƵŲ��ͼ������
DLLEXP void fftShift(CvArr *src_arr, CvArr *dst_arr);


/*
	�����˲���Kernel
	filterType: �˲�������,0:ideal,1:gaussian,2:butterworth
	bHighpass: true: highpass;(not supported) false: lowpass
	d0: ��ֹƵ��(���˲�����С�޹�)
	order: ����(butterworth)
	h: ����ĸ�ͨ�˲���(��һ��)
*/
DLLEXP void GenFilterKernel(CvMat *h, int filterType, /*bool bHighpass,*/ double d0, int order = 1);


// ��3ͨ����1ͨ����8λͼ��ת��Ϊ1ͨ������ͼ��
DLLEXP void Arr2gray(CvArr *src0, CvMat *dest);


// 2άfft��src must be CvMat with CV_32FC1, does fewer checks
// dest should have 2 channels, could be larger than src.
void DLLEXP fft2_M32FC1(CvMat *src, CvMat *dest);


// ����2ͨ����������ķ���
DLLEXP void Magnitude(CvArr *input, CvMat *mag);


// img��8λͼ��򸡵�2ͨ������Ƶ�׾��󣬶�Ƶ�׷�����log�任����ʾ
DLLEXP void ShowMagnitude(CvArr *img, bool has_done_dft);


// Ƶ���˲���kr��ԭ�˲��˵İ뾶(���ڿ��=kr*2+1)��kernelF��Ƶ����˲��ˣ�����Ӧ��
// cvGetOptimalDFTSize( max(pic->rows, pic->cols) + kr*2 )��������� kr*2 ��Ϊ��ȥ��ѭ�����Ӱ��
// ���������Ҫ�����ͬһ���˲��ˣ�����˲����������FFT���١�res������1��2ͨ����
void filter_freq_strict(CvMat *pic, CvMat *kernelF, int kr, CvMat *res);


// Debugģʽ�У���TRACE�����������ʾCvArr
DLLEXP void DispCvArr(CvArr *a, char *varName = VAR_NAME(varName), bool showTranspose = false, char *format = NULL); // for 1D/2D 1 channel mat/img


DLLEXP void DispCvArrMultiChannel(CvArr *a, char *varName = VAR_NAME(varName)); // for 1D/2D N channel mat/img


/*
	concatenate all the arrays in pppa
	e.g. 
	CvMat *m00,*m01,...
	CvMat ***pppa = new CvMat **[2];
	pppa[0] = {m00, m01};
	pppa[1] = {m10, m11, m12};
	=> dst = [m00,m01; m10,m11,m12];
	if every thing is all right.
	��ָ����colNum,���������������ͬ��colNums��ΪNULL������ӦʹcolNum=0
*/
DLLEXP void ConcatArrs( CvArr ***pppa, CvMat *dst, int rowNum, int colNum, int *colNums = NULL);


// ���pppa��������Ƿ��ܹ�concatenate,�����򷵻���size������assert�ж�
DLLEXP CvSize CheckConcat(CvArr ***pppa, int rowNum, int colNum, int *colNums);


// �ļ�����
DLLEXP void WriteStringLine(ofstream &os, CString str);
DLLEXP void ReadStringLine(ifstream &is, CString &str);
DLLEXP void WriteIntText(ofstream &os, int n, CString name = "");
DLLEXP void ReadIntText(ifstream &is, int &n, bool hasName = true);

// ע��mat��Ӧ��һ������ͨ��getsubrect��ʽ�õ����Ӿ��󣬼�mat��stepӦ�õ����� ����*ͨ����*Ԫ���ֽ���
DLLEXP void WriteCvMatBin(ofstream &os, CvMat *mat, CString name = "");
DLLEXP void ReadCvMatBin(ifstream &is, CvMat *mat, bool hasName = true);

inline void operator /=(CvSize &a, double b)
{
	a.height = int(a.height / b);
	a.width = int(a.width / b);
}

inline void operator *=(CvSize &a, double b)
{
	a.height = int(a.height * b);
	a.width = int(a.width * b);
}

inline CvSize operator *(CvSize a, double b)
{
	return cvSize(int(a.width * b), int(a.height * b));
}

inline CvSize operator /(CvSize a, double b)
{
	return cvSize(int(a.width / b), int(a.height / b));
}

DLLEXP void tic(); // ��ʱ��ʼ

DLLEXP double toc(); // ���ش��ϴ�tic�����ڵ�����

#ifdef __cplusplus
}
#endif
