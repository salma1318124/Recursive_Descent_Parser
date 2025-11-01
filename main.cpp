#include "Parser.h"
#include "TokenReader.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <token_file>" << endl;
        cout << "Example: " << argv[0] << " tokens.txt" << endl;
        return 1;
    }
    
    string filename = argv[1];
    
    cout << "========================================" << endl;
    cout << "   C- Language Parser" << endl;
    cout << "========================================" << endl;
    cout << "\nReading tokens from: " << filename << endl;
    
    TokenReader reader;
    vector<Token> tokens = reader.readFile(filename);
    
    if (tokens.size() == 0) {
        cout << "Error: No tokens found" << endl;
        return 1;
    }
    
    if (tokens.size() == 1 && tokens[0].type == END_OF_FILE) {
        cout << "Error: No tokens found" << endl;
        return 1;
    }
    
    cout << "Total tokens: " << tokens.size() - 1 << endl;
    cout << "\nStarting parsing..." << endl;
    cout << "========================================" << endl;
    
    Parser parser(tokens);
    parser.parse();
    
    return 0;
}