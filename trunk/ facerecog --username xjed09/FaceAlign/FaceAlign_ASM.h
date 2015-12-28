/*
	����Yao Wei��asmlibrary (http://code.google.com/p/asmlibrary/) ����˫��������ж��롣
	��׼ȷ���õ���OpenCV 1.0.
	ʹ�÷�����
	���� Init��֮����� GetFace���Զ�Release��
	ע���������ͼƬ̫��ʱ����ܻ�ϳ���Ӧ�ʵ���СͼƬ��С��
	ע�����������Ҫ�� FDFN �ļ��� asmlibrary ��Ҫ�� ASMFN �ļ�
*/
#pragma once

#define ALIGN_HEADER

#include "FaceAlign.h"

#ifdef COMPILE_ALIGN_ASM

#include "../tools/tools.h"
#include "asmfitting.h"

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

	CvRect	m_rcCurFace; // ��ǰ����λ�úʹ�С
	float	m_angle, m_dis; // ���۽ǶȺ;���

	CvPoint2D32f leftEye, rightEye; // ��ǰ����λ��
	CvPoint2D32f *lefta, *righta; // ǰ��������λ�õĻ���

private:
	bool InitAsm();

public:
	 bool Init(CvSize faceSz = cvSize(0,0));


	// �������ۼ��󣬶�input����ͼ�������ת�����š�����
	// ǰ����������������8u����;���useBuf = true�����ǰ�󼸴ζ�λ�������ƽ������������
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
