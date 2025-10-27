#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

using namespace std;

// Token types from our scanner
enum TokenType {
    // Keywords
    KEYWORD_IF, KEYWORD_ELSE, KEYWORD_INT, KEYWORD_FLOAT,
    KEYWORD_RETURN, KEYWORD_VOID, KEYWORD_WHILE, KEYWORD_PROGRAM,
    
    // Identifiers and numbers
    ID, NUM,
    
    // Operators
    OP_ASSIGN, OP_EQ, OP_NE, OP_LT, OP_LE, OP_GT, OP_GE,
    OP_PLUS, OP_MINUS, OP_MULT, OP_DIV,
    
    // Symbols
    SYM_LPAREN, SYM_RPAREN, SYM_LBRACE, SYM_RBRACE,
    SYM_LBRACKET, SYM_RBRACKET, SYM_SEMICOLON, SYM_COMMA,
    
    END_OF_FILE, ERROR_TOKEN
};

// Simple token structure
struct Token {
    TokenType type;
    string lexeme;
    int line;
    int column;
};

class Parser {
private:
    vector<Token> tokens;
    int position;
    Token current;
    bool hasError;
    
    // Basic helper functions
    void nextToken();
    void eat(TokenType expected);
    void error(string expected);
    string tokenName(TokenType type);
    
    // Grammar rule functions
    void program();
    void declarationList();
    void declarationListPrime();
    void declaration();
    void varDeclaration();
    void typeSpecifier();
    void compoundStmt();
    void statementList();
    void statementListPrime();
    void statement();
    void selectionStmt();
    void selectionStmtPrime();
    void iterationStmt();
    void assignmentStmt();
    void var();
    void expression();
    void expressionPrime();
    void relop();
    void additiveExpression();
    void additiveExpressionPrime();
    void addop();
    void term();
    void termPrime();
    void mulop();
    void factor();
    
public:
    Parser(vector<Token> tokenList);
    void parse();
};

#endif