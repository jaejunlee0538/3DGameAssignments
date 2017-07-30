#pragma once
#include <tchar.h>
#define SAFE_DELETE(p)			{if(p) {delete (p); (p)=NULL;}}
#define SAFE_DELETE_ARRAY(p)	{if(p) {delete[] (p); (p)=NULL;}}

namespace SGA {
	template <typename T>
	T clip(T val, T minVal, T maxVal) {
		if (val < minVal)
			return minVal;
		if (val > maxVal)
			return maxVal;
		return val;
	}

	void LogDebugMessage(const TCHAR* fmt, ...);
}