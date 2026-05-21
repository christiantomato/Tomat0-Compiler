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

//keep track of defined string literals during parsing.
static int string_literals = 0;
//keeps track of assigned string labels.
static int string_labels = 0;
//keeps track of if statements, else, and while in program (just for debugging)
static int if_statements = 0;
static int while_loops = 0;

/*
 * Creates and initializes a parser with the tokens list. 
 */

Parser* init_parser(List* tokens) { 
    //create a parser and allocate memory
    Parser* parser = malloc(sizeof(Parser));

    //use the list of tokens passed in
    parser->tokens = tokens;
    //set the initial token to starting position in array
    parser->current_token = parser->tokens->array[0];
    //initialize the index at zero
    parser->index = 0;

    //initalize the root node and global scope
    Scope* global_scope = init_scope(NULL);
    global_scope->name = strdup("global");
    parser->root = init_node(AST_GLOBAL, global_scope);
    parser->current_scope = global_scope;

    //init scopes list and add global scope
    parser->scopes = init_list(5);
    list_add(parser->scopes, global_scope);
    //init strings list
    parser->strings = init_list(5);
    
    return parser;
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
 * @brief For parsing error handling
 * 
 * Skips through until the next line when there is a syntax error.
 * Helpful so we don't get infinte loops and makes debugging easy.
 * 
 * @param parser Pointer to the parser.
 */

static void parser_sync(Parser* parser) {
    printf("syncing parser...\n");
    while(parser->current_token->type != TOKEN_NEWLINE && parser->current_token->type != TOKEN_EOF) {
        //skip through
        parser_advance(parser);
    }

    //go to new line once done
    if(parser->current_token->type == TOKEN_NEWLINE) {
        parser_advance(parser);
    }
}

/**
 * @brief Looks ahead 1 token.
 * 
 * As we are using an LL(1) parser, we look ahead at most 1 token to make decisions.
 * 
 * @param parser Pointer to the parser.
 * @return Pointer to the token.
 */

static Token* parser_peek1(Parser* parser) {
    return parser->tokens->array[parser->index + 1];
}

/**
 * @brief Ignores comments and blank lines, which do not require any logic to be parsed. 
 * 
 * @param parser Pointer to the parser. 
 */

static void parser_skip(Parser* parser) {
    while(true) {
        //skip rchevron comments
        if(parser->current_token->type == TOKEN_TILDA) {
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

/**
 * @brief Resolves the type for a factor.
 * 
 * Used semantically to check types being passed into a function call.
 * 
 * @param parser Pointer to the parser.
 * @param factor Pointer to the node we are checking.
 * @return The expected data type.
 */

static DataType resolve_factor_type(Parser* parser, ASTNode* factor) {
    switch(factor->type) {
        case AST_FUNCTION_CALL: {
            //check the return type
            Symbol* function = lookup_symbol(parser->current_scope, factor->specialization.func_call.function_name);
            return function->data.func_data.return_type;
        }
        case AST_VARIABLE: {
            //check the data type
            Symbol* variable = lookup_symbol(parser->current_scope, factor->specialization.var.variable_name);
            if(variable->kind == SYMBOL_VARIABLE) return variable->data.var_data.type;
            else if(variable->kind == SYMBOL_PARAMETER) return variable->data.param_data.type;
            else if(variable->kind == SYMBOL_STRING) return TYPE_STRING;
        }
        case AST_NEGATION: 
            return factor->specialization.negation.type;
        case AST_NUMBER:
            return TYPE_INT;
        case AST_STRING:
            return TYPE_STRING;
        default: return -1;
    }
}

//forward declare parse boolean expression, as parse factor recursively calls it
static ASTNode* parse_boolean_expression(Parser* parser);
//forward declare parse_function_call, as it can be a factor
static ASTNode* parse_function_call(Parser* parser);

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

        //parse the expression inside parens, must go back all the way up in case boolean expression
        ASTNode* expression_node = parse_boolean_expression(parser);

        //expect a closing parenthesis
        if(parser->current_token->type == TOKEN_RPAREN) {
            //advance past it
            parser_advance(parser);
            //return the expression node
            return expression_node;
        }
        else {
            //problem
            printf("SYNTAX ERROR: Expected closing paren for expression.\n");
            parser_sync(parser);
            return NULL;
        }
    }

    //check for arithmetic negation
    if(parser->current_token->type == TOKEN_HYPHEN) {
        //advance to the factor
        parser_advance(parser);
        //create the negation node
        ASTNode* negation_node = init_node(AST_NEGATION, parser->current_scope);
        //assign
        negation_node->specialization.negation.operand = parse_factor(parser);
        negation_node->specialization.negation.type = TYPE_INT;
        //return the node
        return negation_node;
    }

    //check for a logical negation
    if(parser->current_token->type == TOKEN_KEYWORD_NOT) {
        //advance to the factor
        parser_advance(parser);
        //create the negation node
        ASTNode* negation_node = init_node(AST_NEGATION, parser->current_scope);
        //assign
        negation_node->specialization.negation.operand = parse_factor(parser);
        negation_node->specialization.negation.type = TYPE_BOOL;
        //return
        return negation_node;
    }

    //check for an identifier
    if(parser->current_token->type == TOKEN_ID) {
        //function call
        if(parser_peek1(parser)->type == TOKEN_LPAREN) {
            return parse_function_call(parser);
        }
        //variable call
        else {
            //SEMANTIC ANALYSIS: ensure the variable exists 
            if(lookup_symbol(parser->current_scope, parser->current_token->value) == NULL) {
                //variable doesn't exist
                printf("SEMANTIC ERROR: Variable %s has not been declared.\n", parser->current_token->value);
            }

            //create the AST_VARIABLE
            ASTNode* variable_node = init_node(AST_VARIABLE, parser->current_scope);
            //set the name, remember in parsing value is not resolved yet so we leave that be
            variable_node->specialization.var.variable_name = strdup(parser->current_token->value);
            //advance past
            parser_advance(parser);
            //return the node
            return variable_node;
        }
    }

    //check for a boolean literal
    if(parser->current_token->type == TOKEN_KEYWORD_TRUE || parser->current_token->type == TOKEN_KEYWORD_FALSE) {
        //represent as a number literal
        ASTNode* bool_value_node = init_node(AST_NUMBER, parser->current_scope);
        //true = 1, false = 0
        if(parser->current_token->type == TOKEN_KEYWORD_TRUE) bool_value_node->specialization.num.value = 1;
        else bool_value_node->specialization.num.value = 0;
        //advance past
        parser_advance(parser);
        return bool_value_node;
    }

    //check for an integer literal
    if(parser->current_token->type == TOKEN_NUM) {
        //create the AST_NUMBER
        ASTNode* integer_node = init_node(AST_NUMBER, parser->current_scope);
        //assign and convert char to int
        integer_node->specialization.num.value = atoi(parser->current_token->value);
        //advance past the number
        parser_advance(parser);
        //return the node
        return integer_node;
    }

    //check for string literal
    if(parser->current_token->type == TOKEN_STRING) {
        //create an AST_STRING
        ASTNode* string_node = init_node(AST_STRING, parser->current_scope);
        //assign the string (make sure to duplicate)
        string_node->specialization.string.value = strdup(parser->current_token->value);
        //assign id and increment
        string_node->specialization.string.id = string_literals++;
        //add to strings list
        list_add(parser->strings, strdup(parser->current_token->value));
        //advance past
        parser_advance(parser);
        //return
        return string_node;
    }

    //nothing expected, problem
    printf("SYNTAX ERROR: Didn't find factor.\n");
    parser_sync(parser);
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
        ASTNode* binary_op_node = init_node(AST_BINARY_OPERATION, parser->current_scope);
        binary_op_node->specialization.binary_op.left = left;
        binary_op_node->specialization.binary_op.operator = operator;
        binary_op_node->specialization.binary_op.right = right;

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
        //get the operator 
        char* operator = strdup(parser->current_token->value);

        //move past
        parser_advance(parser);

        //parse the right term
        ASTNode* right = parse_term(parser);

        //create the binary operation node
        ASTNode* binary_op_node = init_node(AST_BINARY_OPERATION, parser->current_scope);
        binary_op_node->specialization.binary_op.left = left;
        binary_op_node->specialization.binary_op.operator = operator;
        binary_op_node->specialization.binary_op.right = right;

        //set created sub tree as new left
        left = binary_op_node;
    }

    //return the node that has built up the expression
    return left;
}

/**
 * @brief Parses integer comparisons. 
 * 
 * Comparisons between integers can be between 2 arithmetic expressions, so that is the next level.
 * ex. x + 2 * 3 < y / 7
 * 
 * @param parser Pointer to the parser.
 * @return Pointer to the comparison node
 */

static ASTNode* parse_comparison(Parser* parser) {
    //next level is expressions
    ASTNode* left = parse_expression(parser);

    //if there is an equality symbol build the bin op
    if(parser->current_token->type == TOKEN_LCHEVRON || parser->current_token->type == TOKEN_RCHEVRON) {
        //get the operator
        char* operator = strdup(parser->current_token->value);

        //move past
        parser_advance(parser);

        //parse the right expression
        ASTNode* right = parse_expression(parser);

        //create the binary operation node
        ASTNode* binary_op_node = init_node(AST_BINARY_OPERATION, parser->current_scope);
        binary_op_node->specialization.binary_op.left = left;
        binary_op_node->specialization.binary_op.operator = operator;
        binary_op_node->specialization.binary_op.right = right;

        return binary_op_node;
    }

    //return left if there are no comparisons
    return left;
}   

/**
 * @brief Parses an equality check.
 * 
 * Equality must have lower precedence to build correct association: 
 * a equals b < c -> a equals (b < c)
 * Also equalities cannot be chained: 
 * a equals b equals c - invalid.
 * a equals b and b equals c - valid.
 * 
 * @param parser Pointer to the parser.
 * @return Pointer to the equality node.
 */

static ASTNode* parse_equality(Parser* parser) {
    //next level is comparisons
    ASTNode* left = parse_comparison(parser);

    //if there is an equality symbol build the bin op
    if(parser->current_token->type == TOKEN_KEYWORD_EQUALS) {
        //get the operator
        char* operator = strdup(parser->current_token->value);

        //move past
        parser_advance(parser);

        //parse the right comparison
        ASTNode* right = parse_comparison(parser);

        //create the binary operation node
        ASTNode* binary_op_node = init_node(AST_BINARY_OPERATION, parser->current_scope);
        binary_op_node->specialization.binary_op.left = left;
        binary_op_node->specialization.binary_op.operator = operator;
        binary_op_node->specialization.binary_op.right = right;

        return binary_op_node;
    }

    //return left if there are no equality comparisons
    return left;
}

/**
 * @brief Parses a boolean term (AND).
 * 
 * Same algorithm as boolean expression, 
 * but with the higher precedence connectve AND (which acts like multiplication).
 * 
 * @param parser Pointer to the parser.
 * @return Pointer to the term node. 
 */

static ASTNode* parse_boolean_term(Parser* parser) {
    //next level are equality comparisons
    ASTNode* left = parse_equality(parser);

    //continue the loop as long as we have ands to build up our boolean expression
    while(parser->current_token->type == TOKEN_KEYWORD_AND) {
        //get the operator
        char* operator = strdup(parser->current_token->value);

        //move past
        parser_advance(parser);

        //parse the right equality
        ASTNode* right = parse_equality(parser);

        //create the binary operation node
        ASTNode* binary_op_node = init_node(AST_BINARY_OPERATION, parser->current_scope);
        binary_op_node->specialization.binary_op.left = left;
        binary_op_node->specialization.binary_op.operator = operator;
        binary_op_node->specialization.binary_op.right = right;

        //set created sub tree as new left
        left = binary_op_node;
    }

    //return when no more terms to parse
    return left;
}

/**
 * @brief Parses a boolean expression (OR).
 * 
 * In boolean algebras, the OR connective acts similarly to addition. It has lower precedence than AND, and so our
 * boolean expressions are made up of sequence of terms connected by ORs. This is also our top level entry for parsing expressions
 * since booleans can contain arithmetic expressions inside comparisons. 
 * 
 * @param parser Pointer to the parser.
 * @return Pointer to the expression node. 
 */

static ASTNode* parse_boolean_expression(Parser* parser) {
    //parse the first boolean term
    ASTNode* left = parse_boolean_term(parser);

    //continue the loop as long as we have ors to build up our boolean expression
    while(parser->current_token->type == TOKEN_KEYWORD_OR) {
        //get the operator (strdup!)
        char* operator = strdup(parser->current_token->value);

        //move past
        parser_advance(parser);

        //parse the right term
        ASTNode* right = parse_boolean_term(parser);

        //create the binary operation node
        ASTNode* binary_op_node = init_node(AST_BINARY_OPERATION, parser->current_scope);
        binary_op_node->specialization.binary_op.left = left;
        binary_op_node->specialization.binary_op.operator = operator;
        binary_op_node->specialization.binary_op.right = right;

        //now the left node can become the sub binary node we just created, and allows us to continue building an expression with the next term in the while loop (if any)
        left = binary_op_node;
    }

    //return the node that has built up the expression
    return left;
}

//forward declare parse block
static ASTNode* parse_block(Parser* parser);

/**
 * @brief Parses an if statement.
 * 
 * @param parser Point to the parser
 * @return Pointer to the if statement node. 
 */

static ASTNode* parse_if_statement(Parser* parser) {
    //the node we will return 
    ASTNode* if_node = init_node(AST_IF_STATEMENT, parser->current_scope);

    //move past keyword 
    parser_advance(parser);

    //expect l paren
    if(parser->current_token->type == TOKEN_LPAREN) parser_advance(parser);
    else {
        //problem
        printf("SYNTAX ERROR: Expected opening bracket on if statement.\n");
        parser_sync(parser);
        return NULL;
    }

    //parse condition
    if_node->specialization.if_statement.condition = parse_boolean_expression(parser);

    //advance past closing paren
    if(parser->current_token->type == TOKEN_RPAREN) parser_advance(parser);
    else {
        //problem
        printf("SYNTAX ERROR: Expected closing bracket on if statement.\n");
        parser_sync(parser);
        return NULL;
    }

    //enter a new scope
    Scope* if_scope = enter_scope(parser->current_scope);
    char if_scope_name[8];
    int this_if = if_statements++;
    snprintf(if_scope_name, sizeof(if_scope_name), "if%d", this_if);
    if_scope->name = strdup(if_scope_name);
    list_add(parser->scopes, if_scope);

    //if and else scopes don't create a new stack frame. Save the old offset and pass it through to the scopes.
    int offset_entering_ifelse = parser->current_scope->current_offset;
    //set the offset for the if scope
    if_scope->current_offset = offset_entering_ifelse;

    //set current scope as if scope
    parser->current_scope = if_scope;

    //parse if block
    if_node->specialization.if_statement.code_block = parse_block(parser);

    //exit scope
    parser->current_scope = exit_scope(parser->current_scope);

    //check for an ELSE!
    parser_skip(parser);
    if(parser->current_token->type == TOKEN_KEYWORD_ELSE) {
        //advance past
        parser_advance(parser);

        //enter the else scope
        Scope* else_scope = enter_scope(parser->current_scope);
        char else_scope_name[16];
        snprintf(else_scope_name, sizeof(else_scope_name), "elseforif%d", this_if);
        else_scope->name = strdup(else_scope_name);
        list_add(parser->scopes, else_scope);
        parser->current_scope = else_scope;

        //set the offset to what it was when entering
        else_scope->current_offset = offset_entering_ifelse;

        //parse else block
        if_node->specialization.if_statement.else_block = parse_block(parser);

        //exit
        parser->current_scope = exit_scope(parser->current_scope);

        //update the offset for the scope. Use whichever scope used more stack space
        if(if_scope->current_offset > else_scope->current_offset) parser->current_scope->current_offset = if_scope->current_offset;
        else parser->current_scope->current_offset = else_scope->current_offset;
    }
    else {
        //update the offset for parent scope
        parser->current_scope->current_offset = if_scope->current_offset;
    }
    return if_node;
}

/**
 * @brief Parses a while loop.
 * 
 * @param parser Pointer to the parser.
 * @return Pointer to the while loop node.
 */

static ASTNode* parse_while_loop(Parser* parser) {
    //the node we will return 
    ASTNode* while_node = init_node(AST_WHILE_LOOP, parser->current_scope);

    //move past keyword 
    parser_advance(parser);

    //expect l paren
    if(parser->current_token->type == TOKEN_LPAREN) parser_advance(parser);
    else {
        //problem
        printf("SYNTAX ERROR: Expected opening bracket on while loop.\n");
        parser_sync(parser);
        return NULL;
    }

    //parse condition
    while_node->specialization.while_loop.condition = parse_boolean_expression(parser);

    //advance past closing paren
    if(parser->current_token->type == TOKEN_RPAREN) parser_advance(parser);
    else {
        //problem
        printf("SYNTAX ERROR: Expected closing bracket on while loop.\n");
        parser_sync(parser);
        return NULL;
    }

    //enter a new scope
    Scope* while_scope = enter_scope(parser->current_scope);
    char while_scope_name[8];
    int this_while = while_loops++;
    snprintf(while_scope_name, sizeof(while_scope_name), "while%d", this_while);
    while_scope->name = strdup(while_scope_name);
    list_add(parser->scopes, while_scope);

    //no new stack frame for while loops. Save the old offset and pass it through to the scope.
    int offset_entering_while = parser->current_scope->current_offset;
    //set the offset for the if scope
    while_scope->current_offset = offset_entering_while;

    //set current scope to while scope
    parser->current_scope = while_scope;

    //parse while block
    while_node->specialization.while_loop.code_block = parse_block(parser);

    //exit scope, set updated offset, and return
    parser->current_scope = exit_scope(parser->current_scope);
    parser->current_scope->current_offset = while_scope->current_offset;
    return while_node;
}

/**
 * @brief Parses a parameter during a function declaration.
 * 
 * @param parser Pointer to the parser.
 * @return Pointer to the parameter node.
 */

static ASTNode* parse_parameter(Parser* parser) {
    //the parameter node we will return
    ASTNode* param_node = init_node(AST_PARAMETER, parser->current_scope);

    //expect a data type
    if(parser->current_token->type == TOKEN_KEYWORD_INT) {
        param_node->specialization.param.parameter_type = TYPE_INT;
    }
    else if(parser->current_token->type == TOKEN_KEYWORD_STRING) {
        param_node->specialization.param.parameter_type = TYPE_STRING;
    }
    else {
        printf("SYNTAX ERROR: Unrecognized parameter type.\n");
        parser_sync(parser);
        return NULL;
    }

    //move past
    parser_advance(parser);

    //expect parameter name
    if(parser->current_token->type == TOKEN_ID) {
        param_node->specialization.param.parameter_name = strdup(parser->current_token->value);
    }
    else {
        printf("SYNTAX ERROR: Expected parameter name.\n");
        parser_sync(parser);
        return NULL;
    }

    //advance past id
    parser_advance(parser);

    return param_node;
}

/**
 * @brief Parses a function call.
 * 
 * @param parser Pointer to the parser.
 * @return Pointer to the function call node.
 */

static ASTNode* parse_function_call(Parser* parser) {
    //SEMANTIC ANALYSIS: ensure the function exists
    Symbol* func_symbol = lookup_symbol(parser->current_scope, parser->current_token->value);
    if(func_symbol == NULL) {
        //function doesn't exist
        printf("SEMANTIC ERROR: Function has not been declared.\n");
        return NULL;
    }

    //create the AST_FUNCTION_CALL
    ASTNode* func_call_node = init_node(AST_FUNCTION_CALL, parser->current_scope);

    //get function name
    func_call_node->specialization.func_call.function_name = strdup(parser->current_token->value);

    //advance past name and expect l paren
    parser_advance(parser);

    if(parser->current_token->type == TOKEN_LPAREN) parser_advance(parser);
    else {
        //problem
        printf("SYNTAX ERROR: Expected opening bracket on function call.\n");
        parser_sync(parser);
        return NULL;
    }

    //keep an index for func params
    int param_index = 0;

    //parse parameters
    while(parser->current_token->type != TOKEN_RPAREN) {
        //parse the parameter, which we will restrict to being a factor for simplicity
        ASTNode* param_node;
        param_node = parse_factor(parser);

        //check if types match
        DataType param_type = resolve_factor_type(parser, param_node);
        Symbol* param_symbol = (Symbol*) func_symbol->data.func_data.parameters->array[param_index];
        DataType expected_type = param_symbol->data.param_data.type;

        if(param_type != expected_type) {
            printf("SEMANTIC ERROR: Parameter type does not match function definition.\n");
            return NULL;
        }

        //skip past comma if there is one
        if(parser->current_token->type == TOKEN_COMMA) {
            parser_advance(parser);
        }

        //add it to the param list
        list_add(func_call_node->specialization.func_call.parameter_inputs, param_node);

        //increase param counter
        param_index++;
    }

    //check if it was the correct amount of arguments
    if(param_index != func_symbol->data.func_data.parameters->num_items) {
        printf("SEMANTIC ERROR: Incorrect amount of arguments.\n");
    }

    //advance past closing
    parser_advance(parser);

    return func_call_node;
}

/**
 * @brief Parses a function declaration. 
 * 
 * @param parser Pointer to the parser.
 * @return Pointer to the function declaration node.
 */

static ASTNode* parse_function_declaration(Parser* parser) {
    //create the node we will return
    ASTNode* func_dec_node = init_node(AST_FUNCTION_DECLARATION, parser->current_scope);

    //advance past the keyword func
    parser_advance(parser);

    //expect the function name
    if(parser->current_token->type != TOKEN_ID) {
        printf("SYNTAX ERROR: Expected function identifier.\n");
        parser_sync(parser);
        return NULL;
    }

    //get the name, add to node, and initalize a symbol
    char* function_name = parser->current_token->value;
    func_dec_node->specialization.func_dec.function_name = strdup(function_name);
    Symbol* func_symbol = init_symbol(function_name, SYMBOL_FUNCTION);

    //add symbol to the current scope (which is global)
    add_symbol(parser->current_scope, func_symbol);

    //enter the function scope
    Scope* func_scope = enter_scope(parser->current_scope);
    func_scope->name = strdup(function_name);
    list_add(parser->scopes, func_scope);
    parser->current_scope = func_scope;

    //advance past name
    parser_advance(parser);

    //expect opening bracket
    if(parser->current_token->type == TOKEN_LPAREN) {
        //advance past bracket
        parser_advance(parser);

        //keep index for func params and assinging register x0-x7
        int param_index = 0;

        //parse parameters until we reach the closing bracket
        while(parser->current_token->type != TOKEN_RPAREN) {
            //parse a parameter
            ASTNode* param_node = parse_parameter(parser);
            //add to the list
            list_add(func_dec_node->specialization.func_dec.parameters, param_node);

            //create the symbol and add it to the scope
            Symbol* param_symbol = init_symbol(param_node->specialization.param.parameter_name, SYMBOL_PARAMETER);
            //set type, register, and offset
            param_symbol->data.param_data.type = param_node->specialization.param.parameter_type;
            param_symbol->data.param_data.reg = param_index++;
            parser->current_scope->current_offset -= 8;
            param_symbol->data.param_data.offset = parser->current_scope->current_offset;
            
            //add to symbol table
            add_symbol(parser->current_scope, param_symbol);
            //add the symbol to the func symbol param list
            list_add(func_symbol->data.func_data.parameters, param_symbol);

            //check for comma for multiple params
            if(parser->current_token->type == TOKEN_COMMA) {
                //skip it
                parser_advance(parser);
            }
        }

        //advance past the closing
        parser_advance(parser);

        //expect the keyword yields, if none specified then void
        if(parser->current_token->type == TOKEN_KEYWORD_YIELDS) {
            parser_advance(parser);
            //expect return type
            if(parser->current_token->type == TOKEN_KEYWORD_INT) {
                func_dec_node->specialization.func_dec.return_type = TYPE_INT;
                func_symbol->data.func_data.return_type = TYPE_INT;
            }
            else if(parser->current_token->type == TOKEN_KEYWORD_STRING) {
                func_dec_node->specialization.func_dec.return_type = TYPE_STRING;
                func_symbol->data.func_data.return_type = TYPE_STRING;
            }
            else if(parser->current_token->type == TOKEN_KEYWORD_BOOL) {
                func_dec_node->specialization.func_dec.return_type = TYPE_BOOL;
                func_symbol->data.func_data.return_type = TYPE_BOOL;
            }
            else {
                printf("SYNTAX ERROR: Unrecognized return type for %s.\n", function_name);
                parser_sync(parser);
                return NULL;
            }

            //advance past type
            parser_advance(parser);
        }
        else {
            //must be void
            func_dec_node->specialization.func_dec.return_type = TYPE_VOID;
            func_symbol->data.func_data.return_type = TYPE_VOID;
        }

        //finally parse the code block and return
        func_dec_node->specialization.func_dec.code_block = parse_block(parser);

        //return to old scope (global)
        parser->current_scope = exit_scope(parser->current_scope);
        return func_dec_node;
    }
    else {
        printf("SYNTAX ERROR: Expected left parentheses.\n");
        parser_sync(parser);
        return NULL;
    }
}

/**
 * @brief Parses a variable declaration instruction. 
 * 
 * Variable declarations in Tomat0 must include a data type and assignment. 
 * Variables are also semantically checked here and added to the scopes symbol table, adding offset when needed.
 * 
 * @param parser Pointer to the parser. 
 * @return Pointer to the variable declaration node. 
 */

static ASTNode* parse_variable_declaration(Parser* parser) {
    //create the node we will return
    ASTNode* var_dec_node = init_node(AST_VARIABLE_DECLARATION, parser->current_scope);

    //determine the data type and save it for the symbol
    DataType type; 
    if(parser->current_token->type == TOKEN_KEYWORD_INT) {
        var_dec_node->specialization.var_dec.data_type = TYPE_INT;
        type = TYPE_INT;
    }
    else if(parser->current_token->type == TOKEN_KEYWORD_BOOL) {
        var_dec_node->specialization.var_dec.data_type = TYPE_BOOL;
        type = TYPE_BOOL;
    }
    else if(parser->current_token->type == TOKEN_KEYWORD_STRING) {
        var_dec_node->specialization.var_dec.data_type = TYPE_STRING;
        type = TYPE_STRING;
    }
    else {
        //no type? problem  
        printf("SYNTAX ERROR: Invalid type.\n");
        parser_sync(parser);
        return NULL;
    }

    //advance to the next token
    parser_advance(parser);

    //expect the variable name
    if(parser->current_token->type == TOKEN_ID) {
        //SEMANTICS: ensure the variable doesn't already exist in this scope
        if(lookup_symbol_in_scope(parser->current_scope, parser->current_token->value) != NULL) {
            //alrady exists
            printf("SEMANTIC ERROR: Variable already exists in this scope.\n");
        }
        //duplicate the token value (fixing double free errors)
        var_dec_node->specialization.var_dec.variable_name = strdup(parser->current_token->value);
    }
    else {
        //problem in variable name
        printf("SYNTAX ERROR: Invalid variable name.\n");
        parser_sync(parser);
        return NULL;
    }

    //create and initalize the symbol
    Symbol* symbol;
    if(type == TYPE_STRING) symbol = init_symbol(parser->current_token->value, SYMBOL_STRING);
    else {
        symbol = init_symbol(parser->current_token->value, SYMBOL_VARIABLE);
        symbol->data.var_data.type = type;
    }

    //generate label for strings
    if(symbol->kind == SYMBOL_STRING) {
        char label[8];
        snprintf(label, sizeof(label), "str%d", string_labels++);
        symbol->data.str_data.label = strdup(label);
    }
    //assign offsets for variables
    else {
        parser->current_scope->current_offset -= 8;
        symbol->data.var_data.offset = parser->current_scope->current_offset;
    }
    //add it to the symbol table of the current scope
    add_symbol(parser->current_scope, symbol);

    //advance to next
    parser_advance(parser);

    //expect an equals for assignment
    if(parser->current_token->type == TOKEN_EQUALS) {
        parser_advance(parser);
    }
    else {
        //problem
        printf("SYNTAX ERROR: Expected equals.\n");
        parser_sync(parser);
        return NULL;
    }

    //recurse down from the top level of hierarchy and determine assignment
    var_dec_node->specialization.var_dec.assignment = parse_boolean_expression(parser);
    
    //return the node once finished
    return var_dec_node;
}

/**
 * @brief Parses a variable assignment instruction (for already created variables).
 * 
 * @param parser Pointer to the parser. 
 * @return Pointer to the created assignment node. 
 */

static ASTNode* parse_variable_assignment(Parser* parser) {
    //SEMANTICS: ensure the variable already exists
    if(lookup_symbol(parser->current_scope, parser->current_token->value) == NULL) {
        //variable doesn't exist
        printf("SEMANTIC ERROR: Variable %s has not been declared.\n", parser->current_token->value);
    }

    //create the node we will return
    ASTNode* var_assignment_node = init_node(AST_VARIABLE_ASSIGNMENT, parser->current_scope);

    //get the variable name to reassign to 
    var_assignment_node->specialization.var_assign.variable_name = strdup(parser->current_token->value);

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
    var_assignment_node->specialization.var_assign.assignment = parse_expression(parser);

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
    ASTNode* print_node = init_node(AST_PRINT_STATEMENT, parser->current_scope);

    //move past the keyword (sout)
    parser_advance(parser);

    //expect an LPAREN
    if(parser->current_token->type == TOKEN_LPAREN) parser_advance(parser);
    else {
        //problem 
        printf("SYNTAX ERROR: Expected LPAREN.\n");
        parser_sync(parser);
        return NULL;
    }

    //evaluate FACTOR in parens (expressions not allowed)
    print_node->specialization.print_statement.operand = parse_factor(parser);
    //resolve type for print statement
    print_node->specialization.print_statement.type = resolve_factor_type(parser, print_node->specialization.print_statement.operand);

    //ensure closing paren
    if(parser->current_token->type == TOKEN_RPAREN) {
        parser_advance(parser);
    }
    else {
        //bad syntax, problem
        printf("SYNTAX ERROR: Expected RPAREN.\n");
        parser_sync(parser);
        return NULL; 
    }

    //done print statement
    return print_node;
}

/**
 * @brief Parses a return statement instruction. 
 * 
 * @param parser Pointer to the parser.
 * @return Pointer to the created return statement node. 
 */

static ASTNode* parse_return_statement(Parser* parser) {
    //create the node we will return
    ASTNode* return_node = init_node(AST_RETURN_STATEMENT, parser->current_scope);

    //move past the keyword (harvest)
    parser_advance(parser);

    //evaluate FACTOR to return
    return_node->specialization.return_statement.operand = parse_factor(parser);
    return_node->specialization.return_statement.type = resolve_factor_type(parser, return_node->specialization.return_statement.operand);

    //done return statement
    return return_node;
}

/**
 * @brief Parses a singular line of source code. 
 * 
 * Determines the instruction by looking at first token, peeking if needed.
 * Tomat0 enforces only 1 instruction per line, since we don't use delimeters.
 * 
 * @param parser Pointer to the parser. 
 * @return Pointer to the created node. 
 */

static ASTNode* parse_line(Parser* parser) {
    switch(parser->current_token->type) {
        case TOKEN_KEYWORD_FUNC: return parse_function_declaration(parser);
        case TOKEN_KEYWORD_INT: return parse_variable_declaration(parser); 
        case TOKEN_KEYWORD_STRING: return parse_variable_declaration(parser);
        case TOKEN_KEYWORD_BOOL: return parse_variable_declaration(parser);
        case TOKEN_KEYWORD_IF: return parse_if_statement(parser);
        case TOKEN_KEYWORD_WHILE: return parse_while_loop(parser);
        case TOKEN_KEYWORD_PRINT: return parse_print_statement(parser);
        case TOKEN_ID: 
            //check for function calls
            if(parser_peek1(parser)->type == TOKEN_LPAREN) return parse_function_call(parser); 
            else return parse_variable_assignment(parser); 
        case TOKEN_KEYWORD_HARVEST: 
            //check if it is a return statement or runtime end
            if(parser_peek1(parser)->type == TOKEN_KEYWORD_TOMATO) {
                //advance past the ending statement
                parser_advance(parser); parser_advance(parser);
                return init_node(AST_RUNTIME_END, parser->current_scope);
            }
            else {
                return parse_return_statement(parser);
            }
        case TOKEN_KEYWORD_SPROUT: {
            //advance past 
            parser_advance(parser);

            //enter the main scope
            Scope* main_scope = enter_scope(parser->current_scope);
            main_scope->name = strdup("main");
            list_add(parser->scopes, main_scope);
            parser->current_scope = main_scope;
            //parse the main function block statement
            ASTNode* main = parse_block(parser);
            parser->current_scope = exit_scope(parser->current_scope);
            //set this block node to the entry point
            main->type = AST_ENTRY_POINT;
            //at this point, parsing should be done...
            return main;
        }
        default: 
            printf("SYNTAX ERROR: Not a valid instruction.\n");
            parser_sync(parser);
            return NULL;
    }
}

/** 
 * @brief Parses a block of code.
 * 
 * For function declarations, ifs, whiles, and main entry point.
 * 
 * @param parser Pointer to the parser. 
 * @return Pointer to the block node.
 */

static ASTNode* parse_block(Parser* parser) {
    //create the node we will return
    ASTNode* block_node = init_node(AST_BLOCK, parser->current_scope);

    //expect the opening curly brace 
    if(parser->current_token->type == TOKEN_LCURLY) {
        parser_advance(parser);
    }
    else {
        printf("SYNTAX ERROR: Expected Left Curly Brace.\n");
        parser_sync(parser);
    }

    //parse lines until we have reached the right curly
    while(parser->current_token->type != TOKEN_RCURLY) {
        //skip any whitespace
        parser_skip(parser);
        //parse a line
        ASTNode* line_node = parse_line(parser);
        //add it to the statements
        list_add(block_node->specialization.block.statements, line_node);
        //skip any white space
        parser_skip(parser);
    }

    //advance past the rcurly to finish
    parser_advance(parser);

    return block_node;
}

/*
 * The main parsing function which will build the abstract syntax tree to the root program node.
 */

Program* parser_parse(Parser* parser) {
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
        //add to the global scope statements
        list_add(parser->root->specialization.block.statements, line_node);
    }
    //return the program struct
    return init_program(parser->root, parser->scopes, parser->strings);
}

/*
 * Frees the parser. 
 * Token freeing happens at this time, but not managed by parser. 
 * ASTNode root must live for the code generation pass. 
 */

void free_parser(Parser* parser) {
    //free parser itself, everything else it built must live on. 
    free(parser);
}
