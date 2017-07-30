#pragma once
#include <exception>
namespace SGA {
	class ResourceFileMissing : public std::exception {
	public:
		ResourceFileMissing(const char* file) {
			sprintf_s(_msg, 256, "%s ������ ã�� �� �����ϴ�.", file);
		}
		ResourceFileMissing(const wchar_t* file) {
			char buffer[200];
			size_t nConverted = 0;
			wcstombs_s(&nConverted, buffer, 200, file, 200);
			sprintf_s(_msg, 256, "%s ������ ã�� �� �����ϴ�.", buffer);
		}
		char const* what() const override {
			return _msg;
		}
	private:
		char _msg[256];
	};
}