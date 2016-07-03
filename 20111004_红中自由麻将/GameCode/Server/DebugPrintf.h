#pragma once
#include   <stdarg.h>


/// ��ӡ������Ϣ
extern void DebugPrintf_debug(const char *p, ...);
/// ��ӡ������Ϣ
extern void ErrorPrintf_debug(const char *p, ...);

#ifndef _DEBUG
# define DebugPrintf __noop
#else
# define DebugPrintf DebugPrintf_debug
#endif
