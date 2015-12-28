/*
	��ȡGabor�����������о��ȳ��������ÿ��������м��㡣
	GaborС����
			  u^2+v^2         -(u^2+v^2)*(x^2+y^2)                               -sigma^2
	G(x,y) = --------- * exp(----------------------) * [ exp(i*(u*x+v*y)) - exp(----------)]
			  sigma^2               2*sigma^2                                       2
				  u         kmax         pi*MU         kmax         pi*MU
	in which, k=(   ), u = ------ * cos(-------), v = ------ * sin(-------).
				  v         f^NU           8           f^NU           8
	in which, kmax = pi/2,f = sqrt(2).
	sigma����G���ƽ��NU����Ƶ�ʡ����߶ȡ���MU�����Ƕ���ת
	ref: Gabor Feature Based Classification Using the Enhanced Fisher Linear 
	Discriminant Model for Face Recognition, Chengjun Liu et al.

	�÷���
		Init -> GetFeature
*/
#pragma once

#define FT_HEADER

#include "FaceFeature.h"

#ifdef COMPILE_FT_GABOR

#include "../tools/tools.h"

#define PI		3.1415926535897932384626433832795
#define CV_FT_FC1	CV_32FC1 // depth = 32 or 64
#define CV_FT_FC2	CV_32FC2


class DLLEXP CFaceFeature
{
public:
	CFaceFeature(void);

	 ~CFaceFeature(void);


	void InitGaborKernel();


	int InitSamplePoints(CvMat *mask, CvSize imgSize);


	/*
		mask:		��ģͼ�񣬲�Ϊ0������Ϊ��Ч��������
		imgSize:	����ͼ��ߴ磬Ҳ����mask�ߴ�
		����:		ԭʼ�����������ȣ��������� * scaleNum * angleNum��
	*/
	int Init(CvSize imgSize, CvMat *mask);


	/*
		faceImg32:	���븡��ͼ��
		ft32:	���ԭʼ����������
	*/
	void GetFeature(CvArr *faceImg32, CvMat *ft32);


	void ShowGaborKernel();
	void GaborConv(CvArr *src, CvMat *dst, int scale, int angle);

	void Release();


	bool	m_bInited;

	/* Gabor��ز���
			scaleNum:	�߶ȳ���������NU�ĳ�����
			angleNum:	�Ƕȳ���������MU�ĳ�������MU = 0,1,...,angleNum-1
			kernelRadius: GaborС���Ĵ������������������Ϊ0�����Զ�����
			scales:		���ΪNULL����߶ȳ���NU = 0,1,...,scaleNum-1; ����scalesָ��
	*/
	double	kmax, f, sigma, th;
	int		scaleNum, angleNum, kernelRadius;
	int		*scales;

	int		kernelWidth;
	CvMat	***G;
	CvMat	*tI, *tC, *tD;


	// ������ز����ͱ���
	int		sampleIntv; // �������

	CvSize	ssz, padSize;
	CvPoint	topleft;
	CvMat	*padded;
	int		*startPts, *endPts;
};

#endif