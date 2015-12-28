#ifdef MNGR_HEADER

#define COMPILE_MNGR_COMPLETE

#define CV_MODEL_FC1 CV_64FC1 // �洢���ݾ���

// flags used in Pic2NormFace, Enroll, Train, PicRecognize...
#define FM_DO_NOT_NORM		0 // �����м��κ͹��չ�һ��
#define FM_DO_FACE_ALIGN	1 // ���м��ι�һ��
#define FM_DO_LIGHT_PREP	2 // ���й��չ�һ��
#define FM_ALIGN_USE_BUF	4 // ���ι�һ��ʱʹ��ƽ��
#define FM_DO_NORM		(FM_DO_FACE_ALIGN | FM_DO_LIGHT_PREP)
// ����flag���ڹ�һ��ʱ
#define FM_SAVE_NORM_FACE	8 // ע���ѵ��ʱ�����һ���������ļ�
#define FM_TRAIN_SAVE2MODEL	16 // ѵ��ʱ��ѵ��ͼ�񱣴浽ģ���
#define FM_SAVE_REL_PATH	32 // ѵ��ʱ����������·��
#define FM_SHOW_DETAIL		64 // coutһЩ��Ϣ



// һЩ��������µ�class ID
#define FM_UNKNOWN_CLASS_ID	-1 
#define FM_RECOG_NOT_DONE	-3

#else

#include "FaceMngr_complete.h"
#include "FaceMngr_fast.h"
//#include "FaceMngr_fast_fast.h"

#endif