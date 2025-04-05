%{
#include <stdio.h>
#include "expr_value.h"
#include "syntax.h"
// #include "symtab.h"
// #include "syntax.tab.h"

extern int column;
extern int line;

void count_chars(void);
void count_newline(void);
void lexical_error(const char *message);

/* For debugging - trace all tokens */
// #define TRACE_TOKEN(t) printf("TOKEN: %s -> '%s'\n", #t, yytext)
%}

digit [0-9]
integer [-]?{digit}+
float {digit}+\.{digit}+
IDF [a-zA-Z_][a-zA-Z0-9_]* 
string \"([^\"\n]|\\\")*\"
charactere '[^'\n]'
commentaire §[^\n]*

%%

"CODE" { count_chars(); return mc_code;}
"DATA" { count_chars(); return mc_data;}

"INTEGER" { 
    count_chars(); 
    // TRACE_TOKEN(mc_integ);
    return mc_integ; 
}
"FLOAT" { count_chars(); return mc_float; }
"CHAR" { count_chars(); return mc_char; }
"STRING" { count_chars(); return mc_string; }
"CONST" { count_chars(); return mc_const; }
"VECTOR" { count_chars(); return mc_vector; }

"START" { count_chars(); return mc_start; } 
"END" { count_chars(); return mc_endp; }
"READ" { count_chars(); return mc_read; }
"DISPLAY" { count_chars(); return mc_display; }
"IF" { count_chars(); return mc_if; }
"ELSE" { count_chars(); return mc_else; }
"FOR" { count_chars(); return mc_for; }

{IDF} { 
    count_chars(); 
    // TRACE_TOKEN(mc_ident);
    yylval.str_val = strdup(yytext);
    return mc_ident; 
}

{float} { 
    count_chars(); 
    yylval.float_val = atof(yytext);
    return FLOAT_CONST;
}

{integer} {
    count_chars();  
    int val = atoi(yytext);
    if (val > 32767 || val < -32768) {
        lexical_error("Integer out of range");
    } else {
        yylval.int_val = val;
        return INT_CONST;
    }
}

[0-9]+ { 
    count_chars(); 
    yylval.int_val = atoi(yytext);
    return mc_cst;
}

{string} { 
    count_chars(); 
    // Remove the quotes from the string to get the actual string value
    int len = strlen(yytext);
    char* str = strdup(yytext + 1);  // Skip first quote
    str[len - 2] = '\0';             // Remove last quote
    yylval.str_val = str;
    return mc_chaine; 
}
{charactere} { 
    count_chars(); 
    yylval.char_val = yytext[1]; // Get the character between quotes
    return mc_charactere; 
}
{commentaire} { count_chars(); return mc_comment; }

";" { count_chars(); return mc_pvg; }
"," { count_chars(); return mc_vg; }
":" { count_chars(); return mc_deuxp; }

"OR" { count_chars(); return mc_or; }
"AND" { count_chars(); return mc_and; }
">" { count_chars(); return mc_sup; }
"<" { count_chars(); return mc_inf; }
">=" { count_chars(); return mc_supeg; }
"<=" { count_chars(); return mc_infeg; }
"==" { count_chars(); return mc_egal; }
"=" { count_chars(); return mc_affectation; }
"!=" { count_chars(); return mc_diff; }
"+" { count_chars(); return mc_plus; }
"-" { count_chars(); return mc_moins; }
"*" { count_chars(); return mc_mult; }
"/" { 
    count_chars(); 
    // printf("mot cle /\n");
    return mc_div; 
}
"[" { count_chars(); return mc_crocheto; }
"]" { count_chars(); return mc_crochetf; }
"|" { count_chars(); return mc_separateur; }
"(" { count_chars(); return mc_paro; }
")" { count_chars(); return mc_parf; }

"@" { count_chars(); return mc_adresse; }

[ \t]+ { 
    count_chars(); 
    /* Skip whitespace */
}

\n {line++; column=1;}
. { 
    count_chars(); 
    // fprintf(stderr, "Lexical error: Unrecognized character '%c' at line %d, column %d\n", 
    //        yytext[0], line, column);
}

"-"{integer} {
    count_chars();
    lexical_error("Negative integer must be wrapped within parentheses");
}
            
%%
 
void count_chars(void) {
    column += yyleng;
}

void count_newline(void){
    line++;
    column = 1;
}

void lexical_error(const char *message){
    fprintf(stderr, "Lexical error at line %d, column %d: %s\n", line, column, message);
}