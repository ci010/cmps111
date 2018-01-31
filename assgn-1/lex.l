%{
    const int max = 256;
    char* argv[256];
    int argc = 0;
%}

%option nounput
%option noinput


WORD        [0-9a-zA-Z\.\/-]+
SPECIAL     [()<>|&;]

%%
    argc = 0; argv[0] = NULL;

{WORD}|{SPECIAL} {
    if (argc < max - 1) {
        argv[argc++] = (char *) strdup(yytext);
        argv[argc] = NULL;
    }
}

\n  return (int) argv;

[ \t]+

.   
%%

char** getline() { 
    return (char**) yylex();
}