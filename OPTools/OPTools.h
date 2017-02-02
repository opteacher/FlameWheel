#pragma once

#ifdef OP_MN_EXPORTS
#define OP_MN_API __declspec(dllexport)
#else
#define OP_MN_API __declspec(dllimport)
#endif

//template class should use this macro
#ifndef TEMPLATE_EXPORTS
#define TEMPLATE_API
#else //EXPORT
#define TEMPLATE_API __declspec(dllexport)
#endif