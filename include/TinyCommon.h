#ifndef TINY_COMMON_H
#define TINY_COMMON_H

#include <string>
#include <unordered_map>
#include <map>

enum class TokenType {
    SEMICOLON, IF, THEN, ELSE, END, REPEAT, UNTIL,
    IDENTIFIER, ASSIGN, READ, WRITE,
    LESSTHAN, EQUAL, PLUS, MINUS, MUL, DIV,
    OPENBRACKET, CLOSEDBRACKET, NUMBER,
    UNKNOWN, END_OF_FILE
};

struct Token {
    std::string value;
    TokenType type;
    
    Token(const std::string& val, TokenType t) : value(val), type(t) {}
    Token() : value(""), type(TokenType::UNKNOWN) {}
};

inline std::string tokenTypeToString(TokenType t) {
    switch(t){
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::IF: return "IF";
        case TokenType::THEN: return "THEN";
        case TokenType::ELSE: return "ELSE";
        case TokenType::END: return "END";
        case TokenType::REPEAT: return "REPEAT";
        case TokenType::UNTIL: return "UNTIL";
        case TokenType::IDENTIFIER: return "Identifier";
        case TokenType::ASSIGN: return "assign";
        case TokenType::READ: return "READ";
        case TokenType::WRITE: return "WRITE";
        case TokenType::LESSTHAN: return "LESSTHAN";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MUL: return "MULT";
        case TokenType::DIV: return "DIV";
        case TokenType::OPENBRACKET: return "OPENBRACKET";
        case TokenType::CLOSEDBRACKET: return "CLOSEDBRACKET";
        case TokenType::NUMBER: return "number";
        case TokenType::UNKNOWN: return "UNKNOWN";
        default: return "EOF";
    }
}

inline TokenType stringToTokenType(const std::string& typeStr) {
    static std::map<std::string, TokenType> typeMap = {
        {"SEMICOLON", TokenType::SEMICOLON},
        {"IF", TokenType::IF},
        {"THEN", TokenType::THEN},
        {"ELSE", TokenType::ELSE},
        {"END", TokenType::END},
        {"REPEAT", TokenType::REPEAT},
        {"UNTIL", TokenType::UNTIL},
        {"Identifier", TokenType::IDENTIFIER},
        {"IDENTIFIER", TokenType::IDENTIFIER},
        {"assign", TokenType::ASSIGN},
        {"ASSIGN", TokenType::ASSIGN},
        {"READ", TokenType::READ},
        {"WRITE", TokenType::WRITE},
        {"LESSTHAN", TokenType::LESSTHAN},
        {"EQUAL", TokenType::EQUAL},
        {"PLUS", TokenType::PLUS},
        {"MINUS", TokenType::MINUS},
        {"MULT", TokenType::MUL},
        {"MUL", TokenType::MUL},
        {"DIV", TokenType::DIV},
        {"OPENBRACKET", TokenType::OPENBRACKET},
        {"CLOSEDBRACKET", TokenType::CLOSEDBRACKET},
        {"number", TokenType::NUMBER},
        {"NUMBER", TokenType::NUMBER}
    };
    
    auto it = typeMap.find(typeStr);
    if (it != typeMap.end()) {
        return it->second;
    }
    return TokenType::UNKNOWN;
}

static std::unordered_map<std::string, TokenType> keywords = {
    {"if", TokenType::IF},
    {"then", TokenType::THEN},
    {"else", TokenType::ELSE},
    {"end", TokenType::END},
    {"repeat", TokenType::REPEAT},
    {"until", TokenType::UNTIL},
    {"read", TokenType::READ},
    {"write", TokenType::WRITE}
};

#endif // TINY_COMMON_H
