#include "./include/compile.h"

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


char* compile(COMPILER* c, PARSER* p) {
    int i = 0;
    int var_count = 0;
    int fun_count = 0;

    while(p->program[i] != NULL) {
        // INITIALIZATION
        if(p->program[i] == "init") {
            if(p->program[i+1] == "graphics")
                c->graph_flag = 1;
            // TODO: add in pad and audio later
            printf("init");

            i += 2;
        }

        // VARIABLES
        if(p->program[i] == "def") {
            c->variables[var_count].type = p->program[i+1];
            c->variables[var_count].name = p->program[i+2];
            c->variables[var_count].status = 1;

            var_count++;
            i += 3;
        }

        // FUNCTIONS
        if(p->program[i] == "setXY3" || p->program[i] == "setRGB0") {
            int k=0;

            c->functions[fun_count].fun = p->program[i];
            c->functions[fun_count].target = p->program[i+1];

            while(p->program[i] != ";") {
                c->functions[fun_count].args[k] = p->program[i];
                i++;
            }
            fun_count++;
            // maybe?
        }

/*
        // COMMENTS
        if(p.program[i] == ";") {
            while(p.program[i] != "\n")
                i++;
        }
*/
        else
            i++;

    }

    // C code generation

    char* output = (char*)malloc(10000 * sizeof(char));
    char* header = (char*)malloc(1000 * sizeof(char));

    // check graph_flag
    // FIXME: clean up this trash
    strcat(header, "#include <stdlib.h>\n#include <libgte.h>\n#include <libgpu.h>\n#define SCREENW 512\n#define SCREENH 512\nDISPENV dispenv[2];\nDRAWENV drawenv[2];\n");
    strcat(header, "int d_buff = 0;\nvoid init_graphics() {\nResetGraph(0);\nSetVideoMode(1);\nSetDefDispEnv(&dispenv[0], 0, 0, SCREENW, SCREENH);\n");
    strcat(header, "SetDefDispEnv(&dispenv[1], 0, 0, SCREENW, SCREENH);\nSetDefDrawEnv(&drawenv[0], 0, 0, SCREENW, SCREENH);\n");
    strcat(header, "SetDefDrawEnv(&drawenv[1], 0, 0, SCREENW, SCREENH);\nPutDispEnv(&dispenv[0]);\nPutDrawEnv(&drawenv[0]);\n}\nvoid display();\n");

    // VARIABLES
    int v = 0;
    while(c->variables[v].status) {
        char* var_decl;
        sprintf(var_decl, "%s %s;\n", c->variables[v].type, c->variables[v].name);
        strcat(output, var_decl);
        v++;
    }

    strcat(output, header);

    // FIXME:
    // just to have something working
    strcat(output, "POLY_F3 pf3;\n");

    strcat(output, "\nint main() {\ninit_graphics();\n");


    // FUNCTIONS
    int f = 0;
    int arg = 0;
    while(f < fun_count) {
        char* fun_call;
        char* call;
        char* target;
        char* args;

        call = c->functions[f].fun;
        target = c->functions[f].target;

        while(c->functions[f].args[arg] != NULL) {
            strcat(args, c->functions[f].args[arg]);
            strcat(args, ", ");
            arg++;
        }
        sprintf(fun_call, "%s(&%s, %s);\n", call, target, args);
        strcat(output, fun_call);

        f++;
    }

    // FIXME:
    // just to have something working
    strcat(output, "setPolyF3(&pf3);\nsetRGB0(&pf3, 255, 255, 255);\nsetXY3(&pf3, 0, 0, 100, 100, 200, 100);");

    // finish the program
    strcat(output, "while(1) {\ndisplay();}\nreturn 0;\n}\n\nvoid display(){\nSetDispMask(1);\nd_buff = !d_buff;\n");

    // VARIABLES
    v = 0;
    while(v < var_count) {
        char* var_dr;
        char* var_ty = c->variables[v].type; 
        // TODO: add more later
        if(var_ty == "POLY_F3") {
            sprintf(var_dr, "DrawPrim(&%s);\n", c->variables[v].name);
            strcat(output, var_dr);
        }
        v++;
    }

    // FIXME:
    // just to have something working
    strcat(output, "\nDrawPrim(&pf3);");
    strcat(output, "\nVSync(0); DrawSync(0);\nPutDrawEnv(&drawenv[d_buff]);\nPutDispEnv(&dispenv[d_buff]);\n}");

    return output;
}
