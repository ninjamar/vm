// takes file -> splits -> turns into numbers -> executes numbers

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "vm.h"


char* loadfileintobuf(FILE *f, long **s){
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *fbuf = malloc(fsize);
    if (fbuf){
        //fread(fbuf, 1, fsize, f);
        int c;
        int n=0;
        while ((c = fgetc(f)) != EOF){
            fbuf[n++] = (char)c;
        }
    }
    *s = fsize;
    return fbuf;
}

int* tokenize(char *buf, long bs, long **s){
    int* outbuf = malloc(bs);
    for (int i=0; i<bs; i++){
        outbuf[i] = 0;
    }
    char *token = strtok(buf, " \n\t");
    int i = 0;
    while (token != NULL) {
        if (strcmp(token, "HLT") == 0){outbuf[i] = HLT; goto ugly_gpoint;}
        if (strcmp(token, "ADD") == 0){outbuf[i] = ADD; goto ugly_gpoint;}
        if (strcmp(token, "SUB") == 0){outbuf[i] = SUB; goto ugly_gpoint;}
        if (strcmp(token, "MUL") == 0){outbuf[i] = MUL; goto ugly_gpoint;}
        if (strcmp(token, "DIV") == 0){outbuf[i] = DIV; goto ugly_gpoint;}
        if (strcmp(token, "MOV") == 0){outbuf[i] = MOV; goto ugly_gpoint;}
        if (strcmp(token, "CPY") == 0){outbuf[i] = CPY; goto ugly_gpoint;}
        if (strcmp(token, "RLOG") == 0){outbuf[i] = RLOG; goto ugly_gpoint;}
        if (strcmp(token, "JE") == 0){outbuf[i] = JE; goto ugly_gpoint;}
        if (strcmp(token, "JNE") == 0){outbuf[i] = JNE; goto ugly_gpoint;}
        if (strcmp(token, "INC") == 0){outbuf[i] = INC; goto ugly_gpoint;}
        if (strcmp(token, "DEC") == 0){outbuf[i] = DEC; goto ugly_gpoint;}
        if (strcmp(token, "LBL") == 0){outbuf[i] = LBL; goto ugly_gpoint;}
        if (strcmp(token, "CMP") == 0){outbuf[i] = CMP; goto ugly_gpoint;}
        if (strcmp(token, "JMP") == 0){outbuf[i] = JMP; goto ugly_gpoint;}
        
        if (strcmp(token, "G1") == 0){outbuf[i] = G1; goto ugly_gpoint;}
        if (strcmp(token, "G2") == 0){outbuf[i] = G2; goto ugly_gpoint;}
        if (strcmp(token, "G3") == 0){outbuf[i] = G3; goto ugly_gpoint;}
        if (strcmp(token, "G4") == 0){outbuf[i] = G4; goto ugly_gpoint;}
        if (strcmp(token, "G5") == 0){outbuf[i] = G5; goto ugly_gpoint;}
        if (strcmp(token, "G6") == 0){outbuf[i] = G6; goto ugly_gpoint;}
        if (strcmp(token, "P1") == 0){outbuf[i] = P1; goto ugly_gpoint;}
        if (strcmp(token, "P2") == 0){outbuf[i] = P2; goto ugly_gpoint;}
        if (strcmp(token, "P3") == 0){outbuf[i] = P3; goto ugly_gpoint;}
        if (strcmp(token, "P4") == 0){outbuf[i] = P4; goto ugly_gpoint;}
        if (strcmp(token, "P5") == 0){outbuf[i] = P5; goto ugly_gpoint;}
        if (strcmp(token, "P6") == 0){outbuf[i] = P6; goto ugly_gpoint;}
        if (strcmp(token, "R1") == 0){outbuf[i] = R1; goto ugly_gpoint;}
        if (strcmp(token, "R2") == 0){outbuf[i] = R2; goto ugly_gpoint;}
        if (strcmp(token, "R3") == 0){outbuf[i] = R3; goto ugly_gpoint;}
        if (strcmp(token, "R4") == 0){outbuf[i] = R4; goto ugly_gpoint;}
        if (strcmp(token, "R5") == 0){outbuf[i] = R5; goto ugly_gpoint;}
        if (strcmp(token, "R6") == 0){outbuf[i] = R6; goto ugly_gpoint;}
        if (strcmp(token, "PC") == 0){outbuf[i] = PC; goto ugly_gpoint;}
        if (strcmp(token, "LOG") == 0){outbuf[i] = LOG; goto ugly_gpoint;}
        if (strcmp(token, "LP") == 0){outbuf[i] = LP; goto ugly_gpoint;}
        if (strcmp(token, "CF") == 0){outbuf[i] = CF; goto ugly_gpoint;}
        
        // numbers etc
        outbuf[i] = atoi(token);

        ugly_gpoint:
        token = strtok(NULL, " \n\t");
        i++;
    }
    
    *s = bs;

    return outbuf;
}

int compilerstuffmain(char *fname){
    FILE *f = fopen(fname, "r");
    if (!f){
        printf("ERROR: File cannot be opened\n");
        exit(1);
    }
    long *lfbufsize;
    long *cbufsize;
    char *buf = loadfileintobuf(f, &lfbufsize);

    int *tokenizedbuf = tokenize(buf, lfbufsize, &cbufsize);
    free(buf);
    execprog(tokenizedbuf);
    fclose(f);
    return 0;
}