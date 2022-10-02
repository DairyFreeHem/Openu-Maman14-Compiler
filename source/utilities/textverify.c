/*
 *File used to verify text's data type, if it's a directive,order,number,register or internal label
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "defines.h"
#include "textverify.h"
#include "formatter.h"

/*Maximum label length*/
#define SymbolMax 30




char *directives[5] = {"data\0","string\0","struct\0","entry\0","extern\0"};
char *orders[16] = {"mov\0","cmp\0","add\0","sub\0","not\0","clr\0","lea\0","inc\0","dec\0","jmp\0","bne\0","get\0","prn\0","jsr\0","rts\0","hlt\0"};


/*Check if value is a valid directive*/
int isDirective (char *line, int index){
    int iterator = 1;
    char directive[7];
    int i;    
    
    if(line[index] == '.'){        
        while((isspace(line[index+iterator]) == FALSE) && (line[index+iterator] !='\0')){
            if(iterator>=7)
                return -1;
            directive[iterator-1]=line[index+iterator];
            iterator++;
        }        
        directive[iterator-1]='\0';
        for (i=0;i<5;i++){
            if(strcmp(directive,directives[i])==0){
                return i+1;
            }
        }
        return NONE;

    }    
    return NONE;
}

/*Check if value is a valid command*/
int isOrder (char *line, int index){
    int iterator=0;
    char order[5];
    int i;    
    while (iterator<4 && line[index+iterator] !='\0' && !isspace(line[index+iterator])){
        order[iterator]=line[index+iterator];
        iterator++;
    }    
    order[iterator]='\0';
    for (i=0;i<16;i++){
        if(strcmp(order,orders[i])==0){
            return i;
        }
    }    
    return -1;
}

/*Check if value is a valid number
 * (both positive and degative numbers)*/
int isNumber(char *line, int index)
{
    int start = index ,end = 0;
    if(start+end < LINELEN && (line[start+end] == '-' || line[start+end] == '+'))
        start++;
    while (line[start+end] != '\0' && line[start+end]>='0' && line[start+end]<='9'){        
        end++;
    }
    return (end == 0)? PARAM_NOT_NUMBER: start+end;
}

/*Check if value received is a valid register value*/
int isRegister(char *line, int index)
{
    int number;
    if (line[index] == 'r')
    {
        index++;
        number = (line[index] - '0');
        if (number >= 1 && number <= 7)
        {
            return TRUE;
        }
        
    }
    return FALSE;
}

/*Check if value received is a valid internal label*/
int isInternalLabel(char *line, int index)
{
    if (isalpha(line[index]))
    {
        while(isalpha(line[index]) || isdigit(line[index]))
        {
            index++;
        }
        if (line[index] == ':')
        {
            return TRUE;
        }
    }
    return FALSE;
}
 