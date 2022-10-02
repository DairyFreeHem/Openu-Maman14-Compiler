
#pragma once

/*If no value was found*/
#ifndef NONE
#define NONE -1
#endif

enum directives{DATA = 1, STRING, STRUCT, ENTRY, EXTERN};


enum orders{MOV = 0, CMP , ADD , SUB , NOT ,CLR, LEA, INC , DEC , JMP , BNE , GET , PRN , JSR , RTS , HLT };


/*Check if value is a valid directive*/
int isDirective (char *line, int index);

/*Check if value is a valid command*/
int isOrder (char *line, int index);

/*Check if value is a valid number
 * (both positive and degative numbers)*/
int isNumber(char *line, int index);

/*Check if value received is a valid register value*/
int isRegister(char *line, int index);

/*Check if value received is a valid internal label*/
int isInternalLabel(char *line, int index);
