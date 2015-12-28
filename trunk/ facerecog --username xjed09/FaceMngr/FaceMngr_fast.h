/*
	�������������ã�������һЩ���ͼ��ȣ����ҽ�ģ����CvMat������vector�洢����Լ��һ��ʱ�䡣
	FaceAlign��Ҫ��COMPILE_USE_COORD.
	�÷���Train->(BatchPicEnroll)->BatchPicRecog
*/

#pragma once

#define MNGR_HEADER

#include "FaceMngr.h"

#ifdef COMPILE_MNGR_FAST

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
	SFInfo(int i,CString s, CvPoint2D32f l = cvPoint2D32f(0,0), CvPoint2D32f r = cvPoint2D32f(0,0)):
		classId(i), picPath(s), leye(l), reye(r){}
};

struct SMatch // ʶ������ڼ�¼��Ϣ�Ľṹ��
{
	int		classId;
	CString	picPath; // ԴͼƬ·��
	double	dist;    // ƥ�����
};

typedef vector<SFInfo>::iterator	sfi_iter;
typedef vector<SMatch>::iterator	smt_iter;


class DLLEXP CFaceMngr
{
public:
	CFaceMngr(void);
	~CFaceMngr(void);

	// ����CFaceAlign,CLightPrep,CFaceFeature�ĳ�ʼ����������ȷ����true
	bool	Init(CvSize faceSz = cvSize(0,0));


/// ע��

	// ��ͼ����м��Ρ����չ�һ�����ɹ�����true��flag�����FaceMngr.h�����磬pic��Ҫ���ι�һ��������Ҫ���չ�һ��������flag = FM_DO_FACE_ALIGN
	bool	Pic2NormFace(CvArr *pic, CvMat *faceImg8, DWORD flag = FM_DO_NORM, CvPoint2D32f *leftEye = NULL, CvPoint2D32f *rightEye = NULL);


	// �Թ�һ��������ȡԭʼ�������������н�ά����Ҫ��ѵ�������򷵻�false
	bool	NormFace2Model(CvArr *faceImg8, CvMat *model);


	// ����ע�ᣬע���һ�����л�ɾȥǰһ�ε�ע�����ݡ�
	// flag������Pic2NormFace����flag��FM_SHOW_DETAIL����ϡ�
	bool	BatchPicEnroll(LPCTSTR rootPath, vector<SFInfo> &enrList, DWORD flag);


/// ѵ��

	/*
		��������ѵ���ļ��ĸ�·���������ǵ���·��+���ѵ��ͶӰ����
		���磬rootPath = D:\\face\\��paths = {{"John_1.bmp",1}, {"John_2.bmp",1}, {"Mike_1.bmp",5},...}
		flag������Pic2NormFace����flag��FM_SAVE_NORM_FACE, FM_SHOW_DETAIL, FM_TRAIN_SAVE2MODEL�����
		Ŀǰֻ��ͼƬ�м��������������ѵ����
	*/
	bool	Train(LPCTSTR rootPath, vector<SFInfo> &trainList, DWORD flag);


	// �����Ƿ���ѵ��
	bool	HasTrained();


/// ʶ��

public:

	// ע���ѵ����Ϻ󣬶������model����ƥ�䣬ƥ������Ϣ����info
	// ��ʱֻ�������ƶ����ģ�Ҳ���Ը�Ϊ����ǰ����
	bool	ModelRecognize(CvMat *model, SMatch *info);


	// ���� Pic2NormFace, NormFace2Model, ModelRecognize��ֱ�Ӷ�δ��һ��ͼƬ����ʶ�𣬽������matchId
	// flag��Pic2NormFace����flag��ͬ
	bool	PicRecognize(CvArr *pic, DWORD flag, SMatch *info, CvPoint2D32f *leye = NULL, CvPoint2D32f *reye = NULL);


	// ����PicRecognize��������ʶ�𣬷�����ȷ�ʡ�
	// testList��Ӧ������ȷId���Ա������ȷ�ʣ�resList�Ǻ�testList�ȳ������飬����matchId
	// flag������Pic2NormFace����flag��FM_SHOW_DETAIL����ϡ�
	double	BatchPicRecog( LPCTSTR rootPath, vector<SFInfo> &testList, vector<SMatch> &resList, DWORD flag );

/// ����/���

	bool	WriteMatToFile( ofstream &os );
	bool	ReadMatFromFile( ifstream &is );

/// ����

	CvMat		*tfaceImg8, *tfaceImg32, *tfeature, *tmodel, *m_models; // �м����

	CFaceAlign		*align;
	CLightPrep		*light;
	CFaceFeature	*feature;
	CSubspace		*ss;

	CvSize	m_faceSz;
	int		m_featureSz, m_modelSz; // ԭʼ����ά���ͽ�ά��ά��
	int		m_trainNum, m_trainclsNum; // ѵ����������ѵ�������
	int		m_enrNum; // ע��������
	int		*m_ids; // ģ��������б�

};

#endif