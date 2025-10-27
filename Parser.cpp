#include "Parser.h"
#include <iostream>
#include <cstdlib>

using namespace std;

// Constructor
Parser::Parser(vector<Token> tokenList) {
    tokens = tokenList;
    position = 0;
    hasError = false;
    
    if (tokens.size() > 0) {
        current = tokens[0];
    }
}

// Move to next token
void Parser::nextToken() {
    if (position < tokens.size() - 1) {
        position++;
        current = tokens[position];
    }
}

// Get token name as string
string Parser::tokenName(TokenType type) {
    if (type == KEYWORD_IF) return "if";
    if (type == KEYWORD_ELSE) return "else";
    if (type == KEYWORD_INT) return "int";
    if (type == KEYWORD_FLOAT) return "float";
    if (type == KEYWORD_WHILE) return "while";
    if (type == KEYWORD_PROGRAM) return "Program";
    if (type == ID) return "identifier";
    if (type == NUM) return "number";
    if (type == OP_ASSIGN) return "=";
    if (type == OP_EQ) return "==";
    if (type == OP_NE) return "!=";
    if (type == OP_LT) return "<";
    if (type == OP_LE) return "<=";
    if (type == OP_GT) return ">";
    if (type == OP_GE) return ">=";
    if (type == OP_PLUS) return "+";
    if (type == OP_MINUS) return "-";
    if (type == OP_MULT) return "*";
    if (type == OP_DIV) return "/";
    if (type == SYM_LPAREN) return "(";
    if (type == SYM_RPAREN) return ")";
    if (type == SYM_LBRACE) return "{";
    if (type == SYM_RBRACE) return "}";
    if (type == SYM_LBRACKET) return "[";
    if (type == SYM_RBRACKET) return "]";
    if (type == SYM_SEMICOLON) return ";";
    if (type == SYM_COMMA) return ",";
    if (type == END_OF_FILE) return "end of file";
    return "unknown";
}

// Match current token with expected
void Parser::eat(TokenType expected) {
    if (current.type == expected) {
        nextToken();
    } else {
        error(tokenName(expected));
    }
}

// Print error and exit
void Parser::error(string expected) {
    cout << "\nSYNTAX ERROR at line " << current.line 
         << ", column " << current.column << endl;
    cout << "Unexpected token: '" << current.lexeme << "'" << endl;
    cout << "Expected: " << expected << endl;
    hasError = true;
    exit(1);
}

// Main parse function
void Parser::parse() {
    program();
    
    if (current.type != END_OF_FILE) {
        error("end of file");
    }
    
    if (!hasError) {
        cout << "\nParsing completed successfully!" << endl;
    }
}

// program → Program ID { declaration-list statement-list }
void Parser::program() {
    eat(KEYWORD_PROGRAM);
    eat(ID);
    eat(SYM_LBRACE);
    declarationList();
    statementList();
    eat(SYM_RBRACE);
}

// declaration-list -> declaration declaration-list^
void Parser::declarationList() {
    declaration();
    declarationListPrime();
}

// declaration-list^ -> declaration declaration-list^ | ε
void Parser::declarationListPrime() {
    // If next token is int or float, we have another declaration
    if (current.type == KEYWORD_INT || current.type == KEYWORD_FLOAT) {
        declaration();
        declarationListPrime();
    }
    // Otherwise done with declarations
}

// declaration → var-declaration
void Parser::declaration() {
    varDeclaration();
}

// var-declaration → type-specifier ID ; | type-specifier ID [ NUM ] ;
void Parser::varDeclaration() {
    typeSpecifier();
    eat(ID);
    
    // Check if array
    if (current.type == SYM_LBRACKET) {
        eat(SYM_LBRACKET);
        eat(NUM);
        eat(SYM_RBRACKET);
    }
    
    eat(SYM_SEMICOLON);
}

// type-specifier → int | float
void Parser::typeSpecifier() {
    if (current.type == KEYWORD_INT) {
        eat(KEYWORD_INT);
    } else if (current.type == KEYWORD_FLOAT) {
        eat(KEYWORD_FLOAT);
    } else {
        error("int or float");
    }
}

// compound-stmt → { statement-list }
void Parser::compoundStmt() {
    eat(SYM_LBRACE);
    statementList();
    eat(SYM_RBRACE);
}

// statement-list -> ε | statement statement-list^
void Parser::statementList() {
    // Check if there is a statement
    if (current.type == ID || current.type == SYM_LBRACE ||
        current.type == KEYWORD_IF || current.type == KEYWORD_WHILE) {
        statement();
        statementListPrime();
    }
    // Otherwise empty list
}

// statement-list^ -> statement statement-list^ | ε
void Parser::statementListPrime() {
    // Keep parsing statements
    if (current.type == ID || current.type == SYM_LBRACE ||
        current.type == KEYWORD_IF || current.type == KEYWORD_WHILE) {
        statement();
        statementListPrime();
    }
    // Otherwise done
}

// statement → assignment-stmt | compound-stmt | selection-stmt | iteration-stmt
void Parser::statement() {
    if (current.type == ID) {
        assignmentStmt();
    } else if (current.type == SYM_LBRACE) {
        compoundStmt();
    } else if (current.type == KEYWORD_IF) {
        selectionStmt();
    } else if (current.type == KEYWORD_WHILE) {
        iterationStmt();
    } else {
        error("statement");
    }
}

// selection-stmt -> if (expression) statement selection-stmt^
void Parser::selectionStmt() {
    eat(KEYWORD_IF);
    eat(SYM_LPAREN);
    expression();
    eat(SYM_RPAREN);
    statement();
    selectionStmtPrime();
}

// selection-stmt^ -> ε | else statement
void Parser::selectionStmtPrime() {
    // Check for else
    if (current.type == KEYWORD_ELSE) {
        eat(KEYWORD_ELSE);
        statement();
    }
    // Otherwise no else
}

// iteration-stmt → while ( expression ) statement
void Parser::iterationStmt() {
    eat(KEYWORD_WHILE);
    eat(SYM_LPAREN);
    expression();
    eat(SYM_RPAREN);
    statement();
}

// assignment-stmt → var = expression
void Parser::assignmentStmt() {
    var();
    eat(OP_ASSIGN);
    expression();
}

// var → ID | ID [ expression ]
void Parser::var() {
    eat(ID);
    
    // Check for array access
    if (current.type == SYM_LBRACKET) {
        eat(SYM_LBRACKET);
        expression();
        eat(SYM_RBRACKET);
    }
}

// expression -> additive-expression expression^
void Parser::expression() {
    additiveExpression();
    expressionPrime();
}

// expression^ -> relop additive-expression expression^ | ε
void Parser::expressionPrime() {
    // Check for relational operator
    if (current.type == OP_LT || current.type == OP_LE ||
        current.type == OP_GT || current.type == OP_GE ||
        current.type == OP_EQ || current.type == OP_NE) {
        relop();
        additiveExpression();
        expressionPrime();
    }
    // Otherwise done
}

// relop → < | ≤ | > | ≥ | == | !=
void Parser::relop() {
    if (current.type == OP_LT) {
        eat(OP_LT);
    } else if (current.type == OP_LE) {
        eat(OP_LE);
    } else if (current.type == OP_GT) {
        eat(OP_GT);
    } else if (current.type == OP_GE) {
        eat(OP_GE);
    } else if (current.type == OP_EQ) {
        eat(OP_EQ);
    } else if (current.type == OP_NE) {
        eat(OP_NE);
    } else {
        error("relational operator");
    }
}

// additive-expression -> term additive-expression^
void Parser::additiveExpression() {
    term();
    additiveExpressionPrime();
}

// additive-expression^ -> addop term additive-expression^ | ε
void Parser::additiveExpressionPrime() {
    // Check for + or -
    if (current.type == OP_PLUS || current.type == OP_MINUS) {
        addop();
        term();
        additiveExpressionPrime();
    }
    // Otherwise done
}

// addop → + | -
void Parser::addop() {
    if (current.type == OP_PLUS) {
        eat(OP_PLUS);
    } else if (current.type == OP_MINUS) {
        eat(OP_MINUS);
    } else {
        error("+ or -");
    }
}

// term -> factor term^
void Parser::term() {
    factor();
    termPrime();
}

// term^ -> mulop factor term^ | ε
void Parser::termPrime() {
    // Check for * or /
    if (current.type == OP_MULT || current.type == OP_DIV) {
        mulop();
        factor();
        termPrime();
    }
    // Otherwise done
}

// mulop → * | /
void Parser::mulop() {
    if (current.type == OP_MULT) {
        eat(OP_MULT);
    } else if (current.type == OP_DIV) {
        eat(OP_DIV);
    } else {
        error("* or /");
    }
}

// factor → ( expression ) | var | NUM
void Parser::factor() {
    if (current.type == SYM_LPAREN) {
        eat(SYM_LPAREN);
        expression();
        eat(SYM_RPAREN);
    } else if (current.type == ID) {
        var();
    } else if (current.type == NUM) {
        eat(NUM);
    } else {
        error("(, identifier, or number");
    }
}