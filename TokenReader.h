#ifndef TOKEN_READER_H
#define TOKEN_READER_H

#include "Parser.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class TokenReader {
private:
    TokenType getTokenType(string name) {
        if (name == "KEYWORD_IF") return KEYWORD_IF;
        if (name == "KEYWORD_ELSE") return KEYWORD_ELSE;
        if (name == "KEYWORD_INT") return KEYWORD_INT;
        if (name == "KEYWORD_FLOAT") return KEYWORD_FLOAT;
        if (name == "KEYWORD_RETURN") return KEYWORD_RETURN;
        if (name == "KEYWORD_VOID") return KEYWORD_VOID;
        if (name == "KEYWORD_WHILE") return KEYWORD_WHILE;
        if (name == "KEYWORD_PROGRAM") return KEYWORD_PROGRAM;
        if (name == "ID") return ID;
        if (name == "NUM") return NUM;
        if (name == "OP_ASSIGN") return OP_ASSIGN;
        if (name == "OP_EQ") return OP_EQ;
        if (name == "OP_NE") return OP_NE;
        if (name == "OP_LT") return OP_LT;
        if (name == "OP_LE") return OP_LE;
        if (name == "OP_GT") return OP_GT;
        if (name == "OP_GE") return OP_GE;
        if (name == "OP_PLUS") return OP_PLUS;
        if (name == "OP_MINUS") return OP_MINUS;
        if (name == "OP_MULT") return OP_MULT;
        if (name == "OP_DIV") return OP_DIV;
        if (name == "SYM_LPAREN") return SYM_LPAREN;
        if (name == "SYM_RPAREN") return SYM_RPAREN;
        if (name == "SYM_LBRACE") return SYM_LBRACE;
        if (name == "SYM_RBRACE") return SYM_RBRACE;
        if (name == "SYM_LBRACKET") return SYM_LBRACKET;
        if (name == "SYM_RBRACKET") return SYM_RBRACKET;
        if (name == "SYM_SEMICOLON") return SYM_SEMICOLON;
        if (name == "SYM_COMMA") return SYM_COMMA;
        return ERROR_TOKEN;
    }
    
public:
    vector<Token> readFile(string filename) {
        vector<Token> tokens;
        ifstream file(filename.c_str());
        
        if (!file.is_open()) {
            cout << "Error: Cannot open file " << filename << endl;
            return tokens;
        }
        
        string line;
        while (getline(file, line)) {
            if (line.empty() || line.find("ERROR") == 0) {
                continue;
            }
            
            if (line.find("Token:") != 0) {
                continue;
            }
            
            Token token;
            
            int start = line.find(":") + 2;
            int end = line.find(",", start);
            string typeStr = line.substr(start, end - start);
            
            while (typeStr.length() > 0 && typeStr[typeStr.length()-1] == ' ') {
                typeStr = typeStr.substr(0, typeStr.length()-1);
            }
            
            token.type = getTokenType(typeStr);
            
            if (token.type == ERROR_TOKEN) {
                continue;
            }
            
            int quoteStart = line.find("'");
            int quoteEnd = line.find("'", quoteStart + 1);
            if (quoteStart >= 0 && quoteEnd >= 0) {
                token.lexeme = line.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
            }
            
            int linePos = line.find("Line:");
            if (linePos >= 0) {
                string lineStr = line.substr(linePos + 6);
                stringstream ss(lineStr);
                ss >> token.line;
            }
            
            int colPos = line.find("Col:");
            if (colPos >= 0) {
                string colStr = line.substr(colPos + 5);
                stringstream ss(colStr);
                ss >> token.column;
            }
            
            tokens.push_back(token);
        }
        
        Token eof;
        eof.type = END_OF_FILE;
        eof.lexeme = "";
        if (tokens.size() > 0) {
            eof.line = tokens[tokens.size()-1].line;
            eof.column = tokens[tokens.size()-1].column + 1;
        } else {
            eof.line = 1;
            eof.column = 1;
        }
        tokens.push_back(eof);
        
        file.close();
        return tokens;
    }
};

#endif