#include "./include/compile.h"

// ERRORS
new_error(UNDEFINED_TYPE, TYPE_ERROR, "Undefined type");
new_error(WRONG_ARGS, FUNC_ERROR, "Wrong number of arguments");

void init_compiler(COMPILER *c) {
    c->graph_flag = 0;

    for(int i=0; i < SIZE-1; i++) {
        c->variables[i].type   = "";
        c->variables[i].name   = "";
        c->variables[i].status = 0;

        c->functions[i].fun = "";
        c->functions[i].target = "";

        for(int j=0; j < SIZE-1; j++)
            c->functions[i].args[j] = NULL;
    }
}

// type check
void type_check(char* buf) {
    if(strcmp(buf, "POLY_F3") != 0 &&
       strcmp(buf, "POLY_F4") != 0)
            if(error(UNDEFINED_TYPE, buf))
                exit(1);
}

// check argument number
void args_check(char* buf, int argno) {
    if(strcmp(buf, "setXY3") == 0 && argno != 6)
        if(error(WRONG_ARGS, "setXY3"))
            exit(1);
    
    if(strcmp(buf, "setXY4") == 0 && argno != 8)
        if(error(WRONG_ARGS, "setXY4"))
            exit(1);

    if(strcmp(buf, "setRGB0") == 0 && argno != 3)
        if(error(WRONG_ARGS, "setRGB0"))
            exit(1);

    if(strcmp(buf, "setPolyF3") == 0 && argno != 0)
        if(error(WRONG_ARGS, "setPolyF3"))
            exit(1);

    if(strcmp(buf, "setPolyF4") == 0 && argno != 0)
        if(error(WRONG_ARGS, "setPolyF4"))
            exit(1);
}

// function check 
int fun_check(char* buf) {
    if(strcmp(buf, "setXY3") == 0    || 
       strcmp(buf, "setXY4") == 0    || 
       strcmp(buf, "setRGB0") == 0   || 
       strcmp(buf, "setPolyF3") == 0 || 
       strcmp(buf, "setPolyF4") == 0)
       return 1;

    return 0;
}


char* compile(COMPILER* c, PARSER* p) {
    int i = 0;
    int var_count = 0;
    int fun_count = 0;

    while(p->program[i] != NULL) {
        // INITIALIZATION
        if(strcmp(p->program[i], "init") == 0) {
            if(strcmp(p->program[i+1], "graphics") == 0)
                c->graph_flag = 1;

            // TODO: add in pad and audio later

            i += 2;
        }

        // VARIABLES
        // TODO: make a function that checks this condition and throw an error if not
        if(strcmp(p->program[i], "def") == 0) {
            c->variables[var_count].type = p->program[i+1];
            c->variables[var_count].name = p->program[i+2];
            c->variables[var_count].status = 1;

            type_check(c->variables[var_count].type);

            var_count++;
            i += 2;
        }

        // FUNCTIONS
        // TODO: make a function that checks this condition and throw an error if not
        if(fun_check(p->program[i])) {
            int k=0;

            c->functions[fun_count].fun = p->program[i];
            c->functions[fun_count].target = p->program[i+1];

            i+=2;

            while(strcmp(p->program[i], ";") != 0) {
                c->functions[fun_count].args[k] = p->program[i];
                i++;
                k++;
            }

            args_check(c->functions[fun_count].fun, k);
            
            fun_count++;
        }

        // COMMENTS
        /*
        if(strcmp(p.program[i], "/") == 0) {
            while(p.program[i] != "\n")
                i++;
        }
        */
        else
            i++;

    }

    // C code generation

    char* output = (char*)malloc(10000 * sizeof(char));
    char* header = (char*)malloc(10000 * sizeof(char));

    // check graph_flag
    // FIXME: clean up this trash
    if(c->graph_flag) {
        strcat(header, "#include <stdlib.h>\n#include <libgte.h>\n#include <libgpu.h>\n#define SCREENW 512\n#define SCREENH 512\nDISPENV dispenv[2];\nDRAWENV drawenv[2];\n");
        strcat(header, "int d_buff = 0;\nvoid init_graphics() {\nResetGraph(0);\nSetVideoMode(1);\nSetDefDispEnv(&dispenv[0], 0, 0, SCREENW, SCREENH);\n");
        strcat(header, "SetDefDispEnv(&dispenv[1], 0, 0, SCREENW, SCREENH);\nSetDefDrawEnv(&drawenv[0], 0, 0, SCREENW, SCREENH);\n");
        strcat(header, "SetDefDrawEnv(&drawenv[1], 0, 0, SCREENW, SCREENH);\nPutDispEnv(&dispenv[0]);\nPutDrawEnv(&drawenv[0]);\n}\nvoid display();\n");
    }

    // VARIABLES
    int v = 0;
    char* var_decl = (char*)malloc(100 * sizeof(char));
    while(c->variables[v].status) {
        sprintf(var_decl, "%s %s;\n", c->variables[v].type, c->variables[v].name);
        strcat(header, var_decl);
        v++;
    }

    strcat(output, header);

    strcat(output, "\nint main() {\ninit_graphics();\n");


    // FUNCTIONS
    int f = 0;
    int arg = 0;
    while(f < fun_count) {
        char* fun_call = (char*)malloc(75 * sizeof(char));
        char* call     = (char*)malloc(10 * sizeof(char));
        char* target   = (char*)malloc(10 * sizeof(char));
        char* args     = (char*)malloc(50 * sizeof(char));

        call = c->functions[f].fun;
        target = c->functions[f].target;

        arg = 0;

        while(c->functions[f].args[arg] != NULL) {
            strcat(args, c->functions[f].args[arg]);
            if(c->functions[f].args[arg+1] != NULL)
                strcat(args, ", ");
            arg++;
        }

        if(strcmp(args, "\0") != 0)
            sprintf(fun_call, "%s(&%s, %s);\n", call, target, args);

        else
            sprintf(fun_call, "%s(&%s);\n", call, target);

        strcat(output, fun_call);

        f++;
    }

    strcat(output, "while(1) {\ndisplay();}\nreturn 0;\n}\n\nvoid display(){\nSetDispMask(1);\nd_buff = !d_buff;\n");

    // VARIABLES
    v = 0;
    while(v < var_count) {
        char* var_dr = (char*)malloc(20 * sizeof(char));
        char* var_ty = c->variables[v].type; 
        // TODO: add more later
        // TODO: make a function that checks this condition and throw an error if not
        if(strcmp(var_ty, "POLY_F3") == 0 || strcmp(var_ty, "POLY_F4") == 0) {
            sprintf(var_dr, "DrawPrim(&%s);\n", c->variables[v].name);
            strcat(output, var_dr);
        }
        v++;
    }

    strcat(output, "\nVSync(0); DrawSync(0);\nPutDrawEnv(&drawenv[d_buff]);\nPutDispEnv(&dispenv[d_buff]);\n}");

    return output;
}
