#pragma once

namespace ArithmeticInterpreter
{
	template <class... ArgTys>
	void Print(const char* Format, ArgTys... Args)
	{
		printf_s(Format, Args...);
	}

	template <class... ArgTys>
	void Print(const std::string& Format, ArgTys... Args)
	{
		printf_s(Format.c_str(), Args...);
	}
}
