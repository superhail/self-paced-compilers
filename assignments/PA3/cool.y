/*
*  cool.y
*              Parser definition for the COOL language.
*
*/
%{
  #include <iostream>
  #include "cool-tree.h"
  #include "stringtab.h"
  #include "utilities.h"
  
  extern char *curr_filename;
  
  
  /* Locations */
  #define YYLTYPE int              /* the type of locations */
  #define cool_yylloc curr_lineno  /* use the curr_lineno from the lexer
  for the location of tokens */
    
    extern int node_lineno;          /* set before constructing a tree node
    to whatever you want the line number
    for the tree node to be */
      
      
      #define YYLLOC_DEFAULT(Current, Rhs, N)         \
      Current = Rhs[1];                             \
      node_lineno = Current;
    
    
    #define SET_NODELOC(Current)  \
    node_lineno = Current;
    
    /* IMPORTANT NOTE ON LINE NUMBERS
    *********************************
    * The above definitions and macros cause every terminal in your grammar to 
    * have the line number supplied by the lexer. The only task you have to
    * implement for line numbers to work correctly, is to use SET_NODELOC()
    * before constructing any constructs from non-terminals in your grammar.
    * Example: Consider you are matching on the following very restrictive 
    * (fictional) construct that matches a plus between two integer constants. 
    * (SUCH A RULE SHOULD NOT BE  PART OF YOUR PARSER):
    
    plus_consts	: INT_CONST '+' INT_CONST 
    
    * where INT_CONST is a terminal for an integer constant. Now, a correct
    * action for this rule that attaches the correct line number to plus_const
    * would look like the following:
    
    plus_consts	: INT_CONST '+' INT_CONST 
    {
      // Set the line number of the current non-terminal:
      // ***********************************************
      // You can access the line numbers of the i'th item with @i, just
      // like you acess the value of the i'th exporession with $i.
      //
      // Here, we choose the line number of the last INT_CONST (@3) as the
      // line number of the resulting expression (@$). You are free to pick
      // any reasonable line as the line number of non-terminals. If you 
      // omit the statement @$=..., bison has default rules for deciding which 
      // line number to use. Check the manual for details if you are interested.
      @$ = @3;
      
      
      // Observe that we call SET_NODELOC(@3); this will set the global variable
      // node_lineno to @3. Since the constructor call "plus" uses the value of 
      // this global, the plus node will now have the correct line number.
      SET_NODELOC(@3);
      
      // construct the result node:
      $$ = plus(int_const($1), int_const($3));
    }
    
    */
    
    
    
    void yyerror(char *s);        /*  defined below; called for each parse error */
    extern int yylex();           /*  the entry point to the lexer  */
    
    /************************************************************************/
    /*                DONT CHANGE ANYTHING IN THIS SECTION                  */
    
    Program ast_root;	      /* the result of the parse  */
    Classes parse_results;        /* for use in semantic analysis */
    int omerrs = 0;               /* number of errors in lexing and parsing */
    %}
    
    /* A union of all the types that can be the result of parsing actions. */
    %union {
      Boolean boolean;
      Symbol symbol;
      Program program;
      Class_ class_;
      Classes classes;
      Feature feature;
      Features features;
      Formal formal;
      Formals formals;
      Case case_;
      Cases cases;
      Expression expression;
      Expressions plain_expressions;
      Expressions param_expressions;
      char *error_msg;
    }
    
    /* 
    Declare the terminals; a few have types for associated lexemes.
    The token ERROR is never used in the parser; thus, it is a parse
    error when the lexer returns it.
    
    The integer following token declaration is the numeric constant used
    to represent that token internally.  Typically, Bison generates these
    on its own, but we give explicit numbers to prevent version parity
    problems (bison 1.25 and earlier start at 258, later versions -- at
    257)
    */
    %token CLASS 258 ELSE 259 FI 260 IF 261 IN 262 
    %token INHERITS 263 LET 264 LOOP 265 POOL 266 THEN 267 WHILE 268
    %token CASE 269 ESAC 270 OF 271 DARROW 272 NEW 273 ISVOID 274
    %token <symbol>  STR_CONST 275 INT_CONST 276 
    %token <boolean> BOOL_CONST 277
    %token <symbol>  TYPEID 278 OBJECTID 279 
    %token ASSIGN 280 NOT 281 LE 282 ERROR 283
    
    /*  DON'T CHANGE ANYTHING ABOVE THIS LINE, OR YOUR PARSER WONT WORK       */
    /**************************************************************************/
    
    /* Complete the nonterminal list below, giving a type for the semantic
    value of each non terminal. (See section 3.6 in the bison 
    documentation for details). */
    
    /* Declare types for the grammar's non-terminals. */
    %type <program> program
    %type <classes> classes
    %type <class_> class

    
    /* You will want to change the following line. */
    %type <features> dummy_features
    %type <features> features
    %type <feature> feature
    %type <formals> dummy_formals
    %type <formals> formals
    %type <formal> formal
    %type <plain_expressions> plain_expressions 
    %type <param_expressions> param_expressions
    %type <expression> expression let_expr
    %type <case_> case_
    %type <cases> cases 
  
    /* Precedence declarations go here. */
    %right ASSIGN
    %left NOT
    %nonassoc '=' '<' LE
    %left '+' '-'
    %left '*' '/'
    %left ISVOID
    %left '~'
    %left '@'
    %left '.'
    
    %%
    /* 
    Save the root of the abstract syntax tree in a global variable.
    */
    program	
      : classes	{ @$ = @1; ast_root = program($1); }
      ;
    
    classes
      : class	';'		/* single class */
        { 
          $$ = single_Classes($1);
          parse_results = $$; 
        }
      | classes class	';'/* several classes */
        { 
          $$ = append_Classes($1,single_Classes($class)); 
          parse_results = $$; 
        }
      | classes error ';'
        {
          $$ = $1;
        }
      | error ';'
      ;
    
    /* If no parent is specified, the class inherits from the Object class. */
    class	/* single dummy class */
      : CLASS TYPEID '{' dummy_features '}' 
        { 
          $$ = class_($2,idtable.add_string("Object"),$dummy_features,
                      stringtable.add_string(curr_filename)); 
        } /* single class */
      | CLASS TYPEID[name] '{' features '}' 
        { 
          $$ = class_($name,idtable.add_string("Object"),$features,
                      stringtable.add_string(curr_filename));
        } /* inherited dummy class */
      | CLASS TYPEID[name] INHERITS TYPEID[parent] '{' dummy_features '}'  
        { 
          $$ = class_($name,$parent,$dummy_features,stringtable.add_string(curr_filename)); 
        } /* inherited class */
      | CLASS TYPEID[name] INHERITS TYPEID[parent] '{' features '}' 
        { 
          $$ = class_($name,$parent,$features,stringtable.add_string(curr_filename)); 
        }
      ;
    
    /* Feature list may be empty, but no empty features in list. */
    dummy_features:		/* empty */
      {  
        $$ = nil_Features(); 
      }
    ;

    features /* single feature */
      : feature ';'
        { 
          $$ = single_Features($feature); 
        }
      | features feature ';' /* several features */
        { 
          $$ = append_Features($1, single_Features($feature)); 
        }
      | features error ';'
        {
          $$ = $1;
        }
      | error ';'
        {}
    ;
    
    feature /* method with formal params */
      : OBJECTID[name] '(' formals ')' ':' TYPEID[ret_type] '{' expression '}'
        {
          $$ = method($name, $formals, $ret_type, $expression);  
        } /* method without formal params */
      | OBJECTID[name] '(' dummy_formals[formals] ')' ':' 
          TYPEID[ret_type] '{' expression '}'
        {
          $$ = method($name, $formals, $ret_type, $expression);
        } /* attribute with init expression */
      | OBJECTID[name] ':' TYPEID[type_decl] ASSIGN expression
        {
          $$ = attr($name, $type_decl, $expression); 
        }
      /* attribute without init expression */
      | OBJECTID[name] ':' TYPEID[type_decl]
        {
          $$ = attr($name, $type_decl, no_expr()); 
        }
    ;

    dummy_formals:
      {
        $$ = nil_Formals();
      }
    ;

    formals /* single formal */
      : formals ',' formal
      {
        $$ = append_Formals($1, single_Formals($formal));
      }
      | formal
      {
        $$ = single_Formals($formal);
      }
    ;
    
    formal
      : OBJECTID[name] ':' TYPEID[type_decl]
      {
        $$ = formal($name, $type_decl); 
      }
    ;
    cases
      : cases case_
      {
        $$ = append_Cases($1, single_Cases($case_));
      }
      | case_
      {
        $$ = single_Cases($case_);
      }
    ;

    case_
      : OBJECTID[name] ':' TYPEID[type_decl] DARROW expression ';'
      {
        $$ = branch($name, $type_decl, $expression);
      }
    ;

    param_expressions
      : param_expressions ',' expression
      {
        $$ = append_Expressions($1, single_Expressions($expression)); 
      }
      | expression
      {
        $$ = single_Expressions($expression);
      }
    ;

    plain_expressions
      : plain_expressions expression ';'
        {
          $$ = append_Expressions($1, single_Expressions($expression)); 
        }
      | expression ';'
        {
          $$ = single_Expressions($expression);
        }
      | plain_expressions error ';'
        {
          $$ = $1;
        }
    ;

    expression /* assign expression */
      : OBJECTID[name] ASSIGN expression
        {
          $$ = assign($name, $3);
        } 
      /* dispatch expression */
      | expression '.' OBJECTID[name] '(' param_expressions[actual] ')'
        {
          $$ = dispatch($1, $name, $actual);
        }
      /* dispatch expression with no params */
      | expression '.' OBJECTID[name] '(' ')'
        {
          $$ = dispatch($1, $name, nil_Expressions());
        }
      /* static dispatch */
      | expression '@' TYPEID[type_name] '.' OBJECTID[name] '(' param_expressions[actual] ')' 
        {
          $$ = static_dispatch($1, $type_name, $name, $actual);
        } 
      /* static dispatch with no params */
      | expression '@' TYPEID[type_name] '.' OBJECTID[name] '(' ')' 
        {
          $$ = static_dispatch($1, $type_name, $name, nil_Expressions());
        } 
      /* self dispatch */
      | OBJECTID[name] '(' param_expressions[actual] ')'
        {
          $$ = dispatch(object(idtable.add_string("self")), $name, $actual);
        }
      /* self dispatch */
      | OBJECTID[name] '(' ')'
        {
          $$ = dispatch(object(idtable.add_string("self")), $name, nil_Expressions());
        }
      /* cond expression */
      | IF expression[pred] THEN expression[then_expr] ELSE expression[else_expr] FI
        {
          $$ = cond($pred, $then_expr, $else_expr);
        }
      /* while expression */
      | WHILE expression[pred] LOOP expression[body] POOL
        {
          $$ = loop($pred, $body);
        }
      /* block expression */
      | '{' plain_expressions '}'
        {
          $$ = block($plain_expressions);
        }
      /* let expression */
      | LET let_expr
        {
          $$ = $2;
        }
      /* case expression */
      | CASE expression OF cases ESAC
        {
          $$ = typcase($2, $cases);
        }
      /* new expression */
      | NEW TYPEID[name]
        {
          $$ = new_($name);
        }
      | ISVOID expression
        {
          $$ = isvoid($2);
        }
      | expression[left] '+' expression[right]
        {
          $$ = plus($left, $right);
        }
      | expression[left] '-' expression[right]
        {
          $$ = sub($left, $right);
        }
      | expression[left] '*' expression[right]
        {
          $$ = mul($left, $right);
        }
      | expression[left] '/' expression[right]
        {
          $$ = divide($left, $right);
        }
      | '~' expression
        {
          $$ = neg($2);
        }
      | expression[left] '<' expression[right]
        {
          $$ = lt($left, $right);
        }
      | expression[left] LE expression[right]
        {
          $$ = leq($left, $right);
        }
      | expression[left] '=' expression[right]
        {
          $$ = eq($left, $right);
        }
      | NOT expression
        {
          $$ = comp($2);
        }
      | '(' expression ')'
        {
          $$ = $2;
        }
      | OBJECTID[name]
        {
          $$ = object($name);
        }
      | INT_CONST[token]
        {
          $$ = int_const($token);
        }
      | BOOL_CONST[val]
        {
          $$ = bool_const($val);
        }
      | STR_CONST[val]
        {
          $$ = string_const($val);
        }
      ;

      let_expr
        : OBJECTID[identifier] ':' TYPEID[type_decl] IN expression[body]
          {
            $$ = let($identifier,$type_decl,no_expr(),$body);
          }
        | OBJECTID[identifier] ':' TYPEID[type_decl] ASSIGN expression[init] IN expression[body]
          {
            $$ = let($identifier,$type_decl,$init,$body);
          }
        | OBJECTID[identifier] ':' TYPEID[type_decl] ',' let_expr[body]
          {
            $$ = let($identifier,$type_decl,no_expr(),$body);
          }
        | OBJECTID[identifier] ':' TYPEID[type_decl] ASSIGN expression[init] ',' let_expr[body]
          {
            $$ = let($identifier,$type_decl,$init,$body);
          }
        | error ',' let_expr[body]
          {
            $$ =  $body;
          }
      ;

    /* end of grammar */
    %%
    
    /* This function is called automatically when Bison detects a parse error. */
    void yyerror(char *s)
    {
      extern int curr_lineno;
      
      cerr << "\"" << curr_filename << "\", line " << curr_lineno << ": " \
      << s << " at or near ";
      print_cool_token(yychar);
      cerr << endl;
      omerrs++;
      
      if(omerrs>50) {fprintf(stdout, "More than 50 errors\n"); exit(1);}
    }
    
    
