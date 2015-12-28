/*
	����OpenCV����Haar Cascade�������������������ۼ�⣬Ȼ�������ۼ��������align
	ʹ�÷�����
	���� Init��֮����� GetFace���Զ�Release��
	ע���������ͼƬ̫��ʱ����ܻ�ϳ���Ӧ�ʵ���СͼƬ��С��
	ע����Ҫ�� FDFN �� EDFN �ļ�.
*/

#pragma once

#include "../tools/CompDefs.h"

#ifdef COMPILE_ALIGN_HAAREYE

#include "../tools/tools.h"

#define FDFN	"haarcascade_frontalface_alt.xml"
#define EDFN	"haarcascade_eye_tree_eyeglasses.xml"


class  DLLEXP CFaceAlign
{
public:
	CFaceAlign(void);
	 ~CFaceAlign(void);

private:

	CvHaarClassifierCascade	*m_FDcascade, *m_EDcascade;
	CvMemStorage			*m_FDstorage, *m_EDstorage;


public:
	CvSize	m_faceSz;
	CvRect	m_rcCurFace;
	CvPoint2D32f leftEye, rightEye;
	float	m_angle, m_dis; // ���۽ǶȺ;���
	float	normRow, normDis; // ��һ�����۾��ľ���ͼ���ϱ߽�ľ�������۾���
	static const int m_nFiltLevel = 3; // �á��˲�����ʽȥ�����۶�λ������ʱ��ƽ���˲����Ŀ��


public:
	 bool Init();
	bool InitHaarFace();
	bool InitHaarEye();
	
	/*
		���� HaarFaceDetect2 ��HaarEyeDetect �� EyeCoord2FaceCrop
		input:		����İ����򲻰���������8Uͼ��
		faceImg:	�ߴ�Ϊ	m_faceSz ��8UC1�������
		useBuf:		���Ϊ�棬���Ա��κ�ǰ filtLevel-1 �����ۼ���������ƽ��������������
					���鵱���յ������ϲ�ʱʹ��
	*/
	 bool GetFace( IplImage *input, CvMat *faceImg, bool useBuf );


	// ��OpenCV 2.0�еĺ�������������⣬�����⵽������true������Rect����m_rcCurFace
	// ��ͬһ��xml�ļ�ʱ��OpenCV 2.0�ļ��Ч��������1.0����Rect�ƺ��Դ�һЩ
	bool HaarFaceDetect2(IplImage *input);


	// Ӧ�ñ�֤input�����ֻ��һֻ�۾�
	bool HaarEyeDetect(IplImage *input, CvPoint2D32f *eye);


	// �������ۼ��󣬶�input����ͼ�������ת�����š�����
	 bool EyeCoord2FaceCrop( IplImage * input, CvMat * faceImg, bool useBuf );


	 void Release();
};

extern "C" DLLEXP CFaceAlign *GetFaceAlign();

#endif