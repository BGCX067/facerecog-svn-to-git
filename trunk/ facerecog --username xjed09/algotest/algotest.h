#include "../FaceMngr/FaceMngr.h"
#include <time.h>

#define COMPILE_DATASET_ORL

extern CFaceMngr fm;
extern ofstream os;
extern CString msg;

extern CString	rootPath; // �������·��
extern DWORD	trainFlag, enrFlag, testFlag; // ѡ��
extern CvSize	faceSz; // �����ߴ�

void run_orl();
void orl_list();

void run_feret();
void feret_list();

void out(LPCTSTR str);