
/*
* Assembler project for The Open University's Computer Science lab course
* Semester: 2022 B
*/

#include<stdio.h>

#include "utilities/input.h"
#include "preprocessor.h"
#include "compiler.h"

int main(int argc, char **argv)
{
    int i;
    
    /*Check if .am files exist*/
    checkFileValidity(argc,argv);
    
    /*Send to preprocessor*/
    for(i=1; i < argc; i++)
    {
        processMacros(*(argv + i));
    }

    /*Compile Assembly code into machine code*/
    for(i=1; i < argc; i++)
    {
        firstPass(*(argv + i));
    }

    /*Fin*/
    return 0;
}
