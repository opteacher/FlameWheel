// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>
#include <iostream>

#ifndef FWMAIN_EXPORTS
#define FWMAIN_EXPORTS
#endif

#ifndef ALGORITHM_EXPORTS
#define ALGORITHM_EXPORTS
#endif

#pragma comment( lib, "../Debug/OPMath.lib")
#pragma comment( lib, "../Debug/OPTools.lib")

//D3D library
#pragma comment( lib, "dxerr.lib")
#pragma comment( lib, "dxguid.lib")
#pragma comment( lib, "d3d9.lib")
#pragma comment( lib, "d3dx9.lib")
