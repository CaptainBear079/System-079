#include <iostream>
#include <string>
#include <vector>
#include <fstream>

namespace Compiler {
    struct Token {
        int type; // Token type z.B. identifier, operator, etc.
        // 0 = keyword
        // 1 = identifier
        // 2 = operator
        // 3 = number
        // 4 = string
        // 5 = hexadecimal
        // 6 = binary
        int num_value;
        std::string str_value;
    };
    Token GenerateToken(std::vector<char> chars, bool binary, bool hex) {
        char c;
        std::string s_chars;
        for(long long unsigned int i = 0; i < chars.size(); i++) {
            c = chars.at(i);
            s_chars.append(1, c);
        }
        if(s_chars == "int") {
            return { 0, 0, "int" };
        }
        else if(s_chars == "=") {
            return { 0, 0, "=" };
        }
        return { -1, -1, "-1" }; // Error token
    }
    std::vector<Token> GenerateTokens(std::ifstream* src) {
        char c;
        std::vector<char> chars;
        std::string s_chars;
        std::vector<Token> Tokens;
        bool binary = false;
        bool hex = false;
        src->get(c);
        for(long long unsigned int i = 0; i < chars.size(); i++) {
            while(std::isalnum(c)) {
                chars.push_back(c);
                src->get(c);
            }
            if(std::isdigit(c)) {
                chars.push_back(c);
                src->get(c);
                while(true) {
                    if(std::isspace(c) || c == '\n') {
                        i++;
                        break;
                    }
                    else if(std::isalpha(c)) {
                        if(c == 'b') {
                            chars.push_back(c);
                            i++;
                            binary = true;
                            break;
                        }
                        else if(c == 'h') {
                            chars.push_back(c);
                            i++;
                            hex = true;
                            break;
                        }
                    }
                    break;
                }
            }
            Token token = GenerateToken(chars, binary, hex);
        }
        return Tokens;
    }
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cerr << "[ERROR][Nullcode Compiler] No source file or target system specified." << std::endl;
        return 405; // Missing argument(s)
    }
    // Open source file
    std::ifstream sourceFile(argv[1]);
    if(!sourceFile.is_open()) {
        std::cerr << "[ERROR][Nullcode Compiler] Failed to open source file: " << argv[1] << std::endl;
        return 404; // Return "File not found"
    }
    // Platform selection
    // Target platform string to lowercase
    std::string targetPlatform = argv[2];
    int ret = 0;
    for(auto& c : targetPlatform) {
        c = tolower(c);
    }
    std::vector<Compiler::Token> tokens;
    if(targetPlatform == "nullmodulecpu") {
        tokens = Compiler::GenerateTokens(&sourceFile);
    }
    sourceFile.close();
    for(auto& token : tokens) {
        std::cout << "Token Type: " << token.type << ", Value: " << token.str_value << std::endl;
    }
    return ret;
}