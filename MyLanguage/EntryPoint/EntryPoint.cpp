// Precompiled headers
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
	int Main(int argc, char* argv[])
	{
		Print("(MyLang Arithmetic Interpreter)\n");
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


			/*
			 * EXECUTION
			 */

			// Create new lexer
			const auto lexer = std::make_unique<CLexer>(currentInput, "[std::cin]");

			// Run lexer
			std::vector<CToken> tokens = lexer->MakeTokens();

			if (!g_ErrorMgr->CheckLastError(true, true))
			{
				continue;
			}

			// Create new parser
			const auto parser = std::make_unique<CParser>(tokens);

			// Run parser
			CNodeBase* syntaxTreeRootNode = parser->Run();

			if (!g_ErrorMgr->CheckLastError(true, true))
			{
				continue;
			}

			// Create new interpreter
			const auto interpreter = std::make_unique<CInterpreter>();

			// Run interpreter
			CNumber result = interpreter->Visit(syntaxTreeRootNode);

			if (!g_ErrorMgr->CheckLastError(true, true))
			{
				continue;
			}

			// Print result
			Print("Result: %d", result.GetValue());


			/*
			 * CLEAN-UP
			 */

			// Clean up for next iteration
			g_ErrorMgr->Clear();
			Print("\n\n");
		}
	}
}
