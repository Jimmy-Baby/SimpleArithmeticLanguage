// Precompiled headers
#include "Pch.hpp"

#include "Parser.hpp"

/*
 * expr: term ((PLUS | MINUS) term)*
 * term: factor ((MUL | DIV) factor)*
 * factor: INT | FLOAT
 */
