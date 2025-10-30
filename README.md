# TINY Language Scanner

A lexical analyzer (scanner) for the TINY programming language that tokenizes source code and outputs a list of tokens with their types.

## Overview

This project implements a scanner that reads TINY language source code and breaks it down into tokens. Each token is identified by its value and type, which is the first phase of a typical compiler pipeline.

## Features

- **Lexical Analysis**: Tokenizes TINY language source code
- **Comment Handling**: Automatically skips comments enclosed in `{}`
- **Keyword Recognition**: Identifies reserved keywords (if, then, end, repeat, until, read, write)
- **Operator Support**: Recognizes assignment (`:=`) and comparison/arithmetic operators
- **UTF-8 BOM Handling**: Automatically removes UTF-8 BOM if present
- **Cross-platform Newlines**: Normalizes Windows CRLF to LF

## Supported Tokens

| Token Type | Example |
|------------|---------|
| SEMICOLON | `;` |
| IF | `if` |
| THEN | `then` |
| END | `end` |
| REPEAT | `repeat` |
| UNTIL | `until` |
| IDENTIFIER | `x`, `abc`, `xyz` |
| ASSIGN | `:=` |
| READ | `read` |
| WRITE | `write` |
| LESSTHAN | `<` |
| EQUAL | `=` |
| PLUS | `+` |
| MINUS | `-` |
| MULT | `*` |
| DIV | `/` |
| OPENBRACKET | `(` |
| CLOSEDBRACKET | `)` |
| NUMBER | `12`, `289` |

## Building

### Prerequisites
- C++ compiler (supporting C++11 or later)
- GCC/MinGW for Windows

### Compilation
```bash
g++ -std=c++11 -o tiny_scanner.exe tiny_scanner.cpp
```

## Usage
```bash
tiny_scanner.exe <input_file> <output_file>
```

### Example
```bash
tiny_scanner.exe input.tiny output.txt
```

### Input Example (input.tiny)
```
{ Sample TINY program }
x := 5;
if x < 10 then
    write x
end
```

### Output Example (output.txt)
```
x , Identifier
:= , assign
5 , number
; , SEMICOLON
if , IF
x , Identifier
< , LESSTHAN
10 , number
then , THEN
write , WRITE
x , Identifier
end , END
```

## Implementation Details

- **Scanner Class**: Manages the input stream and tokenization process
- **Token Structure**: Stores token value and type
- **Keywords Map**: Uses hash map for efficient keyword lookup
- **Case-Insensitive Keywords**: Keywords are recognized regardless of case
- **Whitespace Handling**: Automatically skips whitespace and comments

## Error Handling

- Reports error if input file cannot be opened
- Reports error if output file cannot be created
- Marks unrecognized characters as UNKNOWN tokens

## Project Structure
```
.
├── tiny_scanner.cpp     # Main source file
├── README.md           # This file
├── input.tiny          # Sample input file (optional)
└── output.txt          # Generated output file
```

## Notes

- The scanner is case-insensitive for keywords
- Comments must be enclosed in `{}` braces
- Assignment operator is `:=` (two characters)
- All tokens are written to the output file in the format: `value , type`

## License

This project is created for educational purposes as part of a compiler design course.

## Contributors

    -Hazem Ayman Farouk 2100314
    -Ahmed Nasr El-den 2100652
    -Kareem Mohamed Samy 2101542
    -Micheal Shohda Shohda 2100616

