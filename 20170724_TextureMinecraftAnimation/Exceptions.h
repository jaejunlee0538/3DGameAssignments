#pragma once
#include <exception>
namespace SGA {
	class ResourceFileMissing : public std::exception {
	public:
		ResourceFileMissing(const char* file) {
			sprintf_s(_msg, 256, "%s ������ ã�� �� �����ϴ�.", file);
		}

		char const* what() const override {
			return _msg;
		}
	private:
		char _msg[256];
	};
}