/*
	����OpenCV 2.0�Դ���Haar Cascade��������������⣬��������Ϊ������
	ʹ�÷�������FaceAlign_ASM��Ч���Բ
	ע�����������Ҫ�� FDFN �ļ�.
*/

#pragma once

#define ALIGN_HEADER

#include "FaceAlign.h"

#ifdef COMPILE_ALIGN_HAAR

#include "../tools/tools.h"

#define FDFN	INPUT_PATH "haarcascade_frontalface_alt.xml"


class DLLEXP CFaceAlign
{
public:
	CFaceAlign(void);
	~CFaceAlign(void);

private:
	CvHaarClassifierCascade	*m_FDcascade;
	CvMemStorage			*m_FDstorage;

public:
	CvRect	m_rcCurFace;
	CvSize	m_faceSz; // Ԥ����face size����GetFace�����faceImg��Ϊ����size
	int		m_dis; // ��������۾���
	static const int m_nFiltLevel = 3; // ��������λ�������ƽ���Ľ���

public:
	bool Init();


	// �������۶�λ������ж��룬ֱ�ӽ�Haar�����������faceImg��С����faceImg
	// ��OpenCV 2.0�еĺ�������������⣬�����⵽������true������Rect����m_rcCurFace
	// ���bUseBuf = true������ǰ��m_nFiltLevel��������λ�������ƽ��
	// ��ͬһ��xml�ļ�ʱ��OpenCV 2.0�ļ��Ч��������1.0����Rect�ƺ��Դ�һЩ
	// only support 8-bit unsigned input
	bool GetFace(IplImage *input, CvMat *faceImg, bool bUseBuf);


	void Release();
};


#endif
