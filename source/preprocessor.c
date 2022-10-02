/*
* The Assembler's preprocessor
* Stores out all the macros in our file into a linked list
* and replaces all calls to the macro with it's code.
* 
* Code will be stored in a .AM file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilities/input.h"
#include "utilities/defines.h"
#include "utilities/converters.h"
#include "utilities/formatter.h"
#include "utilities/textverify.h"

enum macroRange {OUT = 0, IN = 1};

/*Linked list of all macros */
typedef struct macro
{
    char *name;
    char *code;
    struct macro* next;
    
}macro;

macro *head = NULL;

/*Do we have any macros ?*/
int macroFlag = FALSE; 

/*Add new macro to list*/
#define ADD_MACRO(node,name,code) {\
                                    node = NEW_NODE(macro);\
                                    node->name = name;\
                                    node->code = code;\
                                  }


/* Clear all macros in our linked list */
void clearMacros()
{
    macro **headptr = &head;
    macro *temp ,*pt;
    /* While we haven't rached the end of the list,
     * free up all nodes in the list
     */
    temp = *headptr;
    while (temp != NULL)
    {
        pt = temp->next;
        free(temp->name);
        free(temp->code);
        free(temp);
        temp = pt;
    }
    head = NULL;
}

/* Insert a new macro into the Linked-List */
int insertMacro(char *name, char  *code)
{
    macro **headptr = &head;
    macro *follow = NULL;

    /*Create new macro node*/
    macro *temp = NEW_NODE(macro);
    CHECKMEM(temp);
    temp->name = name;
    temp->code = code;
    temp->next = NULL;

    /*If node is empty*/
    if (*headptr == NULL)
    {
        *headptr = temp;
        macroFlag = TRUE;
        return TRUE;
    }
    else/*If node isn't empty, add from end*/
    {
        follow = *headptr;
        while (follow->next != NULL)
        {
            follow = follow->next;
        }
        follow->next = temp;
    }
    return TRUE;
}

/* Get a macro by name of macro */
macro *getMacro(char *name, macro *head)
{
    macro *temp = head;
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

/* Check if we've entered, or, left the macro's borders */
int macroBorders(char *line)
{
    static int macroFlag;
    if ((strstr(line, "macro") != NULL))
    {
        macroFlag = IN;
    }
    if ((strstr(line, "endmacro") != NULL))
    {
        macroFlag = OUT;
    }
    return macroFlag;
}

/*Get the name of the macro from input*/
char *getName(char *line, int *currLine)
{
    char *name, *temp , placeholder[LINELEN];
    static const char delim[2] = " ";

    strcpy(placeholder,line);

    /*divide line to multiple parameters*/
    temp = strtok(placeholder,delim);
    temp = strtok(NULL, delim);

    /*Remove newline character (if there is one)*/
    removeNewline(temp); 


    if (isDirective(temp,0) >= 0)
    {
        printf("ERROR in line %d: Macro name must not be a directive \n",*currLine);
        return NULL;
    }
    if (isOrder(temp,0) >= 0)
    {
        printf("ERROR in line %d: Macro name must not be a command \n",*currLine);
        return NULL;
    }
    if (isRegister(temp,0) == TRUE)
    {
        printf("ERROR in line %d: Macro name must not be a register \n",*currLine);
        return NULL;
    }
    
    
    /*Allocate new memory for string*/
    name = (char *) malloc ( strlen(temp));
    CHECKMEM_PTR(name);
    

    /*Copy the name of macro from temp to name*/
    if (temp != NULL && name != NULL)
    {
        strcpy(name,temp);
        /*Return contents of name*/
        return name;
    }
    /*If issue with memory allocation,
      or no name was received, return NULL*/
    printf("ERROR in line %d: no macro name found \n", *currLine );
    return NULL;
}

/*Store contents of macro in: 
* name: The name of the macro.
* code: The code of the macro.
* Store the macro in a new node.
*/
char *macroContents(FILE *in, char *line, int *currLine)
{
    char *tempLine;
    char *name, *code;
    int flag;
    name = getName(line,currLine);
    if (name == NULL)
    { 
        printf("exiting. \n");
        return FALSE;
    }
    
    code = (char*)malloc(LINELEN);
    if (checkAlloc(code) == FALSE)
    {
        printf("ERROR: Issue allocating memory \n");
        FREEUP(name);
        return NULL;
    }
    code[0] = '\0';
    /*Store all code in macro*/
    while (((flag = getCodeLine(&tempLine,in)) != EOF) && macroBorders(tempLine) )
    {   
        code = concat(code, tempLine);
        FREEUP(tempLine); 
        (*currLine)++;
    }

    /*Insert new macro into code*/
    if(insertMacro(name,code) == MEMORY_ERROR)
    {
        printf("ERROR: Issue reallocating memory \n");
        return NULL;
    }
    return tempLine;
}

/*paste our macro into output file*/
void placeMacro(FILE *out,macro *selectedMacro)
{
    fputs(selectedMacro->code,out);
}

/*Function checks to see if line is indeed a macro*/
int checkMacro(char *line, FILE *out)
{
    int commandParams , i, foundMacro = FALSE;
    char **command;
    macro *macro = NULL;
    command = seperateCommands(line, &commandParams, " ");
    if (commandParams == 0)
    {
        free(command);
        goto ENDFUNC;
    }
        for ( i = 0; i < commandParams; i++)
        {
            if ( (macro = getMacro(command[i],head)) != NULL )
            {
                placeMacro(out,macro);
                foundMacro = TRUE;
            }
        }
        clearCommands(&command, commandParams);
     ENDFUNC: return foundMacro;
    
}

/*
* Process macros in our code.
*
* Function creates new file with .am extention.
* New .am file will contain code macro code already placed accordingly.
*/
void processMacros(char *filename)
{
     FILE *in, *out;
     char *line = NULL;
     int currLine = 0,errorFlag = FALSE;
     int endFlag, putMacro;
     /*Get our files*/
     in = getFile(filename, ".as" , "r");
     out = getFile(filename, ".am", "w");
     
     /*Read out the assemmbly code line by line and process it*/
     while ((endFlag = getCodeLine(&line, in)) != EOF)
     {
         
         /*Check if we're in the macro's borders*/
         if (macroBorders(line) == IN)
         {
            if((line = macroContents(in, line, &currLine)) == NULL)
            {
                FREEUP(line);
                errorFlag = TRUE;
                goto CLOSE;
            }
            continue;
         }

         /*Check if there are macros in our list*/
         if (macroFlag == TRUE)
         {
            putMacro = checkMacro(line, out);
            /*Was a macro found?*/
            if (putMacro == TRUE)
            {
                /*If found macro, then skip writing this line*/
                FREEUP(line);
                continue;
            }
         }
         /*If not in macro then, just write line as-is to output file*/
        fputs(line, out);
        FREEUP(line);
        currLine++;
     }

     /*Exit preprocessor*/
CLOSE:clearMacros();
     macroFlag = FALSE;
     closeFile(&in);
     closeFile(&out);
     /*If error was found, then delete the file*/
     if (errorFlag == TRUE)
     {
         removeFile(filename, ".am");
         exit(EXIT_FAILURE);
     }
}