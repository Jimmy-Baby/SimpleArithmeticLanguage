﻿// Precompiled headers
#include "Pch.hpp"

#include "EntryPoint.hpp"

#include "Interpreter/Interpreter.hpp"
#include "IO/Console.hpp"
#include "Lexer/Lexer.hpp"
#include "Parser/Parser.hpp"
#include "Utility/ErrorManager.h"

int main(const int argc, char* argv[])
{
	return MyLang::Main(argc, argv);
}

namespace MyLang
{
	int Main(const int argc, char* argv[])
	{
		UNREFERENCED_PARAMETER(argc);
		UNREFERENCED_PARAMETER(argv);

		Print("MyLang Arithmetic Interpreter (v1.0.1)\n");
		Print("Type 'quit' or 'exit' to exit\n");
		Run();

		return 0;
	}

	void Run()
	{
		std::string currentInput;

		while (true)
		{
			// Get input
			Print(">>> ");
			std::getline(std::cin, currentInput);

			// First check for quit/exit
			{
				// To lowercase
				std::ranges::transform(currentInput.begin(), currentInput.end(), currentInput.begin(),
				                       [](const u8 c)
				                       {
					                       return std::tolower(c);
				                       }
				);

				if (currentInput == "quit" || currentInput == "exit")
				{
					break;
				}
			}

			// Run lexer
			std::vector<CToken> tokens = CLexer::GetInstance()->MakeTokens(&currentInput);

			if (!g_ErrorMgr->CheckLastError(true, true))
			{
				continue;
			}

			// Run parser
			CNodeBase* syntaxTreeRoot = CParser::GetInstance()->GetExpressionResult(&tokens);

			if (!g_ErrorMgr->CheckLastError(true, true))
			{
				continue;
			}

			// Run interpreter
			CNumber result = Interpreter::VisitRoot(syntaxTreeRoot);

			if (!g_ErrorMgr->CheckLastError(true, true))
			{
				continue;
			}

			// Print result
			Print("Result: %d", result.GetValue());

			// Clean up for next iteration
			g_ErrorMgr->Clear();
			Print("\n\n");
		}
	}
}
