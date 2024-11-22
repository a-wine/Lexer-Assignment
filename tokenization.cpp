#ifndef TOKENIZATION_H
#define TOKENIZATION_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>

// Define TokenType
enum class TokenType {
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL, IDENTIFIER, NUMBER,
    AND, ELSE, FALSE, IF, LET, NIL, OR, TRUE, WHILE, PRINT,
    END_OF_FILE
};

// Define Token struct
struct Token {
    TokenType type;
    std::string lexeme;
    int line;

    Token(TokenType type, const std::string &lexeme, int line)
        : type(type), lexeme(lexeme), line(line) {}
};

// Define Lexer class
class Lexer {
public:
    Lexer(const std::string &source) : source(source) {}

    std::vector<Token> scanTokens() {
        while (!isAtEnd()) {
            start = current;
            scanToken();
        }
        tokens.emplace_back(TokenType::END_OF_FILE, "", line);
        return tokens;
    }

private:
    const std::string &source;
    std::vector<Token> tokens;
    size_t start = 0;
    size_t current = 0;
    int line = 1;

    std::unordered_map<std::string, TokenType> keywords = {
        {"and", TokenType::AND}, {"else", TokenType::ELSE},
        {"false", TokenType::FALSE}, {"if", TokenType::IF},
        {"let", TokenType::LET}, {"nil", TokenType::NIL},
        {"or", TokenType::OR}, {"true", TokenType::TRUE},
        {"while", TokenType::WHILE}, {"print", TokenType::PRINT}
    };

    bool isAtEnd() const { return current >= source.length(); }

    char advance() { return source[current++]; }

    void addToken(TokenType type) {
        std::string text = source.substr(start, current - start);
        tokens.emplace_back(type, text, line);
    }

    bool match(char expected) {
        if (isAtEnd() || source[current] != expected) return false;
        current++;
        return true;
    }

    char peek() const { return isAtEnd() ? '\0' : source[current]; }

    void scanToken() {
        char c = advance();
        switch (c) {
            case '(': addToken(TokenType::LEFT_PAREN); break;
            case ')': addToken(TokenType::RIGHT_PAREN); break;
            case '{': addToken(TokenType::LEFT_BRACE); break;
            case '}': addToken(TokenType::RIGHT_BRACE); break;
            case ',': addToken(TokenType::COMMA); break;
            case '.': addToken(TokenType::DOT); break;
            case '-': addToken(TokenType::MINUS); break;
            case '+': addToken(TokenType::PLUS); break;
            case ';': addToken(TokenType::SEMICOLON); break;
            case '*': addToken(TokenType::STAR); break;
            case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
            case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
            case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
            case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
            case '/':
                if (match('/')) {
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    addToken(TokenType::SLASH);
                }
                break;
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                line++;
                break;
            default:
                if (std::isdigit(c)) {
                    number();
                } else if (std::isalpha(c)) {
                    identifier();
                } else {
                    std::cerr << "Unexpected character: " << c << " at line " << line << "\n";
                }
                break;
        }
    }

    void number() {
        while (std::isdigit(peek())) advance();
        if (peek() == '.' && std::isdigit(peekNext())) {
            advance();
            while (std::isdigit(peek())) advance();
        }
        addToken(TokenType::NUMBER);
    }

    char peekNext() const { return current + 1 >= source.length() ? '\0' : source[current + 1]; }

    void identifier() {
        while (std::isalnum(peek()) || peek() == '_') advance();
        std::string text = source.substr(start, current - start);
        TokenType type = keywords.count(text) ? keywords[text] : TokenType::IDENTIFIER;
        addToken(type);
    }
};

#endif
