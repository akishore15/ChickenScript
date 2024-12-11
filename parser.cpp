#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

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
            if (isspace(current)) {
                position++;
            } else if (isalpha(current)) {
                tokens.push_back(tokenizeIdentifierOrKeyword());
            } else if (isdigit(current)) {
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
        while (position < source.length() && (isalnum(source[position]) || source[position] == '_')) {
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
        while (position < source.length() && isdigit(source[position])) {
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

class ASTNode {};
class ASTExpression : public ASTNode {
public:
    Token token;

    ASTExpression(const Token &token) : token(token) {}
};

class ASTStatement : public ASTNode {};
class ASTIfStatement : public ASTStatement {
public:
    ASTExpression condition;
    ASTStatement *body;

    ASTIfStatement(const ASTExpression &condition, ASTStatement *body) : condition(condition), body(body) {}
};

class ASTWhileStatement : public ASTStatement {
public:
    ASTExpression condition;
    ASTStatement *body;

    ASTWhileStatement(const ASTExpression &condition, ASTStatement *body) : condition(condition), body(body) {}
};

class ASTAssignment : public ASTStatement {
public:
    Token variable;
    ASTExpression expression;

    ASTAssignment(const Token &variable, const ASTExpression &expression) : variable(variable), expression(expression) {}
};

class Parser {
public:
    Parser(const std::vector<Token> &tokens) : tokens(tokens), position(0) {}

    ASTStatement *parse() {
        return parseStatement();
    }

private:
    const std::vector<Token> tokens;
    size_t position;

    ASTStatement *parseStatement() {
        Token token = tokens[position];
        if (token.type == TokenType::Keyword) {
            if (token.value == "if") return parseIfStatement();
            if (token.value == "while") return parseWhileStatement();
        } else if (token.type == TokenType::Identifier) {
            return parseAssignment();
        }
        throw std::runtime_error("Unexpected token: " + token.value);
    }

    ASTIfStatement *parseIfStatement() {
        position++; // consume 'if'
        ASTExpression condition = parseExpression();
        ASTStatement *body = parseStatement();
        return new ASTIfStatement(condition, body);
    }

    ASTWhileStatement *parseWhileStatement() {
        position++; // consume 'while'
        ASTExpression condition = parseExpression();
        ASTStatement *body = parseStatement();
        return new ASTWhileStatement(condition, body);
    }

    ASTAssignment *parseAssignment() {
        Token variable = tokens[position];
        position++; // consume variable
        Token equals = tokens[position];
        if (equals.value != "=") throw std::runtime_error("Expected '='");
        position++; // consume '='
        ASTExpression expression = parseExpression();
        return new ASTAssignment(variable, expression);
    }

    ASTExpression parseExpression() {
        Token token = tokens[position];
        position++;
        return ASTExpression(token);
    }
};

int main() {
    std::string code = "if x == 10 while y != 20";
    Lexer lexer(code);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    ASTStatement *ast = parser.parse();

    std::cout << "Parsing completed." << std::endl;

    return 0;
}



