#pragma once

#include "cv.h"
#include "highgui.h"

#pragma comment(lib,"../opencv/cv200d.lib") // ��Release���OpenCV��Ļ���asmlibrary�������ʧ�ܡ�
#pragma comment(lib,"../opencv/cxcore200d.lib")
#pragma comment(lib,"../opencv/highgui200d.lib")

#ifdef DLLSRC
#define DLLEXP __declspec(dllexport)
#else
#define DLLEXP __declspec(dllimport)
#endif

#include <vector>
#include <fstream>
using namespace std;

#include <atlstr.h>
#ifdef _DEBUG
#include <atltrace.h>
#define TRACE ATLTRACE
#endif