// Precompiled headers
#include "Pch.hpp"

#include "EntryPoint.hpp"
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
			CError* err = nullptr;

			// Get input
			Print(">>> ");
			std::getline(std::cin, currentInput);

			// Create new lexer
			const auto lexer = std::make_unique<CLexer>(currentInput, "[std::cin]");

			// Run lexer
			std::vector<CToken> tokens = lexer->MakeTokens();

			err = g_ErrorMgr->GetLastError();
			if (err != nullptr)
			{
				err->Print();
				g_ErrorMgr->Clear();
				continue;
			}

			// Create new parser
			const auto parser = std::make_unique<CParser>(tokens);

			// Run parser
			CNodeBase* syntaxTreeRootNode = parser->Run();

			err = g_ErrorMgr->GetLastError();
			if (err != nullptr)
			{
				err->Print();
				g_ErrorMgr->Clear();
				continue;
			}

			// Print syntax tree
			syntaxTreeRootNode->Print();

			// Clean up for next iteration
			g_ErrorMgr->Clear();
			Print("\n\n");
		}
	}
}
