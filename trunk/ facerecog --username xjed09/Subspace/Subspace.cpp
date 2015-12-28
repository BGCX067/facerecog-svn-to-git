#define DLLSRC

#include "Subspace.h"

#pragma comment(lib, "dggev.lib") // CLAPACK�Ĺ�������ֵ���㺯��


CSubspace::CSubspace(void)
{
	mu_total = W_pcaT = W_fldT = W_pcafldT = NULL;
	m_bTrained = false;
}

CSubspace::~CSubspace(void)
{
	Release();
}

void CSubspace::Pca( CvMat *inputs, double postPcaDimCoef )
{
	int		oriDim = inputs->rows, sampleNum = inputs->cols;
	CvMat	*eigenVecs = cvCreateMat(sampleNum, oriDim, CV_COEF_FC1),
		*eigenVals = cvCreateMat(sampleNum, 1, CV_COEF_FC1);
	//cvReleaseMat(&mu_total);
	mu_total = cvCreateMat(oriDim, 1, CV_COEF_FC1);

	//CvMat *cov_mat = cvCreateMat(sampleNum, sampleNum, CV_COEF_FC1);
	//cvCalcCovarMatrix(inputs, sampleNum, cov_mat, mu_total, CV_COVAR_SCRAMBLED);
	//CvMat *U = cvCreateMat(sampleNum, sampleNum, CV_COEF_FC1),
	//	*S = cvCreateMat(sampleNum, 1, CV_COEF_FC1);
	//W_pcaT = cvCreateMat(postPcaDim, oriDim, CV_COEF_FC1);
	//cvSVD(cov_mat, S, U, NULL, CV_SVD_MODIFY_A | CV_SVD_U_T);

	cvCalcPCA(inputs, mu_total, eigenVals, eigenVecs, CV_PCA_DATA_AS_COL); 
	// ����ֵ�ƺ��ǰ��Ӵ�С���еģ���Matlab������в��
	// ÿ����������Ϊ�������������ƺ������˹�һ��

	postPcaDim = int(postPcaDimCoef);
	if (postPcaDim == 0) // postPcaDimCoef��С�����򰴱���ȷ��PCA��ά��
	{
		double sum = cvSum(eigenVals).val[0], sum1 = 0;
		int i = 0;
		do 
		{
			sum1 += cvmGet(eigenVals, i++, 0);
		} while (sum1 < sum*postPcaDimCoef);
		postPcaDim = i;
		if (postPcaDim < 8) postPcaDim = min(sampleNum, 8); // postPcaDim��Ҫ̫С��8Ϊ�ݶ�
	}

	CvMat sub;
	//cvReleaseMat(&W_pcaT); // ���֮ǰѵ����һ��
	W_pcaT = cvCreateMat(postPcaDim, oriDim, CV_COEF_FC1);
	cvGetSubRect(eigenVecs, &sub, cvRect(0,0, oriDim, postPcaDim));
	cvCopy(&sub, W_pcaT);
	//DispCvArr(W_pcaT, "Wpca");

	cvReleaseMat(&eigenVals);
	cvReleaseMat(&eigenVecs);
}

void CSubspace::CalcSwSb( CvMat *inputs, int *trainIds, CvMat *Sw, CvMat *Sb )
{
	int		oriDim = inputs->rows, sampleNum = inputs->cols;
	int		*id2idx = new int[sampleNum], *smpNumEachClass = new int[sampleNum];
	CvMat	**muPerClass = new CvMat *[sampleNum], sub;
	classNum = 0;

	// �������ھ�ֵ����
	for (int i = 0; i < sampleNum; i++)
	{
		cvGetCol(inputs, &sub, i);

		int j;
		for (j = 0; j < classNum; j++)
		{
			if (id2idx[j] == trainIds[i]) 
			{
				break;
			}
		}
		if (j == classNum)
		{
			muPerClass[j] = cvCloneMat(&sub);
			id2idx[j] = trainIds[i];
			smpNumEachClass[j] = 1;
			classNum++;
		}
		else
		{
			cvAdd(muPerClass[j], &sub, muPerClass[j]);
			smpNumEachClass[j] ++;
		}
	}

	for (int i = 0; i < classNum; i++)
		cvScale(muPerClass[i], muPerClass[i], 1.0/(double(smpNumEachClass[i])));

	// ��ȥ���ھ�ֵ����
	for (int i = 0; i < sampleNum; i++) 
	{
		cvGetCol(inputs, &sub, i);
		int j;
		for (j = 0; j < classNum; j++)
			if (id2idx[j] == trainIds[i]) break;
		cvSub(&sub, muPerClass[j], &sub);
	}

	// ����ɢ������
	cvMulTransposed(inputs, Sw, 0);

	// ���ɢ������
	cvSetZero(Sb);
	CvMat *Sb1 = cvCreateMat(oriDim, oriDim, CV_COEF_FC1);
	for (int i = 0; i < classNum; i++)
	{
		cvMulTransposed(muPerClass[i], Sb1, 0);
		cvAddWeighted(Sb, 1, Sb1, smpNumEachClass[i], 0, Sb);
	}

	//DispCvArr(Sw, "Sw");
	//DispCvArr(Sb, "Sb");
	cvReleaseMat(&Sb1);
	delete []id2idx;
	delete []smpNumEachClass;
	for (int i = 0; i < classNum; i++)
		cvReleaseMat(& muPerClass[i]);
	delete []muPerClass;
}

void CSubspace::CalcLdaSpace( CvMat *Sw64, CvMat *Sb64, int postLdaDim )
{
	int oriDim = Sw64->rows;
	CvMat	*eigenVecs = cvCreateMat(oriDim, oriDim, CV_64FC1);
	double	*ar = new double[oriDim], *ai = new double[oriDim], 
		*be = new double[oriDim];
	double	*A, *B, *E;

	// �������������
	A = Sb64->data.db;
	B = Sw64->data.db;
	E = eigenVecs->data.db;
	GeneralEig(A, B, oriDim, E, ar, ai, be); 
	//DispCvArr(eigenVecs,"vecs", false, "%1.10f\t");


	// ѡȡ�ӿռ�
	CvMat arm = cvMat(1, oriDim, CV_64FC1, ar);
	CvMat bem = cvMat(1, oriDim, CV_64FC1, be);
	CvMat *idxm = cvCreateMat(1, oriDim, CV_32SC1);
	cvAbs(&arm, &arm); // ����ж��Ƿ�������Ӧ��Ӧ��ȡ����ֵ��
	cvAbs(&bem, &bem);
	cvAddS(&bem, cvScalar(.001), &bem); // be is always near zero
	cvDiv(&arm, &bem, &arm);
	//DispCvArr(&arm, "arm");
	cvSort(&arm, &arm, idxm, CV_SORT_DESCENDING | CV_SORT_EVERY_ROW);

	if (postLdaDim == 0)
	{
		postLdaDim = classNum-1;
		if (postLdaDim < 1) postLdaDim = 1;
		//if (postLdaDim < 3) postLdaDim = min(sampleNum, 3);
	}
	this->postLdaDim = postLdaDim;

	//cvReleaseMat(&W_fldT);
	W_fldT = cvCreateMat(postLdaDim, oriDim, CV_64FC1);
	CvMat subSrc, subDst;
	for (int i = 0; i < postLdaDim; i++)
	{
		cvGetRow(eigenVecs, &subSrc, int(cvGetReal1D(idxm, i)));
		cvGetRow(W_fldT, &subDst, i);
		cvCopy(&subSrc, &subDst); // Ӧ�ù�һ��ô��
	}

	cvReleaseMat(&eigenVecs);
	delete []ar;
	delete []ai;
	delete []be;
	cvReleaseMat(&idxm);
}

void CSubspace::FisherLda( CvMat *inputs, int *trainIds, int postLdaDim /*= 0*/ )
{
	int	oriDim = inputs->rows;
	CvMat *Sw = cvCreateMat(oriDim, oriDim, CV_COEF_FC1),
		*Sb = cvCreateMat(oriDim, oriDim, CV_COEF_FC1); 

	CalcSwSb(inputs, trainIds, Sw, Sb);

	if (CV_COEF_FC1 == CV_64FC1)
		CalcLdaSpace(Sw, Sb, postLdaDim);
	else
	{ // ����趨�ľ�����double��
		CvMat	*Sw64 = cvCreateMat(oriDim, oriDim, CV_64FC1),
			*Sb64 = cvCreateMat(oriDim, oriDim, CV_64FC1);
		cvConvert(Sw, Sw64);
		cvConvert(Sb, Sb64);
		CalcLdaSpace(Sw64, Sb64, postLdaDim);

		CvMat *W_fldT64 = cvCloneMat(W_fldT);
		cvReleaseMat(&W_fldT);
		W_fldT = cvCreateMat(postLdaDim, oriDim, CV_COEF_FC1);
		cvConvert(W_fldT64, W_fldT);

		cvReleaseMat(&Sw64);
		cvReleaseMat(&Sb64);
		cvReleaseMat(&W_fldT64);
	}

	cvReleaseMat(&Sw);
	cvReleaseMat(&Sb);
}

void CSubspace::Train( CvMat *inputs, int *trainIds )
{
	Release();
	inputDim = inputs->rows;
	trainNum = inputs->cols;
	Pca(inputs, .97);

	CvMat *pcaFace = cvCreateMat(postPcaDim, trainNum, CV_COEF_FC1); // ��ά������Ϊ������
	cvProjectPCA(inputs, mu_total, W_pcaT, pcaFace);

	// ͬcvProjectPCA
	//CvMat	sub;
	//CvRect	rc = cvRect(0,0, 1, inputDim);
	//for (int i = 0; i < trainNum; i++)
	//{
	//	rc.x = i;
	//	cvGetSubRect(inputs, &sub, rc);
	//	cvSub(&sub, mu_total, &sub);
	//}	
	//cvmMul(W_pcaT, inputs, pcaFace);

	FisherLda(pcaFace, trainIds);

	//cvReleaseMat(&W_pcafldT);
	W_pcafldT = cvCreateMat(postLdaDim, inputDim, CV_COEF_FC1);
	cvmMul(W_fldT, W_pcaT, W_pcafldT);

	//DispCvArr(W_pcaT, "W_p");
	//DispCvArr(W_fldT, "W_f");
	//DispCvArr(W_pcafldT, "W_opt");
	cvReleaseMat(&pcaFace);
	m_bTrained = true;
}

void CSubspace::Project( CvMat *inputs, CvMat *results )
{
	cvProjectPCA(inputs, mu_total, W_pcafldT, results);
}

int CSubspace::GetSubspaceDim()
{
	return postLdaDim;
}

double CSubspace::CalcVectorDist( CvMat *target, CvMat *query )
{
	// use normalized cosine metric
	// other alternative metrics: L1, L2, Mahalanobis ...
	if (target->rows > 1)
		return -(cvDotProduct(target, query) / cvNorm(target) / cvNorm(query));
	else
		return cvNorm(target, query, CV_L2);
}

bool CSubspace::WriteDataToFile( ofstream &os )
{
	WriteIntText(os, W_pcafldT->step / W_pcafldT->cols, "dataBytes:");
	WriteIntText(os, inputDim, "inputDim:");
	WriteIntText(os, postLdaDim, "postLdaDim:");
	WriteCvMatBin(os, mu_total, "mu_total:");
	WriteCvMatBin(os, W_pcafldT, "W_pcafldT:");
	return true;
}

bool CSubspace::ReadDataFromFile( ifstream &is )
{
	cvReleaseMat(&mu_total);
	cvReleaseMat(&W_pcafldT);

	int		dataBytes;
	ReadIntText(is, dataBytes);
	if ((dataBytes == 4 && CV_COEF_FC1 == CV_64FC1) ||
		(dataBytes == 8 && CV_COEF_FC1 == CV_32FC1))
	{
		::MessageBox1("float/double not match!");
		return false;
	}

	ReadIntText(is, inputDim);
	ReadIntText(is, postLdaDim);
	mu_total = cvCreateMat(inputDim, 1, CV_COEF_FC1);
	ReadCvMatBin(is, mu_total);
	W_pcafldT = cvCreateMat(postLdaDim, inputDim, CV_COEF_FC1);
	ReadCvMatBin(is, W_pcafldT);
	
	m_bTrained = true;
	return true;
}

void CSubspace::Release()
{
	m_bTrained = false;
	cvReleaseMat(&mu_total);
	cvReleaseMat(&W_pcaT);
	cvReleaseMat(&W_fldT);
	cvReleaseMat(&W_pcafldT);
}
