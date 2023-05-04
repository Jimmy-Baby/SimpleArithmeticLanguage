// Precompiled headers
#include "Pch.hpp"

#include "EntryPoint.hpp"
#include "IO/Console.hpp"
#include "Lexer/Lexer.hpp"
#include "Parser/Parser.hpp"


int main(const int argc, char* argv[])
{
	return MyLang::Main(argc, argv);
}


namespace MyLang
{
	int Main(int argc, char* argv[])
	{
		Print(">>> MyLang Arithmetic Interpreter <<<\n");
		Run();

		return 0;
	}


	void Run()
	{
		std::string currentInput;

		while (true)
		{
			// Get input
			Print("> ");
			std::getline(std::cin, currentInput);

			// Create new lexer
			const auto lexer = std::make_unique<CLexer>(currentInput, "[std::cin]");

			// Run lexer
			std::vector<CToken> tokens;
			CError error = lexer->MakeTokens(tokens);

			// Check for errors
			if (error())
			{
				// Print error to console
				error.Print();

				continue;
			}

			// Create new parser
			const auto parser = std::make_unique<CParser>(tokens);

			// Run parser
			CBinaryOpNode* syntaxTreeRootNode = parser->Run();

			syntaxTreeRootNode->Print();
			Print("\n");
		}
	}
}
