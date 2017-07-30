#pragma once
#include <string>
#include <locale>
#include <codecvt>
#include <tchar.h>
namespace SGA {
	typedef std::basic_string<TCHAR> tstring;

	template <typename ValT>
	tstring to_tstring(const ValT& val) {
#ifdef UNICODE
		return std::to_wstring(val);
#else
		return std::to_string(val);
#endif
	}

	inline tstring convertToTString(const std::string& str) {
#ifdef UNICODE
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(str);
#else
		return str;
#endif
	}

	inline tstring convertToTString(const std::wstring& wstr) {
#ifdef UNICODE
		return wstr;
#else
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(wstr);
#endif
	}
	
	inline std::string convertToString(const tstring& tstr) {
#ifdef UNICODE
		char buffer[256] = "";
		size_t lenConverted = 0;
		wcstombs_s(&lenConverted, buffer, 256, tstr.c_str(), tstr.size());
		return std::string(buffer);
		//TODO : wstring_convert제대로 동작하지 않는다.
		//std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		//return converter.to_bytes(tstr);
#else
		return tstr;
#endif
	}
}