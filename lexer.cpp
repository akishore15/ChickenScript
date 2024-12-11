#include <iostream>
#include <string>
#include <vector>
#include <cctype>

enum class TokenType {
    StartProgram,
    List,
    Division,
    Multiplication,
    Addition,
    Subtraction,
    Class,
    Math,
    SimpleBinary,
    Comment,
    EasyFunction,
    VariableDeclaration,
    Integer,
    String,
    Boolean,
    ForLoop,
    EndFor,
    WhileLoop,
    ForeverLoop,
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
            } else if (current == '$' && source[position + 1] == '/') {
                position += 2;
                tokens.push_back(Token(TokenType::StartProgram, "$/"));
            } else if (source.compare(position, 4, "lst[]") == 0) {
                position += 4;
                tokens.push_back(Token(TokenType::List, "lst[]"));
            } else if (current == '/') {
                tokens.push_back(Token(TokenType::Division, "/"));
                position++;
            } else if (current == '*') {
                tokens.push_back(Token(TokenType::Multiplication, "*"));
                position++;
            } else if (current == '+') {
                tokens.push_back(Token(TokenType::Addition, "+"));
                position++;
            } else if (source.compare(position, 2, "-.") == 0) {
                tokens.push_back(Token(TokenType::Subtraction, "-."));
                position += 2;
            } else if (source.compare(position, 5, "class") == 0) {
                position += 5;
                tokens.push_back(Token(TokenType::Class, "class"));
            } else if (source.compare(position, 9, "chad.math") == 0) {
                position += 9;
                tokens.push_back(Token(TokenType::Math, "chad.math"));
            } else if (source.compare(position, 6, "chad.0") == 0) {
                position += 6;
                tokens.push_back(Token(TokenType::SimpleBinary, "chad.0"));
            } else if (source.compare(position, 2, "//") == 0) {
                position += 2;
                while (position < source.length() && source[position] != '\n') {
                    position++;
                }
                tokens.push_back(Token(TokenType::Comment, "//"));
            } else if (source.compare(position, 11, "func.easy()") == 0) {
                position += 11;
                tokens.push_back(Token(TokenType::EasyFunction, "func.easy()"));
            } else if (source.compare(position, 3, "var") == 0) {
                position += 3;
                tokens.push_back(Token(TokenType::VariableDeclaration, "var"));
            } else if (source.compare(position, 4, "#int") == 0) {
                position += 4;
                tokens.push_back(Token(TokenType::Integer, "#int"));
            } else if (source.compare(position, 4, "#str") == 0) {
                position += 4;
                tokens.push_back(Token(TokenType::String, "#str"));
            } else if (source.compare(position, 5, "#bool") == 0) {
                position += 5;
                tokens.push_back(Token(TokenType::Boolean, "#bool"));
            } else if (source.compare(position, 3, "for") == 0) {
                position += 3;
                tokens.push_back(Token(TokenType::ForLoop, "for"));
            } else if (source.compare(position, 4, "esac") == 0) {
                position += 4;
                tokens.push_back(Token(TokenType::EndFor, "esac"));
            } else if (source.compare(position, 5, "while") == 0) {
                position += 5;
                tokens.push_back(Token(TokenType::WhileLoop, "while"));
            } else if (source.compare(position, 4, "end.") == 0) {
                position += 4;
                tokens.push_back(Token(TokenType::EndFor, "end."));
            } else if (std::isdigit(current)) {
                tokens.push_back(tokenizeNumber());
            } else if (std::isalpha(current)) {
                tokens.push_back(tokenizeIdentifier());
            } else {
                tokens.push_back(Token(TokenType::Symbol, std::string(1, current)));
                position++;
            }
        }
        tokens.emplace_back(TokenType::EndOfFile, "");
        return tokens;
    }

private:
    const std::string source;
    size_t position;

    Token tokenizeNumber() {
        size_t start = position;
        while (position < source.length() && std::isdigit(source[position])) {
            position++;
        }
        std::string value = source.substr(start, position - start);
        return Token(TokenType::Number, value);
    }

    Token tokenizeIdentifier() {
        size_t start = position;
        while (position < source.length() && (std::isalnum(source[position]) || source[position] == '_')) {
            position++;
        }
        std::string value = source.substr(start, position - start);
        return Token(TokenType::Identifier, value);
    }
};

int main() {
    std::string code = "$/ var x = 10; // This is a comment\n lst[] = [1, 2, 3];\n class div=MyClass{ code_here }\n";
    Lexer lexer(code);
    std::vector<Token> tokens = lexer.tokenize();

    for (const Token &token : tokens) {
        std::cout << "Token(";
        switch (token.type) {
            case TokenType::StartProgram:        std::cout << "StartProgram"; break;
            case TokenType::List:                std::cout << "List"; break;
            case TokenType::Division:            std::cout << "Division"; break;
            case TokenType::Multiplication:      std::cout << "Multiplication"; break;
            case TokenType::Addition:            std::cout << "Addition"; break;
            case TokenType::Subtraction:         std::cout << "Subtraction"; break;
            case TokenType::Class:               std::cout << "Class"; break;
            case TokenType::Math:                std::cout << "Math"; break;
            case TokenType::SimpleBinary:        std::cout << "SimpleBinary"; break;
            case TokenType::Comment:             std::cout << "Comment"; break;
            case TokenType::EasyFunction:        std::cout << "EasyFunction"; break;
            case TokenType::VariableDeclaration: std::cout << "VariableDeclaration"; break;
            case TokenType::Integer:             std::cout << "Integer"; break;
            case TokenType::String:              std::cout << "String"; break;
            case TokenType::Boolean:             std::cout << "Boolean"; break;
            case TokenType::ForLoop:             std::cout << "ForLoop"; break;
            case TokenType::EndFor:              std::cout << "EndFor"; break;
            case TokenType::WhileLoop:           std::cout << "WhileLoop"; break;
            case TokenType::Identifier:          std::cout << "Identifier"; break;
            case TokenType::Number:              std::cout << "Number"; break;
            case TokenType::Symbol:              std::cout << "Symbol"; break;
            case TokenType::EndOfFile:           std::cout << "EndOfFile"; break;
            default:                             std::cout << "Unknown"; break;
        }
        std::cout << ", \"" << token.value << "\")\n";
    }

    return 0;
}
