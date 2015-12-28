// FERET����Ժ������õ���һЩ�ļ��б�txt���б����ԭʼ�ĻҶ�FERET���list��������
#include "algotest.h"

#ifdef COMPILE_DATASET_FERET

vector<SFInfo>	trainList; // ѵ���ļ���Ϣ�б�
vector<SFInfo>	faList, fbList, fcList, dupIList; // ģ��Ͳ����ļ���Ϣ�б�
vector<SMatch>	fbRes, fcRes, dupIRes; // ƥ����
int				trainNum, faNum, fbNum, fcNum, dupINum; // ���ӿ�ͼƬ��


void run_feret()
{
	faceSz = cvSize(65, 80);
	rootPath = "..\\..\\feret_tif\\gray feret\\data\\";
	trainFlag = FM_DO_NORM | FM_SHOW_DETAIL;
	enrFlag = testFlag = trainFlag;

	if(! fm.Init(faceSz)) exit(1);

	feret_list();
	tic(); // ��¼��ʱ
	fm.Train(rootPath, trainList, trainFlag); // ѵ��
	fm.BatchPicEnroll(rootPath, faList, enrFlag); // ע��ģ��

	// ����FB��fc��duplicate I�ӿ�
	double rb = fm.BatchPicRecog(rootPath, fbList, fbRes, testFlag),
		rc = fm.BatchPicRecog(rootPath, fcList, fcRes, testFlag),
		rdI = fm.BatchPicRecog(rootPath, dupIList, dupIRes, testFlag);

	double t = toc();
	msg.Format("rate = %f, %f, %f, time = %fs\n", rb, rc, rdI, t); // ���
	out(msg);
}

// �����ļ���Ϣ�б�
void feret_list()
{
	//CString namesPath = rootPath + "partitions\\by_previously_reported\\feret\\",
		//coordPath = rootPath + "name_value\\";
	//ifstream isNames, isCoord;
	//char name[20];

	/*isNames.open(namesPath + "train_fafb.txt");
	trainNum = 996;
	for (int i = 0; i < trainNum; i++)
	{
		isNames.getline(name, 20);
		isCoord.open(coordPath + name + ".gnd");

	}*/
	ifstream is;
	char name[30];
	int id, leyex, leyey, reyex, reyey;

	is.open("feret_train_list.txt"); // ����Ԥ��׼�����ļ�
	trainNum = 996;
	for (int i = 0; i < trainNum; i++)
	{
		is>>name>>id>>leyex>>leyey>>reyex>>reyey;
		strcat_s(name, ".tif");
		trainList.push_back(SFInfo(id, name, 
			cvPoint2D32f(leyex,leyey), cvPoint2D32f(reyex,reyey))); // �����б�
	}
	is.close();

	is.open("feret_fa_list.txt");
	faNum = 1196;
	for (int i = 0; i < faNum; i++)
	{
		is>>name>>id>>leyex>>leyey>>reyex>>reyey;
		strcat_s(name, ".tif");
		faList.push_back(SFInfo(id, name, 
			cvPoint2D32f(leyex,leyey), cvPoint2D32f(reyex,reyey)));
	}
	is.close();

	is.open("feret_fb_list.txt");
	fbNum = 1195;
	for (int i = 0; i < fbNum; i++)
	{
		is>>name>>id>>leyex>>leyey>>reyex>>reyey;
		strcat_s(name, ".tif");
		fbList.push_back(SFInfo(id, name, 
			cvPoint2D32f(leyex,leyey), cvPoint2D32f(reyex,reyey)));
	}
	is.close();

	is.open("feret_fc_list.txt");
	fcNum = 194;
	for (int i = 0; i < fcNum; i++)
	{
		is>>name>>id>>leyex>>leyey>>reyex>>reyey;
		strcat_s(name, ".tif");
		fcList.push_back(SFInfo(id, name, 
			cvPoint2D32f(leyex,leyey), cvPoint2D32f(reyex,reyey)));
	}
	is.close();

	is.open("feret_dupI_list.txt");
	dupINum = 722;
	for (int i = 0; i < dupINum; i++)
	{
		is>>name>>id>>leyex>>leyey>>reyex>>reyey;
		strcat_s(name, ".tif");
		dupIList.push_back(SFInfo(id, name, 
			cvPoint2D32f(leyex,leyey), cvPoint2D32f(reyex,reyey))); // �����б�
	}
	is.close();
}

#endif