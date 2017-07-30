#pragma once
#include <cctype>
namespace SGA {
	namespace Utils {
		template <typename CharT>
		inline bool isNumber(const CharT* str) {
			assert(str != 0);
			while (*str != 0) {
				if (std::isdigit(*str) == false) 
					return false; 
				str++;
			}
			return true;
		}
	}
}
