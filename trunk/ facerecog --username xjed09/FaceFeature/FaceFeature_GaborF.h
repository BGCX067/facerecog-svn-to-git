/*
	��ȡGabor�����������о��ȳ�������Ƶ����м��㣬�ٶȱȿ�����Լ1����
	�÷���
		Init -> GetFeature
*/
#pragma once

#define FT_HEADER

#include "FaceFeature.h"

#ifdef COMPILE_FT_GABORF

#include "../tools/tools.h"


class DLLEXP CFaceFeature
{
public:
	CFaceFeature(void);

	 ~CFaceFeature(void);


	void InitGaborKernel();

	int InitSamplePoints(CvMat *mask);

	int Init(CvSize imgSize, CvMat *mask);

	// ft32 ������������
	void GetFeature(CvArr *faceImg32, CvMat *ft32);

	void ShowGaborFace(CvArr *faceImg32);

	void Release();

	bool	m_bInited;

	// Gabor��ز����ͱ���
	double	kmax, f, sigma, th;
	int		scaleNum, angleNum, kernelRadius;
	int		*scales;

	CvSize	m_faceSz;
	int		kernelWidth, fsz;
	CvMat	***G;
	CvMat	*tPicR, *tPicI, *tPicF0, *tPicF;


	// ������ز����ͱ���
	int		sampleIntv;

	int		smpPtNum;
	int		*smpPts; // ���������ţ�����ֱ��ָ�����
};

#endif