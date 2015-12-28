/*
	������������������������롣
	ʹ�÷�������FaceAlign_ASM��
*/
#pragma once

#define ALIGN_HEADER

#include "FaceAlign.h"

#ifdef COMPILE_ALIGN_COORD

#include "../tools/tools.h"


class DLLEXP CFaceAlign
{
public:
	CFaceAlign(void);
	~CFaceAlign(void){}

	bool Init(CvSize faceSz = cvSize(0,0));

	// �����leftEye��ͼ�������۾����꣬Ҳ����ͼ�����˵�����
	bool GetFace(IplImage *pic8, CvMat *faceImg, CvPoint2D32f *leftEye, CvPoint2D32f *rightEye);

	CvSize	m_faceSz;
	float	normRow, normDis; // ��һ�����۾��ľ���ͼ���ϱ߽�ľ�������۾���

	float	m_angle, m_dis; // ���۽ǶȺ;���
};

#endif