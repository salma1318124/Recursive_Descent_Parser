#include "Parser.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Parser::Parser(vector<Token> tokenList) {
    tokens = tokenList;
    position = 0;
    hasError = false;
    
    if (tokens.size() > 0) {
        current = tokens[0];
    }
}

void Parser::nextToken() {
    if (position < tokens.size() - 1) {
        position++;
        current = tokens[position];
    }
}

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

void Parser::eat(TokenType expected) {
    if (current.type == expected) {
        nextToken();
    } else {
        error(tokenName(expected));
    }
}

void Parser::error(string expected) {
    cout << "\nSYNTAX ERROR at line " << current.line 
         << ", column " << current.column << endl;
    cout << "Unexpected token: '" << current.lexeme << "'" << endl;
    cout << "Expected: " << expected << endl;
    hasError = true;
    exit(1);
}

void Parser::parse() {
    program();
    
    if (current.type != END_OF_FILE) {
        error("end of file");
    }
    
    if (!hasError) {
        cout << "\nParsing completed successfully!" << endl;
    }
}

void Parser::program() {
    eat(KEYWORD_PROGRAM);
    eat(ID);
    eat(SYM_LBRACE);
    declarationList();
    statementList();
    eat(SYM_RBRACE);
}

void Parser::declarationList() {
    declaration();
    declarationListPrime();
}

void Parser::declarationListPrime() {
    if (current.type == KEYWORD_INT || current.type == KEYWORD_FLOAT) {
        declaration();
        declarationListPrime();
    }
 
}

void Parser::declaration() {
    varDeclaration();
}

void Parser::varDeclaration() {
    typeSpecifier();
    eat(ID);
    
    if (current.type == SYM_LBRACKET) {
        eat(SYM_LBRACKET);
        eat(NUM);
        eat(SYM_RBRACKET);
    }
    
    eat(SYM_SEMICOLON);
}

void Parser::typeSpecifier() {
    if (current.type == KEYWORD_INT) {
        eat(KEYWORD_INT);
    } else if (current.type == KEYWORD_FLOAT) {
        eat(KEYWORD_FLOAT);
    } else {
        error("int or float");
    }
}

void Parser::compoundStmt() {
    eat(SYM_LBRACE);
    statementList();
    eat(SYM_RBRACE);
}

void Parser::statementList() {
    if (current.type == ID || current.type == SYM_LBRACE ||
        current.type == KEYWORD_IF || current.type == KEYWORD_WHILE) {
        statement();
        statementListPrime();
    }
    
}

void Parser::statementListPrime() {
    if (current.type == ID || current.type == SYM_LBRACE ||
        current.type == KEYWORD_IF || current.type == KEYWORD_WHILE) {
        statement();
        statementListPrime();
    }
}

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

void Parser::selectionStmt() {
    eat(KEYWORD_IF);
    eat(SYM_LPAREN);
    expression();
    eat(SYM_RPAREN);
    statement();
    selectionStmtPrime();
}

void Parser::selectionStmtPrime() {
    if (current.type == KEYWORD_ELSE) {
        eat(KEYWORD_ELSE);
        statement();
    }
}

void Parser::iterationStmt() {
    eat(KEYWORD_WHILE);
    eat(SYM_LPAREN);
    expression();
    eat(SYM_RPAREN);
    statement();
}

void Parser::assignmentStmt() {
    var();
    eat(OP_ASSIGN);
    expression();
}

void Parser::var() {
    eat(ID);
    
    if (current.type == SYM_LBRACKET) {
        eat(SYM_LBRACKET);
        expression();
        eat(SYM_RBRACKET);
    }
}

void Parser::expression() {
    additiveExpression();
    expressionPrime();
}

void Parser::expressionPrime() {
    if (current.type == OP_LT || current.type == OP_LE ||
        current.type == OP_GT || current.type == OP_GE ||
        current.type == OP_EQ || current.type == OP_NE) {
        relop();
        additiveExpression();
        expressionPrime();
    }
}

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

void Parser::additiveExpression() {
    term();
    additiveExpressionPrime();
}

void Parser::additiveExpressionPrime() {
    if (current.type == OP_PLUS || current.type == OP_MINUS) {
        addop();
        term();
        additiveExpressionPrime();
    }
}

void Parser::addop() {
    if (current.type == OP_PLUS) {
        eat(OP_PLUS);
    } else if (current.type == OP_MINUS) {
        eat(OP_MINUS);
    } else {
        error("+ or -");
    }
}

void Parser::term() {
    factor();
    termPrime();
}

void Parser::termPrime() {
    if (current.type == OP_MULT || current.type == OP_DIV) {
        mulop();
        factor();
        termPrime();
    }
}

void Parser::mulop() {
    if (current.type == OP_MULT) {
        eat(OP_MULT);
    } else if (current.type == OP_DIV) {
        eat(OP_DIV);
    } else {
        error("* or /");
    }
}

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