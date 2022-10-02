/*
* Compiler's code table.
* Represents the code table,
* and parses data for each line and parameter
*/

#include <stdio.h>
#include <stdlib.h>
#include "../utilities/input.h"
#include "../utilities/textverify.h"
#include "../utilities/converters.h"
#include "../utilities/formatter.h"
#include "../utilities/defines.h"
#include "labelTable.h"
#include "LabelReferenceTable.h"

#include "codeTable.h"

#define MAX_CODE_IC 4

int caseOfNumber(char *params, int *index);
int caseOfStructLabel(char *params, int *index);
int caseOfLabel(char *params, int *index);


codeList *codeHead = NULL;

/*Create new codeList node and insert into the linked-list*/
int insertCode(int memloc,int IC,int *bin, int *params) 
{

    codeList **headptr = &codeHead;
    codeList *temp = NULL;
    codeList *follow = NULL;

    /*Create new codeList node*/
    temp = NEW_NODE(codeList);
    CHECKMEM(temp);
    temp->bin = bin;
    temp->IC = IC;
    temp->addressInDec = memloc;
    temp->paramsInDec = params;
    temp->next = NULL;

    /*Inserts temp to end of table*/
    ADD_TO_TABLE(codeHead,follow,temp,headptr);

    return ALL_CLEAR;
}

/*Clear entire code list*/
void clearCodeList(void)
{ 
    codeList *temp , *follow;
    codeList **headptr = &codeHead;
    temp = *headptr;

    /*If table is empty, exit*/
    if (codeHead == NULL)
    {
        return;
    }
    /* While we haven't rached the end of the list,
     * free up all nodes in the list
     */
    while (temp != NULL)
    {
        follow = temp->next;
        FREEUP(temp->bin);
        FREEUP(temp->paramsInDec);
        FREEUP(temp);
        temp = follow;
    }
    
    codeHead = NULL;
}

/*Get head of codeList*/
codeList *getCodeHead(void)
{
    return codeHead;
}

codeList *getCodeByLoc(int memeloc)
{
    codeList *temp = codeHead ;
    while (temp != NULL)
    {
        if (temp->addressInDec == memeloc)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}


int getLabelMemoryPos(char *line,int memeloc)
{
     labels *label = NULL;
     char *labelName = NULL;
     int memoryPos = NONE;

    if (getLabelName(line,&labelName,LINE_START,CODE) <= ERROR_AREA_START)
    {
        return MEMORY_ERROR;
    }

    label = getLabel(labelName);
    if (label != NULL)
    {
        memoryPos = label->addressInDec;
    }

    FREEUP(labelName);
    FREEUP(label);

    return memoryPos;
    
}

int getBinOneParam(char *line, int **bin, int index, int type)
{
    int L = 0, response = 0;
    int binVal;


    if (line[index] == '#')
    {
        if (type != PRN)
        {
            return INVALID_CODE_PARAM;
        }
        
        response +=  caseOfNumber(line, &index);
        binVal = NUMBERCASE;
    }
    /*If Register*/
    else if (line[index] == 'r')
    {
        if (isRegister(line,index))
        {
            response += 1;           
            binVal = REGISTERCASE;
            index = jumpWord(line,index); 
        }
        else
        {
            response = INVALID_REGISTER;
        }
    }
    /*If Struct label*/
    else if (line[index] == '.')
    {
        response += caseOfStructLabel(line, &index);
        binVal = STRUCTCASE;
    }
    /*Check if regular label*/
    else
    {
        response += caseOfLabel(line, &index);
        binVal = LABELCASE;
    }
    if(line[index] != '\0')
    {
        return PARAM_OVERFLOW;
    }
    if (response <= 0 )
    {
        return response;
    }

    (*bin)[2] = binVal;
    L += response;

    return L;
}


int getBinTwoParam(char *line, int **bin, int index, int type)
{
    char **params = NULL;
    int paramCounter;
    int L = 0,i;
    int response = 0;
    int FlagRegister = FALSE;
    
    index = 0;
    params = seperateCommands(line, &paramCounter, ",");
    CHECKMEM(params);

    /*Check if params received are one or more*/
    if (paramCounter < 2 )
    {
        clearCommands(&params, paramCounter);
        return PARAM_UNDERFLOW;
    }
    if (paramCounter > 2 )
    {
        clearCommands(&params, paramCounter);
        return PARAM_OVERFLOW;
    }

    /*Start of read*/
    for (i = 0; i < paramCounter; i++)
    {
        index = jumpSpace(params[i],LINE_START);
        /*If number*/
        if (params[i][index] == '#')
        {
            /*Only CMP command can have a number as their DST*/
            if (i == DST && type != CMP)
            {
                return INVALID_CODE_PARAM;
            }
            
           response +=  caseOfNumber(params[i], &index);
           (*bin)[i+1] = 0;
        }
        /*If Register*/
        else if (params[i][index] == 'r')
        {
            /*If our source is a register,
             *then well add both source and dest to the same code line */
            if (FlagRegister == TRUE)
            {
                response--;
            }
            if (isRegister(params[i],index))
            {
                response += 1;           
                index = jumpWord(params[i],index); 
                (*bin)[i+1] = 3;
                FlagRegister = TRUE;
            }
            else
            {
                response = INVALID_REGISTER;
            }
        }
        /*If Struct label*/
        else if (strchr(params[i] , '.'))
        {
           response += caseOfStructLabel(params[i], &index);
           (*bin)[i+1] = 2;
        }
        /*Check if regular label*/
        else
        {
           response += caseOfLabel(params[i], &index);
           (*bin)[i+1] = 1;
        }
        if(params[i][index] != '\0')
        {
            clearCommands(&params, paramCounter);
            return PARAM_OVERFLOW;
        }
    }
    if (response <= 0 )
    {
        L = response;
    }
    else
    {
        L += response;
    }
    clearCommands(&params, paramCounter);
    return L;
}

/*If facing a number*/
int caseOfNumber(char *params, int *index)
{
    int L = 0;
    int number;
    (*index)++;
    number = addNumber(params,*index);
    if (number <= ERROR_AREA_START)
    {
        return number;
    }
    *index = jumpWord(params, *index);
    L++;
    
    return L;
}

/*If facing a struct*/
int caseOfStructLabel(char *params, int *index)
{
    int L = 0;
    int number;
    char *point = strchr(params, '.');
    point++;

    number = addNumber(point,*index);
    if (number <= 0 && number > 2)
    {
        return INVALID_STRUCT_POS;
    }

    *index = jumpWord(params,*index);

    L += 2;
    return L;
}

/*If facing a label*/
int caseOfLabel(char *params,int *index)
{
    int L = 0;
    *index = jumpWord(params,*index);

    L++;
    return L;
}

/*Function tests if commands were inputted successfully
  and accomodates memory location accordingly*/
int processCode(char *line, int index, int *memloc,int *IC, int type)
{
    int L = 1, i;
    int *bin = (int *)malloc(4 * sizeof(int));
    char *nuLine = (char *)malloc(LINELEN * sizeof(char));
    int *params = NULL;
    
    if (nuLine == NULL)
    {
        FREEUP(bin);
        return MEMORY_ERROR;
    }
    
    for (i = 0; i < 4; i++)
    {
        bin[i] = 0;
    }
    
    CHECKMEM(bin);
    strcpy(nuLine, (line+index));
    index = 0;

    bin[COMMANDPOS] = type;
    if ((type >= NOT && type <= JSR) || (type == CLR) || (type == LEA))
    {
        L += getBinOneParam(nuLine,&bin,index,type);
    }
    else if(type >= MOV && type <= LEA)
    {
        
        L += getBinTwoParam(nuLine,&bin,index,type);
    }
    else
    {
        FREEUP(nuLine);
    }
    if (L <= ERROR_AREA_START)
    {
        return L;
        FREEUP(bin);
        FREEUP(nuLine);
    }
    if (nuLine != NULL)
    {
        nuLine = (char *)realloc(nuLine, strlen(nuLine) * sizeof(char));
        if (nuLine == NULL)
        {
            FREEUP(bin);
            return MEMORY_ERROR;
        }
    }
    params = (int *)malloc(L * sizeof(int));
    CHECKMEM(params);
    params[0] = codeHeadtoInt(bin);
    insertCode(*memloc,L,bin,params);
    UPDATE_MEMORY(IC,memloc,L);
    
    
    return ALL_CLEAR;
}

int processLabelParam(char *command,int memloc)
{
    int numberVal[2] = {0};
    int response;
    numberVal[1] = RELOCATABLE;
    response = getLabelMemLoc(command);
    if (response == NONE)
    {
        if (doesLabelReferenceExist(command,EXT) == TRUE)
        {
            processLabel(command,LINE_START,memloc,EXT);
            response = 0;
            numberVal[1] = EXTERNAL;
        }
        else
        {
            return LABEL_NOT_FOUND;
        }
            
        }
          numberVal[0] = response;
          return binCodeCreator(numberVal,8);
}

/*Return decimal representation of struct label value*/
int processStructName(char *command)
{
    int i,response;
    int numberVal[2] = {0};
    char label[LABELMAXLEN];
    for (i = 0; command[i] != '.'; i++)
    {
        label[i] = command[i];
    }
    label[i] = '\0';
    response = getLabelMemLoc(label);
    if (response == NONE)
    {
        return LABEL_NOT_FOUND;
    }
    numberVal[0] = response; 
    numberVal[1] = RELOCATABLE;
    return binCodeCreator(numberVal,8);
}
/*Return decimal representation of struct position*/
int processStructNum(char *command)
{
    int i;
    int numberVal[2] = {0};
    for (i = 0; command[i] != '.'; i++)
        ;
    i++;
    numberVal[0] = (int)(command[i] - '0');
    numberVal[1] = ABSOLUTE;
    return binCodeCreator(numberVal,8);
}

int processNumberParam(char *command)
{
    int response;
    int numberVal[2] = {0};
    response = atoi(command);
    numberVal[0] = response;
          
    return binCodeCreator(numberVal,8);
}

int processParams(codeList *code,char *line, int index)
{
    int i,type, param = 1,response,structFlag = FALSE;
    char *command = NULL;
    int registerVal[2] = {0};
    char *nuLine = (char *)malloc(LINELEN * sizeof(char));
    type = code->bin[0];

    CHECKMEM(nuLine);
    strcpy(nuLine, (line+index));
    if ((type >= NOT && type <= JSR) || (type == CLR) || (type == LEA))
    {
        param = 2;
    }

    for (i = 1; i < code->IC; i++,param++)
    {
      if (structFlag == TRUE)
      {
          response = processStructNum(command);
          structFlag = FALSE;
          param--;
      }
      else
      {
        if (command == NULL)
        {
            command = getCodeParam(nuLine,i);
        }
        CHECKMEM(command);
        switch (code->bin[param])
        {
            case NUMBERCASE:
                response = processNumberParam(command);
                break;
            case LABELCASE:
                response = processLabelParam(command,(code->addressInDec + i));
                break;
            case STRUCTCASE:
                response = processStructName(command);
                structFlag = TRUE;
                break;
            case REGISTERCASE:
                if (i == 1)
                {
                    registerVal[0] = (int)(command[1] - '0');

                    if (code->bin[i+1] == REGISTERCASE)
                    {
                        FREEUP(command);
                        command = getCodeParam(nuLine,(i+1));
                        CHECKMEM(command);
                        registerVal[1] = (int)(command[1] - '0');
                    }
                }
                else
                {
                    registerVal[1] = (int)(command[1] - '0');
                }
                
            
                response = binRegisterCreator(registerVal);
                break;
        }
      }
    if (structFlag == FALSE)
    {
           FREEUP(command);
    }
    if (response <= ERROR_AREA_START)
    {
        return response;
    }
      
      code->paramsInDec[i] = response;
    }
    FREEUP(nuLine);
    return ALL_CLEAR; 
}
