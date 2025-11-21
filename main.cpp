#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "lexer.h"
#include "parser.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: force <file.cl>\n";
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << argv[1] << endl;
        return 1;
    }

    map<string, int> environment;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        // Tokenize one line at a time
        vector<Token> tokens = tokenize(line);
        size_t pos = 0;

        if (tokens.empty()) continue;

        if (tokens[pos].type == Let) {
            parseMakeStatement(tokens, environment, pos);
        }
        else if (tokens[pos].type == Output) {
            parseOutputStatement(tokens, pos, environment);
        }
    }

    return 0;
}
