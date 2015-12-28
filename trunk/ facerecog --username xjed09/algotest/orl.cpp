// ORL��Ĳ��Ժ���
#include "algotest.h"

#ifdef COMPILE_DATASET_ORL

vector<SFInfo>	trainList;
vector<SFInfo>	testList;
vector<SMatch>	resList;


void run_orl()
{
	faceSz = cvSize(92, 112)*.5;
	rootPath = "D:\\facePics\\orl_png\\";
	trainFlag = FM_DO_NOT_NORM | FM_SHOW_DETAIL | FM_TRAIN_SAVE2MODEL; // ORL�ⲻ�ù�һ������ѵ���õ���������ģ��
	testFlag = FM_DO_NOT_NORM | FM_SHOW_DETAIL;

	if(! fm.Init(faceSz)) exit(1);

	int runTimes = 1; // �ظ����Դ���
	double rate = 0;
	for (int i = 0; i < runTimes; i++)
	{
		orl_list();
		tic();
		fm.Train(rootPath, trainList, trainFlag);
		double r = fm.BatchPicRecog(rootPath, testList, resList, testFlag);
		double t = toc();
		msg.Format("rate = %f, time = %fs\n", r, t);
		out(msg);
		rate += r/runTimes;
	}

	msg.Format("mean rate: %f\n", rate);
	out(msg);
}

// �����ļ���Ϣ�б�
void orl_list()
{
	trainList.clear();
	testList.clear();
	resList.clear();

	int classNum = 40, classPopu = 10, trainNum = 2; // �������ÿ��������������������ѵ����������
	int v1[] = {1,5,9,2,3,4,6,7,8,10};

	CString picPath;
	//srand((UINT)time(0)); // �����д˾䣬��һ�����ÿ�β�����ͬ��˳��
	//random_shuffle( v1, v1+10 ); // �������˳��

	for (int i = 1; i <= classNum; i++)
	{
		for (int j = 0; j < trainNum; j++)
		{
			picPath.Format("s%d\\%d.png", i, v1[j]);
			trainList.push_back(SFInfo(i, picPath));
		}
		for (int j = trainNum; j < classPopu; j++)
		{
			picPath.Format("s%d\\%d.png", i, v1[j]);
			testList.push_back(SFInfo(i, picPath));
		}
	}
}

#endif