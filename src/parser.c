/**
 * @file parser.c
 * @brief Implements the parsing algorithm using top down recursive descent.
 * 
 * Grammar for our LL(1) Recursive Descent Parser For Expressions, Terms, and Factors:
 * E -> T E'
 * E' -> (+ -) T E' | epsilon
 * T -> F T'
 * T' -> (* /) F T' | epsilon
 * F -> TOKEN_ID | Literal | (E) | -F
 * This grammar helps us follow the correct rules for order of operations in arithmetic expressions.
 * We recurse down until we hit a terminal (factors) and represent complicated expressions as many nested binary operations.
 */

#include "include/parser.h"
#include <stdlib.h>
#include <string.h>

//start at 0
int current_string_id = 0;

/*
 * Creates and initializes a parser with the tokens list. 
 */

Parser* init_parser(List* tokens) {
    //create a parser and allocate memory
    Parser* parser = malloc(sizeof(Parser));
    //initialize the root node to be a PROGRAM node
    parser->root = init_node(AST_GLOBAL);
    //use the list of tokens passed in
    parser->tokens = tokens;
    //set the initial token to starting position in array
    parser->current_token = parser->tokens->array[0];
    //initialize the index at zero
    parser->index = 0;
    return parser;
}

/**
 * @brief Helper function to count strings and give an id. 
 * 
 * @return the string id. 
 */

static int next_string_id() {
    return current_string_id++;
}

/**
 * @brief Advances the parser to the next token in the list. 
 * 
 * @param parser Pointer to the parser.
 */

static void parser_advance(Parser* parser) {
    parser->index++;
    //update current
    parser->current_token = parser->tokens->array[parser->index];
}

/**
 * @brief Ignores comments and blank lines, which do not require any logic to be parsed. 
 * 
 * @param parser Pointer to the parser. 
 */

static void parser_skip(Parser* parser) {
    while(true) {
        //skip rchevron comments
        if(parser->current_token->type == TOKEN_RCHEVRON) {
            //skip everything until we hit a newline (then check again for comments), or end token
            while(parser->current_token->type != TOKEN_NEWLINE && parser->current_token->type != TOKEN_EOF) {
                parser_advance(parser);
            }
        }
        //if we have a blank line
        else if(parser->current_token->type == TOKEN_NEWLINE) {
            //skip it
            parser_advance(parser);
        }
        else {
            //once all the unneccesary stuff is skipped, exit the loop and start parsing the line
            break;
        }
    }
}

//forward declare parse expression, as parse factor recursively calls it
static ASTNode* parse_expression(Parser* parser);

/**
 * @brief Parses a factor. 
 * 
 * Here you can find the base cases in the recursive algorithm, like integer or string literals. 
 * You can also find the operations of highest precedence, like parenthesis or negations which recurse back to an expression. 
 * 
 * @param parser Pointer to the parser. 
 * @return Pointer to the created node. 
 */

static ASTNode* parse_factor(Parser* parser) {
    //check for parenthesized expression
    if(parser->current_token->type == TOKEN_LPAREN) {
        //advance to the expression
        parser_advance(parser);

        //parse the expression inside parens
        ASTNode* expression_node = parse_expression(parser);

        //expect a closing parenthesis
        if(parser->current_token->type == TOKEN_RPAREN) {
            //advance past it
            parser_advance(parser);
            //return the expression node
            return expression_node;
        }
        else {
            //problem
            return NULL;
        }
    }

    //check for negation
    if(parser->current_token->type == TOKEN_HYPHEN) {
        //advance to the factor
        parser_advance(parser);
        //create the negation node
        ASTNode* negation_node = init_node(AST_NEGATION);
        //assign
        negation_node->specialization.negation.factor = parse_factor(parser);
        //return the node
        return negation_node;
    }

    //check for an identifier
    if(parser->current_token->type == TOKEN_ID) {
        //create the AST_VARIABLE
        ASTNode* variable_node = init_node(AST_VARIABLE);
        //set the name, remember in parsing value is not resolved yet so we leave that be
        variable_node->specialization.variable.variable_name = strdup(parser->current_token->value);
        //advance past
        parser_advance(parser);
        //return the node
        return variable_node;
    }

    //check for an integer literal
    if(parser->current_token->type == TOKEN_NUM) {
        //create the AST_INTEGER
        ASTNode* integer_node = init_node(AST_INTEGER);
        //assign
        integer_node->specialization.integer_literal.value = atoi(parser->current_token->value);
        //advance past the number
        parser_advance(parser);
        //return the node
        return integer_node;
    }

    //check for string literal
    if(parser->current_token->type == TOKEN_STRING) {
        //create an AST_STRING
        ASTNode* string_node = init_node(AST_STRING);
        //assign the string (make sure to duplicate)
        string_node->specialization.string_literal.value = strdup(parser->current_token->value);
        //assign the string id
        string_node->specialization.string_literal.string_id = next_string_id();
        //advance past
        parser_advance(parser);
        //return
        return string_node;
    }

    //nothing expected, problem
    return NULL;
}

/**
 * @brief Parses a term. 
 * 
 * Terms are made up of any sequence of factors combined by * or /
 * The algorithm used here is the exact same as the parse expression, but deals with the operators with the next highest precedence. 
 * From here we recurse into parsing factors. 
 * 
 * @param parser Pointer to the parser.
 * @return Pointer to the created node. 
 */

static ASTNode* parse_term(Parser* parser) {
    //parse the factor
    ASTNode* left = parse_factor(parser);

    //continue the loop as long as we are multiplying or dividing to build up our tree
    while(parser->current_token->type == TOKEN_ASTERISK || parser->current_token->type == TOKEN_FSLASH) {
        //get operand and parse right term (strdup!)
        char* operator = strdup(parser->current_token->value);
        parser_advance(parser);
        ASTNode* right = parse_factor(parser);

        //create the binary operation node
        ASTNode* binary_op_node = init_node(AST_BINARY_OPERATION);
        binary_op_node->specialization.binary_operation.left = left;
        binary_op_node->specialization.binary_operation.operator = operator;
        binary_op_node->specialization.binary_operation.right = right;

        //set left as the binary operation to keep building the nested binary operations
        left = binary_op_node;
    }

    //return once no more terms to be parsed
    return left;
}

/**
 * @brief Parses an expression. 
 * 
 * An Expression is made up of any sequence of terms combined by + or - (lowest precedence operators). 
 * The while loop will continue to build binary nodes as long as there are more additions or subtractions. 
 * this avoids left recursion and correctly builds the tree using left associativity. 
 * 
 * @param parser Pointer to the parser. 
 * @return Pointer to the created node. 
 */

static ASTNode* parse_expression(Parser* parser) {
    //parse the first term
    ASTNode* left = parse_term(parser);

    //continue the loop as long as we are doing addition or subtraction to build up our expression
    while(parser->current_token->type == TOKEN_PLUS || parser->current_token->type == TOKEN_HYPHEN) {
        //get the operator (strdup!)
        char* operator = strdup(parser->current_token->value);

        //move past
        parser_advance(parser);

        //parse the right term
        ASTNode* right = parse_term(parser);

        //create the binary operation node
        ASTNode* binary_op_node = init_node(AST_BINARY_OPERATION);
        binary_op_node->specialization.binary_operation.left = left;
        binary_op_node->specialization.binary_operation.operator = operator;
        binary_op_node->specialization.binary_operation.right = right;

        //now the left node can become the sub binary node we just created, and allows us to continue building an expression with the next term in the while loop (if any)
        left = binary_op_node;
    }

    //return the node that has built up the expression
    return left;
}

/**
 * @brief Parses a variable declaration instruction. 
 * 
 * Variable declarations in Tomat0 must include a data type and assignment. 
 * 
 * @param parser Pointer to the parser. 
 * @return Pointer to the variable declaration node. 
 */

static ASTNode* parse_variable_declaration(Parser* parser) {
    //create the node we will return
    ASTNode* var_dec_node = init_node(AST_VARIABLE_DECLARATION);

    //determine the data type
    if(parser->current_token->type == TOKEN_KEYWORD_INT) {
        var_dec_node->specialization.variable_declaration.data_type = TYPE_INTEGER;
    }
    else if(parser->current_token->type == TOKEN_KEYWORD_STRING) {
        var_dec_node->specialization.variable_declaration.data_type = TYPE_STRING;
    }
    else {
        //no type? problem  
        return NULL;
    }

    //advance to the next token
    parser_advance(parser);


    //expect a variable name
    if(parser->current_token->type == TOKEN_ID) {
        //duplicate the token value (fixing double free errors)
        var_dec_node->specialization.variable_declaration.variable_name = strdup(parser->current_token->value);
    }
    else {
        //problem in syntax
        return NULL;
    }

    //advance to next
    parser_advance(parser);

    //expect an equals for assignment
    if(parser->current_token->type == TOKEN_EQUALS) {
        parser_advance(parser);
    }
    else {
        //problem
        return NULL;
    }

    //recurse down and parse the assignment value
    var_dec_node->specialization.variable_declaration.assignment = parse_expression(parser);
    
    //return the node once finished
    return var_dec_node;
}

/**
 * @brief Parses a variable assignment instruction (for reassignemnts).
 * 
 * @param parser Pointer to the parser. 
 * @return Pointer to the created assignment node. 
 */

static ASTNode* parse_variable_assignment(Parser* parser) {
    //create the node we will return
    ASTNode* var_assignment_node = init_node(AST_VARIABLE_ASSIGNMENT);

    //get the variable name to reassign to 
    var_assignment_node->specialization.variable_assignment.variable_name = strdup(parser->current_token->value);

    //advance and expect equals
    parser_advance(parser);
    //expect an equals for assignment
    if(parser->current_token->type == TOKEN_EQUALS) {
        parser_advance(parser);
    }
    else {
        //problem
        return NULL;
    }
    //parse the expression of the reassignment
    var_assignment_node->specialization.variable_assignment.assignment = parse_expression(parser);

    return var_assignment_node;
}

/**
 * @brief Parses a print statement instruction. 
 * 
 * @param parser Pointer to the parser.
 * @return Pointer to the created print statement node. 
 */

static ASTNode* parse_print_statement(Parser* parser) {
    //create the node we will return
    ASTNode* print_node = init_node(AST_PRINT_STATEMENT);

    //move past the keyword (sout)
    parser_advance(parser);

    //expect an LPAREN
    if(parser->current_token->type == TOKEN_LPAREN) {
        parser_advance(parser);
    }
    else {
        //problem 
        return NULL;
    }

    //evaluate expression in parens
    print_node->specialization.print_statement.statement = parse_expression(parser);

    //ensure closing paren
    if(parser->current_token->type == TOKEN_RPAREN) {
        parser_advance(parser);
    }
    else {
        //bad syntax, problem
        return NULL; 
    }

    //done print statement
    return print_node;
}

/**
 * @brief Parses a singular line of source code.
 * 
 * Tomat0 enforces only 1 instruction per line, since we don't use delimeters.
 * Current available instructions: 
 * - variable declarations
 * - variable assignments
 * - print statements
 * 
 * @param parser Pointer to the parser. 
 * @return Pointer to the created node. 
 */

static ASTNode* parse_line(Parser* parser) {
    //go through each type of statement it could be 
    if(parser->current_token->type == TOKEN_KEYWORD_INT || parser->current_token->type == TOKEN_KEYWORD_STRING) {
       return parse_variable_declaration(parser);
    }
    else if(parser->current_token->type == TOKEN_ID) {
        return parse_variable_assignment(parser);
    }
    else if(parser->current_token->type == TOKEN_KEYWORD_SOUT) {
        return parse_print_statement(parser);
    }
    else {
        //invalid statement
        return NULL;
    }
}

/*
 * The main parsing function which will build the abstract syntax tree to the root program node.
 */

ASTNode* parser_parse(Parser* parser, SymbolTable* table) {
    //parse until we reach the end of file token
    while(parser->current_token->type != TOKEN_EOF) {
        //first skip everything that does not need to be parsed
        parser_skip(parser);
        //check again for end of file again after skipping
        if(parser->current_token->type == TOKEN_EOF) {
            //finished parsing
            break;
        }
        //parse a line and keep a pointer to the created node
        ASTNode* line_node = parse_line(parser);
        //if it is a declaration of some sort, add the new symbol
        if(line_node->type == AST_VARIABLE_DECLARATION) {
            add_to_table(table, strdup(line_node->specialization.variable_declaration.variable_name), data_type_as_str(line_node->specialization.variable_declaration.data_type));
        }
        //then add each parsed statement to the children of the program node 
        list_add(parser->root->children, line_node);
    }
    //return the root node 
    return parser->root;
}

/*
 * Frees the parser. 
 * Token freeing happens at this time, but not managed by parser. 
 * ASTNode root must live for the code generation pass. 
 */

int free_parser(Parser* parser) {
    //make sure parser isn't garbage
    if(parser == NULL) {
        return 1;
    }
    //free parser itself
    free(parser);
    return 0;
}
