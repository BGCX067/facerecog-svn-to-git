#pragma once

/*
	����asmlibrary�������������ۼ�⡣
	ʹ�÷�����
	���� Init��֮����� GetFace���Զ�Release��
	ע���������ͼƬ̫��ʱ����ܻ�ϳ���Ӧ�ʵ���СͼƬ��С��
	ע�����������Ҫ�� FDFN �ļ��� asmlibrary ��Ҫ�� ASMFN �ļ�
*/

#include "../tools/tools.h"
#include "asmfitting.h"

#define INPUT_PATH	"..\\input\\"

#define FDFN	INPUT_PATH "haarcascade_frontalface_alt.xml"
#define ASMFN	INPUT_PATH "AsmModel.amf"


class DLLEXP CFaceAlign
{
public:
	CFaceAlign(void);
	 ~CFaceAlign(void);

private:
	asmfitting			m_AsmFit;
	asm_vjfacedetect	m_FDinAsm;
	asm_shape			m_shape;

public:
	CvSize	m_faceSz;
	float	normRow, normDis; // ��һ�����۾��ľ���ͼ���ϱ߽�ľ�������۾���
	int		m_nFiltLevel; // �á��˲�����ʽȥ�����۶�λ������ʱ��ƽ���˲����Ŀ��

	CvRect	m_rcCurFace;
	float	m_angle, m_dis; // ���۽ǶȺ;���

	int idx;
	CvPoint2D32f leftEye, rightEye;
	CvPoint2D32f *lefta;
	CvPoint2D32f *righta;

private:
	bool InitAsm();

public:
	 bool Init(CvSize faceSz = cvSize(0,0));


	// �������ۼ��󣬶�input����ͼ�������ת�����š�����
	 bool EyeCoord2FaceCrop( IplImage * pic8, CvMat * faceImg, bool useBuf );


	// ���faceIdx = -1�򷵻���������������ѡ��faceIdx(=0,1,...)����������û����ô�����򷵻�false
	// Ϊ���Ч�ʣ�faceIdx>0ʱ�����¼�����������Ҫһ�μ��������ʱӦ��������
	// ����asmlibrary���ṩ�ķ��������������(ʵ���õ���OpenCV 1.0�е�Haar Cascade)�����������㶨λ
	 bool AsmDetectFeaturePoints(IplImage *pic8, int faceIdx = -1);

	// ���� AsmDetectFeaturePoints �� EyeCoord2FaceCrop
	// �������� HaarFacesCropByEye
	 bool GetFace(IplImage *pic8, CvMat *faceImg, bool useBuf);

};

#endif