#ifndef TINY_SCANNER_H
#define TINY_SCANNER_H

#include "TinyCommon.h"
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

class Scanner {
    std::string input;
    size_t pos = 0;
    size_t len = 0;

public:
    Scanner(const std::string &s): input(s), pos(0), len(s.size()) {}

    char peek() const {
        if (pos < len) return input[pos];
        return '\0';
    }

    char get() {
        if (pos < len) return input[pos++];
        return '\0';
    }

    void skipWhitespace() {
        while (std::isspace((unsigned char)peek())) get();
    }

    void skipComment() {
        if (peek() == '{') {
            get();
            while (peek() != '\0' && peek() != '}') get();
            if (peek() == '}') get();
        }
    }

    Token nextToken() {
        while (true) {
            skipWhitespace();
            if (peek() == '{') { skipComment(); continue; }
            break;
        }

        char c = peek();
        if (c == '\0') return {"", TokenType::END_OF_FILE};

        if (c == ':') {
            get();
            if (peek() == '=') {
                get();
                return {":=", TokenType::ASSIGN};
            } else {
                return {":", TokenType::UNKNOWN};
            }
        }

        if (c == ';') { get(); return {";", TokenType::SEMICOLON}; }
        if (c == '<') { get(); return {"<", TokenType::LESSTHAN}; }
        if (c == '=') { get(); return {"=", TokenType::EQUAL}; }
        if (c == '+') { get(); return {"+", TokenType::PLUS}; }
        if (c == '-') { get(); return {"-", TokenType::MINUS}; }
        if (c == '*') { get(); return {"*", TokenType::MUL}; }
        if (c == '/') { get(); return {"/", TokenType::DIV}; }
        if (c == '(') { get(); return {"(", TokenType::OPENBRACKET}; }
        if (c == ')') { get(); return {")", TokenType::CLOSEDBRACKET}; }

        if (std::isdigit((unsigned char)c)) {
            std::string num;
            while (std::isdigit((unsigned char)peek())) num.push_back(get());
            return {num, TokenType::NUMBER};
        }

        if (std::isalpha((unsigned char)c)) {
            std::string word;
            while (std::isalpha((unsigned char)peek())) word.push_back(get());

            std::string lw = word;
            std::transform(lw.begin(), lw.end(), lw.begin(), [](unsigned char ch){ return std::tolower(ch); });
            auto it = keywords.find(lw);

            if (it != keywords.end()) {
                return {word, it->second};
            }
            else {
                return {word, TokenType::IDENTIFIER};
            }
        }

        std::string unk(1, get());
        return {unk, TokenType::UNKNOWN};
    }

    // Scan all tokens and return as a vector
    std::vector<Token> scanAll() {
        std::vector<Token> tokens;
        while (true) {
            Token tok = nextToken();
            if (tok.type == TokenType::END_OF_FILE) break;
            tokens.push_back(tok);
        }
        return tokens;
    }
};

#endif // TINY_SCANNER_H
