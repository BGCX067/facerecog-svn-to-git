/*
	�����ӿռ��ͶӰ��Ŀǰʹ�õ���Fisherface����
	ʹ�÷�����
		Train �� LoadDataFromFile -> Project -> CalcVectorDist
	����������Ӧ����������
*/

#pragma once

#include "../tools/tools.h"
#include "dggev.h"

#define CV_COEF_FC1 CV_64FC1 // ͶӰ����ľ��ȣ�depth = 32 or 64, 64 seems to be better


class DLLEXP CSubspace
{
public:
	CSubspace(void);
	 ~CSubspace(void);

	/*
		��inputs������������������� mu_total �� W_pcaT��
		inputs:	���������������гɵľ���
		postPcaDimCoef:	�������������ֱ��ָʾPCA���ά�ȣ�
						�����0~1��С����������ֵ��ռ���������ӿռ�ά�ȣ�Ŀǰʹ��.97��
	*/
	void Pca(CvMat *inputs, double postPcaDimCoef);


	/*
		inputs:		����Pca����ÿ�о�ֵӦ��0(��Ӧ���ȼ�ȥ��ֵ����)��inputs�ᱻ�ı䡣
		trainIds:	ѵ�����������ID�����鳤��Ӧ��inputs��������ͬ��
		postLdaDim:	LDA���ά�ȡ����ȡ0�����Զ�ѡΪ �����-1��
					����P. N. Belhumeur��Eigenfaces vs. fisherfaces: recognition using 
					class specific linear projection��
					��ʱ��� �����-1 < 3��Ϊ��ֹά����С��ά��ȡΪ min(ѵ��������, 3)��

		Sw,Sb��Matlab����һ�£�clapack����Ĺ�������ֵ��������������Matlab������
		����CalcSwSb��CalcLdaSpace��ѵ��������浽 W_fldT
	*/
	void FisherLda(CvMat *inputs, int *trainIds, int postLdaDim = 0);

	// ���� Pca �� FisherLda���������� FisherLda��ѵ��������浽 W_pcafldT
	void Train(CvMat *inputs, int *trainIds);


	// inputs ������һ�����������������гɵľ���
	void Project(CvMat *inputs, CvMat *results);


	// �����ӿռ�ά��
	int GetSubspaceDim();


	// �������ά������1���������Ҿ�����෴��������ԽСԽ���ƣ����������ŷ�Ͼ��롣
	double CalcVectorDist(CvMat *target, CvMat *query);


	// ���ַ����Ͷ����ƻ�ϵķ�ʽ������/���������󾫶ȣ�ԭʼ����ά�ȣ���ά��ά�ȣ�mu_total��W_pcafldT
	bool WriteDataToFile(ofstream &os);
	bool ReadDataFromFile(ifstream &is);


	void Release();

private:

	// �������ں����ɢ������
	void CalcSwSb(CvMat *inputs, int *trainIds, CvMat *Sw, CvMat *Sb);

	// ����Fisherface�ӿռ�
	void CalcLdaSpace(CvMat *Sw64, CvMat *Sb64, int postLdaDim);

public:

	CvMat	*mu_total;
	CvMat	*W_pcaT, *W_fldT, *W_pcafldT;
	int		inputDim, postPcaDim, postLdaDim;
	int		trainNum, classNum; // ѵ��ʱ���������������
	bool	m_bTrained;
};
