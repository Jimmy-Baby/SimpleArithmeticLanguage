#pragma once

namespace MyLang
{
	template <class ...Args>
	void Print(const char* format, Args ...args)
	{
		printf_s(format, args...);
	}
}
