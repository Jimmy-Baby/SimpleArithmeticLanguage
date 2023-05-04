#pragma once

namespace MyLang
{
	template <class ...Args>
	void Print(const char* format, Args ...args)
	{
		printf_s(format, args...);
	}


	template <class ...Args>
	void Print(const std::string& format, Args ...args)
	{
		printf_s(format.c_str(), args...);
	}
}
