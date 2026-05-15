# Tomat0 Compiler README
Compiler for my own language, Tomat0. Tomat0 focuses on increasing readibility and removing unecessary syntax. It compiles quickly and efficiently as it uses a single pass for parsing and semantics, and a single pass for arm64 assembly generation. 

## Compiling Tomat0 Source Code:

### Dependencies
You must have an ARM64 CPU. x86-64 assembly is not generated. 

### Instructions
1. Clone eepo 
2. Write Tomat0 source code in main.tmt (see notes/syntax.txt or wiki for syntax rules).
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
  - [ ] parse conditional statements (if, while)

ARM64 Assembly Generation
  - [x] print integer values to terminal
  - [x] compute binary and unary operations
  - [x] generate entry point stack frame
  - [x] store, load, and use integer variables
  - [ ] strings
  - [ ] subroutines
  - [ ] loops and conditionals

### TODO
- generate asm for strings
- generate asm for subroutines

