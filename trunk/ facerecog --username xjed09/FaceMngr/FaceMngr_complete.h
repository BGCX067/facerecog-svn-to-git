 /*
	����CFaceAlign,CLightPrep,CFaceFeature,CSubspace�ĸ��࣬
	ʵ�ֽ��������⡢ѵ����ʶ��Ⱦ��幦�ܡ�
*/

#pragma once

#define MNGR_HEADER

#include "FaceMngr.h"

#ifdef COMPILE_MNGR_COMPLETE

#include "../FaceAlign/FaceAlign.h" // ������Щinclude���ŵ� FaceMngr.h �е�ԭ���ǣ�Ϊ�˼���#ifdefǰ������ı���ʱ��
#include "../LightPrep/LightPrep.h"
#include "../FaceFeature/FaceFeature.h"
#include "../Subspace/Subspace.h"
#include "../tools/tools.h"

#pragma comment(lib, "../FaceAlign/asmlibrary.lib")
#pragma comment(lib, "../Subspace/dggev.lib")

#pragma warning(disable: 4251) // ����ʾ��Ϣ��class��std::vector<_Ty>����Ҫ�� dll �ӿ��� class��CFaceMngr���Ŀͻ���ʹ��

struct SModel // ģ��������ڼ�¼��Ϣ�Ľṹ��
{
	int		classId;
	CString	picPath; // ԴͼƬ·��
	CvMat	*model;
};

struct SMatch // ʶ������ڼ�¼��Ϣ�Ľṹ��
{
	int		classId;
	CString	picPath; // ԴͼƬ·��
	double	dist;    // ƥ�����
};

struct SFInfo // Struct of File Info��ѵ������������������ʱ���������ļ���Ϣ�Ľṹ��
{
	int		classId;
	CString picPath;
	CvPoint2D32f leye;
	CvPoint2D32f reye;
	SFInfo(int i,CString s, CvPoint2D32f l = cvPoint2D32f(0,0), CvPoint2D32f r = cvPoint2D32f(0,0)):
		classId(i), picPath(s), leye(l), reye(r){}
};

typedef vector<SModel>::iterator	smd_iter;
typedef vector<SFInfo>::iterator	sfi_iter;
typedef vector<SMatch>::iterator	smt_iter;

#define DEF_PIC_PATH	"facePics\\" // Ĭ��ͼƬ·���͹�һ�������洢·��
#define FACE_REL_PATH	"faces\\" // ������Լ����·������಻��'\',�ļ��е��Ҳ��'\'


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


	// ��ģ����Ϣ��������������ģ��⡣
	void	SaveToModel(LPCTSTR path, int classId, CvMat *model);


	/*
		��ͼƬ�е�����������еĻ�������ģ���
		strPath:	ͼƬ·��
		classId:	���������ţ�������FM_UNKNOWN_CLASS_ID
		flag:		������Pic2NormFace����flag��FM_SAVE_NORM_FACE�����
	*/
	bool	SavePicToModel(LPCTSTR strPath, int classId, DWORD flag);
	

	// ���ģ���
	void	ClearList();


	// ����ģ������
	int		GetModelCount();


/// ѵ��

	/*
		��������ѵ���ļ��ĸ�·���������ǵ���·��+���ѵ��ͶӰ����
		���磬rootPath = D:\\face\\��paths = {{"John_1.bmp",1}, {"John_2.bmp",1}, {"Mike_1.bmp",5},...}
		flag������Pic2NormFace����flag��FM_SAVE_NORM_FACE, FM_SHOW_DETAIL, FM_TRAIN_SAVE2MODEL�����
		Ŀǰֻ��ͼƬ�м��������������ѵ����
	*/
	bool	Train(LPCTSTR rootPath, vector<SFInfo> &paths, DWORD flag);


	// �����Ƿ���ѵ��
	bool	HasTrained();
private:

	// ���� m_lstModel ��������ѵ���ľ��� inputs �����ID trainIds
	void	FormTrainMat(CvMat *inputs, int *trainIds);


	// ѵ���󣬽�ѵ������ͶӰ������ģ���
	void	TrainResSave2Model();


/// ʶ��

public:

	// ע���ѵ����Ϻ󣬶������model����ƥ�䣬ƥ������Ϣ����info
	// ��ʱֻ�������ƶ����ģ�Ҳ���Ը�Ϊ����ǰ����
	bool	ModelRecognize(CvMat *model, SMatch *info);


	// ���� Pic2NormFace, NormFace2Model, ModelRecognize��ֱ�Ӷ�δ��һ��ͼƬ����ʶ�𣬽������info
	// flag��Pic2NormFace����flag��ͬ
	bool	PicRecognize(CvArr *pic, DWORD flag, SMatch *info);


	// ��testList�е��ļ�����ʶ�𣬽������resList��flag��Pic2NormFace����flag��ͬ
	double	BatchPicRecog( LPCTSTR rootPath, vector<SFInfo> &testList, vector<SMatch> &resList, DWORD flag );

/// ����/���

	bool	WriteMatToFile( ofstream &os );
	bool	WriteModelToFile( ofstream &os );
	bool	ReadMatFromFile( ifstream &is );
	int		ReadModelFromFile( ifstream &is );

/// ����

	vector<SModel>	m_lstModel; // ģ���

	CvMat		*tfaceImg8, *tfaceImg32, *tfeature, *tmodel; // �м����

	CFaceAlign		*align;
	CLightPrep		*light;
	CFaceFeature	*feature;
	CSubspace		*ss;

	CString m_rtPath;
	CvSize	m_faceSz;
	int		m_featureSz, m_modelSz; // ԭʼ����ά���ͽ�ά��ά��
	int		m_trainNum, m_trainclsNum; // ѵ����������ѵ�������

	int	stamp; // Ϊ�˷�ֹ�����modelŪ�죬ÿ��ѵ���õ��ľ��󶼻���һ��stamp(��ʱ��ʱ��)���洢���ļ��С�
};

#endif