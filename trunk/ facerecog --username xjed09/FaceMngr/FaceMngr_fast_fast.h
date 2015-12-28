 /*
	����CFaceAlign,CLightPrep,CFaceFeature,CSubspace�ĸ��࣬
	ʵ�ֽ��������⡢ѵ����ʶ��Ⱦ��幦�ܡ�
*/

#pragma once

#define MNGR_HEADER

#include "FaceMngr.h"

#ifdef COMPILE_MNGR_FAST_FAST

#include "../FaceAlign/FaceAlign.h"
#include "../LightPrep/LightPrep.h"
#include "../FaceFeature/FaceFeature.h"
#include "../Subspace/Subspace.h"
#include "../tools/tools.h"

#pragma comment(lib, "../Subspace/dggev.lib")

struct SFInfo // Struct of File Info��ѵ������������������ʱ���������ļ���Ϣ�Ľṹ��
{
	int		classId;
	CString picPath;
	CvPoint2D32f leye;
	CvPoint2D32f reye;
	SFInfo(int i,CString s):classId(i),picPath(s){}
};

typedef vector<SFInfo>::iterator	sfi_iter;


class DLLEXP CFaceMngr
{
public:
	CFaceMngr(void);

	~CFaceMngr(void);

	bool	Init(CvSize faceSz);

	double run_orl( LPCTSTR rp, vector<SFInfo> & trainList, vector<SFInfo> & testList, int * resultList );

private:
	int		NormFaceRecog(CvMat *faceImg32);


	CvMat		*tfaceImg8, *tfaceImg32, *tfeature, *tmodel; // �м����
	CvMat		*m_models;

	CFaceAlign		*align;
	CLightPrep		*light;
	CFaceFeature	*feature;
	CSubspace		*ss;

	CvSize	m_faceSz;
	int		m_featureSz, m_modelSz; // ԭʼ����ά���ͽ�ά��ά��
	int		m_trainNum, m_classNum; // ѵ����������ѵ�������

	int		*trainIds;
};

#endif