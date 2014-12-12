#ifndef SCANNER_H_
#define SCANNER_H_

#include <istream>
#include <ostream>

// Macro `#if defined(yyFlexLexer) || ! defined(yyFlexLexerOnce)` in
// `FlexLexer.h` would cause the the definition of class yyFlexLexer being
// included more than once, since the flex generated file would always define
// `yyFlexLexer`. Following macro could this behavior.
#ifndef __FLEX_LEXER_H
#include "FlexLexer.h"
#endif

// `generated_parser.h` use macro YY_NULLPTR, which is not defined.
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif
// then, include the header.
#include "generated_parser.h"

class FlexGeneratedScanner : public yyFlexLexer {
 public:
	FlexGeneratedScanner(std::istream *arg_yyin = 0,
                       std::ostream *arg_yyout = 0)
      : yyFlexLexer(arg_yyin, arg_yyout) { /* empty */ }
  // Implemented the pure virtual function.
  yy::BisonGeneratedParser::symbol_type lex();
};

#endif // SCANNER_H_
