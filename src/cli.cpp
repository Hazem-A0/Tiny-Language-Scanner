#include "../include/TinyScanner.h"
#include "../include/TinyParser.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void printUsage(const char* progName) {
    cout << "TINY Language Compiler - Scanner & Parser\n";
    cout << "==========================================\n\n";
    cout << "Usage:\n";
    cout << "  " << progName << " <input_file> [output_file]\n\n";
    cout << "Arguments:\n";
    cout << "  <input_file>    TINY source code file to compile\n";
    cout << "  [output_file]   Optional: Output file for syntax tree (default: <input>.tree)\n";
    cout << "\nExample:\n";
    cout << "  " << progName << " input.txt\n";
    cout << "  " << progName << " input.txt output.tree\n";
}

string readSourceFile(const string& filename) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file) {
        throw runtime_error("Cannot open file: " + filename);
    }

    string src((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // Remove UTF-8 BOM if present
    if (src.size() >= 3 &&
        (unsigned char)src[0] == 0xEF &&
        (unsigned char)src[1] == 0xBB &&
        (unsigned char)src[2] == 0xBF) {
        src.erase(0, 3);
    }

    // Normalize newlines (Windows CRLF -> LF)
    replace(src.begin(), src.end(), '\r', '\n');

    return src;
}

void compileFile(const string& inputFile, const string& outputFile) {
    cout << "\n=== TINY Language Compiler ===\n";
    cout << "Input: " << inputFile << "\n";
    cout << "Output: " << outputFile << "\n\n";

    try {
        // Step 1: Read source code
        cout << "Step 1: Reading source file...\n";
        string sourceCode = readSourceFile(inputFile);
        cout << "--- Source Code ---\n";
        cout << sourceCode << "\n";

        // Step 2: Scan (Lexical Analysis)
        cout << "\nStep 2: Scanning (Lexical Analysis)...\n";
        Scanner scanner(sourceCode);
        vector<Token> tokens = scanner.scanAll();

        cout << "--- Tokens Generated ---\n";
        for (const auto& tok : tokens) {
            cout << tok.value << " , " << tokenTypeToString(tok.type) << "\n";
        }
        cout << "Total tokens: " << tokens.size() << "\n";

        // Step 3: Parse (Syntax Analysis)
        cout << "\nStep 3: Parsing (Syntax Analysis)...\n";
        TinyParser parser;
        auto result = parser.parse(tokens);

        // Step 4: Report Results
        cout << "\n===========================================\n";
        if (result.success) {
            cout << "SUCCESS: Input ACCEPTED by TINY language\n";
            cout << "===========================================\n\n";

            cout << "--- Syntax Tree ---\n";
            string treeStr = parser.getTreeString(result.ast);
            cout << treeStr;

            // Save text tree to output file
            ofstream outFile(outputFile);
            if (outFile) {
                outFile << "TINY Language Parse Tree\n";
                outFile << "========================\n\n";
                outFile << "Input File: " << inputFile << "\n\n";
                outFile << "Result: ACCEPTED\n\n";
                outFile << "Syntax Tree:\n";
                outFile << treeStr;
                outFile.close();
                cout << "\n Syntax tree saved to: " << outputFile << "\n";
            }

            // Generate PNG visualization
            cout << "\nStep 4: Generating visual tree (PNG)...\n";
            string pngFile = outputFile.substr(0, outputFile.find_last_of('.')) + ".png";
            if (parser.generateTreePNG(result.ast, pngFile)) {
                cout << "  Visual tree saved to: " << pngFile << "\n";
                cout << "  (DOT source saved to: " << pngFile << ".dot)\n";
            } else {
                cout << "  Warning: Could not generate PNG image.\n";


                // Save DOT file anyway
                string dotFile = pngFile + ".dot";
                ofstream dotOut(dotFile);
                if (dotOut) {
                    dotOut << parser.getTreeDot(result.ast);
                    dotOut.close();
                    cout << "  DOT file saved to: " << dotFile << "\n";
                    cout << "  You can manually convert it: dot -Tpng " << dotFile << " -o " << pngFile << "\n";
                }
            }
        } else {
            cout << "✗ FAILED: Input REJECTED by TINY language\n";
            cout << "===========================================\n\n";

            cout << "--- Parse Errors ---\n";
            for (const auto& error : result.errors) {
                cout << "  ERROR: " << error << "\n";
            }

            // Save errors to output file
            ofstream outFile(outputFile);
            if (outFile) {
                outFile << "TINY Language Parse Result\n";
                outFile << "==========================\n\n";
                outFile << "Input File: " << inputFile << "\n\n";
                outFile << "Result: REJECTED\n\n";
                outFile << "Errors:\n";
                for (const auto& error : result.errors) {
                    outFile << "  " << error << "\n";
                }
                outFile.close();
                cout << "\n✓ Errors saved to: " << outputFile << "\n";
            }
        }

    } catch (const exception& e) {
        cerr << "\nFATAL ERROR: " << e.what() << "\n";
        throw;
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    string inputFile = argv[1];
    string outputFile;
    
    // Determine output file
    if (argc >= 3) {
        outputFile = argv[2];
    } else {
        // Default output file: input filename + .tree extension
        outputFile = inputFile + ".tree";
    }

    try {
        compileFile(inputFile, outputFile);
        return 0;
    } catch (const exception& e) {
        return 1;
    }
}
