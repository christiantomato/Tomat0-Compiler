# Tomat0 Compiler README
Compiler for my own language, Tomat0. Tomat0 is a small but efficient language. It saves compilation time by combining parsing and semantics in one pass and walking the AST for code gen in one pass also. It uses clear, readable, syntax aswell as less characters than the average language to increase readability and typing speed. Tomat0's strengths would be computational based. 

## Compiling Tomat0 Source Code:

### Dependencies
- macOS with Apple Silicon (ARM64 based chip)
- Clang to assemble and link generated ARM64 assembly

### Instructions
1. Clone repo 
2. Write Tomat0 source code in main.tmt (see [wiki](https://github.com/christiantomato/Tomat0-Compiler/wiki) for documentation on syntax).
3. Compile the compiler using make:

```
make
```

4. Compile and run the source code using the executable

```
./tomat0.out
```

## Road Map

### Lexical Analysis
* [x] Lexer
      
Syntactic Analysis
  - [x] abstract syntax tree representation
  - [x] parse variable declarations and assignments
  - [x] parse print statements
  - [x] parse expressions, terms, and factors
  - [x] parse function declarations and calls
  - [x] parse boolean expressions, terms, comparisions
  - [x] parse conditional statements (if, while)

ARM64 Assembly Generation
  - [x] print integer values to terminal
  - [x] compute binary and unary operations
  - [x] generate entry point stack frame
  - [x] store, load, and use integer variables
  - [x] strings
  - [x] subroutines
  - [x] loops and conditionals

### TODO
- [x] optimize register allocation to allow for deeper nesting levels 
- [ ] deploy vscode extension for syntax highlighting
