// Legacy standalone scanner - outputs tokens to a file
// For the integrated compiler, use cli.cpp instead

#include "../include/TinyScanner.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

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
