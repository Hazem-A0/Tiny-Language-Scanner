#include <bits/stdc++.h>
using namespace std;

enum class TokenType {
    SEMICOLON, IF, THEN, END, REPEAT, UNTIL,
    IDENTIFIER, ASSIGN, READ, WRITE,
    LESSTHAN, EQUAL, PLUS, MINUS, MUL, DIV,
    OPENBRACKET, CLOSEDBRACKET, NUMBER,
    UNKNOWN, END_OF_FILE
};

string tokenTypeToString(TokenType t) {
    switch(t){
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::IF: return "IF";
        case TokenType::THEN: return "THEN";
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

static unordered_map<string, TokenType> keywords = {
    {"if", TokenType::IF},
    {"then", TokenType::THEN},
    {"end", TokenType::END},
    {"repeat", TokenType::REPEAT},
    {"until", TokenType::UNTIL},
    {"read", TokenType::READ},
    {"write", TokenType::WRITE}
};

struct Token {
    string value;
    TokenType type;
};

class Scanner {
    string input;
    size_t pos = 0;
    size_t len = 0;

public:
    Scanner(const string &s): input(s), pos(0), len(s.size()) {}

    char peek() const {
        if (pos < len) return input[pos];
        return '\0';
    }
    char get() {
        if (pos < len) return input[pos++];
        return '\0';
    }
    void skipWhitespace() {
        while (isspace((unsigned char)peek())) get();
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

        if (isdigit((unsigned char)c)) {
            string num;
            while (isdigit((unsigned char)peek())) num.push_back(get());
            return {num, TokenType::NUMBER};
        }


        if (isalpha((unsigned char)c)) {
            string word;
            while (isalnum((unsigned char)peek())) word.push_back(get());
    
            string lw = word;
            transform(lw.begin(), lw.end(), lw.begin(), [](unsigned char ch){ return tolower(ch); });
            auto it = keywords.find(lw);
            if (it != keywords.end()) {
                return {word, it->second};
            } else {
                return {word, TokenType::IDENTIFIER};
            }
        }

        string unk(1, get());
        return {unk, TokenType::UNKNOWN};
    }
};

int main(int argc, char **argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 3) {
        cerr << "Usage: tiny_scanner.exe <input_file> <output_file>\n";
        return 1;
    }

    string inpath = argv[1];
    string outpath = argv[2];

    // Read input file into a string
    ifstream fin(inpath, ios::in | ios::binary);
    if (!fin) {
        cerr << "Error: cannot open input file: " << inpath << "\n";
        return 2;
    }
    string src((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    fin.close();

    // remove UTF-8 BOM if present
    if (src.size() >= 3 &&
        (unsigned char)src[0] == 0xEF &&
        (unsigned char)src[1] == 0xBB &&
        (unsigned char)src[2] == 0xBF) {
        src.erase(0, 3);
    }

    // normalize newlines (Windows CRLF -> LF)
    replace(src.begin(), src.end(), '\r', '\n');

    Scanner scanner(src);
    ofstream fout(outpath);
    if (!fout) {
        cerr << "Error: cannot open output file: " << outpath << "\n";
        return 3;
    }

    while (true) {
        Token tok = scanner.nextToken();
        if (tok.type == TokenType::END_OF_FILE) break;
        fout << tok.value << " , " << tokenTypeToString(tok.type) << "\n";
    }

    fout.close();
    cout << "Scanning complete. Output written to: " << outpath << "\n";
    return 0;
}
