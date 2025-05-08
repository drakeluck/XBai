#ifndef XB_UTILS_PLATFORMUTILS_H
#define XB_UTILS_PLATFORMUTILS_H

#include <string>

namespace XBai
{
	class FileDialogs
	{
	public:
		//失败则返回空字符串（empty strings）
		static std::string OpenFile(const char* filter);
		//失败则返回空字符串（empty strings）
		static std::string SaveFile(const char* filter);
	};
}

#endif

