// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
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
