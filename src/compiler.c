#include "compiler.h"

int main(int argc, char *argv[]){
    if (argc > 1){
        compilerstuffmain(argv[1]);
    } else {
        //compilerstuffmain("test.vm");
        printf("ERROR: File not specified\n");
        printf("USAGE: %s <FILENAME>\n", argv[0]);
        exit(EXIT_FAILURE);
        
    }
    return 0;
}