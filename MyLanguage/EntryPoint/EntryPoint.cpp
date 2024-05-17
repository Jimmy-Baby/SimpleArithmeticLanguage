// Precompiled headers
#include "Pch.h"

#include "EntryPoint.h"

#include "Interpreter/Interpreter.h"
#include "IO/Console.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Utility/ErrorManager.h"

int main(const int Argc, char* Argv[])
{
	return ArithmeticInterpreter::Main(Argc, Argv);
}

namespace ArithmeticInterpreter
{
	int Main(const int Argc, char* Argv[])
	{
		UNREFERENCED_PARAMETER(Argc);
		UNREFERENCED_PARAMETER(Argv);

		Print("Type 'quit' to exit the interpreter\n");

		std::string CurrentInput;

		while (true)
		{
			// Get input
			Print(">>> ");
			std::getline(std::cin, CurrentInput);

			if (CurrentInput == "quit")
			{
				break;
			}

			// Run lexer
			std::vector<Token> Tokens = Lexer::GetTokens(CurrentInput);

			if (!GErrorMgr->CheckLastError(true, true))
			{
				continue;
			}
	
			// Run parser
			NodeBase* SyntaxTreeRoot = Parser::GetExpressionResult(Tokens);

			if (!GErrorMgr->CheckLastError(true, true))
			{
				continue;
			}

			// Run interpreter
			const Number Result = Interpreter::Visit(SyntaxTreeRoot);

			if (!GErrorMgr->CheckLastError(true, true))
			{
				continue;
			}

			// Print result
			Print("Result: %d", Result.Value);

			// Clean up for next iteration
			GErrorMgr->Clear();
			Print("\n\n");
		}

		return 0;
	}
}
