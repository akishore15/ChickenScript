#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

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

class ASTNode {};
class ASTExpression : public ASTNode {
public:
    Token token;

    ASTExpression(const Token &token) : token(token) {}
};

class ASTStatement : public ASTNode {};
class ASTVariableDeclaration : public ASTStatement {
public:
    Token variable;
    ASTExpression expression;

    ASTVariableDeclaration(const Token &variable, const ASTExpression &expression) : variable(variable), expression(expression) {}
};

class ASTFunction : public ASTStatement {
public:
    std::string functionName;

    ASTFunction(const
