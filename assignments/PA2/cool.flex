/*
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>
#include <stdio.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;
int comment_match();

/*
 *  Add Your own definitions here
 */
%}
%x Class

/*
 * Define names for regular expressions here.
 */
letter          [a-zA-Z]
uppercase         [A-Z]
lowercase       [a-z]
digit           [0-9]
delim           [ \n\f\r\t\v]
ws              {delim}+
class           (c|C)(l|L)(a|A)(s|S)(s|S)
else            (e|E)(l|L)(s|S)(e|E)
fi              (f|F)(i|I)
if              (i|I)(f|F)
in              (i|I)(n|N)
inherits        (i|I)(n|N)(h|H)(e|E)(r|R)(i|I)(t|T)(s|S)
let             (l|L)(e|E)(t|T)
loop            (l|L)(o|O)(o|O)(p|P)
pool            (p|P)(o|O)(o|O)(l|L)
then            (t|T)(h|H)(e|E)(n|N)
while           (w|W)(h|H)(i|I)(l|L)(e|E)
case            (c|C)(a|A)(s|S)(e|E)
esac            (e|E)(s|S)(a|A)(c|C)
of              (o|O)(f|F)
darrow          =>
new             (n|N)(e|E)(w|W)
isvoid          (i|I)(s|S)(v|V)(o|O)(i|I)(d|D)
str_const       \"
int_const       {digit}+
bool_const      (t(r|R)(u|U)(e|E))|(f(a|A)(l|L)(s|S)(e|E))
/*bool_const      true|false */
typeid          {uppercase}({digit}|{letter}|_)*
objectid        {lowercase}({digit}|{letter}|_)*
assign          "<-"
not             (n|N)(o|O)(t|T)  
le              "<="
unnested        --
nested          "(*"


%%

{ws}            {
  for(unsigned int i = 0; i < yyleng; i++) {
    if(yytext[i] == '\n') {
      curr_lineno++;
    }
  } 
}
{unnested}  {
  char c;
  while((c = yyinput()) != '\n' && c!=EOF); 
  if(c == '\n') {
    curr_lineno++;
  }
}
{nested}  {
  int result = comment_match();
  if(result != -1) { /**return error**/
    return result;
  }
}
{str_const}              { /**str_const**/
  int curr_strlen = 0;
  int null_flag = 0;
  char c;
  while((c = yyinput()) != '\n' && c != '\"' && c != EOF) {
    if(curr_strlen < MAX_STR_CONST) {
      curr_strlen++;
    }
    if(c == '\\') {
      if((c = yyinput()) == 'b') {
        string_buf[curr_strlen - 1] = '\b';
      } else if(c == 't') {
        string_buf[curr_strlen - 1] = '\t';
      } else if(c == 'n') {
        string_buf[curr_strlen - 1] = '\n';
      } else if(c == 'f') {
        string_buf[curr_strlen - 1] = '\f';
      } else if(c == '\0') { // eat up all other valid/invalid character in quote
        null_flag = 1; 
      } else if(c == EOF) {
        cool_yylval.error_msg = "EOF in string constant";
        return (ERROR);
      } else {
        if(c == '\n') {
          curr_lineno++;
        }
        string_buf[curr_strlen - 1] = c;
      } 
    } else if(c == '\0') {
      null_flag = 1;
    } else {
      string_buf[curr_strlen - 1] = c;
    } 
  } 
  if(c == '\n') {
    curr_lineno++;
    cool_yylval.error_msg = "Unterminated string constant";
    return (ERROR);
  }
  if(c == EOF) {
    cool_yylval.error_msg = "EOF in string constant";
    return (ERROR);
  }
  if(c == '\"') {
    if(null_flag == 1) {
      cool_yylval.error_msg = "String contains null value";
      return (ERROR);
    } else if(curr_strlen >= MAX_STR_CONST) {
      cool_yylval.error_msg = "String constant too long";
      return (ERROR);
    } else {
      cool_yylval.symbol = stringtable.add_string(string_buf, curr_strlen);
      return (STR_CONST);
    }
  } 
}
{class}         { return (CLASS); }
{else}          { return (ELSE); }    
{fi}            { return (FI); } 
{if}            { return (IF); } 
{in}            { return (IN); } 
{inherits}      { return (INHERITS); }
{let}           { return (LET); }
{loop}          { return (LOOP); }
{pool}          { return (POOL); }
{then}          { return (THEN); }
{while}         { return (WHILE); }
{case}          { return (CASE); }
{esac}          { return (ESAC); }
{of}            { return (OF); }
{new}           { return (NEW); }
{isvoid}        { return (ISVOID); }
{assign}        { return (ASSIGN); }
{not}           { return (NOT); }
{int_const}     { 
  cool_yylval.symbol = inttable.add_string(yytext);
  return (INT_CONST); 
}
{bool_const}    {
  if(yytext[0] == 't') {
    cool_yylval.boolean = true;
  } else if(yytext[0] == 'f') {
    cool_yylval.boolean = false;
  }
  return (BOOL_CONST); 
}
{objectid}      {
  cool_yylval.symbol = idtable.add_string(yytext);
  return (OBJECTID); 
}
{typeid} {
  cool_yylval.symbol = idtable.add_string(yytext);
  return (TYPEID);
}
"+"             { return '+'; }
"/"             { return '/'; }
"-"             { return '-'; }
"*"             { return '*'; }
"="             { return '='; }
"<"             { return '<'; }
"."             { return '.'; }
"~"             { return '~'; }
","             { return ','; }
";"             { return ';'; }
":"             { return ':'; }
"("             { return '('; }
")"             { return ')'; }
"@"             { return '@'; }
"{"             { return '{'; }
"}"             { return '}'; }
{le}            { return (LE); }
{darrow}        { return (DARROW); }
"*)"  {
  cool_yylval.error_msg = "Unmatched *)";
  return (ERROR);
}
.             {
  cool_yylval.error_msg = yytext;
  return (ERROR); 
}

%%

int comment_match() {
  char c;
  while((c = yyinput()) != EOF) {
    if(c == '\n') {
      curr_lineno++;
    }
    if(c == '\\') { /**deal with escape in comments*/
      if((c = yyinput()) == EOF) {
        cool_yylval.error_msg = "EOF in comment";
        return (ERROR);
      }
    }
    if(c == '(') {
      while((c = yyinput()) == '(') {}
      if(c == '*') { /**start a new comment match*/
        comment_match();
      } else if(c == '\n') {
        curr_lineno++;
      } else if(c == '\\') { /**deal with escape in comments*/
        if((c = yyinput()) == EOF) {
          cool_yylval.error_msg = "EOF in comment";
          return (ERROR);
        }
      } else if(c == EOF) {
        cool_yylval.error_msg = "EOF in comment";
        return (ERROR);
      }
    }
    if(c == '*') {
      while((c = yyinput()) == '*') {}
      if(c == ')') { /**end comment*/
        return -1;
      } else if(c == '\n') {
        curr_lineno++;
      } else if(c == '\\') { /**deal with escape in comments*/
        if((c = yyinput()) == EOF) {
          cool_yylval.error_msg = "EOF in comment";
          return (ERROR);
        }
      } else if(c == EOF) {
        cool_yylval.error_msg = "EOF in comment";
        return (ERROR);
      }
    }
  }
  if(c == EOF) {
    cool_yylval.error_msg = "EOF in comment";
    return (ERROR);
  }
}
