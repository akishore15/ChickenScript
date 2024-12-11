#include <iostream>
#include <string>
#include <vector>
#include <cctype>

enum class TokenType {
    Keyword,
    Identifier,
    Number,
    Symbol,
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, std::string v) : type(t), value(std::move(v)) {}
};

class Lexer {
public:
    Lexer(const std::string &source) : source(source), position(0) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (position < source.length()) {
            char current = source[position];
            if (std::isspace(current)) {
                position++;
            } else if (std::isalpha(current)) {
                tokens.push_back(tokenizeIdentifierOrKeyword());
            } else if (std::isdigit(current)) {
                tokens.push_back(tokenizeNumber());
            } else {
                tokens.push_back(tokenizeSymbol());
            }
        }
        tokens.emplace_back(TokenType::EndOfFile, "");
        return tokens;
    }

private:
    const std::string source;
    size_t position;

    Token tokenizeIdentifierOrKeyword() {
        size_t start = position;
        while (position < source.length() && (std::isalnum(source[position]) || source[position] == '_')) {
            position++;
        }
        std::string value = source.substr(start, position - start);
        if (value == "if" || value == "else" || value == "while") {  // Add more keywords here
            return Token(TokenType::Keyword, value);
        }
        return Token(TokenType::Identifier, value);
    }

    Token tokenizeNumber() {
        size_t start = position;
        while (position < source.length() && std::isdigit(source[position])) {
            position++;
        }
        std::string value = source.substr(start, position - start);
        return Token(TokenType::Number, value);
    }

    Token tokenizeSymbol() {
        char current = source[position];
        position++;
        return Token(TokenType::Symbol, std::string(1, current));
    }
};

int main() {
    std::string code = "if x == 10 while y != 20";
    Lexer lexer(code);
    std::vector<Token> tokens = lexer.tokenize();

    for (const Token &token : tokens) {
        std::cout << "Token(";
        switch (token.type) {
            case TokenType::Keyword:    std::cout << "Keyword"; break;
            case TokenType::Identifier: std::cout << "Identifier"; break;
            case TokenType::Number:     std::cout << "Number"; break;
            case TokenType::Symbol:     std::cout << "Symbol"; break;
            case TokenType::EndOfFile:  std::cout << "EndOfFile"; break;
            default:                    std::cout << "Unknown"; break;
        }
        std::cout << ", \"" << token.value << "\")\n";
    }

    return 0;
}
