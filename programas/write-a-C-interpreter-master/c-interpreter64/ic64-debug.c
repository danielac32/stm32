// this file is used for tutorial to build the compiler step by step

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

 #define SUPPORT_DEBUG

#ifdef SUPPORT_DEBUG
long long print_spec_text(long long *cur_text);
void print_text();
void print_data();
void print_symbol_table();
void print_source_code();
void debug_usage();
#define MAX_BREAK_POINT 10
long long break_points[MAX_BREAK_POINT];
long long last_bp;
long long meet_break_point(long long *pc);
#endif

long long token;                    // current token
long long token_val;                // value of current token (mainly for number)
char *src_begin;
char *src, *old_src;          // polong longer to source code string;
long long poolsize;                 // default size of text/data/stack
long long line;                     // line number
long long *begin_text;
long long *begin_stack;
char *begin_data;
long long *text,                    // text segment
    *old_text,                // for dump text segment
    *stack;                   // stack
char *data;                   // data segment
long long *pc, *bp, *sp, ax, cycle; // virtual machine registers
long long *current_id,              // current parsed ID
    *symbols;                 // symbol table
long long *idmain;                  // the `main` function

// instructions
enum { LEA ,IMM ,JMP ,CALL,JZ  ,JNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PUSH,
       OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,
       OPEN,READ,CLOS,PRTF,MALC,MSET,MCMP,EXIT };

#ifdef SUPPORT_DEBUG
const char *inst_str[] =
{
  "LEA","IMM","JMP","CALL","JZ ","JNZ","ENT","ADJ","LEV","LI ","LC ","SI ","SC ","PUSH"," OR ","XOR","AND","EQ ","NE ","LT ","GT ","LE ","GE ","SHL","SHR","ADD","SUB","MUL","DIV","MOD"," OPEN","READ","CLOS","PRTF","MALC","MSET","MCMP","EXIT"
};

#define INST_LEN (sizeof(inst_str)/sizeof(char*))

int inst_has_argu(long long inst)
{
  if ( (LEA <= inst) && (inst <= ADJ))
    return 1;
  else
    return 0;
}

const char* inst_2_str(long long inst)
{
  if ( (0 <= inst) && (inst < (long long)INST_LEN))
  {
    return inst_str[inst];
  }
  else
    return 0;
}
#endif

// tokens and classes (operators last and in precedence order)
enum {
  Num = 128, Fun, Sys, Glo, Loc, Id,
  Char, Else, Enum, If, Int, Return, Sizeof, While,
  Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
};

#ifdef SUPPORT_DEBUG
const char *class_str_array[] =
{
  "Num", "Fun", "Sys", "Glo", "Loc", "Id", "Char", "Else", "Enum", "If", "Int", "Return", "Sizeof", "While", "Assign", "Cond", "Lor", "Lan", "Or", "Xor", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Shl", "Shr", "Add", "Sub", "Mul", "Div", "Mod", "Inc", "Dec", "Brak"
};

const char *class_str(long long cls)
{
  long long class_index = cls-Num;

  if (0 <= class_index && class_index <= Brak)
    return class_str_array[class_index];
  else
    return "no such class";
}
#endif


// fields of identifier
enum {Token, Hash, Name, Type, Class, Value, BType, BClass, BValue, IdSize};

// types of variable/function
enum { CHAR, INT, PTR };

#ifdef SUPPORT_DEBUG
const char *type_str_array[] = {"CHAR", "INT", "PTR"};
const char * type_str(long long type)
{
  if (0 <= type && type <= PTR)
    return type_str_array[type];
  else
    return "no such type";
}
#endif

long long basetype;    // the type of a declaration, make it global for convenience
long long expr_type;   // the type of an expression

// function frame
//
// 0: arg 1
// 1: arg 2
// 2: arg 3
// 3: return address
// 4: old bp polong longer  <- index_of_bp
// 5: local var 1
// 6: local var 2
long long index_of_bp; // index of bp polong longer on stack

void next() {
    char *last_pos;
    long long hash;

    while ((token = *src) != 0) {
        ++src;

        // parse token here
        if (token == '\n') {
            ++line;
        }
        else if (token == '#') {
            // skip macro, because we will not support it
            while (*src != 0 && *src != '\n') {
                src++;
            }
        }
        else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_')) {

            // parse identifier
            last_pos = src - 1;
            hash = token;

            while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_')) {
                hash = hash * 147 + *src;
                src++;
            }

            // look for existing identifier, linear search
            current_id = symbols;
            while (current_id[Token]) {
                if (current_id[Hash] == hash && !memcmp((char *)current_id[Name], last_pos, src - last_pos)) {
                    //found one, return
                    token = current_id[Token];
                    return;
                }
                current_id = current_id + IdSize;
            }


            // store new ID
            current_id[Name] = (long long)last_pos;
            current_id[Hash] = hash;
            token = current_id[Token] = Id;
            return;
        }
        else if (token >= '0' && token <= '9') {
            // parse number, three kinds: dec(123) hex(0x123) oct(017)
            token_val = token - '0';
            if (token_val > 0) {
                // dec, starts with [1-9]
                while (*src >= '0' && *src <= '9') {
                    token_val = token_val*10 + *src++ - '0';
                }
            } else {
                // starts with 0
                if (*src == 'x' || *src == 'X') {
                    //hex
                    token = *++src;
                    while ((token >= '0' && token <= '9') || (token >= 'a' && token <= 'f') || (token >= 'A' && token <= 'F')) {
                        token_val = token_val * 16 + (token & 15) + (token >= 'A' ? 9 : 0);
                        token = *++src;
                    }
                } else {
                    // oct
                    while (*src >= '0' && *src <= '7') {
                        token_val = token_val*8 + *src++ - '0';
                    }
                }
            }

            token = Num;
            return;
        }
        else if (token == '"' || token == '\'') {
            // parse string literal, currently, the only supported escape
            // character is '\n', store the string literal long longo data.
            last_pos = data;
            while (*src != 0 && *src != token) {
                token_val = *src++;
                if (token_val == '\\') {
                    // escape character
                    token_val = *src++;
                    if (token_val == 'n') {
                        token_val = '\n';
                    }
                }

                if (token == '"') {
                    *data++ = token_val;
                }
            }

            src++;
            // if it is a single character, return Num token
            if (token == '"') {
                token_val = (long long)last_pos;
            } else {
                token = Num;
            }

            return;
        }
        else if (token == '/') {
            if (*src == '/') {
                // skip comments
                while (*src != 0 && *src != '\n') {
                    ++src;
                }
            } else {
                // divide operator
                token = Div;
                return;
            }
        }
        else if (token == '=') {
            // parse '==' and '='
            if (*src == '=') {
                src ++;
                token = Eq;
            } else {
                token = Assign;
            }
            return;
        }
        else if (token == '+') {
            // parse '+' and '++'
            if (*src == '+') {
                src ++;
                token = Inc;
            } else {
                token = Add;
            }
            return;
        }
        else if (token == '-') {
            // parse '-' and '--'
            if (*src == '-') {
                src ++;
                token = Dec;
            } else {
                token = Sub;
            }
            return;
        }
        else if (token == '!') {
            // parse '!='
            if (*src == '=') {
                src++;
                token = Ne;
            }
            return;
        }
        else if (token == '<') {
            // parse '<=', '<<' or '<'
            if (*src == '=') {
                src ++;
                token = Le;
            } else if (*src == '<') {
                src ++;
                token = Shl;
            } else {
                token = Lt;
            }
            return;
        }
        else if (token == '>') {
            // parse '>=', '>>' or '>'
            if (*src == '=') {
                src ++;
                token = Ge;
            } else if (*src == '>') {
                src ++;
                token = Shr;
            } else {
                token = Gt;
            }
            return;
        }
        else if (token == '|') {
            // parse '|' or '||'
            if (*src == '|') {
                src ++;
                token = Lor;
            } else {
                token = Or;
            }
            return;
        }
        else if (token == '&') {
            // parse '&' and '&&'
            if (*src == '&') {
                src ++;
                token = Lan;
            } else {
                token = And;
            }
            return;
        }
        else if (token == '^') {
            token = Xor;
            return;
        }
        else if (token == '%') {
            token = Mod;
            return;
        }
        else if (token == '*') {
            token = Mul;
            return;
        }
        else if (token == '[') {
            token = Brak;
            return;
        }
        else if (token == '?') {
            token = Cond;
            return;
        }
        else if (token == '~' || token == ';' || token == '{' || token == '}' || token == '(' || token == ')' || token == ']' || token == ',' || token == ':') {
            // directly return the character as token;
            return;
        }
    }
    return;
}

void match(long long tk) {
    if (token == tk) {
        next();
    } else {
    printf("%lld: expected token: %lld\n", line, tk);
        exit(-1);
    }
}

void expression(long long level) {
    // expressions have various format.
    // but majorly can be divided long longo two parts: unit and operator
    // for example `(char) *a[10] = (long long *) func(b > 0 ? 10 : 20);
    // `a[10]` is an unit while `*` is an operator.
    // `func(...)` in total is an unit.
    // so we should first parse those unit and unary operators
    // and then the binary ones
    //
    // also the expression can be in the following types:
    //
    // 1. unit_unary ::= unit | unit unary_op | unary_op unit
    // 2. expr ::= unit_unary (bin_op unit_unary ...)

    // unit_unary()
    long long *id;
    long long tmp;
    long long *addr;
    {
        if (!token) {
            printf("%lld: unexpected token EOF of expression\n", line);
            exit(-1);
        }
        if (token == Num) {
            match(Num);

            // emit code
            *++text = IMM;
            *++text = token_val;
            expr_type = INT;
        }
        else if (token == '"') {
            // continous string "abc" "abc"


            // emit code
            *++text = IMM;
            *++text = token_val;

            match('"');
            // store the rest strings
            while (token == '"') {
                match('"');
            }

            // append the end of string character '\0', all the data are default
            // to 0, so just move data one position forward.
            data = (char *)(((long long)data + sizeof(long long)) & (-sizeof(long long)));
            // *data = 0;
            expr_type = PTR;
        }
        else if (token == Sizeof) {
            // sizeof is actually an unary operator
            // now only `sizeof(long long)`, `sizeof(char)` and `sizeof(*...)` are
            // supported.
            match(Sizeof);
            match('(');
            expr_type = INT;

            if (token == Int) {
                match(Int);
            } else if (token == Char) {
                match(Char);
                expr_type = CHAR;
            }

            while (token == Mul) {
                match(Mul);
                expr_type = expr_type + PTR;
            }

            match(')');

            // emit code
            *++text = IMM;
            *++text = (expr_type == CHAR) ? sizeof(char) : sizeof(long long);

            expr_type = INT;
        }
        else if (token == Id) {
            // there are several type when occurs to Id
            // but this is unit, so it can only be
            // 1. function call
            // 2. Enum variable
            // 3. global/local variable
            match(Id);

            id = current_id;

            if (token == '(') {
                // function call
                match('(');

                // pass in arguments
                tmp = 0; // number of arguments
                while (token != ')') {
                    expression(Assign);
                    *++text = PUSH;
                    tmp ++;

                    if (token == ',') {
                        match(',');
                    }

                }
                match(')');

                // emit code
                if (id[Class] == Sys) {
                    // system functions
                    *++text = id[Value];
                }
                else if (id[Class] == Fun) {
                    // function call
                    *++text = CALL;
                    *++text = id[Value];
                }
                else {
                    printf("%lld: bad function call\n", line);
                    exit(-1);
                }

                // clean the stack for arguments
                if (tmp > 0) {
                    *++text = ADJ;
                    *++text = tmp;
                }
                expr_type = id[Type];
            }
            else if (id[Class] == Num) {
                // enum variable
                *++text = IMM;
                *++text = id[Value];
                expr_type = INT;
            }
            else {
                // variable
                if (id[Class] == Loc) {
                    *++text = LEA;
                    *++text = index_of_bp - id[Value];
                }
                else if (id[Class] == Glo) {
                    *++text = IMM;
                    *++text = id[Value];
                }
                else {
                    printf("%lld: undefined variable\n", line);
                    exit(-1);
                }

                // emit code, default behaviour is to load the value of the
                // address which is stored in `ax`
                expr_type = id[Type];
                *++text = (expr_type == Char) ? LC : LI;
            }
        }
        else if (token == '(') {
            // cast or parenthesis
            match('(');
            if (token == Int || token == Char) {
                tmp = (token == Char) ? CHAR : INT; // cast type
                match(token);
                while (token == Mul) {
                    match(Mul);
                    tmp = tmp + PTR;
                }

                match(')');

                expression(Inc); // cast has precedence as Inc(++)

                expr_type  = tmp;
            } else {
                // normal parenthesis
                expression(Assign);
                match(')');
            }
        }
        else if (token == Mul) {
            // dereference *<addr>
            match(Mul);
            expression(Inc); // dereference has the same precedence as Inc(++)

            if (expr_type >= PTR) {
                expr_type = expr_type - PTR;
            } else {
                printf("%lld: bad dereference\n", line);
                exit(-1);
            }

            *++text = (expr_type == CHAR) ? LC : LI;
        }
        else if (token == And) {
            // get the address of
            match(And);
            expression(Inc); // get the address of
            if (*text == LC || *text == LI) {
                text --;
            } else {
                printf("%lld: bad address of\n", line);
                exit(-1);
            }

            expr_type = expr_type + PTR;
        }
        else if (token == '!') {
            // not
            match('!');
            expression(Inc);

            // emit code, use <expr> == 0
            *++text = PUSH;
            *++text = IMM;
            *++text = 0;
            *++text = EQ;

            expr_type = INT;
        }
        else if (token == '~') {
            // bitwise not
            match('~');
            expression(Inc);

            // emit code, use <expr> XOR -1
            *++text = PUSH;
            *++text = IMM;
            *++text = -1;
            *++text = XOR;

            expr_type = INT;
        }
        else if (token == Add) {
            // +var, do nothing
            match(Add);
            expression(Inc);

            expr_type = INT;
        }
        else if (token == Sub) {
            // -var
            match(Sub);

            if (token == Num) {
                *++text = IMM;
                *++text = -token_val;
                match(Num);
            } else {

                *++text = IMM;
                *++text = -1;
                *++text = PUSH;
                expression(Inc);
                *++text = MUL;
            }

            expr_type = INT;
        }
        else if (token == Inc || token == Dec) {
            tmp = token;
            match(token);
            expression(Inc);
            if (*text == LC) {
                *text = PUSH;  // to duplicate the address
                *++text = LC;
            } else if (*text == LI) {
                *text = PUSH;
                *++text = LI;
            } else {
                printf("%lld: bad lvalue of pre-increment\n", line);
                exit(-1);
            }
            *++text = PUSH;
            *++text = IMM;
            *++text = (expr_type > PTR) ? sizeof(long long) : sizeof(char);
            *++text = (tmp == Inc) ? ADD : SUB;
            *++text = (expr_type == CHAR) ? SC : SI;
        }
        else {
            printf("%lld: bad expression\n", line);
            exit(-1);
        }
    }

    // binary operator and postfix operators.
    {
        while (token >= level) {
            // handle according to current operator's precedence
            tmp = expr_type;
            if (token == Assign) {
                // var = expr;
                match(Assign);
                if (*text == LC || *text == LI) {
                    *text = PUSH; // save the lvalue's polong longer
                } else {
                    printf("%lld: bad lvalue in assignment\n", line);
                    exit(-1);
                }
                expression(Assign);

                expr_type = tmp;
                *++text = (expr_type == CHAR) ? SC : SI;
            }
            else if (token == Cond) {
                // expr ? a : b;
                match(Cond);
                *++text = JZ;
                addr = ++text;
                expression(Assign);
                if (token == ':') {
                    match(':');
                } else {
                    printf("%lld: missing colon in conditional\n", line);
                    exit(-1);
                }
                *addr = (long long)(text + 3);
                *++text = JMP;
                addr = ++text;
                expression(Cond);
                *addr = (long long)(text + 1);
            }
            else if (token == Lor) {
                // logic or
                match(Lor);
                *++text = JNZ;
                addr = ++text;
                expression(Lan);
                *addr = (long long)(text + 1);
                expr_type = INT;
            }
            else if (token == Lan) {
                // logic and
                match(Lan);
                *++text = JZ;
                addr = ++text;
                expression(Or);
                *addr = (long long)(text + 1);
                expr_type = INT;
            }
            else if (token == Or) {
                // bitwise or
                match(Or);
                *++text = PUSH;
                expression(Xor);
                *++text = OR;
                expr_type = INT;
            }
            else if (token == Xor) {
                // bitwise xor
                match(Xor);
                *++text = PUSH;
                expression(And);
                *++text = XOR;
                expr_type = INT;
            }
            else if (token == And) {
                // bitwise and
                match(And);
                *++text = PUSH;
                expression(Eq);
                *++text = AND;
                expr_type = INT;
            }
            else if (token == Eq) {
                // equal ==
                match(Eq);
                *++text = PUSH;
                expression(Ne);
                *++text = EQ;
                expr_type = INT;
            }
            else if (token == Ne) {
                // not equal !=
                match(Ne);
                *++text = PUSH;
                expression(Lt);
                *++text = NE;
                expr_type = INT;
            }
            else if (token == Lt) {
                // less than
                match(Lt);
                *++text = PUSH;
                expression(Shl);
                *++text = LT;
                expr_type = INT;
            }
            else if (token == Gt) {
                // greater than
                match(Gt);
                *++text = PUSH;
                expression(Shl);
                *++text = GT;
                expr_type = INT;
            }
            else if (token == Le) {
                // less than or equal to
                match(Le);
                *++text = PUSH;
                expression(Shl);
                *++text = LE;
                expr_type = INT;
            }
            else if (token == Ge) {
                // greater than or equal to
                match(Ge);
                *++text = PUSH;
                expression(Shl);
                *++text = GE;
                expr_type = INT;
            }
            else if (token == Shl) {
                // shift left
                match(Shl);
                *++text = PUSH;
                expression(Add);
                *++text = SHL;
                expr_type = INT;
            }
            else if (token == Shr) {
                // shift right
                match(Shr);
                *++text = PUSH;
                expression(Add);
                *++text = SHR;
                expr_type = INT;
            }
            else if (token == Add) {
                // add
                match(Add);
                *++text = PUSH;
                expression(Mul);

                expr_type = tmp;
                if (expr_type > PTR) {
                    // polong longer type, and not `char *`
                    *++text = PUSH;
                    *++text = IMM;
                    *++text = sizeof(long long);
                    *++text = MUL;
                }
                *++text = ADD;
            }
            else if (token == Sub) {
                // sub
                match(Sub);
                *++text = PUSH;
                expression(Mul);
                if (tmp > PTR && tmp == expr_type) {
                    // polong longer subtraction
                    *++text = SUB;
                    *++text = PUSH;
                    *++text = IMM;
                    *++text = sizeof(long long);
                    *++text = DIV;
                    expr_type = INT;
                } else if (tmp > PTR) {
                    // polong longer movement
                    *++text = PUSH;
                    *++text = IMM;
                    *++text = sizeof(long long);
                    *++text = MUL;
                    *++text = SUB;
                    expr_type = tmp;
                } else {
                    // numeral subtraction
                    *++text = SUB;
                    expr_type = tmp;
                }
            }
            else if (token == Mul) {
                // multiply
                match(Mul);
                *++text = PUSH;
                expression(Inc);
                *++text = MUL;
                expr_type = tmp;
            }
            else if (token == Div) {
                // divide
                match(Div);
                *++text = PUSH;
                expression(Inc);
                *++text = DIV;
                expr_type = tmp;
            }
            else if (token == Mod) {
                // Modulo
                match(Mod);
                *++text = PUSH;
                expression(Inc);
                *++text = MOD;
                expr_type = tmp;
            }
            else if (token == Inc || token == Dec) {
                // postfix inc(++) and dec(--)
                // we will increase the value to the variable and decrease it
                // on `ax` to get its original value.
                if (*text == LI) {
                    *text = PUSH;
                    *++text = LI;
                }
                else if (*text == LC) {
                    *text = PUSH;
                    *++text = LC;
                }
                else {
                    printf("%lld: bad value in increment\n", line);
                    exit(-1);
                }

                *++text = PUSH;
                *++text = IMM;
                *++text = (expr_type > PTR) ? sizeof(long long) : sizeof(char);
                *++text = (token == Inc) ? ADD : SUB;
                *++text = (expr_type == CHAR) ? SC : SI;
                *++text = PUSH;
                *++text = IMM;
                *++text = (expr_type > PTR) ? sizeof(long long) : sizeof(char);
                *++text = (token == Inc) ? SUB : ADD;
                match(token);
            }
            else if (token == Brak) {
                // array access var[xx]
                match(Brak);
                *++text = PUSH;
                expression(Assign);
                match(']');

                if (tmp > PTR) {
                    // polong longer, `not char *`
                    *++text = PUSH;
                    *++text = IMM;
                    *++text = sizeof(long long);
                    *++text = MUL;
                }
                else if (tmp < PTR) {
                    printf("%lld: polong longer type expected\n", line);
                    exit(-1);
                }
                expr_type = tmp - PTR;
                *++text = ADD;
                *++text = (expr_type == CHAR) ? LC : LI;
            }
            else {
                printf("%lld: compiler error, token = %lld\n", line, token);
                exit(-1);
            }
        }
    }
}

void statement() {
    // there are 6 kinds of statements here:
    // 1. if (...) <statement> [else <statement>]
    // 2. while (...) <statement>
    // 3. { <statement> }
    // 4. return xxx;
    // 5. <empty statement>;
    // 6. expression; (expression end with semicolon)

    long long *a, *b; // bess for branch control

    if (token == If) {
        // if (...) <statement> [else <statement>]
        //
        //   if (...)           <cond>
        //                      JZ a
        //     <statement>      <statement>
        //   else:              JMP b
        // a:                 a:
        //     <statement>      <statement>
        // b:                 b:
        //
        //
        match(If);
        match('(');
        expression(Assign);  // parse condition
        match(')');

        // emit code for if
        *++text = JZ;
        b = ++text;

        statement();         // parse statement
        if (token == Else) { // parse else
            match(Else);

            // emit code for JMP B
            *b = (long long)(text + 3);
            *++text = JMP;
            b = ++text;

            statement();
        }

        *b = (long long)(text + 1);
    }
    else if (token == While) {
        //
        // a:                     a:
        //    while (<cond>)        <cond>
        //                          JZ b
        //     <statement>          <statement>
        //                          JMP a
        // b:                     b:
        match(While);

        a = text + 1;

        match('(');
        expression(Assign);
        match(')');

        *++text = JZ;
        b = ++text;

        statement();

        *++text = JMP;
        *++text = (long long)a;
        *b = (long long)(text + 1);
    }
    else if (token == '{') {
        // { <statement> ... }
        match('{');

        while (token != '}') {
            statement();
        }

        match('}');
    }
    else if (token == Return) {
        // return [expression];
        match(Return);

        if (token != ';') {
            expression(Assign);
        }

        match(';');

        // emit code for return
        *++text = LEV;
    }
    else if (token == ';') {
        // empty statement
        match(';');
    }
    else {
        // a = b; or function_call();
        expression(Assign);
        match(';');
    }
}

void function_parameter() {
    long long type;
    long long params;
    params = 0;
    while (token != ')') {
        // long long name, ...
        type = INT;
        if (token == Int) {
            match(Int);
        } else if (token == Char) {
            type = CHAR;
            match(Char);
        }

        // polong longer type
        while (token == Mul) {
            match(Mul);
            type = type + PTR;
        }

        // parameter name
        if (token != Id) {
            printf("%lld: bad parameter declaration\n", line);
            exit(-1);
        }
        if (current_id[Class] == Loc) {
            printf("%lld: duplicate parameter declaration\n", line);
            exit(-1);
        }

        match(Id);
        // store the local variable
        current_id[BClass] = current_id[Class]; current_id[Class]  = Loc;
        current_id[BType]  = current_id[Type];  current_id[Type]   = type;
        current_id[BValue] = current_id[Value]; current_id[Value]  = params++;   // index of current parameter

        if (token == ',') {
            match(',');
        }
    }
    index_of_bp = params+1;
}

void function_body() {
    // type func_name (...) {...}
    //                   -->|   |<--

    // ... {
    // 1. local declarations
    // 2. statements
    // }

    long long pos_local; // position of local variables on the stack.
    long long type;
    pos_local = index_of_bp;

    while (token == Int || token == Char) {
        // local variable declaration, just like global ones.
        basetype = (token == Int) ? INT : CHAR;
        match(token);

        while (token != ';') {
            type = basetype;
            while (token == Mul) {
                match(Mul);
                type = type + PTR;
            }

            if (token != Id) {
                // invalid declaration
                printf("%lld: bad local declaration\n", line);
                exit(-1);
            }
            if (current_id[Class] == Loc) {
                // identifier exists
                printf("%lld: duplicate local declaration\n", line);
                exit(-1);
            }
            match(Id);

            // store the local variable
            current_id[BClass] = current_id[Class]; current_id[Class]  = Loc;
            current_id[BType]  = current_id[Type];  current_id[Type]   = type;
            current_id[BValue] = current_id[Value]; current_id[Value]  = ++pos_local;   // index of current parameter

            if (token == ',') {
                match(',');
            }
        }
        match(';');
    }

    // save the stack size for local variables
    *++text = ENT;
    *++text = pos_local - index_of_bp;

    // statements
    while (token != '}') {
        statement();
    }

    // emit code for leaving the sub function
    *++text = LEV;
}

void function_declaration() {
    // type func_name (...) {...}
    //               | this part

    match('(');
    function_parameter();
    match(')');
    match('{');
    function_body();
    //match('}');

    // unwind local variable declarations for all local variables.
    current_id = symbols;
    while (current_id[Token]) {
        if (current_id[Class] == Loc) {
            current_id[Class] = current_id[BClass];
            current_id[Type]  = current_id[BType];
            current_id[Value] = current_id[BValue];
        }
        current_id = current_id + IdSize;
    }
}

void enum_declaration() {
    // parse enum [id] { a = 1, b = 3, ...}
    long long i;
    i = 0;
    while (token != '}') {
        if (token != Id) {
            printf("%lld: bad enum identifier %lld\n", line, token);
            exit(-1);
        }
        next();
        if (token == Assign) {
            // like {a=10}
            next();
            if (token != Num) {
                printf("%lld: bad enum initializer\n", line);
                exit(-1);
            }
            i = token_val;
            next();
        }

        current_id[Class] = Num;
        current_id[Type] = INT;
        current_id[Value] = i++;

        if (token == ',') {
            next();
        }
    }
}

void global_declaration() {
    // global_declaration ::= enum_decl | variable_decl | function_decl
    //
    // enum_decl ::= 'enum' [id] '{' id ['=' 'num'] {',' id ['=' 'num'} '}'
    //
    // variable_decl ::= type {'*'} id { ',' {'*'} id } ';'
    //
    // function_decl ::= type {'*'} id '(' parameter_decl ')' '{' body_decl '}'


    long long type; // tmp, actual type for variable
    //long long i; // tmp

    basetype = INT;

    // parse enum, this should be treated alone.
    if (token == Enum) {
        // enum [id] { a = 10, b = 20, ... }
        match(Enum);
        if (token != '{') {
            match(Id); // skip the [id] part
        }
        if (token == '{') {
            // parse the assign part
            match('{');
            enum_declaration();
            match('}');
        }

        match(';');
        return;
    }

    // parse type information
    if (token == Int) {
        match(Int);
    }
    else if (token == Char) {
        match(Char);
        basetype = CHAR;
    }

    // parse the comma seperated variable declaration.
    while (token != ';' && token != '}') {
        type = basetype;
        // parse polong longer type, note that there may exist `long long ****x;`
        while (token == Mul) {
            match(Mul);
            type = type + PTR;
        }

        if (token != Id) {
            // invalid declaration
            printf("%lld: bad global declaration\n", line);
            exit(-1);
        }
        if (current_id[Class]) {
            // identifier exists
            printf("%lld: duplicate global declaration\n", line);
            exit(-1);
        }
        match(Id);
        current_id[Type] = type;

        if (token == '(') {
            current_id[Class] = Fun;
            current_id[Value] = (long long)(text + 1); // the memory address of function
            function_declaration();
        } else {
            // variable declaration
            current_id[Class] = Glo; // global variable
            current_id[Value] = (long long)data; // assign memory address
            data = data + sizeof(long long);
            // *data = 0;
        }

        if (token == ',') {
            match(',');
        }
    }
    next();
}

void program() {
    // get next token
    next();
    while (token > 0) {
        global_declaration();
    }
}

void show_regs()
{
  printf("ax: %#llx(%lld)\n", ax, ax);
  printf("sp: %p\n", sp);
  printf("bp: %p\n", bp);
  printf("pc: %p\n", pc);
}

long long is_data(unsigned long long addr)
{
  if (((unsigned long long)begin_data <= addr) && (addr < (unsigned long long)data))
    return 1;
  else
    return 0;
}

long long is_text(unsigned long long addr)
{
  if (((unsigned long long)begin_text <= addr) && (addr < (unsigned long long)text))
    return 1;
  else
    return 0;
}

long long is_stack(unsigned long long addr)
{
  if (((unsigned long long)begin_stack <= addr) && (addr < (unsigned long long)begin_stack + poolsize))
    return 1;
  else
    return 0;
}

long long run_debug_func(char *cmd_line)
{
  switch (cmd_line[0])
  {
    case 'q':
    {
      exit(1);
    }
    case 'd':
    {
      print_data();
      break;
    }
    case 't':
    {
      print_symbol_table();
      break;
    }
    case 'h':
    case '?':
    {
      debug_usage();
      break;
    }
    case 'l':
    {
      print_source_code();
      break;
    }
    case 'e':
    {
      print_text();
      break;
    }
    case 'b': // break polong longer, ex: b 0xf756301c
    {
      unsigned long long addr;
      char cmd;

      sscanf(cmd_line, "%c %llx\n", &cmd, &addr);
      if (last_bp < MAX_BREAK_POINT)
      {
        break_points[last_bp] = addr;
        printf("set break_points[%lld]: %llx\n", last_bp, addr);
        ++last_bp;
      }
      else
      {
        printf("exceed %d break points\n", MAX_BREAK_POINT);
      }
      break;
    }
    case 'x': // show data segment content, ex: x 0xf756301c
    {
      char cmd_str[5];

      switch (cmd_line[1])
      {
        case 's':
        {
          break;
        }
        case 'c':
        {
          break;
        }
        case 'i':
        {
          break;
        }
        case 'x':
        {
          break;
        }
        default:
        {
          printf("xxx\n");
          return -1;
        }

      }
      unsigned long long addr;

      sscanf(cmd_line, "%s %llx\n", cmd_str, &addr);
      printf("cmd: %s, addr: %#llx\n", cmd_str, addr);
      #if 1
      is_text(addr);
      is_stack(addr);
      //if (((unsigned long long)begin_data <= addr) && (addr < (unsigned long long)data))
      if (is_data(addr) )
      {
        if (cmd_line[1] == 's')
          printf("data seg: %s\n", (char *)addr);
        if (cmd_line[1] == 'i')
          printf("data seg: %#llx\n", *(long long *)addr);
        if (cmd_line[1] == 'x')
          printf("data seg: %#llx\n", *(long long *)addr);
      }
      else if (is_text(addr) )
             printf("text seg: %#llx(%lld)\n", *((long long *)addr), *((long long *)addr));
           else if (is_stack(addr) )
                  printf("stack area: %#llx(%lld)\n", *((long long *)addr), *((long long *)addr));
                else
                {
                  printf("%llx is not in \ntext segment (%p ~ %p)\ndata segment (%p ~ %p)\nstack range (%p ~ %p)\n", addr, begin_text, text, begin_data, data, begin_stack, begin_stack + poolsize);

                }

      //printf("%#x(%lld)\n", *((long long *)addr));
      #endif
      break;
    }
    case 'r': // show registers
    {
      show_regs();
      break;
    }
    default:
    {
      break;
    }

  } // end switch (input_str[0])
  return 0;
}

void debug_usage()
{
  printf("command\n");
  printf("s: step\n");
  printf("q: quit\n");
  printf("c: continue\n");
  printf("r: print all register content\n");
  printf("d: print data\n");
  printf("e: print text\n");
  printf("l: print source code\n");
  printf("t: print symbol table\n");
  printf("xs address: print text/data segment stack area content as string\n");
  printf("xc address: not yet complete. print text/data segment stack area content as char\n");
  printf("xi address: print text/data segment stack area content as long long\n");
  printf("xx address: print text/data segment stack area content as hex\n");
  printf("b address: set breakpoint, max breakpoint is %d\n", MAX_BREAK_POINT);
}

#define INPUT_SIZE 20
int eval() {
  long long line=0;
  //int ch;
  //unsigned long long break_point = 0;
  long long continue_run = 0;
  long long meet_bp = 0;
  char input_str[INPUT_SIZE+1];
    long long op, *tmp;
    while (1)
    {
#ifdef SUPPORT_DEBUG
        print_spec_text(pc);
        //if (break_polong long == (unsigned long long)pc || continue_run == 0)

  debug_input:
        while(continue_run == 0)
        {
          printf("%lld ## debug> ", line++);
          fgets(input_str, INPUT_SIZE, stdin);

          if (input_str[0] == 's' || input_str[0] == '\n')
          {
            continue_run = 0;
            break;
          }

          if (input_str[0] == 'c')
          {
            continue_run = 1;
            break;
          }
          else
          {
            run_debug_func(input_str);
            continue_run = 0;
          }
        }

        if (meet_bp == 0 && meet_break_point(pc))
        {
          meet_bp = 1;
          // printf("meet break polong longer: %x\n", (unsigned long long)pc);
          continue_run = 0;
          goto debug_input;
        }
        else
        {
          meet_bp = 0;
        }

#endif
          op = *pc++; // get next operation code

        if (op == IMM)       {ax = *pc++;}                                     // load immediate value to ax
        else if (op == LC)   {ax = *(char *)ax;}                               // load character to ax, address in ax
        else if (op == LI)   {ax = *(long long *)ax;}                                // load long longeger to ax, address in ax
        else if (op == SC)   {ax = *(char *)*sp++ = ax;}                       // save character to address, value in ax, address on stack
        else if (op == SI)   {*(long long *)*sp++ = ax;}                             // save long longeger to address, value in ax, address on stack
        else if (op == PUSH) {*--sp = ax;}                                     // push the value of ax onto the stack
        else if (op == JMP)  {pc = (long long *)*pc;}                                // jump to the address
        else if (op == JZ)   {pc = ax ? pc + 1 : (long long *)*pc;}                   // jump if ax is zero
        else if (op == JNZ)  {pc = ax ? (long long *)*pc : pc + 1;}                   // jump if ax is zero
        else if (op == CALL) {*--sp = (long long)(pc+1); pc = (long long *)*pc;}           // call subroutine
        //else if (op == RET)  {pc = (long long *)*sp++;}                              // return from subroutine;
        else if (op == ENT)  {*--sp = (long long)bp; bp = sp; sp = sp - *pc++;}      // make new stack frame
        else if (op == ADJ)  {sp = sp + *pc++;}                                // add esp, <size>
        else if (op == LEV)  {sp = bp; bp = (long long *)*sp++; pc = (long long *)*sp++;}  // restore call frame and PC
        else if (op == ENT)  {*--sp = (long long)bp; bp = sp; sp = sp - *pc++;}      // make new stack frame
        else if (op == ADJ)  {sp = sp + *pc++;}                                // add esp, <size>
        else if (op == LEV)  {sp = bp; bp = (long long *)*sp++; pc = (long long *)*sp++;}  // restore call frame and PC
        else if (op == LEA)  {ax = (long long)(bp + *pc++);}                         // load address for arguments.

        else if (op == OR)  ax = *sp++ | ax;
        else if (op == XOR) ax = *sp++ ^ ax;
        else if (op == AND) ax = *sp++ & ax;
        else if (op == EQ)  ax = *sp++ == ax;
        else if (op == NE)  ax = *sp++ != ax;
        else if (op == LT)  ax = *sp++ < ax;
        else if (op == LE)  ax = *sp++ <= ax;
        else if (op == GT)  ax = *sp++ >  ax;
        else if (op == GE)  ax = *sp++ >= ax;
        else if (op == SHL) ax = *sp++ << ax;
        else if (op == SHR) ax = *sp++ >> ax;
        else if (op == ADD) ax = *sp++ + ax;
        else if (op == SUB) ax = *sp++ - ax;
        else if (op == MUL) ax = *sp++ * ax;
        else if (op == DIV) ax = *sp++ / ax;
        else if (op == MOD) ax = *sp++ % ax;


        else if (op == EXIT) { printf("exit(%lld)", *sp); return *sp;}
        else if (op == OPEN) { ax = open((char *)sp[1], sp[0]); }
        else if (op == CLOS) { ax = close(*sp);}
        else if (op == READ) { ax = read(sp[2], (char *)sp[1], *sp); }
        else if (op == PRTF) { tmp = sp + pc[1]; ax = printf((char *)tmp[-1], tmp[-2], tmp[-3], tmp[-4], tmp[-5], tmp[-6]); }
        else if (op == MALC) { ax = (long long)malloc(*sp);}
        else if (op == MSET) { ax = (long long)memset((char *)sp[2], sp[1], *sp);}
        else if (op == MCMP) { ax = memcmp((char *)sp[2], (char *)sp[1], *sp);}
        else {
            printf("unknown instruction:%lld\n", op);
            return -1;
        }
    }
    return 0;
}

#ifdef SUPPORT_DEBUG
long long meet_break_point(long long *pc)
{
  long long i;
  long long cur_addr = (long long)pc;
  //printf("cur_addr: %x\n", cur_addr);

  for (i = 0 ; i < last_bp ; ++i)
  {
    //printf("cur break_polong longs[%lld]: %x\n", i, break_polong longs[i]);
    if (cur_addr == break_points[i])
    {
      printf("break_points[%lld]: %llx\n", i, break_points[i]);
      return 1;
    }
  }
  return 0;
}

#ifdef SUPPORT_DEBUG
void print_symbol_table()
{
  long long *cur_id;
  cur_id = symbols;

  printf("symbol table:\n");

  while(cur_id[Token])
  {
    printf("cur_id[Name]: %s\n", (char*)cur_id[Name]);
    printf("cur_id[Hash]: %#llx\n", cur_id[Hash]);
    printf("cur_id[Type]: %s (%lld)\n", type_str(cur_id[Type]), cur_id[Type]);
    printf("cur_id[Class]: %s (%lld)\n", class_str(cur_id[Class]), cur_id[Class]);
    printf("cur_id[Value]: %#llx (%lld)\n", cur_id[Value], cur_id[Value]);
    cur_id = cur_id + IdSize;
  }
}
#endif

// return 1: has argument
// return 0: has no argument
long long print_spec_text(long long *cur_text)
{
  long long has_argu=0;

  const char* inst_str = inst_2_str(*cur_text);
  has_argu = inst_has_argu(*cur_text);

  if (inst_str)
    printf("addr %p ## %s", cur_text, inst_str);
  else
    printf("addr %p ## %llx", cur_text, *cur_text);

  if (has_argu)
  {
    ++cur_text;
    printf(" %#llx(%lld)", *cur_text, *cur_text);
  }
  printf("\n");
  return has_argu;
}

void print_source_code()
{
  printf("%s\n", src_begin);
}

void print_text()
{
  printf("text segment:\n");

  long long *cur_text = begin_text+1;
  //long long i;
  long long has_argu=0;
  while (cur_text != text)
  {
    has_argu = print_spec_text(cur_text);
    if (has_argu)
      cur_text += 2;
    else
      ++cur_text;
#if 0
    const char* inst_str = inst_2_str(*cur_text);
    has_argu = inst_has_argu(*cur_text);
    if (*cur_text == -1)
    {
      ++cur_text;
      continue;
    }
    if (inst_str)
      printf("addr %p ## %s\n", cur_text, inst_str);
    else
      printf("addr %p ## %x\n", cur_text, *cur_text);

    if (has_argu)
    {
      ++cur_text;
      printf("addr %p ## %#x (%lld)\n", cur_text, *cur_text, *cur_text);
      has_argu = 0;
    }
#endif
  }

}

void print_data()
{
  char *cur_data = (char*)begin_data;
  long long print_addr = 1;

  printf("data segment:\n");

  while(cur_data != data)
  {
    if (*cur_data != 0)
    {
      if (print_addr)
      {
        printf("%p: ", cur_data);
        print_addr = 0;
      }
      printf("%c", *cur_data);
    }
    else
    {
      printf("\n");
      print_addr = 1;
    }
    ++cur_data;
  }
}
#endif

int main(int argc, char **argv)
{

    long long i, fd;
    long long *tmp;

    argc--;
    argv++;

    poolsize = 256 * 1024; // arbitrary size
    line = 1;

    if ((fd = open(*argv, 0)) < 0) {
        printf("could not open(%s)\n", *argv);
        return -1;
    }

    // allocate memory for virtual machine
    if (!(text = old_text =(long long *) malloc(poolsize))) {
        printf("could not malloc(%lld) for text area\n", poolsize);
        return -1;
    }
    begin_text = text;

    if (!(data =(char*)malloc(poolsize))) {
        printf("could not malloc(%lld) for data area\n", poolsize);
        return -1;
    }

    begin_data = data;

    if (!(stack =(long long *) malloc(poolsize))) {
        printf("could not malloc(%lld) for stack area\n", poolsize);
        return -1;
    }

    begin_stack = stack;

    if (!(symbols = (long long* )malloc(poolsize))) {
        printf("could not malloc(%lld) for symbol table\n", poolsize);
        return -1;
    }

#ifdef SUPPORT_DEBUG
    printf("text: %p\n", text);
    printf("data: %p\n", data);
    printf("stack: %p\n", stack);
#endif

    memset(text, 0, poolsize);
    memset(data, 0, poolsize);
    memset(stack, 0, poolsize);
    memset(symbols, 0, poolsize);
    bp = sp = (long long *)((long long)stack + poolsize);
    ax = 0;

    src =(char*) "char else enum if int return sizeof while "
          "open read close printf malloc memset memcmp exit void main";

     // add keywords to symbol table
    i = Char;
    while (i <= While) {
        next();
        current_id[Token] = i++;
    }

    // add library to symbol table
    i = OPEN;
    while (i <= EXIT) {
        next();
        current_id[Class] = Sys;
        current_id[Type] = INT;
        current_id[Value] = i++;
    }

    next(); current_id[Token] = Char; // handle void type
    next(); idmain = current_id; // keep track of main


    // read the source file
    if ((fd = open(*argv, 0)) < 0) {
        printf("could not open(%s)\n", *argv);
        return -1;
    }

    if (!(src = old_src = (char*)malloc(poolsize))) {
        printf("could not malloc(%lld) for source area\n", poolsize);
        return -1;
    }
    // read the source file
    if ((i = read(fd, src, poolsize-1)) <= 0) {
        printf("read() returned %lld\n", i);
        return -1;
    }
    src[i] = 0; // add EOF character
    src_begin = src;
    close(fd);

    program();

    if (!(pc = (long long *)idmain[Value])) {
        printf("main() not defined\n");
        return -1;
    }

    // setup stack
    sp = (long long *)((long long)stack + poolsize);

#ifdef SUPPORT_DEBUG
    printf("sp: %llx\n", (long long)sp);
#endif

    *--sp = EXIT; // call exit if main returns
    *--sp = PUSH; tmp = sp;
    *--sp = argc;
    *--sp = (long long)argv;
    *--sp = (long long)tmp;

#ifdef SUPPORT_DEBUG
    //prlong long_text();
    //prlong long_data();
    // prlong long_symbol_table();
#endif
    return eval();
}
