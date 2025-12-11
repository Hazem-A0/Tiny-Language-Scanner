#ifndef TINY_PARSER_H
#define TINY_PARSER_H

#include "TinyCommon.h"
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <cstdlib>

// AST Node base class
class ASTNode {
public:
    std::string nodeType;
    std::vector<std::shared_ptr<ASTNode>> children;
    std::string value;
    int nodenum=-1;


    ASTNode(const std::string& type) : nodeType(type), value("") {}
    ASTNode(const std::string& type, const std::string& val) : nodeType(type), value(val) {}

    virtual ~ASTNode() = default;

    void addChild(std::shared_ptr<ASTNode> child) {
        if (child != nullptr) {
            children.push_back(child);
        }
    }

    std::string toString(int level = 0) const {
        std::string indent(level * 2, ' ');
        std::string result = indent + nodeType;
        if (!value.empty()) {
            result += " (" + value + ")";
        }
        result += "\n";

        for (const auto& child : children) {
            result += child->toString(level + 1);
        }
        return result;
    }

    // Generate GraphViz DOT format for visualization
    std::string toDot(int& nodeCounter)  {
        int myId = nodeCounter;
        nodenum = myId;
        int flag=0;
        std::string result;

        // Create node label
        std::string label = nodeType;
        if (!value.empty()) {
            label += "\\n" + value;
        }

        // Escape special characters in label
        size_t pos = 0;
        while ((pos = label.find("\"", pos)) != std::string::npos) {
            label.replace(pos, 1, "\\\"");
            pos += 2;
        }

        // Decide shape
        std::string shape = "ellipse";
        if (nodeType.find("Statement") != std::string::npos || nodeType == "Program") {
            shape = "box";
        }

        // Add node with proper shape
        if(nodeType.find("Sequence") == -1 && nodeType.find("Program") == -1) {
            flag=1;
            nodeCounter++;

            result += "  node" + std::to_string(myId) + " [shape=" + shape + ", fontname=\"Arial\", label=\"" + label + "\"];\n";
        }

        std::shared_ptr<ASTNode> firstchild = nullptr;
        int firstChildId=-1;
        // Add edges to children
        for (const auto& child : children) {
            int childStartId = nodeCounter;
            if(firstChildId == -1 && (child->nodeType).find("Statement") !=-1 ) {
                firstchild = child;
                firstChildId = childStartId;
            }
            result += child->toDot(nodeCounter);
            if(flag) {
                if((child->nodeType).find("Statement") !=-1) {
                    if(firstchild == child)
                        result += "  node" + std::to_string(myId) + " -- node" + std::to_string(childStartId) + ";\n";
                    else {
                        if(this->nodeType == "If-Statement" && children.size()>2 && child==children.back()) {
                            result += "  node" + std::to_string(myId) + " -- node" + std::to_string(childStartId) + ";\n";
                        }else {
                            result += "  node" + std::to_string(firstChildId) + " -- node" + std::to_string(childStartId) + ";\n";
                            result += " { rank = same; node" + std::to_string(firstChildId) + " ; node" + std::to_string(childStartId) + ";}\n";
                        }

                    }
                }else {
                    result += "  node" + std::to_string(myId) + " -- node" + std::to_string(childStartId) + ";\n";
                    
                }
            }
        }
        if(nodeType.find("Sequence") != -1) {
            for (int i=1; i<children.size(); i++) {
                result += "  node" + std::to_string(children[i-1]->nodenum) + " -- node" + std::to_string(children[i]->nodenum) + ";\n";
            }
            result += " subgraph sub"+std::to_string(myId++) +" { ";
            result+= "rank = same;" ;
            for (int i=0; i<children.size(); i++) {
                result += " node" + std::to_string(children[i]->nodenum) + " ; " ;
            }
            result +="}\n";
        }
        if(nodeType.find("If") != -1) {
            result += "  node" + std::to_string(children[0]->nodenum) + " -- node" + std::to_string(children[1]->nodenum) + "[style=invis];\n";
            result += " { rank = same; node" + std::to_string(children[0]->nodenum) + " ; node" + std::to_string(children[1]->nodenum) + ";}\n";
        }
        if(nodeType.find("Repeat") != -1) {
            result += "  node" + std::to_string((children[children.size()-2]->children)[((children[children.size()-2])->children.size())-1]->nodenum) + " -- node" + std::to_string(children[children.size()-1]->nodenum) + "[style=invis];\n";
            result += " { rank = same; node" + std::to_string(children[children.size()-1]->nodenum) + " ; node" + std::to_string((children[children.size()-2]->children)[((children[children.size()-2])->children.size())-1]->nodenum) + ";}\n";
        }
        return result;
    }

     std::string toGraphViz()  {
        int counter = 0;
        std::string result = "graph SyntaxTree {\n";
        result += "  graph [rankdir=TB];\n";
        result += "  node [fontname=\"Arial\"];\n";
        result += "  edge [fontname=\"Arial\"];\n\n";

        //bfs for level view in image


        result += toDot(counter);
        result += "}\n";

        return result;
    }
};

// Parser Exception
class ParserException : public std::runtime_error {
public:
    ParserException(const std::string& msg) : std::runtime_error(msg) {}
};

// TINY Parser Class
class TinyParser {
private:
    std::vector<Token> tokens;
    size_t currentIndex;
    Token* currentToken;
    std::vector<std::string> errors;

    // Get next token
    void advance() {
        currentIndex++;
        if (currentIndex < tokens.size()) {
            currentToken = &tokens[currentIndex];
        } else {
            currentToken = nullptr;
        }
    }

    // Match expected token type
    void match(TokenType expected) {
        if (currentToken == nullptr) {
            throw ParserException("Unexpected end of input");
        }
        if (currentToken->type != expected) {
            throw ParserException("Expected different token type at '" + currentToken->value + "'");
        }
        advance();
    }

    // Grammar rules implementation

    // program -> stmt-sequence
    std::shared_ptr<ASTNode> parseProgram() {
        auto root = std::make_shared<ASTNode>("Program");
        auto stmtSeq = parseStmtSequence();
        root->addChild(stmtSeq);
        return root;
    }

    // stmt-sequence -> statement { ; statement }
    std::shared_ptr<ASTNode> parseStmtSequence() {
        auto node = std::make_shared<ASTNode>("Statement-Sequence");

        node->addChild(parseStatement());

        while (currentToken != nullptr && currentToken->type == TokenType::SEMICOLON) {
            advance(); // consume semicolon
            if (currentToken != nullptr &&
                currentToken->type != TokenType::END &&
                currentToken->type != TokenType::UNTIL &&
                currentToken->type != TokenType::ELSE) {
                node->addChild(parseStatement());
            }
        }

        return node;
    }

    // statement -> if-stmt | repeat-stmt | assign-stmt | read-stmt | write-stmt
    std::shared_ptr<ASTNode> parseStatement() {
        if (currentToken == nullptr) {
            throw ParserException("Unexpected end of input in statement");
        }

        switch (currentToken->type) {
            case TokenType::IF:
                return parseIfStmt();
            case TokenType::REPEAT:
                return parseRepeatStmt();
            case TokenType::READ:
                return parseReadStmt();
            case TokenType::WRITE:
                return parseWriteStmt();
            case TokenType::IDENTIFIER:
                return parseAssignStmt();
            default:
                throw ParserException("Invalid statement starting with '" + currentToken->value + "'");
        }
    }

    // if-stmt -> IF exp THEN stmt-sequence [ ELSE stmt-sequence ] END
    std::shared_ptr<ASTNode> parseIfStmt() {
        auto node = std::make_shared<ASTNode>("If-Statement");

        match(TokenType::IF);
        node->addChild(parseExp());
        match(TokenType::THEN);
        node->addChild(parseStmtSequence());

        // Handle optional ELSE clause
        if (currentToken != nullptr && currentToken->type == TokenType::ELSE) {
            advance(); // consume ELSE
            node->addChild(parseStmtSequence());
        }

        match(TokenType::END);

        return node;
    }

    // repeat-stmt -> REPEAT stmt-sequence UNTIL exp
    std::shared_ptr<ASTNode> parseRepeatStmt() {
        auto node = std::make_shared<ASTNode>("Repeat-Statement");

        match(TokenType::REPEAT);
        node->addChild(parseStmtSequence());
        match(TokenType::UNTIL);
        node->addChild(parseExp());

        return node;
    }

    // assign-stmt -> identifier := exp
    std::shared_ptr<ASTNode> parseAssignStmt() {
        auto node = std::make_shared<ASTNode>("Assign-Statement");

        auto idNode = std::make_shared<ASTNode>("Identifier", currentToken->value);
        node->addChild(idNode);

        match(TokenType::IDENTIFIER);
        match(TokenType::ASSIGN);
        node->addChild(parseExp());

        return node;
    }

    // read-stmt -> READ identifier
    std::shared_ptr<ASTNode> parseReadStmt() {
        auto node = std::make_shared<ASTNode>("Read-Statement");

        match(TokenType::READ);

        auto idNode = std::make_shared<ASTNode>("Identifier", currentToken->value);
        node->addChild(idNode);

        match(TokenType::IDENTIFIER);

        return node;
    }

    // write-stmt -> WRITE exp
    std::shared_ptr<ASTNode> parseWriteStmt() {
        auto node = std::make_shared<ASTNode>("Write-Statement");

        match(TokenType::WRITE);
        node->addChild(parseExp());

        return node;
    }

    // exp -> simple-exp [ comparison-op simple-exp ]
    std::shared_ptr<ASTNode> parseExp() {
        auto left = parseSimpleExp();

        if (currentToken != nullptr &&
            (currentToken->type == TokenType::LESSTHAN ||
             currentToken->type == TokenType::EQUAL)) {

            std::string op = (currentToken->type == TokenType::LESSTHAN) ? "<" : "=";
            auto node = std::make_shared<ASTNode>("Comparison-Op", op);
            advance();

            node->addChild(left);
            node->addChild(parseSimpleExp());

            return node;
        }

        return left;
    }

    // simple-exp -> term { addop term }
    std::shared_ptr<ASTNode> parseSimpleExp() {
        auto left = parseTerm();

        while (currentToken != nullptr &&
               (currentToken->type == TokenType::PLUS ||
                currentToken->type == TokenType::MINUS)) {

            std::string op = (currentToken->type == TokenType::PLUS) ? "+" : "-";
            auto node = std::make_shared<ASTNode>("Additive-Op", op);
            advance();

            node->addChild(left);
            node->addChild(parseTerm());

            left = node;
        }

        return left;
    }

    // term -> factor { mulop factor }
    std::shared_ptr<ASTNode> parseTerm() {
        auto left = parseFactor();

        while (currentToken != nullptr &&
               (currentToken->type == TokenType::MUL ||
                currentToken->type == TokenType::DIV)) {

            std::string op = (currentToken->type == TokenType::MUL) ? "*" : "/";
            auto node = std::make_shared<ASTNode>("Multiplicative-Op", op);
            advance();

            node->addChild(left);
            node->addChild(parseFactor());

            left = node;
        }

        return left;
    }

    // factor -> ( exp ) | number | identifier
    std::shared_ptr<ASTNode> parseFactor() {
        if (currentToken == nullptr) {
            throw ParserException("Unexpected end of input in factor");
        }

        if (currentToken->type == TokenType::OPENBRACKET) {
            advance();
            auto exp = parseExp();
            match(TokenType::CLOSEDBRACKET);
            return exp;
        }
        else if (currentToken->type == TokenType::NUMBER) {
            auto node = std::make_shared<ASTNode>("Number", currentToken->value);
            advance();
            return node;
        }
        else if (currentToken->type == TokenType::IDENTIFIER) {
            auto node = std::make_shared<ASTNode>("Identifier", currentToken->value);
            advance();
            return node;
        }
        else {
            throw ParserException("Invalid factor: '" + currentToken->value + "'");
        }
    }





public:
    TinyParser() : currentIndex(0), currentToken(nullptr) {}

    // Parse result structure
    struct ParseResult {
        std::shared_ptr<ASTNode> ast;
        std::vector<std::string> errors;
        bool success;
    };

    // Parse from scanner output file format: "value , TYPE"
    ParseResult parseFromFile(const std::string& content) {
        ParseResult result;
        result.success = false;

        try {
            tokens.clear();
            errors.clear();

            std::istringstream iss(content);
            std::string line;

            while (std::getline(iss, line)) {
                if (line.empty()) continue;

                // Parse format: "value , TYPE"
                size_t commaPos = line.find(" , ");
                if (commaPos == std::string::npos) {
                    commaPos = line.find(",");
                    if (commaPos == std::string::npos) continue;
                }

                std::string value = line.substr(0, commaPos);
                std::string typeStr = line.substr(commaPos + (line[commaPos + 1] == ',' ? 2 : 3));

                // Trim whitespace
                value.erase(0, value.find_first_not_of(" \t\r\n"));
                value.erase(value.find_last_not_of(" \t\r\n") + 1);
                typeStr.erase(0, typeStr.find_first_not_of(" \t\r\n"));
                typeStr.erase(typeStr.find_last_not_of(" \t\r\n") + 1);

                TokenType type = stringToTokenType(typeStr);
                if (type == TokenType::UNKNOWN && typeStr != "UNKNOWN") {
                    errors.push_back("Unknown token type: " + typeStr);
                    result.errors = errors;
                    return result;
                }

                tokens.emplace_back(value, type);
            }

            if (tokens.empty()) {
                errors.push_back("Error: Empty token list");
                result.errors = errors;
                return result;
            }

            currentIndex = 0;
            currentToken = &tokens[0];

            // Parse the program
            result.ast = parseProgram();

            // Check if all tokens were consumed
            if (currentToken != nullptr && currentToken->type != TokenType::END_OF_FILE) {
                errors.push_back("Unexpected tokens after end of program");
            }

            result.success = errors.empty();
            result.errors = errors;

        } catch (const ParserException& e) {
            errors.push_back(std::string("Parse error: ") + e.what());
            result.errors = errors;
            result.success = false;
        } catch (const std::exception& e) {
            errors.push_back(std::string("Unexpected error: ") + e.what());
            result.errors = errors;
            result.success = false;
        }

        return result;
    }

    // Parse from vector of tokens (for direct integration)
    ParseResult parse(const std::vector<Token>& tokenList) {
        ParseResult result;
        result.success = false;

        try {
            tokens = tokenList;
            errors.clear();

            if (tokens.empty()) {
                errors.push_back("Error: Empty token list");
                result.errors = errors;
                return result;
            }

            currentIndex = 0;
            currentToken = &tokens[0];

            // Parse the program
            result.ast = parseProgram();

            // Check if all tokens were consumed
            if (currentToken != nullptr && currentToken->type != TokenType::END_OF_FILE) {
                errors.push_back("Unexpected tokens after end of program");
            }

            result.success = errors.empty();
            result.errors = errors;

        } catch (const ParserException& e) {
            errors.push_back(std::string("Parse error: ") + e.what());
            result.errors = errors;
            result.success = false;
        } catch (const std::exception& e) {
            errors.push_back(std::string("Unexpected error: ") + e.what());
            result.errors = errors;
            result.success = false;
        }

        return result;
    }

    // Get syntax tree as string
    std::string getTreeString(const std::shared_ptr<ASTNode>& root) {
        if (root == nullptr) {
            return "Empty tree";
        }
        return root->toString();
    }

    // Get syntax tree as GraphViz DOT format
    std::string getTreeDot(const std::shared_ptr<ASTNode>& root) {
        if (root == nullptr) {
            return "digraph SyntaxTree { empty [label=\"Empty Tree\"]; }";
        }
        return root->toGraphViz();
    }

    // Generate PNG image of syntax tree (requires GraphViz installed)
    bool generateTreePNG(const std::shared_ptr<ASTNode>& root, const std::string& outputPath) {
        if (root == nullptr) {
            return false;
        }

        // Generate DOT file
        std::string dotPath = outputPath + ".dot";
        std::ofstream dotFile(dotPath);
        if (!dotFile) {
            return false;
        }

        dotFile << root->toGraphViz();
        dotFile.close();

        // Call GraphViz dot command to generate PNG
        std::string command = "dot -Tpng \"" + dotPath + "\" -o \"" + outputPath + "\"";
        int result = system(command.c_str());

        // Clean up DOT file (optional - comment out to keep it)
        // remove(dotPath.c_str());

        return result == 0;
    }
};

#endif // TINY_PARSER_H
