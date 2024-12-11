using System;
using System.Collections.Generic;

public enum TokenType {
    Keyword,
    Identifier,
    Number,
    Symbol,
    EndOfFile,
    Unknown
}

public class Token {
    public TokenType Type { get; }
    public string Value { get; }

    public Token(TokenType type, string value) {
        Type = type;
        Value = value;
    }
}

public class Lexer {
    private readonly string _source;
    private int _position;

    public Lexer(string source) {
        _source = source;
        _position = 0;
    }

    public List<Token> Tokenize() {
        var tokens = new List<Token>();
        while (_position < _source.Length) {
            var current = _source[_position];
            if (char.IsWhiteSpace(current)) {
                _position++;
            } else if (char.IsLetter(current)) {
                tokens.Add(TokenizeIdentifierOrKeyword());
            } else if (char.IsDigit(current)) {
                tokens.Add(TokenizeNumber());
            } else {
                tokens.Add(TokenizeSymbol());
            }
        }
        tokens.Add(new Token(TokenType.EndOfFile, string.Empty));
        return tokens;
    }

    private Token TokenizeIdentifierOrKeyword() {
        var start = _position;
        while (_position < _source.Length && (char.IsLetterOrDigit(_source[_position]) || _source[_position] == '_')) {
            _position++;
        }
        var value = _source.Substring(start, _position - start);
        return value switch {
            "if" or "else" or "while" => new Token(TokenType.Keyword, value),
            _ => new Token(TokenType.Identifier, value)
        };
    }

    private Token TokenizeNumber() {
        var start = _position;
        while (_position < _source.Length && char.IsDigit(_source[_position])) {
            _position++;
        }
        var value = _source.Substring(start, _position - start);
        return new Token(TokenType.Number, value);
    }

    private Token TokenizeSymbol() {
        var value = _source[_position].ToString();
        _position++;
        return new Token(TokenType.Symbol, value);
    }
}

public abstract class ASTNode {}

public class ASTExpression : ASTNode {
    public Token Token { get; }

    public ASTExpression(Token token) {
        Token = token;
    }
}

public class ASTStatement : ASTNode {}

public class ASTIfStatement : ASTStatement {
    public ASTExpression Condition { get; }
    public ASTStatement Body { get; }

    public ASTIfStatement(ASTExpression condition, ASTStatement body) {
        Condition = condition;
        Body = body;
    }
}

public class ASTWhileStatement : ASTStatement {
    public ASTExpression Condition { get; }
    public ASTStatement Body { get; }

    public ASTWhileStatement(ASTExpression condition, ASTStatement body) {
        Condition = condition;
        Body = body;
    }
}

public class ASTAssignment : ASTStatement {
    public Token Variable { get; }
    public ASTExpression Expression { get; }

    public ASTAssignment(Token variable, ASTExpression expression) {
        Variable = variable;
        Expression = expression;
    }
}

public class Parser {
    private readonly List<Token> _tokens;
    private int _position;

    public Parser(List<Token> tokens) {
        _tokens = tokens;
        _position = 0;
    }

    public ASTStatement Parse() {
        return ParseStatement();
    }

    private ASTStatement ParseStatement() {
        var token = _tokens[_position];
        if (token.Type == TokenType.Keyword) {
            if (token.Value == "if") return ParseIfStatement();
            if (token.Value == "while") return ParseWhileStatement();
        } else if (token.Type == TokenType.Identifier) {
            return ParseAssignment();
        }
        throw new Exception($"Unexpected token: {token.Value}");
    }

    private ASTIfStatement ParseIfStatement() {
        _position++; // consume 'if'
        var condition = ParseExpression();
        var body = ParseStatement();
        return new ASTIfStatement(condition, body);
    }

    private ASTWhileStatement ParseWhileStatement() {
        _position++; // consume 'while'
        var condition = ParseExpression();
        var body = ParseStatement();
        return new ASTWhileStatement(condition, body);
    }

    private ASTAssignment ParseAssignment() {
        var variable = _tokens[_position];
        _position++; // consume variable
        var equals = _tokens[_position];
        if (equals.Value != "=") throw new Exception("Expected '='");
        _position++; // consume '='
        var expression = ParseExpression();
        return new ASTAssignment(variable, expression);
    }

    private ASTExpression ParseExpression() {
        var token = _tokens[_position];
        _position++;
        return new ASTExpression(token);
    }
}

public class SharkaraInterpreter {
    public static void Main(string[] args) {
        string code = "if x == 10 while y != 20";
        Lexer lexer = new Lexer(code);
        List<Token> tokens = lexer.Tokenize();

        Parser parser = new Parser(tokens);
        ASTStatement ast = parser.Parse();

        Console.WriteLine("Parsing completed.");
    }
}
