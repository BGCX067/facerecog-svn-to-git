/*
	������#pragma once����Ϊ���ͷ�ļ������ٽ������α��룬
	����FaceAlign_ASM.h��ͷ�ļ�����ʱ�����ڶ�����ALIGN_HEADER��
	�������#else֮ǰ�Ĳ��֣���FaceMngr.h����ʱ��������Ǻ�һ���֡�
	�������#pragma once�������ֵݹ������
	�����ģ�飺Feature��Mngr�ȵĶ���ṹ�����ơ�
*/

#ifdef ALIGN_HEADER // ���������㷨�ֱ��ͷ�ļ�����ʱ

#define COMPILE_ALIGN_ASM // �����ĸ��㷨

 // ����Align�㷨����Ҫ��һЩ����

#define INPUT_PATH	""

#else // ����FaceMngr��Ŀ����ʱ

// �����㷨�ֱ��ͷ�ļ�
#include "FaceAlign_ASM.h"
#include "FaceAlign_coord.h"
#include "FaceAlign_Haar.h"

#endif